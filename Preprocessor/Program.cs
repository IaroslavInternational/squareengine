using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;

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

        private static Bitmap ResizeImage(Image image, int width, int height)
        {
            var destRect = new Rectangle(0, 0, width, height);
            var destImage = new Bitmap(width, height);

            destImage.SetResolution(image.HorizontalResolution, image.VerticalResolution);

            using (var graphics = Graphics.FromImage(destImage))
            {
                graphics.CompositingMode = CompositingMode.SourceCopy;
                graphics.CompositingQuality = CompositingQuality.AssumeLinear;
                graphics.InterpolationMode = InterpolationMode.NearestNeighbor;
                graphics.SmoothingMode = SmoothingMode.HighQuality;
                graphics.PixelOffsetMode = PixelOffsetMode.None;

                using (var wrapMode = new ImageAttributes())
                {
                    wrapMode.SetWrapMode(WrapMode.TileFlipXY);
                    graphics.DrawImage(image, destRect, 0, 0, image.Width, image.Height, GraphicsUnit.Pixel, wrapMode);
                }
            }

            return destImage;
        }
        
        private static Resolution GetResolution(string path)
        {
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

            return resolutionData;
        }

        private static void SetResolution(string path, Resolution resolution)
        {
            string text = "d_width:" + resolution.width + ";\r\nd_height:" + resolution.height;

            using (StreamWriter sw = new StreamWriter($"{path}\\settings.ppinfo", false, System.Text.Encoding.Default))
            {
                sw.WriteLine(text);
            }
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

            /* Если разрешение не установлено */
            if (resolutionData.width == 0 && resolutionData.height == 0)
            {
                Rectangle resolution = Screen.PrimaryScreen.Bounds;
                resolutionData.width = resolution.Width;
                resolutionData.height = resolution.Height;

                /* Если реальное разрешение соответсвтует исходному */
                if (resolution.Width != sourceWidth && resolution.Height != sourceHeight)
                {
                    resolutionData.koef_w = (double)resolutionData.width  / (double)sourceWidth;
                    resolutionData.koef_h = (double)resolutionData.height / (double)sourceHeight;

                    SetResolution(path, resolutionData);
                    IsCalculated = false;
                }
                else
                {
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
                    IsCalculated = true;
                }
                else /* Если установленное разрешение не соотвтествует реальному */
                {
                    resolutionData.koef_w = (double)resolution.Width  / (double)resolutionData.width;                                          
                    resolutionData.koef_h = (double)resolution.Height / (double)resolutionData.height;

                    resolutionData.width  = resolution.Width;
                    resolutionData.height = resolution.Height;

                    SetResolution(path, resolutionData);
                    IsCalculated = false;
                }
            }

            /**********************************************************************************/

            if (!IsCalculated)
            {
                List<string> assets = new List<string>();

                DirectoryInfo dir = new DirectoryInfo(path);

                foreach (var item in dir.GetFiles())
                {
                    if (item.Extension != ".ppinfo")
                    {
                        assets.Add(item.FullName);
                        Console.WriteLine(item.Name);
                    }
                }

                foreach (var asset in assets)
                {
                    Image img = Image.FromFile(asset);
                    Bitmap bitmap;
                    
                    if (img.Width == img.Height)
                    {
                        int nSize = (int)Math.Ceiling((img.Width * resolutionData.koef_w));

                        bitmap = ResizeImage(img, nSize, nSize);
                    }
                    else
                    {
                        int nWidth  = (int)Math.Ceiling((img.Width  * resolutionData.koef_w));
                        int nHeight = (int)Math.Ceiling((img.Height * resolutionData.koef_h));

                        bitmap = ResizeImage(img, nWidth, nHeight);
                    }

                    img.Dispose();
                    File.Delete(asset);

                    bitmap.Save(asset, ImageFormat.Bmp);
                    bitmap.Dispose();
                }

                Console.ReadKey();
            }
        }
    }
}
