using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace BaseLineGUI.RulesChecker
{
    /// <summary>
    /// 用于调用C++ DLL的结构体
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct CheckResultStruct
    {
        public int status;
        public string value;
    }
}
