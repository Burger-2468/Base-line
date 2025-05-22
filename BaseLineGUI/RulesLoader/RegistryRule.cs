using BaseLineGUI.StateStorage;

namespace BaseLineGUI.RulesLoader
{
    public class RegistryRule : RuleItem
    {
        private readonly string itemName;
        private readonly string registryPath;
        private readonly string registryName;
        private readonly string valueType;
        private readonly string expectedValue;
        private readonly string page;

        public RegistryRule(string itemName, string registryPath, string registryName, string valueType, string expectedValue, string page)
        {
            this.itemName = itemName;
            this.registryPath = registryPath;
            this.registryName = registryName;
            this.valueType = valueType;
            this.expectedValue = expectedValue;
            this.page = page;
        }

        public string ItemName
        {
            get { return itemName; }
        }

        public string RegistryPath
        {
            get { return registryPath; }
        }

        public string RegistryName
        {
            get { return registryName; }
        }

        public string ValueType
        {
            get { return valueType; }
        }
        public string ExpectedValue
        {
            get { return expectedValue; }
        }

        public string Page
        {
            get { return page; }
        }
    }
}
