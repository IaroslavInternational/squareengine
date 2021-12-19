using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
//using System.Drawing.Drawing2D;
//using System.Drawing.Imaging;
using System.Windows;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;
using LazZiya.ImageResize;
using Newtonsoft.Json;

namespace Preprocessor
{
    class Program
    {
        private struct Resolution
        {
            public int    width;
            public int    height;
            public double koef_w;
            public double koef_h;
        };
        /*
        private static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode    = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.AssumeLinear;
                graphics.InterpolationMode  = InterpolationMode.NearestNeighbor;
                graphics.SmoothingMode      = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode    = PixelOffsetMode.None;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }
            }

            return destImage;
        }
        */
        private static Resolution GetResolution(string path)
        {
            Console.WriteLine("Получение разрешения...");

            Resolution resolutionData = new Resolution();

            using (FileStream fstream = File.OpenRead($"{path}\\settings.ppinfo"))
            {
                byte[] array = new byte[fstream.Length];
                fstream.Read(array, 0, array.Length);

                string textFromFile = System.Text.Encoding.Default.GetString(array);
                textFromFile = Regex.Replace(textFromFile, @"\r\n", "");

                var data = textFromFile.Split(';');

                resolutionData.width  = Convert.ToInt32(data[0].Split(':').GetValue(1));
                resolutionData.height = Convert.ToInt32(data[1].Split(':').GetValue(1));
            }

            Console.WriteLine("Разрешение получено.");

            return resolutionData;
        }

        private static void SetResolution(string path, Resolution resolution)
        {
            Console.WriteLine("Сохранение разрешения...");

            string text = "d_width:" + resolution.width + ";\r\nd_height:" + resolution.height;

            using (StreamWriter sw = new StreamWriter($"{path}\\settings.ppinfo", false, System.Text.Encoding.Default))
            {
                sw.WriteLine(text);
            }

            Console.WriteLine("Разрешение сохранено.");

        }

        static void Main(string[] args)
        {
            //var currentPath = AppDomain.CurrentDomain.BaseDirectory;
            //Console.WriteLine(currentPath);

            bool       IsCalculated;
            const uint sourceWidth    = 1366;
            const uint sourceHeight   = 768;
            string     path           = @"C:\Users\Yaros\source\projects\squareengine\squareengine\Assets\Images\";
            Resolution resolutionData = GetResolution(path);

            /**********************************************************************************/

            Console.WriteLine("-> Проеверка установленного разрешения монитора ...");
            
            /* Если разрешение не установлено */
            if (resolutionData.width == 0 && resolutionData.height == 0)
            {
                Console.WriteLine("-> Разрешение не установлено. Настройка...");

                Rectangle resolution = Screen.PrimaryScreen.Bounds;
                resolutionData.width = resolution.Width;
                resolutionData.height = resolution.Height;

                /* Если реальное разрешение соответсвтует исходному */
                if (resolution.Width != sourceWidth || resolution.Height != sourceHeight)
                {
                    Console.Write("-> Разрешение устройства: ");
                    Console.Write(resolution.Width);
                    Console.Write("x");
                    Console.Write(resolution.Height);
                    Console.WriteLine("");

                    Console.WriteLine("-> Расчёт коэффициентов...");
                    
                    resolutionData.koef_w = Math.Round((double)resolutionData.width  / (double)sourceWidth, 2);
                    resolutionData.koef_h = Math.Round((double)resolutionData.height / (double)sourceHeight, 2);

                    Console.Write("-> Коэффициент масштабирования ширины экрана: ");
                    Console.WriteLine(resolutionData.koef_w);
                    Console.Write("-> Коэффициент масштабирования высоты экрана: ");
                    Console.WriteLine(resolutionData.koef_h);
                    
                    SetResolution(path, resolutionData);
                    IsCalculated = false;
                }
                else
                {
                    Console.WriteLine("-> Настройка не требуется.\nЗапуск...");

                    SetResolution(path, resolutionData);
                    IsCalculated = true;
                }
            }
            else /* Если разрешение установлено */
            {
                // Реальное разрешение
                Rectangle resolution = Screen.PrimaryScreen.Bounds;

                /* Если установленное разрешение соотвтествует реальному */
                if (resolutionData.width == resolution.Width && resolutionData.height == resolution.Height)
                {
                    Console.WriteLine("-> Разрешение установлено.\nЗапуск...");
                    IsCalculated = true;
                }
                else /* Если установленное разрешение не соотвтествует реальному */
                {
                    Console.WriteLine("-> Настройки разрешения поменялись.");

                    Console.Write("-> Настроенное разрешение устройства: ");
                    Console.Write(resolutionData.width);
                    Console.Write("x");
                    Console.Write(resolutionData.height);
                    Console.WriteLine("");

                    Console.Write("-> Реальное разрешение устройства: ");
                    Console.Write(resolution.Width);
                    Console.Write("x");
                    Console.Write(resolution.Height);
                    Console.WriteLine("");

                    Console.WriteLine("-> Расчёт коэффициентов...");

                    resolutionData.koef_w = Math.Round((double)resolution.Width  / (double)resolutionData.width, 2);                                          
                    resolutionData.koef_h = Math.Round((double)resolution.Height / (double)resolutionData.height, 2);

                    Console.Write("-> Коэффициент масштабирования ширины экрана: ");
                    Console.WriteLine(resolutionData.koef_w);
                    Console.Write("-> Коэффициент масштабирования высоты экрана: ");
                    Console.WriteLine(resolutionData.koef_h);

                    resolutionData.width  = resolution.Width;
                    resolutionData.height = resolution.Height;

                    SetResolution(path, resolutionData);
                    IsCalculated = false;
                }
            }

            /**********************************************************************************/

            if (!IsCalculated)
            {
                Console.WriteLine("-> Масштабирование ассетов...");

                List<string> assets = new List<string>();

                Console.WriteLine("-> Получение списка ассетов...");

                DirectoryInfo dir = new DirectoryInfo(path);

                foreach (var item in dir.GetFiles())
                {
                    if (item.Extension != ".ppinfo")
                    {
                        assets.Add(item.FullName);
                        Console.Write("+ ");
                        Console.WriteLine(item.Name);
                    }
                }

                Console.WriteLine("-> Обработка...");

                /*
                Способ 1
                foreach (var asset in assets)
                {
                    Console.WriteLine("\n************Ассет************\n");

                    Image  img = Image.FromFile(asset);
                    Bitmap bitmap;
                    
                    if (img.Width == img.Height)
                    {
                        int nSize = (int)Math.Ceiling((img.Width * resolutionData.koef_w));
                        
                        Console.Write("* ");
                        Console.Write(img.Width);
                        Console.Write("x");
                        Console.Write(img.Height);

                        Console.Write(" -> ");
                        Console.Write(nSize);
                        Console.Write("x");
                        Console.Write(nSize);
                        Console.WriteLine("");

                        bitmap = ResizeImage(img, nSize, nSize);
                    }
                    else
                    {
                        int nWidth  = (int)Math.Ceiling((img.Width  * resolutionData.koef_w));
                        int nHeight = (int)Math.Ceiling((img.Height * resolutionData.koef_h));

                        Console.Write("* ");
                        Console.Write(img.Width);
                        Console.Write("x");
                        Console.Write(img.Height);

                        Console.Write(" -> ");
                        Console.Write(nWidth);
                        Console.Write("x");
                        Console.Write(nHeight);
                        Console.WriteLine("");

                        bitmap = ResizeImage(img, nWidth, nHeight);
                    }

                    img.Dispose();
                    File.Delete(asset);

                    Console.WriteLine("-> Сохранение ассета...");

                    bitmap.Save(asset, ImageFormat.Bmp);
                    bitmap.Dispose();

                    Console.WriteLine("-> Ассет сохранён и масштабирован.");

                    Console.WriteLine("\n*****************************");
                }
                */

                foreach (var asset in assets)
                {
                    Image img = Image.FromFile(asset);
                    Bitmap bitmap;

                    int nWidth;
                    int nHeight;

                    if (img.Width == img.Height)
                    {
                        nWidth = (int)Math.Ceiling((img.Width * resolutionData.koef_w));
                        nHeight = nWidth;
                    }
                    else
                    {
                        nWidth = (int)Math.Ceiling((img.Width * resolutionData.koef_w));
                        nHeight = (int)Math.Ceiling((img.Height * resolutionData.koef_h));
                    }

                    bitmap = (Bitmap)img.Scale(nWidth, nHeight);
                    
                    img.Dispose();
                    File.Delete(asset);

                    bitmap.SaveAs(asset, 100);
                }

                Console.WriteLine("Запуск...");
            }

            Console.ReadKey(); // Убрать
        }
    }
}
