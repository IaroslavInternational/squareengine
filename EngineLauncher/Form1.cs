using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace EngineLauncher
{
    public partial class Launcher : Form
    {
        private Button currentButton;
        private Form currentForm;
        private Random random;
        private int tempIndex;

        public Launcher()
        {
            InitializeComponent();

            random = new Random();
        }

        private Color SelectThemeColor()
        {
            int index = random.Next(ThemeColor.ColotList.Count);

            while (tempIndex == index)
            {
                index = random.Next(ThemeColor.ColotList.Count);
            }

            tempIndex = index;

            string color = ThemeColor.ColotList[index];

            return ColorTranslator.FromHtml(color);
        }

        private void ActivateButton(object sender)
        {
            if (sender != null)
            {
                if (currentButton != (Button)sender)
                {
                    DisableButton();

                    Color color = SelectThemeColor();

                    currentButton = (Button)sender;
                    currentButton.BackColor = color;
                    currentButton.ForeColor = Color.White;
                    currentButton.Font = new System.Drawing.Font("Segoe UI Light", 16.0F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
                    panelNav.BackColor = color;
                    lblTitle.Text = currentButton.Text;
                }
            }
        }

        private void DisableButton()
        {
            foreach (Control previousBtn in panelMenu.Controls)
            {
                if (previousBtn.GetType() == typeof(Button))
                {
                    previousBtn.BackColor = Color.FromArgb(84, 2, 110);
                    previousBtn.ForeColor = Color.Gainsboro;
                    previousBtn.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
                }
            }
        }

        private void OpenChildForm(Form form, object sender)
        {
            if(currentForm != null)
            {
                currentForm.Close();
            }

            ActivateButton(sender);
           
            currentForm = form;
            form.TopLevel = false;
            form.FormBorderStyle = FormBorderStyle.None;
            form.Dock = DockStyle.Fill;

            this.panelDesktop.Controls.Add(form);
            this.panelDesktop.Tag = form;

            form.BringToFront();
            form.Show(); 
        }

        private void btnMain_Click(object sender, EventArgs e)
        {
            ActivateButton(sender);
        }
    }
}
