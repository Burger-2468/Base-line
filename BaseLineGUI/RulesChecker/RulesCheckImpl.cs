using BaseLineGUI.RulesLoader;
using BaseLineGUI.StateStorage;

namespace BaseLineGUI.RulesChecker
{
    /// <summary>
    /// 用来供前端直接调用的检查和修复方法，封装了DllFunctions
    /// </summary>
    public class RulesCheckImpl
    {
        /// <summary>
        /// 检测注册表规则
        /// </summary>
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
        }

        /// <summary>
        /// 检测审计策略规则
        /// </summary>
        public static void CheckAuditpolRule(AuditPolicyRule ruleItem)
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
        }

        /// <summary>
        /// 修复注册表规则
        /// </summary>
        public static void FixRegistryRule(RegistryRule ruleItem)
        {
            string registryPath = ruleItem.RegistryPath;
            string itemName = ruleItem.ItemName;
            string itemType = ruleItem.ValueType;
            string expectedValue = ruleItem.ExpectedValue;
            // 对注册表值类型进行预处理
            itemType = itemType.Replace("REG_", ""); // 去掉前缀
            itemType = itemType.Replace("SZ", "STRING"); // 将SZ转换为STRING
            CheckResultStruct resultStruct = new CheckResultStruct();
            //DllFunctions.DllFunctions.FixRegistryRule(registryPath, itemName, itemType, expectedValue, out resultStruct);
            //DllFunctions.DllFunctions.FixRegistryRule("HKEY_LOCAL_MACHINE\\SOFTWARE\\7-Zip", "Path", "STRING", "C:\\Program Files\\7-Zip64\\", out resultStruct);
            // 获取修复结果
            if (resultStruct.status == 0)
            {
                ruleItem.CheckResult = CheckResult.Fixed;
            } else
            {
                ruleItem.CheckResult = CheckResult.FixFailed;
            }
        }

        /// <summary>
        /// 修复审计策略规则
        /// </summary>
        public static void FixAuditpolRule(AuditPolicyRule ruleItem)
        {
            string subCategory = ruleItem.SubCategory;
            int expectedValue = ruleItem.ExpectedValue;
            CheckResultStruct resultStruct;
            //DllFunctions.DllFunctions.FixAuditpolRule(subCategory, expectedValue, out resultStruct);
            DllFunctions.DllFunctions.FixAuditpolRule("{0CCE9211-69AE-11D9-BED3-505054503030}", 1, out resultStruct);
            // 获取修复结果  
            if (resultStruct.status == 0)
            {
                ruleItem.CheckResult = CheckResult.Fixed;
            }
            else
            {
                ruleItem.CheckResult = CheckResult.FixFailed;
            }
        }
    }
}
