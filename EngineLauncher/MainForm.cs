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
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            introLbl.SelectionAlignment = HorizontalAlignment.Center;
            introTxt.SelectionAlignment = HorizontalAlignment.Left;
        }

        private void toDiscordBtn_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://discord.gg/5a62AWfUfp");
        }

        private void toVkBtn_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://vk.com/thethornpath");
        }
    }
}
