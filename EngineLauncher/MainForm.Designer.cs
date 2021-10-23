
namespace EngineLauncher
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.introLbl = new System.Windows.Forms.RichTextBox();
            this.introTxt = new System.Windows.Forms.RichTextBox();
            this.toVkBtn = new System.Windows.Forms.Button();
            this.toDiscordBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // introLbl
            // 
            this.introLbl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(129)))), ((int)(((byte)(6)))), ((int)(((byte)(169)))));
            this.introLbl.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.introLbl.Cursor = System.Windows.Forms.Cursors.Default;
            this.introLbl.Dock = System.Windows.Forms.DockStyle.Top;
            this.introLbl.Font = new System.Drawing.Font("Segoe UI Light", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.introLbl.ForeColor = System.Drawing.Color.Gainsboro;
            this.introLbl.Location = new System.Drawing.Point(0, 0);
            this.introLbl.Name = "introLbl";
            this.introLbl.ReadOnly = true;
            this.introLbl.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            this.introLbl.Size = new System.Drawing.Size(649, 36);
            this.introLbl.TabIndex = 3;
            this.introLbl.Text = "Добро пожаловать !";
            // 
            // introTxt
            // 
            this.introTxt.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(129)))), ((int)(((byte)(6)))), ((int)(((byte)(169)))));
            this.introTxt.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.introTxt.Cursor = System.Windows.Forms.Cursors.Default;
            this.introTxt.Dock = System.Windows.Forms.DockStyle.Top;
            this.introTxt.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.introTxt.ForeColor = System.Drawing.Color.Gainsboro;
            this.introTxt.Location = new System.Drawing.Point(0, 36);
            this.introTxt.Name = "introTxt";
            this.introTxt.ReadOnly = true;
            this.introTxt.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            this.introTxt.Size = new System.Drawing.Size(649, 321);
            this.introTxt.TabIndex = 5;
            this.introTxt.Text = "Приветсвтуем Вас в Square Engine - игровом 2-D движке ! Начните с создания проект" +
    "а по шаблону или создайте пустой, следуя инструкциям.\n\nВерсия Pre-Alpha 1.0.";
            // 
            // toVkBtn
            // 
            this.toVkBtn.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.toVkBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.toVkBtn.FlatAppearance.BorderSize = 0;
            this.toVkBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.toVkBtn.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toVkBtn.ForeColor = System.Drawing.Color.Gainsboro;
            this.toVkBtn.Image = global::EngineLauncher.Properties.Resources.vk;
            this.toVkBtn.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.toVkBtn.Location = new System.Drawing.Point(94, 363);
            this.toVkBtn.Name = "toVkBtn";
            this.toVkBtn.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.toVkBtn.Size = new System.Drawing.Size(83, 68);
            this.toVkBtn.TabIndex = 7;
            this.toVkBtn.UseVisualStyleBackColor = true;
            this.toVkBtn.Click += new System.EventHandler(this.toVkBtn_Click);
            // 
            // toDiscordBtn
            // 
            this.toDiscordBtn.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.toDiscordBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.toDiscordBtn.FlatAppearance.BorderSize = 0;
            this.toDiscordBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.toDiscordBtn.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.toDiscordBtn.ForeColor = System.Drawing.Color.Gainsboro;
            this.toDiscordBtn.Image = global::EngineLauncher.Properties.Resources.discord;
            this.toDiscordBtn.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.toDiscordBtn.Location = new System.Drawing.Point(5, 363);
            this.toDiscordBtn.Name = "toDiscordBtn";
            this.toDiscordBtn.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.toDiscordBtn.Size = new System.Drawing.Size(83, 68);
            this.toDiscordBtn.TabIndex = 6;
            this.toDiscordBtn.UseVisualStyleBackColor = true;
            this.toDiscordBtn.Click += new System.EventHandler(this.toDiscordBtn_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(129)))), ((int)(((byte)(6)))), ((int)(((byte)(169)))));
            this.ClientSize = new System.Drawing.Size(649, 443);
            this.Controls.Add(this.toVkBtn);
            this.Controls.Add(this.toDiscordBtn);
            this.Controls.Add(this.introTxt);
            this.Controls.Add(this.introLbl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "MainForm";
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.RichTextBox introLbl;
        private System.Windows.Forms.RichTextBox introTxt;
        private System.Windows.Forms.Button toDiscordBtn;
        private System.Windows.Forms.Button toVkBtn;
    }
}