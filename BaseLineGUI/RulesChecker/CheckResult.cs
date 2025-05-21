using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace BaseLineGUI.RulesChecker
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CheckResult
    {
        public int status;
        public string value;
    }
}
