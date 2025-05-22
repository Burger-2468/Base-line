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
            List<RuleItem> rules = RulesStorage.GetRules();
            foreach (RuleItem rule in rules)
            {
                if (rule is RegistryRule rule2)
                {
                    this.dateGridOverview.Rows.Add(
                        rule2.ItemName,
                        "注册表",
                        rule2.RegistryPath,
                        rule2.RegistryName,
                        rule2.ExpectedValue,
                        rule.DetectedValue,
                        CheckResultClass.GetCheckResultName(rule.CheckResult),
                        false
                    );
                }
                else if (rule is AuditPolicyRule rule1)
                {
                    this.dateGridOverview.Rows.Add(
                        rule1.ItemName,
                        "审计策略",
                        "",
                        rule1.SubCategory,
                        rule1.ExpectedValue,
                        rule.DetectedValue,
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
