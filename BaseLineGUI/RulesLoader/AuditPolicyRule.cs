using BaseLineGUI.StateStorage;

namespace BaseLineGUI.RulesLoader
{
    /// <summary>
    /// 审计策略规则项
    /// </summary>
    public class AuditPolicyRule : RuleItem
    {
        private readonly string subCategory;
        private readonly int expectedValue;

        public AuditPolicyRule(string itemName, string subCategory, int expectedValue, string page) : base(itemName, page)
        {
            this.subCategory = subCategory;
            this.expectedValue = expectedValue;
        }

        /// <summary>
        /// 审计策略子类别，用GUID表示
        /// </summary>
        public string SubCategory
        {
            get { return subCategory; }
        }

        /// <summary>
        /// 期望值，0为无审核，1为成功审核，2为失败审核，3为成功和失败审核。获取字符串形式请使用ExpectedValueString属性
        /// </summary>
        public int ExpectedValue
        {
            get { return expectedValue; }
        }

        /// <summary>
        /// 获取期望值的字符串形式
        /// </summary>
        public string ExpectedValueString
        {
            get
            {
                switch (expectedValue)
                {
                    case 0:
                        return "无审核";
                    case 1:
                        return "成功审核";
                    case 2:
                        return "失败审核";
                    case 3:
                        return "成功和失败审核";
                    default:
                        return "未知";
                }
            }
        }
    }
}
