using BaseLineGUI.RulesLoader;
using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;

namespace BaseLineGUI
{
    internal static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            // 开始读取数据
            string registryRulesPath = Path.Combine(Application.StartupPath, "Rules", "RegistryRules.xml");
            string auditPolicyRulesPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Rules", "AuditPolicyRules.xml");
            List<RegistryRule> registryRules = new List<RegistryRule>();
            List<AuditPolicyRule> auditPolicyRules = new List<AuditPolicyRule>();
            // 检查文件是否存在
            if (!File.Exists(registryRulesPath))
            {
                MessageBox.Show($"{registryRulesPath} 文件不存在，请检查路径。");
                return;
            }
            if (!File.Exists(auditPolicyRulesPath))
            {
                MessageBox.Show($"{auditPolicyRulesPath} 文件不存在，请检查路径。");
                return;
            }
            // 开始读取
            try
            {
                registryRules = RulesParser.ParseRegistryRules(registryRulesPath);
                auditPolicyRules = RulesParser.ParseAuditPolicyRule(auditPolicyRulesPath);
            }
            catch (Exception e)
            {
                MessageBox.Show($"读取规则文件时发生错误，请检查文件格式。\n\n{e}");
                return;
            }

            // 将读取到的规则存储到静态类中
            foreach (var rule in registryRules)
            {
                StateStorage.RulesStorage.AddRule(rule);
            }
            foreach (var rule in auditPolicyRules)
            {
                StateStorage.RulesStorage.AddRule(rule);
            }
            // MessageBox.Show(registryRules.ToString());
            // MessageBox.Show(auditPolicyRules.ToString());
            // 显示程序窗口
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainWindow());
        }
    }
}
