using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using BaseLineGUI.DllFunctions;
using BaseLineGUI.RulesChecker;

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
                    this.dataGridOverview.Rows.Add(
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
                    this.dataGridOverview.Rows.Add(
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
            this.dataGridOverview.AutoResizeColumn(0);
            this.dataGridOverview.AutoResizeColumn(1);
            this.dataGridOverview.AutoResizeColumn(3);
            this.dataGridOverview.AutoResizeColumn(4);
            this.dataGridOverview.AutoResizeColumn(5);
            this.dataGridOverview.AutoResizeColumn(6);
        }

        /// <summary>
        /// 处理用户点击复选框选中规则的操作
        /// </summary>
        private void dataGridOverview_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if(e.ColumnIndex == 7)
            {
                // 获取当前行
                DataGridViewRow row = this.dataGridOverview.Rows[e.RowIndex];
                // 获取当前行的规则项
                RuleItem rule = RulesStorage.GetRules()[e.RowIndex];
                // 改变选中状态
                rule.IsSelectedToFix = !rule.IsSelectedToFix;
                // 更新选中状态
                row.Cells[7].Value = rule.IsSelectedToFix;
            }
        }

        private void checkButton_Click(object sender, System.EventArgs e)
        {
            List<RuleItem> rules = RulesStorage.GetRules();//创建检测项容器

            for(int i = 0; i < rules.Count; i++)//遍历容器执行查询
            {
                RuleItem rule = rules[i];

                try
                {
                    if(rule is RegistryRule registryRule)
                    {
                        //CheckResultStruct result = BaseLineGUI.DllFunctions.DllFunctions.CheckRegistryRule(
                        //    registryRule.RegistryPath,
                        //    registryRule.RegistryName,
                        //    registryRule.ValueType,
                        //    registryRule.ExpectedValue);
                        RulesCheckImpl.CheckRegistryRule(rule);

                        //registryRule.DetectedValue = result.value;
                        //registryRule.CheckResult = (CheckResultStatus)result.status;
                    }
                }
            }


        }

        private void fixSelectedButton_Click(object sender, System.EventArgs e)
        {

        }

        private void buttonExportFile_Click(object sender, System.EventArgs e)
        {

        }
    }
}
