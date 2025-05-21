using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.RulesChecker
{
    internal class RulesCheckImpl
    {
        protected static CheckResult CheckRegistryRule(string registryPath, string itemName, string itemType, string expectedValue)
        {
            CheckResult result = new CheckResult();
            return result;
        }

        protected static CheckResult CheckAuditpolRule(string auditSubCategory, int expectedValue)
        {
            CheckResult result = new CheckResult();
            return result;
        }

        protected static bool FixRegistryRule(string registryPath, string itemName, string itemType, string expectedValue)
        {
            return false;
        }

        protected static bool FixAuditpolRule(string auditSubCategory, int expectedValue)
        {
            return false;
        }
    }
}
