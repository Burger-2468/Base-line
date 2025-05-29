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
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct CheckResultStruct
    {
        // 检测结果
        public int status; // 0: 正确，1: 错误，2: 检测失败
        // 检测到的值
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1024)]
        public string value;
    }
}
