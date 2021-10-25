using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace EngineLauncher
{
    public partial class Project2DForm : Form
    {
        private JsonSerializer serializer = new JsonSerializer();

        private struct ProjectSettings
        {
            public string name;
            public List<SortedDictionary<string, bool>> scenes;
        };

        private struct CameraControl
        {
            public bool nc_able;
            public float nc_speed;
            public float pos_x;
            public float pos_y;
        }

        private struct CameraSettings
        {
            public List<CameraControl> camera;
        };

        private struct HeroControl
        {
            public int a_fa;
            public int a_fh;
            public float a_ft;
            public int a_fw;
            public int a_pe;
            public int a_ps;
            public int camera_mode;
            public bool eff_a;
            public float eff_d;
            public float eff_t;
            public float hb_ltx;
            public float hb_lty;
            public float hb_rbx;
            public float hb_rby;
            public int layer;
            public string name;
            public string path;
            public float pos_x;
            public float pos_y;
            public float speed;
        };

        private struct HeroSettings
        {
            public List<HeroControl> mainperson;
        }

        private struct SceneSettings
        {
            public List<SortedDictionary<string, string>> Scene;
        };

        private struct ProjectSetupInfo
        {
            public List<SortedDictionary<string, string>> project;
        }

        private Thread thread;
        Form popup;

        public Project2DForm()
        {
            InitializeComponent();
        }

        private void createBtn_Click(object sender, EventArgs e)
        {
            if(projectName.Text == "")
            {
                projectName.Text = "Введите имя проекта !";

                return;
            }

            popup = new PopupForm("", "");
            popup.Show();

            mainTimer.Start();

            thread = new Thread(new ThreadStart(CreateProject));
            thread.Start();
        }

        private void CreateProject()
        {
            string path = @"C:\Users\Yaros\source\projects\squareengine\squareengine\Projects\" + projectName.Text;

            DirectoryInfo dirInfo = new DirectoryInfo(path);

            if (!dirInfo.Exists)
            {
                dirInfo.Create();
            }
            else
            {
                projectName.Text = "Имя проекта занято !";

                return;
            }

            dirInfo.CreateSubdirectory(@"Scenes");

            for (int i = 1; i <= Convert.ToInt32(scenesAmount.Text); i++)
            {
                dirInfo.CreateSubdirectory(@"Scenes\Scene " + i);
                dirInfo.CreateSubdirectory(@"Scenes\Scene " + i + @"\Objects");
            }

            /* Создание настроек проекта */

            ProjectSettings projectSettings = new ProjectSettings();
            projectSettings.name = projectName.Text;
            projectSettings.scenes = new List<SortedDictionary<string, bool>>();

            bool state = true;
            SortedDictionary<string, bool> scenesMap = new SortedDictionary<string, bool>();

            for (int i = 1; i <= Convert.ToInt32(scenesAmount.Text); i++)
            {
                scenesMap.Add("scene " + Convert.ToString(i), state);
                state = false;
            }

            projectSettings.scenes.Add(scenesMap);

            string pathToProjectSettings = path + @"\project_settings.json";

            using (StreamWriter sw = new StreamWriter(pathToProjectSettings))
            using (JsonWriter writer = new JsonTextWriter(sw))
            {
                serializer.Serialize(writer, projectSettings);
            }

            /*****************************/

            /* Создание настроек камеры */

            CameraSettings cameraSettings = new CameraSettings();
            cameraSettings.camera = new List<CameraControl>();

            CameraControl cam = new CameraControl();
            cam.nc_able = true;
            cam.nc_speed = 500.0f;
            cam.pos_x = 0.0f;
            cam.pos_y = 0.0f;

            cameraSettings.camera.Add(cam);

            string cameraSettingsStr = JsonConvert.SerializeObject(cameraSettings);
            cameraSettingsStr = cameraSettingsStr.Replace("_", "-");

            string pathToCameraSettings = path + @"\Scenes\camera.json";

            CreateAndSaveJson(pathToCameraSettings, cameraSettingsStr);

            /***************************/

            /* Создание настроек главного персонажа */

            HeroSettings heroSettings = new HeroSettings();
            heroSettings.mainperson = new List<HeroControl>();

            HeroControl h = new HeroControl();
            h.a_fa = 4;
            h.a_fh = 90;
            h.a_ft = 0.1720000058412552f;
            h.a_fw = 90;
            h.a_pe = 90;
            h.a_ps = 90;
            h.camera_mode = 1;
            h.eff_a = false;
            h.eff_d = 1.3070000410079956f;
            h.eff_t = 1.3170925378799438f;
            h.hb_ltx = 594.1419067382813f;
            h.hb_lty = 289.8402099609375f;
            h.hb_rbx = 628.1419067382813f;
            h.hb_rby = 355.8402099609375f;
            h.layer = 4;
            h.name = "mainperson";
            h.path = "Images\\link90x90.bmp";
            h.pos_x = 568.9295654296875f;
            h.pos_y = 279.4468688964844f;
            h.speed = 104.58000183105469f;

            heroSettings.mainperson.Add(h);

            string heroSettingsStr = JsonConvert.SerializeObject(heroSettings);
            heroSettingsStr = heroSettingsStr.Replace("_", "-");

            string pathToHeroSettings = path + @"\Scenes\main_person.json";

            CreateAndSaveJson(pathToHeroSettings, heroSettingsStr);

            /****************************************/

            /* Создание настроек сцен */

            for (int i = 1; i <= Convert.ToInt32(scenesAmount.Text); i++)
            {
                SceneSettings sceneSettings = new SceneSettings();
                sceneSettings.Scene = new List<SortedDictionary<string, string>>();

                SortedDictionary<string, string> SceneMap = new SortedDictionary<string, string>();
                SceneMap.Add("objectsPath", @"Projects\" + projectName.Text + @"\Scenes\Scene " + i + @"\Objects\objects_scene_" + i + ".json");
                SceneMap.Add("interactableObjectsPath", @"Projects\" + projectName.Text + @"\Scenes\Scene " + i + @"\Objects\interactable_objects_scene_" + i + ".json");
                SceneMap.Add("mainPersonPath", @"Projects\" + projectName.Text + @"\Scenes\main_person.json");
                SceneMap.Add("physicsPath", @"Projects\" + projectName.Text + @"\Scenes\Scene " + i + @"\physics.json");
                SceneMap.Add("cameraPath", @"Projects\" + projectName.Text + @"\Scenes\camera.json");

                sceneSettings.Scene.Add(SceneMap);

                string sceneSettingsStr = JsonConvert.SerializeObject(sceneSettings);
                sceneSettingsStr = sceneSettingsStr.Replace("\"Scene\":", "\"Scene " + i + "\":");

                string pathToSceneSettings = path + @"\Scenes\Scene " + i + @"\scene_" + i + ".json";

                CreateAndSaveJson(pathToSceneSettings, sceneSettingsStr);
            }

            /**************************/

            /* Создание копий настроек интерактивных объектов и персонажей */

            for (int i = 1; i <= Convert.ToInt32(scenesAmount.Text); i++)
            {
                string pathToIobjSettings = path + @"\Scenes\Scene " + i + @"\Objects\";

                string firstPath = @"C:\Users\Yaros\source\projects\squareengine\squareengine\Projects\Hello World\Scenes\Scene 1\Objects\";
                string secondPath = pathToIobjSettings;

                DirectoryInfo dir = new DirectoryInfo(firstPath);

                foreach (FileInfo file in dir.GetFiles())
                {
                    string result = file.Name.Replace("_1", "_" + i);
                    File.Copy(file.FullName, secondPath + result, true);
                }
            }

            /***************************************************************/

            /* Создание копий настроек физики */

            for (int i = 1; i <= Convert.ToInt32(scenesAmount.Text); i++)
            {
                string pathToIobjSettings = path + @"\Scenes\Scene " + i + @"\";

                string firstPath = @"C:\Users\Yaros\source\projects\squareengine\squareengine\Projects\Hello World\Scenes\Scene 1\";
                string secondPath = pathToIobjSettings;

                DirectoryInfo dir = new DirectoryInfo(firstPath);

                foreach (FileInfo file in dir.GetFiles())
                {
                    if (file.Name == "physics.json")
                    {
                        File.Copy(file.FullName, secondPath + @"\physics.json", true);
                    }
                }
            }

            /**********************************/

            /* Создание настроек проекта */

            ProjectSetupInfo setupInfo = new ProjectSetupInfo();
            setupInfo.project = new List<SortedDictionary<string, string>>();

            SortedDictionary<string, string> nameMap = new SortedDictionary<string, string>();
            nameMap.Add("name", projectName.Text);

            setupInfo.project.Add(nameMap);

            string setupSettingsStr = JsonConvert.SerializeObject(setupInfo);

            string pathToSetupSettings = @"C:\Users\Yaros\source\projects\squareengine\squareengine\current_project_setup.json";

            CreateAndSaveJson(pathToSetupSettings, setupSettingsStr);

            /*****************************/
        }

        private void projectName_MouseClick(object sender, MouseEventArgs e)
        {
            if (projectName.Text == "Введите имя проекта !")
            {
                projectName.Text = "";
            } 
            else if (projectName.Text == "Имя проекта занято !")
            {
                projectName.Text = "";
            }
        }

        private void CreateAndSaveJson(string path, string json)
        {
            using (StreamWriter sw = new StreamWriter(path))
            using (JsonWriter writer = new JsonTextWriter(sw))
            {
                serializer.Serialize(writer, JsonConvert.DeserializeObject(json));
            }
        }

        private void mainTimer_Tick(object sender, EventArgs e)
        {
            if(!thread.IsAlive)
            {
                popup.Close();
                mainTimer.Stop();
            }
        }
    }
}
