
namespace EngineLauncher
{
    partial class Project2DForm
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
            this.projectName = new System.Windows.Forms.TextBox();
            this.lblTitle = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.scenesAmount = new System.Windows.Forms.TextBox();
            this.cbIncludeStartPackage = new System.Windows.Forms.CheckBox();
            this.createBtn = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ProgressBar();
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
            this.introLbl.TabIndex = 4;
            this.introLbl.Text = "Создание проекта > 2D";
            // 
            // projectName
            // 
            this.projectName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(20)))), ((int)(((byte)(135)))));
            this.projectName.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.projectName.ForeColor = System.Drawing.Color.Gainsboro;
            this.projectName.Location = new System.Drawing.Point(17, 77);
            this.projectName.Name = "projectName";
            this.projectName.Size = new System.Drawing.Size(270, 26);
            this.projectName.TabIndex = 5;
            this.projectName.MouseClick += new System.Windows.Forms.MouseEventHandler(this.projectName_MouseClick);
            // 
            // lblTitle
            // 
            this.lblTitle.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.lblTitle.AutoSize = true;
            this.lblTitle.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.lblTitle.ForeColor = System.Drawing.Color.Gainsboro;
            this.lblTitle.Location = new System.Drawing.Point(12, 49);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(129, 25);
            this.lblTitle.TabIndex = 6;
            this.lblTitle.Text = "Имя проекта*:";
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.ForeColor = System.Drawing.Color.Gainsboro;
            this.label1.Location = new System.Drawing.Point(12, 106);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(119, 25);
            this.label1.TabIndex = 8;
            this.label1.Text = "Кол-во сцен:";
            // 
            // scenesAmount
            // 
            this.scenesAmount.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(20)))), ((int)(((byte)(135)))));
            this.scenesAmount.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.scenesAmount.ForeColor = System.Drawing.Color.Gainsboro;
            this.scenesAmount.Location = new System.Drawing.Point(17, 134);
            this.scenesAmount.Name = "scenesAmount";
            this.scenesAmount.Size = new System.Drawing.Size(270, 26);
            this.scenesAmount.TabIndex = 7;
            // 
            // cbIncludeStartPackage
            // 
            this.cbIncludeStartPackage.AutoSize = true;
            this.cbIncludeStartPackage.Checked = true;
            this.cbIncludeStartPackage.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbIncludeStartPackage.Cursor = System.Windows.Forms.Cursors.Hand;
            this.cbIncludeStartPackage.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.cbIncludeStartPackage.Location = new System.Drawing.Point(17, 166);
            this.cbIncludeStartPackage.Name = "cbIncludeStartPackage";
            this.cbIncludeStartPackage.Size = new System.Drawing.Size(251, 29);
            this.cbIncludeStartPackage.TabIndex = 9;
            this.cbIncludeStartPackage.Text = "Включить стартовый набор";
            this.cbIncludeStartPackage.UseVisualStyleBackColor = true;
            // 
            // createBtn
            // 
            this.createBtn.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(0)))), ((int)(((byte)(98)))));
            this.createBtn.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.createBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.createBtn.FlatAppearance.BorderSize = 0;
            this.createBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.createBtn.Font = new System.Drawing.Font("Segoe UI Light", 15.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.createBtn.ForeColor = System.Drawing.Color.Gainsboro;
            this.createBtn.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.createBtn.Location = new System.Drawing.Point(362, 371);
            this.createBtn.Name = "createBtn";
            this.createBtn.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.createBtn.Size = new System.Drawing.Size(275, 60);
            this.createBtn.TabIndex = 10;
            this.createBtn.Text = "Создать";
            this.createBtn.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.createBtn.UseVisualStyleBackColor = false;
            this.createBtn.Click += new System.EventHandler(this.createBtn_Click);
            // 
            // progressBar
            // 
            this.progressBar.ForeColor = System.Drawing.SystemColors.Desktop;
            this.progressBar.Location = new System.Drawing.Point(17, 408);
            this.progressBar.MarqueeAnimationSpeed = 50;
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(339, 23);
            this.progressBar.TabIndex = 11;
            this.progressBar.Visible = false;
            // 
            // Project2DForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(129)))), ((int)(((byte)(6)))), ((int)(((byte)(169)))));
            this.ClientSize = new System.Drawing.Size(649, 443);
            this.Controls.Add(this.progressBar);
            this.Controls.Add(this.createBtn);
            this.Controls.Add(this.cbIncludeStartPackage);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.scenesAmount);
            this.Controls.Add(this.lblTitle);
            this.Controls.Add(this.projectName);
            this.Controls.Add(this.introLbl);
            this.Font = new System.Drawing.Font("Segoe UI Light", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.ForeColor = System.Drawing.Color.Gainsboro;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(5, 6, 5, 6);
            this.Name = "Project2DForm";
            this.Text = "Project2DForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.RichTextBox introLbl;
        private System.Windows.Forms.TextBox projectName;
        private System.Windows.Forms.Label lblTitle;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox scenesAmount;
        private System.Windows.Forms.CheckBox cbIncludeStartPackage;
        private System.Windows.Forms.Button createBtn;
        private System.Windows.Forms.ProgressBar progressBar;
    }
}