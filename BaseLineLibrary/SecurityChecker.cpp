#include "SecurityChecker.h"
#include <fstream>
#include <regex>

bool ParseRegPath(const std::wstring& fullPath /* fullPath是regPath，不含注册表项名 */, HKEY& rootKey, std::wstring& keyPath) {
    // 解析注册表路径
    size_t pos = fullPath.find_first_of(L"\\");
    if (pos == std::wstring::npos) pos = fullPath.size() - 1;

    std::wstring rootStr = fullPath.substr(0, pos);
    keyPath = fullPath.substr(pos + 1);
    //把keyPath中的反斜杠转为正斜杠
    if (rootStr == L"HKEY_LOCAL_MACHINE") rootKey = HKEY_LOCAL_MACHINE;
    else if (rootStr == L"HKEY_CURRENT_USER") rootKey = HKEY_CURRENT_USER;
    else return false;
    return true;
}

// ANSI 转宽字符
std::wstring AnsiToWide(const std::string& str) {
    int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], sizeNeeded);
    return wstr;
}

// 宽字符转 ANSI
std::string WideToAnsi(const std::wstring& wstr) {
    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    return str;
}


// 检测单个项，返回CheckResult
CheckResult SecurityChecker::CheckOne(CheckResult rule) {
    CheckResult result = rule;

    if (rule.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        //std::wstring wideRegPath = AnsiToWide(rule.regPath);
        if (!ParseRegPath(rule.regPath, rootKey, keyPath)) {
            // 路径解析失败
            result.currentString = L"路径解析失败";
            result.isTrueDetect = false;//没检测到
            return result;
        }
        //std::wstring wideValueName = AnsiToWide(rule.valueName);
        RegistryHelper helper(rootKey, keyPath, rule.valueName);
        result.currentString = AnsiToWide(helper.ReadValue("", true)); // createIfMissing设为true自动创建路径

        if (rule.valueType == ValueType::DWORD) {
            try {
                DWORD currentValue = std::stoul(result.currentString);
                // 处理特殊标准值（如远程桌面端口要求不等于3389）
                if (rule.standardString == L"!3389") {
                    result.isCompliant = (currentValue != 3389);
                }
                else {
                    result.isCompliant = (currentValue == rule.standardDword);
                }
            }
            catch (...) {
                result.isTrueDetect = false;
            }
        }
        else {
            // 字符串类型直接比较（适用于路径、账户列表等）
            result.isCompliant = (result.currentString == rule.standardString);
        }
    }
    else if (rule.checkType == CheckType::AuditPolicy) {
        bool auditSuccess = false;//成功事件1:0
        bool auditFailure = false;//失败事件2:0

        if (SecurityPolicyHelper::GetAuditPolicy(WideToAnsi(rule.auditSubcategory),
            auditSuccess, auditFailure)) {
            DWORD currentValue = (auditSuccess ? 1 : 0) + (auditFailure ? 2 : 0);
            result.currentString = AnsiToWide(std::to_string(currentValue));//返回字符串类型的数字
            result.isCompliant = (currentValue == rule.standardDword);
        }
        else {
            result.currentString = L"无法获取";
            result.isTrueDetect = false;
        }
    }

    return result;
}



// 修复单个项，返回是否修复成功
CheckResult SecurityChecker::RepairOne(CheckResult& result) {
    // 执行修复操作主体逻辑
    bool repairActionSuccess = false;

    // 处理注册表类型修复
    if (result.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        // 解析注册表路径为根键和子路径
        if (!ParseRegPath(result.regPath, rootKey, keyPath)) {
            result.repairSuccess = false;
            result.currentString = L"注册表路径解析失败";
            return result;
        }

        // 初始化注册表操作助手类
        RegistryHelper helper(rootKey, keyPath, result.valueName);
        // 根据值类型执行不同的修复操作
        if (result.valueType == ValueType::DWORD) {
            // 处理特殊值!3389（无法自动生成随机端口）
            repairActionSuccess = (result.standardString == L"!3389") ?
                false : helper.WriteDWORD(result.standardDword, true);
        }
        else if (result.valueType == ValueType::STRING) {
            // 转换标准字符串为ANSI格式并写入注册表
            std::string ansiValue = WideToAnsi(result.standardString);
            repairActionSuccess = helper.WriteString(ansiValue, true);
        }
        // 记录修复动作是否成功
        result.repairSuccess = repairActionSuccess;
    }
    // 处理审核策略类型修复
    else if (result.checkType == CheckType::AuditPolicy) {
        // 解析标准DWORD值为成功/失败审核标志
        bool auditSuccess = (result.standardDword & 1) != 0;
        bool auditFailure = (result.standardDword & 2) != 0;
        // 调用安全策略助手设置审核策略
        repairActionSuccess = SecurityPolicyHelper::SetAuditPolicy(
            WideToAnsi(result.auditCategory), WideToAnsi(result.auditSubcategory), auditSuccess, auditFailure);
        result.repairSuccess = repairActionSuccess;
    }

    // 修复动作失败时返回错误状态
    if (!repairActionSuccess) {
        result.currentString = L"修复动作执行失败";
        return result;
    }

    // 修复成功后调用检测函数验证结果
    CheckResult detectedResult = CheckOne(result);

    // 合并检测结果到修复结果中
    // 保留修复后读取的currentString，仅更新合规性和检测状态
    result.isCompliant = detectedResult.isCompliant;
    result.isTrueDetect = detectedResult.isTrueDetect;

    // 最终修复成功条件：修复动作成功且检测结果合规
    result.repairSuccess = repairActionSuccess && result.isCompliant;
    return result;
}






// 生成报告
bool SecurityChecker::GenerateReport(const std::vector<CheckResult>& results, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "序号,检测项名称,检测类型,注册表路径,字段名,审核类别,审核子类别,值类型,标准值,当前值,是否符合,修复结果\n";

    for (size_t i = 0; i < results.size(); i++) {
        const auto& result = results[i];
        std::string checkTypeStr = (result.checkType == CheckType::Registry) ? "注册表" : "审核策略";
        std::string valueTypeStr = (result.valueType == ValueType::DWORD) ? "DWORD" : "STRING";
        std::string standardValue = (result.valueType == ValueType::DWORD) ?
            (result.standardString.empty() ? std::to_string(result.standardDword) : WideToAnsi(result.standardString)) :
            WideToAnsi(result.standardString);

        // 处理特殊标准值显示（如!3389）
        if (result.valueType == ValueType::DWORD && !result.standardString.empty()) {
            standardValue = WideToAnsi(result.standardString);
        }

        file << i + 1 << ","
            << WideToAnsi(result.name) << ","
            << checkTypeStr << ","
            << WideToAnsi(result.regPath) << ","
            << WideToAnsi(result.valueName) << ","
            << WideToAnsi(result.auditCategory) << ","
            << WideToAnsi(result.auditSubcategory) << ","
            << valueTypeStr << ","
            << standardValue << ","
            << WideToAnsi(result.currentString) << ","
            << (result.isCompliant ? "是" : "否") << ","
            << (result.repairSuccess ? "成功" : "失败") << "\n";
    }

    file << "\n统计信息\n";
    file << "检测项总数," << results.size() << "\n";
    file << "符合标准项," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.isCompliant; }) << "\n";
    file << "修复成功项," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.repairSuccess; }) << "\n";

    file.close();
    return true;
}