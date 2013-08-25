namespace rgbled
{
    partial class Form1
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
            this.components = new System.ComponentModel.Container();
            this.tbRed = new System.Windows.Forms.TrackBar();
            this.tbGreen = new System.Windows.Forms.TrackBar();
            this.tbBlue = new System.Windows.Forms.TrackBar();
            this.pbColour = new System.Windows.Forms.PictureBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cbGamma = new System.Windows.Forms.CheckBox();
            this.cbCapScreen = new System.Windows.Forms.CheckBox();
            this.nudBlue = new System.Windows.Forms.NumericUpDown();
            this.nudGreen = new System.Windows.Forms.NumericUpDown();
            this.nudRed = new System.Windows.Forms.NumericUpDown();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lvEEPROM = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.btnEEPROMWrite = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.btnEEPROMRead = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.nudEEPROMLocation = new System.Windows.Forms.NumericUpDown();
            this.nudEEPROMValue = new System.Windows.Forms.NumericUpDown();
            this.txtLog = new System.Windows.Forms.TextBox();
            this.btnOpen = new System.Windows.Forms.Button();
            this.btnClose = new System.Windows.Forms.Button();
            this.btnInfo = new System.Windows.Forms.Button();
            this.btnReset = new System.Windows.Forms.Button();
            this.nudIdleTime = new System.Windows.Forms.NumericUpDown();
            this.btnIdleTime = new System.Windows.Forms.Button();
            this.pbUsbOk = new System.Windows.Forms.PictureBox();
            this.tmrPoke = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.tbRed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbBlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbColour)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudBlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudGreen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudRed)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudEEPROMLocation)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudEEPROMValue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudIdleTime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbUsbOk)).BeginInit();
            this.SuspendLayout();
            // 
            // tbRed
            // 
            this.tbRed.Location = new System.Drawing.Point(6, 19);
            this.tbRed.Name = "tbRed";
            this.tbRed.Size = new System.Drawing.Size(264, 40);
            this.tbRed.TabIndex = 0;
            this.tbRed.Scroll += new System.EventHandler(this.tb_Scroll);
            // 
            // tbGreen
            // 
            this.tbGreen.Location = new System.Drawing.Point(6, 65);
            this.tbGreen.Name = "tbGreen";
            this.tbGreen.Size = new System.Drawing.Size(264, 40);
            this.tbGreen.TabIndex = 1;
            this.tbGreen.Scroll += new System.EventHandler(this.tb_Scroll);
            // 
            // tbBlue
            // 
            this.tbBlue.Location = new System.Drawing.Point(6, 111);
            this.tbBlue.Name = "tbBlue";
            this.tbBlue.Size = new System.Drawing.Size(264, 40);
            this.tbBlue.TabIndex = 2;
            this.tbBlue.Scroll += new System.EventHandler(this.tb_Scroll);
            // 
            // pbColour
            // 
            this.pbColour.Location = new System.Drawing.Point(357, 19);
            this.pbColour.Name = "pbColour";
            this.pbColour.Size = new System.Drawing.Size(44, 132);
            this.pbColour.TabIndex = 3;
            this.pbColour.TabStop = false;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.cbGamma);
            this.groupBox1.Controls.Add(this.cbCapScreen);
            this.groupBox1.Controls.Add(this.nudBlue);
            this.groupBox1.Controls.Add(this.pbColour);
            this.groupBox1.Controls.Add(this.nudGreen);
            this.groupBox1.Controls.Add(this.nudRed);
            this.groupBox1.Controls.Add(this.tbRed);
            this.groupBox1.Controls.Add(this.tbGreen);
            this.groupBox1.Controls.Add(this.tbBlue);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(407, 165);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Colour";
            // 
            // cbGamma
            // 
            this.cbGamma.AutoSize = true;
            this.cbGamma.Checked = true;
            this.cbGamma.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbGamma.Location = new System.Drawing.Point(239, 142);
            this.cbGamma.Name = "cbGamma";
            this.cbGamma.Size = new System.Drawing.Size(112, 17);
            this.cbGamma.TabIndex = 15;
            this.cbGamma.Text = "Gamma correction";
            this.cbGamma.UseVisualStyleBackColor = true;
            // 
            // cbCapScreen
            // 
            this.cbCapScreen.AutoSize = true;
            this.cbCapScreen.Location = new System.Drawing.Point(6, 142);
            this.cbCapScreen.Name = "cbCapScreen";
            this.cbCapScreen.Size = new System.Drawing.Size(227, 17);
            this.cbCapScreen.TabIndex = 14;
            this.cbCapScreen.Text = "Use average screen colour (might be slow)";
            this.cbCapScreen.UseVisualStyleBackColor = true;
            // 
            // nudBlue
            // 
            this.nudBlue.Location = new System.Drawing.Point(276, 111);
            this.nudBlue.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudBlue.Name = "nudBlue";
            this.nudBlue.Size = new System.Drawing.Size(74, 20);
            this.nudBlue.TabIndex = 5;
            this.nudBlue.ValueChanged += new System.EventHandler(this.nud_ValueChanged);
            // 
            // nudGreen
            // 
            this.nudGreen.Location = new System.Drawing.Point(276, 65);
            this.nudGreen.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudGreen.Name = "nudGreen";
            this.nudGreen.Size = new System.Drawing.Size(74, 20);
            this.nudGreen.TabIndex = 5;
            this.nudGreen.ValueChanged += new System.EventHandler(this.nud_ValueChanged);
            // 
            // nudRed
            // 
            this.nudRed.Location = new System.Drawing.Point(277, 20);
            this.nudRed.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudRed.Name = "nudRed";
            this.nudRed.Size = new System.Drawing.Size(74, 20);
            this.nudRed.TabIndex = 3;
            this.nudRed.ValueChanged += new System.EventHandler(this.nud_ValueChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.lvEEPROM);
            this.groupBox2.Controls.Add(this.btnEEPROMWrite);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Controls.Add(this.btnEEPROMRead);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.nudEEPROMLocation);
            this.groupBox2.Controls.Add(this.nudEEPROMValue);
            this.groupBox2.Location = new System.Drawing.Point(425, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(200, 460);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "EEPROM";
            // 
            // lvEEPROM
            // 
            this.lvEEPROM.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.lvEEPROM.FullRowSelect = true;
            this.lvEEPROM.GridLines = true;
            this.lvEEPROM.Location = new System.Drawing.Point(10, 46);
            this.lvEEPROM.MultiSelect = false;
            this.lvEEPROM.Name = "lvEEPROM";
            this.lvEEPROM.Size = new System.Drawing.Size(181, 336);
            this.lvEEPROM.TabIndex = 8;
            this.lvEEPROM.UseCompatibleStateImageBehavior = false;
            this.lvEEPROM.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Location";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Value";
            // 
            // btnEEPROMWrite
            // 
            this.btnEEPROMWrite.Location = new System.Drawing.Point(10, 427);
            this.btnEEPROMWrite.Name = "btnEEPROMWrite";
            this.btnEEPROMWrite.Size = new System.Drawing.Size(181, 23);
            this.btnEEPROMWrite.TabIndex = 7;
            this.btnEEPROMWrite.Text = "Write";
            this.btnEEPROMWrite.UseVisualStyleBackColor = true;
            this.btnEEPROMWrite.Click += new System.EventHandler(this.btnEEPROMWrite_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(101, 385);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Value";
            // 
            // btnEEPROMRead
            // 
            this.btnEEPROMRead.Location = new System.Drawing.Point(10, 17);
            this.btnEEPROMRead.Name = "btnEEPROMRead";
            this.btnEEPROMRead.Size = new System.Drawing.Size(181, 23);
            this.btnEEPROMRead.TabIndex = 6;
            this.btnEEPROMRead.Text = "Read";
            this.btnEEPROMRead.UseVisualStyleBackColor = true;
            this.btnEEPROMRead.Click += new System.EventHandler(this.btnEEPROMRead_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 385);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Location";
            // 
            // nudEEPROMLocation
            // 
            this.nudEEPROMLocation.Location = new System.Drawing.Point(10, 401);
            this.nudEEPROMLocation.Name = "nudEEPROMLocation";
            this.nudEEPROMLocation.Size = new System.Drawing.Size(80, 20);
            this.nudEEPROMLocation.TabIndex = 1;
            // 
            // nudEEPROMValue
            // 
            this.nudEEPROMValue.Location = new System.Drawing.Point(104, 401);
            this.nudEEPROMValue.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudEEPROMValue.Name = "nudEEPROMValue";
            this.nudEEPROMValue.Size = new System.Drawing.Size(87, 20);
            this.nudEEPROMValue.TabIndex = 3;
            // 
            // txtLog
            // 
            this.txtLog.Location = new System.Drawing.Point(12, 183);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.Size = new System.Drawing.Size(407, 227);
            this.txtLog.TabIndex = 6;
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(12, 420);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(75, 23);
            this.btnOpen.TabIndex = 7;
            this.btnOpen.Text = "Open";
            this.btnOpen.UseVisualStyleBackColor = true;
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(264, 420);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 8;
            this.btnClose.Text = "Close";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnInfo
            // 
            this.btnInfo.Location = new System.Drawing.Point(93, 420);
            this.btnInfo.Name = "btnInfo";
            this.btnInfo.Size = new System.Drawing.Size(75, 23);
            this.btnInfo.TabIndex = 9;
            this.btnInfo.Text = "Get info";
            this.btnInfo.UseVisualStyleBackColor = true;
            this.btnInfo.Click += new System.EventHandler(this.btnInfo_Click);
            // 
            // btnReset
            // 
            this.btnReset.Location = new System.Drawing.Point(174, 420);
            this.btnReset.Name = "btnReset";
            this.btnReset.Size = new System.Drawing.Size(84, 23);
            this.btnReset.TabIndex = 10;
            this.btnReset.Text = "Reset device";
            this.btnReset.UseVisualStyleBackColor = true;
            this.btnReset.Click += new System.EventHandler(this.btnReset_Click);
            // 
            // nudIdleTime
            // 
            this.nudIdleTime.Location = new System.Drawing.Point(12, 452);
            this.nudIdleTime.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.nudIdleTime.Name = "nudIdleTime";
            this.nudIdleTime.Size = new System.Drawing.Size(75, 20);
            this.nudIdleTime.TabIndex = 11;
            // 
            // btnIdleTime
            // 
            this.btnIdleTime.Location = new System.Drawing.Point(93, 449);
            this.btnIdleTime.Name = "btnIdleTime";
            this.btnIdleTime.Size = new System.Drawing.Size(75, 23);
            this.btnIdleTime.TabIndex = 12;
            this.btnIdleTime.Text = "Set idle time";
            this.btnIdleTime.UseVisualStyleBackColor = true;
            this.btnIdleTime.Click += new System.EventHandler(this.btnIdleTime_Click);
            // 
            // pbUsbOk
            // 
            this.pbUsbOk.Location = new System.Drawing.Point(391, 417);
            this.pbUsbOk.Name = "pbUsbOk";
            this.pbUsbOk.Size = new System.Drawing.Size(26, 26);
            this.pbUsbOk.TabIndex = 13;
            this.pbUsbOk.TabStop = false;
            // 
            // tmrPoke
            // 
            this.tmrPoke.Enabled = true;
            this.tmrPoke.Interval = 200;
            this.tmrPoke.Tick += new System.EventHandler(this.tmrPoke_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(636, 483);
            this.Controls.Add(this.pbUsbOk);
            this.Controls.Add(this.btnIdleTime);
            this.Controls.Add(this.nudIdleTime);
            this.Controls.Add(this.btnReset);
            this.Controls.Add(this.btnInfo);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.txtLog);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.tbRed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbBlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbColour)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudBlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudGreen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudRed)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudEEPROMLocation)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudEEPROMValue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudIdleTime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pbUsbOk)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar tbRed;
        private System.Windows.Forms.TrackBar tbGreen;
        private System.Windows.Forms.TrackBar tbBlue;
        private System.Windows.Forms.PictureBox pbColour;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown nudBlue;
        private System.Windows.Forms.NumericUpDown nudGreen;
        private System.Windows.Forms.NumericUpDown nudRed;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnEEPROMWrite;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnEEPROMRead;
        private System.Windows.Forms.NumericUpDown nudEEPROMValue;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nudEEPROMLocation;
        private System.Windows.Forms.TextBox txtLog;
        private System.Windows.Forms.Button btnOpen;
        private System.Windows.Forms.ListView lvEEPROM;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Button btnInfo;
        private System.Windows.Forms.Button btnReset;
        private System.Windows.Forms.NumericUpDown nudIdleTime;
        private System.Windows.Forms.Button btnIdleTime;
        private System.Windows.Forms.PictureBox pbUsbOk;
        private System.Windows.Forms.Timer tmrPoke;
        private System.Windows.Forms.CheckBox cbCapScreen;
        private System.Windows.Forms.CheckBox cbGamma;
    }
}

