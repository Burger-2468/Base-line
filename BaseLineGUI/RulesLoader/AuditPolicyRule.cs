namespace BaseLineGUI.RulesLoader
{
    public class AuditPolicyRule
    {
        private readonly string itemName;
        private readonly string category;
        private readonly string subCategory;
        private readonly int expectedValue;

        public AuditPolicyRule(string itemName, string category, string subCategory, int expectedValue)
        {
            this.itemName = itemName;
            this.category = category;
            this.subCategory = subCategory;
            this.expectedValue = expectedValue;
        }

        public string ItemName
        {
            get { return itemName; }
        }

        public string Category
        {
            get { return category; }
        }

        public string SubCategory
        {
            get { return subCategory; }
        }

        public int ExpectedValue
        {
            get { return expectedValue; }
        }
    }
}
