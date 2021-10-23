
namespace EngineLauncher
{
    partial class AddProject
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
            this.panelDesktop = new System.Windows.Forms.Panel();
            this.btn2D = new System.Windows.Forms.Button();
            this.btn3D = new System.Windows.Forms.Button();
            this.panelDesktop.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelDesktop
            // 
            this.panelDesktop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(129)))), ((int)(((byte)(6)))), ((int)(((byte)(169)))));
            this.panelDesktop.Controls.Add(this.btn3D);
            this.panelDesktop.Controls.Add(this.btn2D);
            this.panelDesktop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelDesktop.Location = new System.Drawing.Point(0, 0);
            this.panelDesktop.Name = "panelDesktop";
            this.panelDesktop.Size = new System.Drawing.Size(649, 443);
            this.panelDesktop.TabIndex = 4;
            // 
            // btn2D
            // 
            this.btn2D.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(0)))), ((int)(((byte)(98)))));
            this.btn2D.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.btn2D.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btn2D.Dock = System.Windows.Forms.DockStyle.Left;
            this.btn2D.FlatAppearance.BorderSize = 0;
            this.btn2D.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn2D.Font = new System.Drawing.Font("Bauhaus 93", 96F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn2D.ForeColor = System.Drawing.Color.Gainsboro;
            this.btn2D.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btn2D.Location = new System.Drawing.Point(0, 0);
            this.btn2D.Name = "btn2D";
            this.btn2D.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.btn2D.Size = new System.Drawing.Size(324, 443);
            this.btn2D.TabIndex = 5;
            this.btn2D.Text = "2D";
            this.btn2D.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btn2D.UseVisualStyleBackColor = false;
            this.btn2D.Click += new System.EventHandler(this.btn2D_Click);
            // 
            // btn3D
            // 
            this.btn3D.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(0)))), ((int)(((byte)(80)))));
            this.btn3D.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.btn3D.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btn3D.Dock = System.Windows.Forms.DockStyle.Right;
            this.btn3D.FlatAppearance.BorderSize = 0;
            this.btn3D.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btn3D.Font = new System.Drawing.Font("Bauhaus 93", 96F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn3D.ForeColor = System.Drawing.Color.Gainsboro;
            this.btn3D.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btn3D.Location = new System.Drawing.Point(325, 0);
            this.btn3D.Name = "btn3D";
            this.btn3D.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.btn3D.Size = new System.Drawing.Size(324, 443);
            this.btn3D.TabIndex = 6;
            this.btn3D.Text = "3D";
            this.btn3D.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btn3D.UseVisualStyleBackColor = false;
            // 
            // AddProject
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(129)))), ((int)(((byte)(6)))), ((int)(((byte)(169)))));
            this.ClientSize = new System.Drawing.Size(649, 443);
            this.Controls.Add(this.panelDesktop);
            this.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ForeColor = System.Drawing.Color.Gainsboro;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
            this.Name = "AddProject";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "AddProject";
            this.panelDesktop.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelDesktop;
        private System.Windows.Forms.Button btn2D;
        private System.Windows.Forms.Button btn3D;
    }
}