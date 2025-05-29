using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;

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
            CheckResultStruct resultStruct;
            // 对注册表值类型进行预处理
            itemType = itemType.Replace("REG_", ""); // 去掉前缀
            itemType = itemType.Replace("SZ", "STRING"); // 将SZ转换为STRING
            //DllFunctions.DllFunctions.CheckRegistryRule("HKEY_LOCAL_MACHINE", "ServiceLastKnownStatus", "DWORD", "2", out resultStruct);
            DllFunctions.DllFunctions.CheckRegistryRule(registryPath, itemName, itemType, expectedValue, out resultStruct);
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
            string subCategory = ruleItem.SubCategory;
            int expectedValue = ruleItem.ExpectedValue;
            CheckResultStruct resultStruct;
            DllFunctions.DllFunctions.CheckAuditpolRule(subCategory, expectedValue, out resultStruct);
            //DllFunctions.DllFunctions.CheckAuditpolRule("{0CCE9210-69AE-11D9-BED3-505054503030}", 0, out resultStruct);
            // 获取检测结果  
            switch (resultStruct.status)
            {
                case 0: ruleItem.CheckResult = CheckResult.Passed; break;
                case 1: ruleItem.CheckResult = CheckResult.NotPassed; break;
                case 2: ruleItem.CheckResult = CheckResult.Failed; break;
                default: ruleItem.CheckResult = CheckResult.Failed; break;
            }
            // 获取检测值
            if (ruleItem.CheckResult == CheckResult.Failed)
            {
                ruleItem.DetectedValue = -1;
            }
            else
            {
                ruleItem.DetectedValue = int.Parse(resultStruct.value);
            }

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
