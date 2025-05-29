using BaseLineGUI.StateStorage;

namespace BaseLineGUI.RulesLoader
{
    /// <summary>
    /// 注册表规则项
    /// </summary>
    public class RegistryRule : RuleItem//意思是继承RuleItem
    {
        private readonly string registryPath;
        private readonly string registryName;
        private readonly string valueType;
        private readonly string expectedValue;
        private string detectedValue = "(未知)";

        public RegistryRule(string itemName, string registryPath, string registryName, string valueType, string expectedValue, string page)
            : base(itemName, page)
        {
            this.registryPath = registryPath;
            this.registryName = registryName;
            this.valueType = valueType;
            this.expectedValue = expectedValue;
        }

        /// <summary>
        /// 注册表项路径
        /// </summary>
        public string RegistryPath
        {
            get { return registryPath; }
        }

        /// <summary>
        /// 注册表项名称
        /// </summary>
        public string RegistryName
        {
            get { return registryName; }
        }

        /// <summary>
        /// 注册表项值类型（REG_SZ，REG_DWORD等等）
        /// </summary>
        public string ValueType
        {
            get { return valueType; }
        }

        /// <summary>
        /// 期望值
        /// </summary>
        public string ExpectedValue
        {
            get { return expectedValue; }
        }

        /// <summary>
        /// 检测到的值
        /// </summary>
        public string DetectedValue
        {
            get { return detectedValue; }
            set { detectedValue = value; }
        }
    }
}
