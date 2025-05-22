using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;
using System.Collections.Generic;
using System.Windows.Forms;

namespace BaseLineGUI
{
    public partial class MainWindow : Form
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void MainWindow_Load(object sender, System.EventArgs e)
        {
            // 将规则列表中的规则项添加到DataGridView中
            List<RuleItem> rules = RulesStorage.GetRules();
            foreach (RuleItem rule in rules)
            {
                // 分不同规则类型进行处理
                if (rule is RegistryRule rule1)
                {
                    this.dateGridOverview.Rows.Add(
                        rule.ItemName,
                        "注册表",
                        rule1.RegistryPath,
                        rule1.RegistryName,
                        rule1.ExpectedValue,
                        rule1.DetectedValue,
                        CheckResultClass.GetCheckResultName(rule.CheckResult),
                        false
                    );
                }
                else if (rule is AuditPolicyRule rule2)
                {
                    this.dateGridOverview.Rows.Add(
                        rule.ItemName,
                        "审计策略",
                        "",
                        rule2.SubCategory,
                        rule2.ExpectedValueString,
                        rule2.DetectedValueString,
                        CheckResultClass.GetCheckResultName(rule.CheckResult),
                        true
                    );
                }
                else
                {
                    MessageBox.Show("未知规则类型，请检查规则文件。");
                }
            }
        }
    }
}
