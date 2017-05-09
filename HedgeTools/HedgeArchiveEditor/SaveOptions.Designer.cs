﻿namespace HedgeArchiveEditor
{
    partial class SaveOptions
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
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.ComboBox1 = new System.Windows.Forms.ComboBox();
            this.NumericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.CheckBox1 = new System.Windows.Forms.CheckBox();
            this.okBtn = new System.Windows.Forms.Button();
            this.cancelBtn = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.NumericUpDown2 = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.CheckBox2 = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.NumericUpDown1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NumericUpDown2)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.label1.Location = new System.Drawing.Point(12, 8);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(116, 25);
            this.label1.TabIndex = 0;
            this.label1.Text = "Archive Type:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(12, 42);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(81, 25);
            this.label2.TabIndex = 1;
            this.label2.Text = "Padding:";
            // 
            // ComboBox1
            // 
            this.ComboBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.ComboBox1.FormattingEnabled = true;
            this.ComboBox1.Items.AddRange(new object[] {
            "Generations/Unleashed",
            "Lost World",
            "Story Books",
            "Heroes/Shadow"});
            this.ComboBox1.Location = new System.Drawing.Point(146, 9);
            this.ComboBox1.Name = "ComboBox1";
            this.ComboBox1.Size = new System.Drawing.Size(220, 28);
            this.ComboBox1.TabIndex = 3;
            this.ComboBox1.SelectedIndexChanged += new System.EventHandler(this.ComboBox1_SelectedIndexChanged);
            // 
            // NumericUpDown1
            // 
            this.NumericUpDown1.Hexadecimal = true;
            this.NumericUpDown1.Location = new System.Drawing.Point(146, 43);
            this.NumericUpDown1.Maximum = 0xFFFFFFFF;
            this.NumericUpDown1.Name = "NumericUpDown1";
            this.NumericUpDown1.Size = new System.Drawing.Size(220, 26);
            this.NumericUpDown1.TabIndex = 4;
            this.NumericUpDown1.Value = new decimal(new int[] {
            64,
            0,
            0,
            0});
            // 
            // CheckBox1
            // 
            this.CheckBox1.AutoSize = true;
            this.CheckBox1.Checked = true;
            this.CheckBox1.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckBox1.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.CheckBox1.Location = new System.Drawing.Point(344, 77);
            this.CheckBox1.Name = "CheckBox1";
            this.CheckBox1.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.CheckBox1.Size = new System.Drawing.Size(22, 21);
            this.CheckBox1.TabIndex = 5;
            this.CheckBox1.UseVisualStyleBackColor = true;
            // 
            // okBtn
            // 
            this.okBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.okBtn.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.okBtn.Location = new System.Drawing.Point(160, 185);
            this.okBtn.Name = "okBtn";
            this.okBtn.Size = new System.Drawing.Size(100, 29);
            this.okBtn.TabIndex = 6;
            this.okBtn.Text = "OK";
            this.okBtn.UseVisualStyleBackColor = true;
            this.okBtn.Click += new System.EventHandler(this.OkBtn_Click);
            // 
            // cancelBtn
            // 
            this.cancelBtn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelBtn.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.cancelBtn.Location = new System.Drawing.Point(266, 185);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(100, 29);
            this.cancelBtn.TabIndex = 7;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.UseVisualStyleBackColor = true;
            this.cancelBtn.Click += new System.EventHandler(this.CancelBtn_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.label3.Location = new System.Drawing.Point(12, 74);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(150, 25);
            this.label3.TabIndex = 8;
            this.label3.Text = "Generate ARL file:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.label4.Location = new System.Drawing.Point(12, 104);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(196, 25);
            this.label4.TabIndex = 10;
            this.label4.Text = "Split Into Multiple Files:";
            // 
            // NumericUpDown2
            // 
            this.NumericUpDown2.Hexadecimal = true;
            this.NumericUpDown2.Location = new System.Drawing.Point(246, 134);
            this.NumericUpDown2.Maximum = new decimal(new int[] {
            -1,
            0,
            0,
            0});
            this.NumericUpDown2.Name = "NumericUpDown2";
            this.NumericUpDown2.Size = new System.Drawing.Size(120, 26);
            this.NumericUpDown2.TabIndex = 11;
            this.NumericUpDown2.Value = new decimal(new int[] {
            10485760,
            0,
            0,
            0});
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.label5.Location = new System.Drawing.Point(12, 134);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(87, 25);
            this.label5.TabIndex = 12;
            this.label5.Text = "Split Size:";
            // 
            // CheckBox2
            // 
            this.CheckBox2.AutoSize = true;
            this.CheckBox2.Checked = true;
            this.CheckBox2.CheckState = System.Windows.Forms.CheckState.Checked;
            this.CheckBox2.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.CheckBox2.Location = new System.Drawing.Point(344, 107);
            this.CheckBox2.Name = "CheckBox2";
            this.CheckBox2.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.CheckBox2.Size = new System.Drawing.Size(22, 21);
            this.CheckBox2.TabIndex = 13;
            this.CheckBox2.UseVisualStyleBackColor = true;
            this.CheckBox2.CheckedChanged += new System.EventHandler(this.CheckBox2_CheckedChanged);
            // 
            // SaveOptions
            // 
            this.AcceptButton = this.okBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.CancelButton = this.cancelBtn;
            this.ClientSize = new System.Drawing.Size(378, 228);
            this.Controls.Add(this.CheckBox2);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.NumericUpDown2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.okBtn);
            this.Controls.Add(this.CheckBox1);
            this.Controls.Add(this.NumericUpDown1);
            this.Controls.Add(this.ComboBox1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SaveOptions";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Save Options";
            ((System.ComponentModel.ISupportInitialize)(this.NumericUpDown1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NumericUpDown2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button okBtn;
        private System.Windows.Forms.Button cancelBtn;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        public System.Windows.Forms.ComboBox ComboBox1;
        public System.Windows.Forms.NumericUpDown NumericUpDown1;
        public System.Windows.Forms.CheckBox CheckBox1;
        public System.Windows.Forms.NumericUpDown NumericUpDown2;
        private System.Windows.Forms.Label label5;
        public System.Windows.Forms.CheckBox CheckBox2;
    }
}