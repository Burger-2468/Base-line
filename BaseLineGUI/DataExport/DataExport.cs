using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BaseLineGUI.DataExport
{
    public class DataExport
    {
        public static void ExportData()
        {
            // 创建保存文件对话框
            using (SaveFileDialog saveFileDialog = new SaveFileDialog())
            {
                saveFileDialog.Filter = "CSV文件 (*.csv)|*.csv";
                saveFileDialog.Title = "导出规则数据";
                saveFileDialog.FileName = "安全基线规则_" + DateTime.Now.ToString("yyyyMMdd") + ".csv";
                saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);

                // 显示对话框并等待用户响应
                if (saveFileDialog.ShowDialog() == DialogResult.OK)
                {
                    try
                    {
                        // 获取所有规则
                        List<RuleItem> rules = RulesStorage.GetRules();

                        // 创建CSV内容
                        StringBuilder csvContent = new StringBuilder();

                        // 添加CSV表头
                        csvContent.AppendLine("序号,规则名,规则类别,注册表路径,注册表项名/Auditpol子类别,期望值,检测值,检查结果");

                        // 遍历所有规则
                        for (int i = 0; i < rules.Count; i++)
                        {
                            RuleItem rule = rules[i];

                            // 处理注册表规则
                            if (rule is RegistryRule registryRule)
                            {
                                csvContent.AppendLine(
                                    $"{i}," +
                                    $"\"{escapeCsvField(registryRule.ItemName)}\"," +
                                    $"\"注册表\"," +
                                    $"\"{escapeCsvField(registryRule.RegistryPath)}\"," +
                                    $"\"{escapeCsvField(registryRule.RegistryName)}\"," +
                                    $"\"{escapeCsvField(registryRule.ExpectedValue)}\"," +
                                    $"\"{escapeCsvField(registryRule.DetectedValue)}\"," +
                                    $"\"{CheckResultClass.GetCheckResultName(registryRule.CheckResult)}\","
                                );
                            }
                            // 处理审计策略规则
                            else if (rule is AuditPolicyRule auditPolicyRule)
                            {
                                csvContent.AppendLine(
                                    $"{i}," +
                                    $"\"{escapeCsvField(auditPolicyRule.ItemName)}\"," +
                                    $"\"审计策略\"," +
                                    $"\"\"," +
                                    $"\"{escapeCsvField(auditPolicyRule.SubCategory)}\"," +
                                    $"\"{escapeCsvField(auditPolicyRule.ExpectedValueString)}\"," +
                                    $"\"{escapeCsvField(auditPolicyRule.DetectedValueString)}\"," +
                                    $"\"{CheckResultClass.GetCheckResultName(auditPolicyRule.CheckResult)}\","
                                );
                            }
                        }

                        // 写入文件
                        File.WriteAllText(saveFileDialog.FileName, csvContent.ToString(), Encoding.UTF8);

                        MessageBox.Show($"成功导出 {rules.Count} 条规则到:\n{saveFileDialog.FileName}", "导出成功", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show($"导出失败: {ex.Message}", "错误", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
        }

        // CSV字段转义函数（处理包含逗号、引号等特殊字符）
        private static string escapeCsvField(string input)
        {
            if (string.IsNullOrEmpty(input))
                return "";

            // 处理包含引号的情况（将单个引号转义为两个引号）
            if (input.Contains("\"") || input.Contains(",") || input.Contains("\n") || input.Contains("\r"))
            {
                return input.Replace("\"", "\"\"");
            }

            return input;
        }
    }
}
