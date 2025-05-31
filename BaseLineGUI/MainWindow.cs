using BaseLineGUI.RulesChecker;
using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;
using System.Collections.Generic;
using System.Windows.Forms;

namespace BaseLineGUI
{

    public partial class MainWindow : Form
    {
        private readonly string defaultAccuracyLabelText = "请点击“检测”按钮开始检测！";
        private readonly string fixSuccessfulAccutacyLabelText = "修复成功！请点击“检测”按钮再次检测。";

        public MainWindow()
        {
            InitializeComponent();
        }

        private void MainWindow_Load(object sender, System.EventArgs e)
        {
            // 将规则列表中的规则项添加到DataGridView中
            refreshTable();
            accuracyLabel.Text = defaultAccuracyLabelText;
        }

        /// <summary>
        /// 处理用户点击复选框选中规则的操作
        /// </summary>
        private void dataGrid_CellClick(object sender, DataGridViewCellEventArgs e)
        {

            if (e.ColumnIndex == 8 && sender is DataGridView dataGridView)
            {
                // 获取当前行
                DataGridViewRow row = dataGridView.Rows[e.RowIndex];
                // 获取当前行的规则项
                RuleItem rule = RulesStorage.GetRules()[(int)row.Cells[0].Value];
                // 改变选中状态
                rule.IsSelectedToFix = !rule.IsSelectedToFix;
                // 更新选中状态
                row.Cells[8].Value = rule.IsSelectedToFix;
            }

        }

        private void checkButton_Click(object sender, System.EventArgs e)
        {
            List<RuleItem> rules = RulesStorage.GetRules();//创建检测项容器
            int checkedCount = 0;//已检测的规则数量
            int passedCount = 0;//通过检测的规则数量
            int notCheckedCount = 0;//未检测的规则数量

            for (int i = 0; i < rules.Count; i++)//遍历容器执行查询
            {
                RuleItem rule = rules[i];

                if (rule is RegistryRule registryRule)
                {
                    RulesCheckImpl.CheckRegistryRule(registryRule);
                }
                else if (rule is AuditPolicyRule auditPolicyRule)
                {
                    RulesCheckImpl.CheckAuditpolRule(auditPolicyRule);
                }
                else
                {
                    MessageBox.Show("未知规则类型。");
                }

                // 更新统计数据
                if (rule.CheckResult != CheckResult.NotChecked)
                {
                    checkedCount++;
                    if (rule.CheckResult == CheckResult.Passed)
                    {
                        passedCount++;
                    }
                }
                else
                {
                    notCheckedCount++;
                }
            }

            // 计算统计数据并更新界面
            accuracyLabel.Text = $"合格率：{(((float)passedCount / checkedCount) * 100).ToString("0.00")}%（{passedCount}/{checkedCount}），未检测：{notCheckedCount}";

            refreshTable();//刷新表格
        }

        private void fixSelectedButton_Click(object sender, System.EventArgs e)
        {
            List<RuleItem> rules = RulesStorage.GetRules();//创建检测项容器

            for (int i = 0; i < rules.Count; i++)//遍历容器执行查询
            {
                RuleItem rule = rules[i];
                if (rule is RegistryRule registryRule)//判断规则类型：注册表
                {
                    if (rule.IsSelectedToFix && rule.CheckResult != CheckResult.Passed)
                    {
                        RulesCheckImpl.FixRegistryRule(registryRule);
                    }
                }
                else if (rule is AuditPolicyRule auditPolicyRule)//判断规则类型：审计策略
                {
                    if (rule.IsSelectedToFix && rule.CheckResult != CheckResult.Passed)
                    {
                        RulesCheckImpl.FixAuditpolRule(auditPolicyRule);
                    }
                }
                else
                {
                    MessageBox.Show("未知规则类型。");
                }
            }

            // 更新提示数据
            accuracyLabel.Text = fixSuccessfulAccutacyLabelText;

            refreshTable();//刷新表格
        }

        private void buttonExportFile_Click(object sender, System.EventArgs e)
        {
            DataExport.DataExport.ExportData();
        }

        /// <summary>
        /// 刷新表格，将数据重新填充到DataGridView中
        /// </summary>
        private void refreshTable()
        {
            List<RuleItem> rules = RulesStorage.GetRules();
            refreshSingleTable(dataGridOverview, rules);
            refreshSingleTable(dataGridAccount, rules.FindAll(e => e.Page == "账户策略"));
            refreshSingleTable(dataGridLogs, rules.FindAll(e => e.Page == "日志"));
            refreshSingleTable(dataGridNetwork, rules.FindAll(e => e.Page == "网络配置"));
            refreshSingleTable(dataGridOthers, rules.FindAll(e => e.Page == "其他配置"));
        }

        /// <summary>
        /// 填充单个DataGridView
        /// </summary>
        private void refreshSingleTable(DataGridView dataGridView, List<RuleItem> rules)
        {
            dataGridView.Rows.Clear();
            foreach (RuleItem rule in rules)
            {
                // 分不同规则类型进行处理
                if (rule is RegistryRule rule1)
                {
                    dataGridView.Rows.Add(
                        rule.Index,
                        rule1.ItemName,
                        "注册表",
                        rule1.RegistryPath,
                        rule1.RegistryName,
                        rule1.ExpectedValue,
                        rule1.DetectedValue,
                        CheckResultClass.GetCheckResultName(rule1.CheckResult),
                        rule1.IsSelectedToFix
                    );
                }
                else if (rule is AuditPolicyRule rule2)
                {
                    dataGridView.Rows.Add(
                        rule.Index,
                        rule2.ItemName,
                        "审计策略",
                        "",
                        rule2.SubCategory,
                        rule2.ExpectedValueString,
                        rule2.DetectedValueString,
                        CheckResultClass.GetCheckResultName(rule2.CheckResult),
                        rule2.IsSelectedToFix
                    );
                }
                else
                {
                    MessageBox.Show("未知规则类型");
                }
            }
            dataGridView.AutoResizeColumn(0);
            dataGridView.AutoResizeColumn(2);
            dataGridView.AutoResizeColumn(4);
            dataGridView.AutoResizeColumn(5);
            dataGridView.AutoResizeColumn(6);
            dataGridView.AutoResizeColumn(7);
        }

        /// <summary>
        /// 全选按钮的点击事件处理函数
        /// </summary>
        private void selectAllButton_Click(object sender, System.EventArgs e)
        {
            List<RuleItem> rules = RulesStorage.GetRules();
            foreach (RuleItem rule in rules)
            {
                if (rule.CheckResult == CheckResult.NotPassed)
                {
                    rule.IsSelectedToFix = true;
                }
            }
            refreshTable(); // 刷新表格显示
        }

        /// <summary>
        /// 选项卡切换事件处理函数
        /// </summary>
        private void tabControl1_SelectedIndexChanged(object sender, System.EventArgs e)
        {
            refreshTable(); // 切换选项卡时刷新表格显示
        }
    }
}
