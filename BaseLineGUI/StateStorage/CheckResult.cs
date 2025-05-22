using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.StateStorage
{
    public enum CheckResult
    {
        NotChecked,
        Passed,
        NotPassed,
        Failed,
        Fixed,
        FixFailed
    }

    public class CheckResultClass
    {
        public static string GetCheckResultName(CheckResult result)
        {
            switch (result)
            {
                case CheckResult.NotChecked:
                    return "未检测";
                case CheckResult.Passed:
                    return "通过";
                case CheckResult.NotPassed:
                    return "未通过";
                case CheckResult.Failed:
                    return "检测失败";
                case CheckResult.Fixed:
                    return "已修复";
                case CheckResult.FixFailed:
                    return "修复失败";
                default:
                    return "未知";
            }
        }
    }
}
