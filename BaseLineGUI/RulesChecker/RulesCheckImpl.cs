using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace BaseLineGUI.RulesChecker
{
    /// <summary>
    /// 用来供前端直接调用的检查和修复方法，封装了DllFunctions
    /// </summary>
    public class RulesCheckImpl
    {
        public static void CheckRegistryRule(RegistryRule ruleItem)
        {
            
            string registryPath = ruleItem.RegistryPath;
            string itemName = ruleItem.ItemName;
            string itemType = ruleItem.ValueType;
            string expectedValue = ruleItem.ExpectedValue;
            CheckResultStruct resultStruct = DllFunctions.DllFunctions.CheckRegistryRule(registryPath, itemName, itemType, expectedValue);
            // 获取检测结果  
            switch (resultStruct.status)
            {
                case 0: ruleItem.CheckResult = CheckResult.Passed; break;
                case 1: ruleItem.CheckResult = CheckResult.NotPassed; break;
                case 2: ruleItem.CheckResult = CheckResult.Failed; break;
                default: ruleItem.CheckResult = CheckResult.Failed; break;
            }
            // 获取检测值
            ruleItem.DetectedValue = resultStruct.value;
            //return ruleItem;
        }

        public static AuditPolicyRule CheckAuditpolRule(AuditPolicyRule ruleItem)
        {
            
            return ruleItem;
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
