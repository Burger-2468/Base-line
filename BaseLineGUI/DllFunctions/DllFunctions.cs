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

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResult CheckRegistryRule(string registryPath, string itemName, string itemType, string expectedValue);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResult CheckAuditpolRule(string auditCategory, string auditSubcategory, int expectedValue);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResult FixRegistryRule(string registryPath, string itemName, string itemType, string expectedValue);

        [DllImport("BaseLineLibrary.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern CheckResult FixAuditpolRule(string auditCategory, string auditSubcategory, int expectedValue);
    }
}
