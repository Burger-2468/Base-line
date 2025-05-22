using BaseLineGUI.StateStorage;

namespace BaseLineGUI.RulesLoader
{
    public class AuditPolicyRule : RuleItem
    {
        private readonly string itemName;
        private readonly string subCategory;
        private readonly int expectedValue;
        private readonly string page;

        public AuditPolicyRule(string itemName, string subCategory, int expectedValue, string page)
        {
            this.itemName = itemName;
            this.subCategory = subCategory;
            this.expectedValue = expectedValue;
            this.page = page;
        }

        public string ItemName
        {
            get { return itemName; }
        }

        public string SubCategory
        {
            get { return subCategory; }
        }

        public int ExpectedValue
        {
            get { return expectedValue; }
        }

        public string Page
        {
            get { return page; }
        }
    }
}
