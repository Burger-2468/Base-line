using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.StateStorage
{
    /// <summary>
    /// 全局存储规则列表，用于界面显示和界面交互
    /// </summary>
    public class RulesStorage
    {
        /// <summary>
        /// 内部存储的规则列表
        /// </summary>
        private static readonly List<RuleItem> rules = new List<RuleItem>();

        /// <summary>
        /// 向规则列表中添加规则项
        /// </summary>
        public static void AddRule(RuleItem rule)
        {
            rule.Index = rules.Count; // 设置规则项的索引为当前列表的长度
            rules.Add(rule);
        }

        /// <summary>
        /// 从规则列表中删除规则
        /// </summary>
        public static void RemoveRule(RuleItem rule)
        {
            rule.Index = rules.Count; // 设置规则项的索引为当前列表的长度
            rules.Remove(rule);
        }

        /// <summary>
        /// 获取规则列表
        /// </summary>
        public static List<RuleItem> GetRules() {
            return rules; 
        }
    }
}
