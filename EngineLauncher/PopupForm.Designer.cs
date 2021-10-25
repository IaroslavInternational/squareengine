
namespace EngineLauncher
{
    partial class PopupForm
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
            this.headerLbl = new System.Windows.Forms.RichTextBox();
            this.messageLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // headerLbl
            // 
            this.headerLbl.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(90)))));
            this.headerLbl.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.headerLbl.Cursor = System.Windows.Forms.Cursors.Default;
            this.headerLbl.Dock = System.Windows.Forms.DockStyle.Top;
            this.headerLbl.Font = new System.Drawing.Font("Segoe UI Light", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.headerLbl.ForeColor = System.Drawing.Color.Gainsboro;
            this.headerLbl.Location = new System.Drawing.Point(0, 0);
            this.headerLbl.Name = "headerLbl";
            this.headerLbl.ReadOnly = true;
            this.headerLbl.ScrollBars = System.Windows.Forms.RichTextBoxScrollBars.None;
            this.headerLbl.Size = new System.Drawing.Size(500, 36);
            this.headerLbl.TabIndex = 5;
            this.headerLbl.Text = "  Создание проекта...";
            // 
            // messageLbl
            // 
            this.messageLbl.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.messageLbl.AutoSize = true;
            this.messageLbl.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.messageLbl.ForeColor = System.Drawing.Color.Gainsboro;
            this.messageLbl.Location = new System.Drawing.Point(9, 39);
            this.messageLbl.Name = "messageLbl";
            this.messageLbl.Size = new System.Drawing.Size(413, 25);
            this.messageLbl.TabIndex = 7;
            this.messageLbl.Text = "Идёт создание проекта, пожалуйста, подождите.";
            // 
            // PopupForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(50)))), ((int)(((byte)(169)))));
            this.ClientSize = new System.Drawing.Size(500, 155);
            this.Controls.Add(this.messageLbl);
            this.Controls.Add(this.headerLbl);
            this.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ForeColor = System.Drawing.Color.Gainsboro;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
            this.Name = "PopupForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "PopupForm";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox headerLbl;
        private System.Windows.Forms.Label messageLbl;
    }
}