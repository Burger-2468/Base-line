using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BaseLineGUI.StateStorage
{
    public abstract class RuleItem
    {
        private bool isSelectedToFix = false;
        private CheckResult checkResult = CheckResult.NotChecked;
        private string detectedValue = "";

        public bool IsSelectedToFix
        {
            get { return isSelectedToFix; }
            set { isSelectedToFix = value; }
        }

        public CheckResult CheckResult
        {
            get { return checkResult; }
            set { checkResult = value; }
        }

        public string DetectedValue
        {
            get { return detectedValue; }
            set { detectedValue = value; }
        }
    }
}
