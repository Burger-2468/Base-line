using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using BaseLineGUI.RulesChecker;

namespace BaseLineGUI.DllFunctions
{
    public class DllFunctions
    {

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern CheckResultStruct CheckRegistryRuleInternal(string registryPath, string itemName, string itemType, string expectedValue);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResultStruct CheckAuditpolRuleInternal(string auditCategory, string auditSubcategory, int expectedValue);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResultStruct FixRegistryRuleInternal(string registryPath, string itemName, string itemType, string expectedValue);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResultStruct FixAuditpolRuleInternal(string auditCategory, string auditSubcategory, int expectedValue);

        ///
        /// 调用方式
        /// // 调用 CheckRegistryRule 函数
        /// CheckResult result = DllFunctions.CheckRegistryRule("path", "name", "type", "value");
        /// 
    }
}
