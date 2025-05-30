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
        public static extern void CheckRegistryRule(string registryPath, string itemName, string itemType, string expectedValue, out CheckResultStruct result);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void CheckAuditpolRule(string auditSubcategory, int expectedValue, out CheckResultStruct result);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void FixRegistryRule(string registryPath, string itemName, string itemType, string expectedValue, out CheckResultStruct result);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void FixAuditpolRule(string auditSubcategory, int expectedValue, out CheckResultStruct result);

        ///
        /// 调用方式
        /// // 调用 CheckRegistryRule 函数
        /// CheckResult result = DllFunctions.CheckRegistryRule("path", "name", "type", "value");
        /// 
    }
}
