namespace BaseLineGUI
{
    partial class MainWindow
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.dateGridOverview = new System.Windows.Forms.DataGridView();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.panelBottomContainer = new System.Windows.Forms.Panel();
            this.bottomButtonsPanel = new System.Windows.Forms.TableLayoutPanel();
            this.checkButton = new System.Windows.Forms.Button();
            this.fixSelectedButton = new System.Windows.Forms.Button();
            this.selectAllButton = new System.Windows.Forms.Button();
            this.bottomLeftTextPanel = new System.Windows.Forms.TableLayoutPanel();
            this.label2 = new System.Windows.Forms.Label();
            this.tabControlContainerPanel = new System.Windows.Forms.Panel();
            this.buttonExportFile = new System.Windows.Forms.Button();
            this.ColumnRuleName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnItemType = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnPath = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnPropertyName = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnExpectedValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnDetectedValue = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnDetectResult = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ColumnSelected = new System.Windows.Forms.DataGridViewCheckBoxColumn();
            this.tableLayoutPanel1.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dateGridOverview)).BeginInit();
            this.panelBottomContainer.SuspendLayout();
            this.bottomButtonsPanel.SuspendLayout();
            this.bottomLeftTextPanel.SuspendLayout();
            this.tabControlContainerPanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(1768, 57);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(1762, 57);
            this.label1.TabIndex = 0;
            this.label1.Text = "Windows Server 2016 基线卫士";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1768, 573);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.dateGridOverview);
            this.tabPage1.Location = new System.Drawing.Point(4, 28);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage1.Size = new System.Drawing.Size(1760, 541);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "全部";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // dateGridOverview
            // 
            this.dateGridOverview.AllowUserToAddRows = false;
            this.dateGridOverview.AllowUserToDeleteRows = false;
            this.dateGridOverview.AllowUserToResizeRows = false;
            this.dateGridOverview.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dateGridOverview.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.ColumnRuleName,
            this.ColumnItemType,
            this.ColumnPath,
            this.ColumnPropertyName,
            this.ColumnExpectedValue,
            this.ColumnDetectedValue,
            this.ColumnDetectResult,
            this.ColumnSelected});
            this.dateGridOverview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dateGridOverview.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
            this.dateGridOverview.Location = new System.Drawing.Point(3, 3);
            this.dateGridOverview.Name = "dateGridOverview";
            this.dateGridOverview.ReadOnly = true;
            this.dateGridOverview.RowHeadersWidth = 62;
            this.dateGridOverview.RowTemplate.Height = 30;
            this.dateGridOverview.Size = new System.Drawing.Size(1754, 535);
            this.dateGridOverview.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 28);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage2.Size = new System.Drawing.Size(1224, 390);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "账户策略";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Location = new System.Drawing.Point(4, 28);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage3.Size = new System.Drawing.Size(1224, 390);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "日志";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // tabPage4
            // 
            this.tabPage4.Location = new System.Drawing.Point(4, 28);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage4.Size = new System.Drawing.Size(1224, 390);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "网络配置";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // tabPage5
            // 
            this.tabPage5.Location = new System.Drawing.Point(4, 28);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPage5.Size = new System.Drawing.Size(1224, 390);
            this.tabPage5.TabIndex = 4;
            this.tabPage5.Text = "其他配置";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // panelBottomContainer
            // 
            this.panelBottomContainer.Controls.Add(this.bottomButtonsPanel);
            this.panelBottomContainer.Controls.Add(this.bottomLeftTextPanel);
            this.panelBottomContainer.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panelBottomContainer.Location = new System.Drawing.Point(0, 630);
            this.panelBottomContainer.Name = "panelBottomContainer";
            this.panelBottomContainer.Size = new System.Drawing.Size(1768, 99);
            this.panelBottomContainer.TabIndex = 0;
            // 
            // bottomButtonsPanel
            // 
            this.bottomButtonsPanel.ColumnCount = 4;
            this.bottomButtonsPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.bottomButtonsPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.bottomButtonsPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.bottomButtonsPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.bottomButtonsPanel.Controls.Add(this.checkButton, 0, 0);
            this.bottomButtonsPanel.Controls.Add(this.fixSelectedButton, 2, 0);
            this.bottomButtonsPanel.Controls.Add(this.selectAllButton, 1, 0);
            this.bottomButtonsPanel.Controls.Add(this.buttonExportFile, 3, 0);
            this.bottomButtonsPanel.Dock = System.Windows.Forms.DockStyle.Right;
            this.bottomButtonsPanel.Location = new System.Drawing.Point(868, 0);
            this.bottomButtonsPanel.Name = "bottomButtonsPanel";
            this.bottomButtonsPanel.RowCount = 1;
            this.bottomButtonsPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.bottomButtonsPanel.Size = new System.Drawing.Size(900, 99);
            this.bottomButtonsPanel.TabIndex = 1;
            // 
            // checkButton
            // 
            this.checkButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.checkButton.Location = new System.Drawing.Point(3, 3);
            this.checkButton.Name = "checkButton";
            this.checkButton.Size = new System.Drawing.Size(219, 93);
            this.checkButton.TabIndex = 2;
            this.checkButton.Text = "检测";
            this.checkButton.UseVisualStyleBackColor = true;
            // 
            // fixSelectedButton
            // 
            this.fixSelectedButton.AutoSize = true;
            this.fixSelectedButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.fixSelectedButton.Location = new System.Drawing.Point(453, 3);
            this.fixSelectedButton.Name = "fixSelectedButton";
            this.fixSelectedButton.Size = new System.Drawing.Size(219, 93);
            this.fixSelectedButton.TabIndex = 0;
            this.fixSelectedButton.Text = "修复所选项";
            this.fixSelectedButton.UseVisualStyleBackColor = true;
            // 
            // selectAllButton
            // 
            this.selectAllButton.AutoSize = true;
            this.selectAllButton.Dock = System.Windows.Forms.DockStyle.Fill;
            this.selectAllButton.Location = new System.Drawing.Point(228, 3);
            this.selectAllButton.Name = "selectAllButton";
            this.selectAllButton.Size = new System.Drawing.Size(219, 93);
            this.selectAllButton.TabIndex = 1;
            this.selectAllButton.Text = "全选";
            this.selectAllButton.UseVisualStyleBackColor = true;
            // 
            // bottomLeftTextPanel
            // 
            this.bottomLeftTextPanel.ColumnCount = 1;
            this.bottomLeftTextPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.bottomLeftTextPanel.Controls.Add(this.label2, 0, 0);
            this.bottomLeftTextPanel.Dock = System.Windows.Forms.DockStyle.Left;
            this.bottomLeftTextPanel.Location = new System.Drawing.Point(0, 0);
            this.bottomLeftTextPanel.Name = "bottomLeftTextPanel";
            this.bottomLeftTextPanel.RowCount = 1;
            this.bottomLeftTextPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.bottomLeftTextPanel.Size = new System.Drawing.Size(496, 99);
            this.bottomLeftTextPanel.TabIndex = 0;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label2.ForeColor = System.Drawing.Color.Red;
            this.label2.Location = new System.Drawing.Point(3, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(490, 99);
            this.label2.TabIndex = 0;
            this.label2.Text = "合格率：100%";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // tabControlContainerPanel
            // 
            this.tabControlContainerPanel.Controls.Add(this.tabControl1);
            this.tabControlContainerPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlContainerPanel.Location = new System.Drawing.Point(0, 57);
            this.tabControlContainerPanel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.tabControlContainerPanel.Name = "tabControlContainerPanel";
            this.tabControlContainerPanel.Size = new System.Drawing.Size(1768, 573);
            this.tabControlContainerPanel.TabIndex = 2;
            // 
            // buttonExportFile
            // 
            this.buttonExportFile.Dock = System.Windows.Forms.DockStyle.Fill;
            this.buttonExportFile.Location = new System.Drawing.Point(678, 3);
            this.buttonExportFile.Name = "buttonExportFile";
            this.buttonExportFile.Size = new System.Drawing.Size(219, 93);
            this.buttonExportFile.TabIndex = 3;
            this.buttonExportFile.Text = "导出报表";
            this.buttonExportFile.UseVisualStyleBackColor = true;
            // 
            // ColumnRuleName
            // 
            this.ColumnRuleName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnRuleName.HeaderText = "规则名";
            this.ColumnRuleName.MinimumWidth = 8;
            this.ColumnRuleName.Name = "ColumnRuleName";
            this.ColumnRuleName.ReadOnly = true;
            this.ColumnRuleName.Width = 91;
            // 
            // ColumnItemType
            // 
            this.ColumnItemType.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnItemType.HeaderText = "规则类型";
            this.ColumnItemType.MinimumWidth = 8;
            this.ColumnItemType.Name = "ColumnItemType";
            this.ColumnItemType.ReadOnly = true;
            this.ColumnItemType.Width = 91;
            // 
            // ColumnPath
            // 
            this.ColumnPath.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnPath.HeaderText = "注册表路径";
            this.ColumnPath.MinimumWidth = 8;
            this.ColumnPath.Name = "ColumnPath";
            this.ColumnPath.ReadOnly = true;
            this.ColumnPath.Width = 108;
            // 
            // ColumnPropertyName
            // 
            this.ColumnPropertyName.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnPropertyName.HeaderText = "注册表项/Auditpol子类别";
            this.ColumnPropertyName.MinimumWidth = 8;
            this.ColumnPropertyName.Name = "ColumnPropertyName";
            this.ColumnPropertyName.ReadOnly = true;
            this.ColumnPropertyName.Width = 1082;
            // 
            // ColumnExpectedValue
            // 
            this.ColumnExpectedValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnExpectedValue.HeaderText = "期望值";
            this.ColumnExpectedValue.MinimumWidth = 8;
            this.ColumnExpectedValue.Name = "ColumnExpectedValue";
            this.ColumnExpectedValue.ReadOnly = true;
            this.ColumnExpectedValue.Width = 91;
            // 
            // ColumnDetectedValue
            // 
            this.ColumnDetectedValue.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnDetectedValue.HeaderText = "检测值";
            this.ColumnDetectedValue.MinimumWidth = 8;
            this.ColumnDetectedValue.Name = "ColumnDetectedValue";
            this.ColumnDetectedValue.ReadOnly = true;
            this.ColumnDetectedValue.Width = 91;
            // 
            // ColumnDetectResult
            // 
            this.ColumnDetectResult.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.None;
            this.ColumnDetectResult.HeaderText = "检测结果";
            this.ColumnDetectResult.MinimumWidth = 8;
            this.ColumnDetectResult.Name = "ColumnDetectResult";
            this.ColumnDetectResult.ReadOnly = true;
            this.ColumnDetectResult.Width = 91;
            // 
            // ColumnSelected
            // 
            this.ColumnSelected.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.AllCells;
            this.ColumnSelected.HeaderText = "勾选";
            this.ColumnSelected.MinimumWidth = 8;
            this.ColumnSelected.Name = "ColumnSelected";
            this.ColumnSelected.ReadOnly = true;
            this.ColumnSelected.Width = 45;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1768, 729);
            this.Controls.Add(this.tabControlContainerPanel);
            this.Controls.Add(this.panelBottomContainer);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "MainWindow";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.MainWindow_Load);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dateGridOverview)).EndInit();
            this.panelBottomContainer.ResumeLayout(false);
            this.bottomButtonsPanel.ResumeLayout(false);
            this.bottomButtonsPanel.PerformLayout();
            this.bottomLeftTextPanel.ResumeLayout(false);
            this.bottomLeftTextPanel.PerformLayout();
            this.tabControlContainerPanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button fixSelectedButton;
        private System.Windows.Forms.Button selectAllButton;
        private System.Windows.Forms.Panel panelBottomContainer;
        private System.Windows.Forms.Button checkButton;
        private System.Windows.Forms.TableLayoutPanel bottomLeftTextPanel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TableLayoutPanel bottomButtonsPanel;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.DataGridView dateGridOverview;
        private System.Windows.Forms.Panel tabControlContainerPanel;
        private System.Windows.Forms.Button buttonExportFile;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnRuleName;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnItemType;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnPath;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnPropertyName;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnExpectedValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnDetectedValue;
        private System.Windows.Forms.DataGridViewTextBoxColumn ColumnDetectResult;
        private System.Windows.Forms.DataGridViewCheckBoxColumn ColumnSelected;
    }
}

