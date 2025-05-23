﻿using System.Collections.Generic;
using System.Xml;

namespace BaseLineGUI.RulesLoader
{
    /// <summary>
    /// 提供从XML文件中读取规则列表的功能
    /// </summary>
    public class RulesParser
    {
        /// <summary>
        /// 读取XML文件，解析注册表规则项
        /// </summary>
        public static List<RegistryRule> ParseRegistryRules(string path)
        {
            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.Load(path);
            List<RegistryRule> registryRules = new List<RegistryRule>();
            XmlNodeList registryNodes = xmlDocument.SelectNodes("/registry-rules/rule-item");
            foreach (XmlNode registryNode in registryNodes)
            {
                string itemName = registryNode.SelectSingleNode("item-name").InnerText;
                string registryPath = registryNode.SelectSingleNode("registry-path").InnerText;
                string registryName = registryNode.SelectSingleNode("registry-name").InnerText;
                string valueType = registryNode.SelectSingleNode("value-type").InnerText;
                string expectedValue = registryNode.SelectSingleNode("expected-value").InnerText;
                string page = registryNode.SelectSingleNode("page").InnerText;
                RegistryRule rule = new RegistryRule(itemName, registryPath, registryName, valueType, expectedValue, page);
                registryRules.Add(rule);
            }
            return registryRules;
        }

        /// <summary>
        /// 读取XML文件，解析审计策略规则项
        /// </summary>
        public static List<AuditPolicyRule> ParseAuditPolicyRule(string path)
        {
            XmlDocument xmlDocument = new XmlDocument();
            xmlDocument.Load(path);
            List<AuditPolicyRule> auditPolicyRules = new List<AuditPolicyRule>();
            XmlNodeList auditPolicyNodes = xmlDocument.SelectNodes("/auditpolicy-rules/rule-item");
            foreach (XmlNode auditPolicyNode in auditPolicyNodes)
            {
                string itemName = auditPolicyNode.SelectSingleNode("item-name").InnerText;
                string subCategory = auditPolicyNode.SelectSingleNode("sub-category").InnerText;
                int expectedValue = int.Parse(auditPolicyNode.SelectSingleNode("expected-value").InnerText);
                string page = auditPolicyNode.SelectSingleNode("page").InnerText;
                AuditPolicyRule rule = new AuditPolicyRule(itemName, subCategory, expectedValue, page);
                auditPolicyRules.Add(rule);
            }
            return auditPolicyRules;
        }
    }
}
