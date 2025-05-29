using BaseLineGUI.StateStorage;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.RulesChecker
{
    public class RulesCheckImpl
    {
        public static RuleItem CheckRegistryRule(RuleItem ruleItem)
        {
            //  
            string registryPath = ruleItem.RegistryPath;
            string itemName = ruleItem.ItemName;
            string itemType = ruleItem.ItemType;
            string expectedValue = ruleItem.ExpectedValue;
            return ruleItem;
        }

        public static CheckResultStruct CheckAuditpolRule(string auditSubCategory, int expectedValue)
        {
            CheckResultStruct result = new CheckResultStruct();
            return result;
        }

        public static bool FixRegistryRule(string registryPath, string itemName, string itemType, string expectedValue)
        {
            return false;
        }

        public static bool FixAuditpolRule(string auditSubCategory, int expectedValue)
        {
            return false;
        }
    }
}
