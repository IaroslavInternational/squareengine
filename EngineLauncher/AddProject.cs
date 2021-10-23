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
    public partial class AddProject : Form
    {
        public AddProject()
        {
            InitializeComponent();
        }

        private void btn2D_Click(object sender, EventArgs e)
        {
            this.panelDesktop.Controls.Remove(btn2D);
            this.panelDesktop.Controls.Remove(btn3D);

            Form form = new Project2DForm();

            form.TopLevel = false;
            form.FormBorderStyle = FormBorderStyle.None;
            form.Dock = DockStyle.Fill;

            this.panelDesktop.Controls.Add(form);
            this.panelDesktop.Tag = form;

            form.BringToFront();
            form.Show();
        }
    }
}
