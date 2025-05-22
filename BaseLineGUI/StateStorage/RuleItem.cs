using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.StateStorage
{
    /// <summary>
    /// 程序运行时存储的规则列表中的规则项，包含了运行时需要的状态，并用于界面显示
    /// </summary>
    public abstract class RuleItem
    {
        private readonly string itemName;
        private bool isSelectedToFix = false;
        private CheckResult checkResult = CheckResult.NotChecked;
        private readonly string page;

        protected RuleItem(string itemName, string page)
        {
            this.itemName = itemName;
            this.page = page;
        }

        /// <summary>
        /// 规则项名称
        /// </summary>
        public string ItemName
        {
            get { return this.itemName; }
        }

        /// <summary>
        /// 用户是否选中该项进行修复
        /// </summary>
        public bool IsSelectedToFix
        {
            get { return isSelectedToFix; }
            set { isSelectedToFix = value; }
        }

        /// <summary>
        /// 规则项检查结果
        /// </summary>
        public CheckResult CheckResult
        {
            get { return checkResult; }
            set { checkResult = value; }
        }

        /// <summary>
        /// 规则项所属的类别
        /// </summary>
        public string Page
        {
            get { return page; }
        }
    }
}
