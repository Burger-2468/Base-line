using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.StateStorage
{
    public class RulesStorage
    {
        private static readonly List<RuleItem> rules = new List<RuleItem>();

        public static void AddRule(RuleItem rule)
        {
            rules.Add(rule);
        }

        public static void RemoveRule(RuleItem rule)
        {
            rules.Remove(rule);
        }

        public static List<RuleItem> GetRules() {
            return rules; 
        }
    }
}
