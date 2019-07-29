namespace can_monitor
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel3 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel4 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel5 = new System.Windows.Forms.ToolStripStatusLabel();
            this.timer10ms = new System.Windows.Forms.Timer(this.components);
            this.comboBox_CanCom = new System.Windows.Forms.ComboBox();
            this.button_CanCom = new System.Windows.Forms.Button();
            this.button_Clear = new System.Windows.Forms.Button();
            this.listView_Can = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.button_SortId = new System.Windows.Forms.Button();
            this.button_SortTm = new System.Windows.Forms.Button();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1,
            this.toolStripProgressBar1,
            this.toolStripStatusLabel2,
            this.toolStripStatusLabel3,
            this.toolStripStatusLabel4,
            this.toolStripStatusLabel5});
            this.statusStrip1.Location = new System.Drawing.Point(0, 431);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(592, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)));
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(33, 17);
            this.toolStripStatusLabel1.Text = "V1.3";
            // 
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            // 
            // toolStripStatusLabel2
            // 
            this.toolStripStatusLabel2.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)));
            this.toolStripStatusLabel2.Name = "toolStripStatusLabel2";
            this.toolStripStatusLabel2.Size = new System.Drawing.Size(51, 17);
            this.toolStripStatusLabel2.Text = "Monitor";
            // 
            // toolStripStatusLabel3
            // 
            this.toolStripStatusLabel3.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)));
            this.toolStripStatusLabel3.Name = "toolStripStatusLabel3";
            this.toolStripStatusLabel3.Size = new System.Drawing.Size(51, 17);
            this.toolStripStatusLabel3.Text = "Can Dev";
            // 
            // toolStripStatusLabel4
            // 
            this.toolStripStatusLabel4.BorderSides = ((System.Windows.Forms.ToolStripStatusLabelBorderSides)((System.Windows.Forms.ToolStripStatusLabelBorderSides.Left | System.Windows.Forms.ToolStripStatusLabelBorderSides.Right)));
            this.toolStripStatusLabel4.Name = "toolStripStatusLabel4";
            this.toolStripStatusLabel4.Size = new System.Drawing.Size(27, 17);
            this.toolStripStatusLabel4.Text = "Sec";
            // 
            // toolStripStatusLabel5
            // 
            this.toolStripStatusLabel5.Name = "toolStripStatusLabel5";
            this.toolStripStatusLabel5.Size = new System.Drawing.Size(41, 17);
            this.toolStripStatusLabel5.Text = "Status";
            // 
            // timer10ms
            // 
            this.timer10ms.Enabled = true;
            this.timer10ms.Interval = 10;
            this.timer10ms.Tick += new System.EventHandler(this.timer10ms_Tick);
            // 
            // comboBox_CanCom
            // 
            this.comboBox_CanCom.FormattingEnabled = true;
            this.comboBox_CanCom.Location = new System.Drawing.Point(473, 72);
            this.comboBox_CanCom.Name = "comboBox_CanCom";
            this.comboBox_CanCom.Size = new System.Drawing.Size(88, 20);
            this.comboBox_CanCom.TabIndex = 1;
            // 
            // button_CanCom
            // 
            this.button_CanCom.Location = new System.Drawing.Point(473, 123);
            this.button_CanCom.Name = "button_CanCom";
            this.button_CanCom.Size = new System.Drawing.Size(75, 23);
            this.button_CanCom.TabIndex = 2;
            this.button_CanCom.Text = "Can Com";
            this.button_CanCom.UseVisualStyleBackColor = true;
            this.button_CanCom.Click += new System.EventHandler(this.button_CanCom_Click);
            // 
            // button_Clear
            // 
            this.button_Clear.Location = new System.Drawing.Point(473, 227);
            this.button_Clear.Name = "button_Clear";
            this.button_Clear.Size = new System.Drawing.Size(75, 23);
            this.button_Clear.TabIndex = 3;
            this.button_Clear.Text = "Clear";
            this.button_Clear.UseVisualStyleBackColor = true;
            this.button_Clear.Click += new System.EventHandler(this.button_Clear_Click);
            // 
            // listView_Can
            // 
            this.listView_Can.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4});
            this.listView_Can.FullRowSelect = true;
            this.listView_Can.GridLines = true;
            this.listView_Can.Location = new System.Drawing.Point(21, 31);
            this.listView_Can.Name = "listView_Can";
            this.listView_Can.Size = new System.Drawing.Size(408, 376);
            this.listView_Can.TabIndex = 5;
            this.listView_Can.UseCompatibleStateImageBehavior = false;
            this.listView_Can.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "TYPE";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "ID";
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "TIME";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "DATA";
            this.columnHeader4.Width = 200;
            // 
            // button_SortId
            // 
            this.button_SortId.Location = new System.Drawing.Point(473, 287);
            this.button_SortId.Name = "button_SortId";
            this.button_SortId.Size = new System.Drawing.Size(75, 23);
            this.button_SortId.TabIndex = 6;
            this.button_SortId.Text = "Sort Id";
            this.button_SortId.UseVisualStyleBackColor = true;
            this.button_SortId.Click += new System.EventHandler(this.button_Sort_Click);
            // 
            // button_SortTm
            // 
            this.button_SortTm.Location = new System.Drawing.Point(473, 343);
            this.button_SortTm.Name = "button_SortTm";
            this.button_SortTm.Size = new System.Drawing.Size(75, 23);
            this.button_SortTm.TabIndex = 7;
            this.button_SortTm.Text = "Sort Tm";
            this.button_SortTm.UseVisualStyleBackColor = true;
            this.button_SortTm.Click += new System.EventHandler(this.button_SortTm_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(592, 453);
            this.Controls.Add(this.button_SortTm);
            this.Controls.Add(this.button_SortId);
            this.Controls.Add(this.listView_Can);
            this.Controls.Add(this.button_Clear);
            this.Controls.Add(this.button_CanCom);
            this.Controls.Add(this.comboBox_CanCom);
            this.Controls.Add(this.statusStrip1);
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "CanMonitor";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel2;
        private System.Windows.Forms.Timer timer10ms;
        private System.Windows.Forms.ComboBox comboBox_CanCom;
        private System.Windows.Forms.Button button_CanCom;
        private System.Windows.Forms.Button button_Clear;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel3;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel4;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel5;
        private System.Windows.Forms.ListView listView_Can;
        private System.Windows.Forms.Button button_SortId;
        private System.Windows.Forms.Button button_SortTm;
    }
}

