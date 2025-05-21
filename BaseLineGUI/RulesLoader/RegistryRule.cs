namespace BaseLineGUI.RulesLoader
{
    public class RegistryRule
    {
        private readonly string itemName;
        private readonly string registryPath;
        private readonly string registryName;
        private readonly string valueType;
        private readonly string expectedValue;

        public RegistryRule(string itemName, string registryPath, string registryName, string valueType, string expectedValue)
        {
            this.itemName = itemName;
            this.registryPath = registryPath;
            this.registryName = registryName;
            this.valueType = valueType;
            this.expectedValue = expectedValue;
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
    }
