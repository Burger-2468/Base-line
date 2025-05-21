#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include "RegistryHelper.h"
#include "SecurityPolicyHelper.h"

enum class ValueType { DWORD, STRING };
enum class CheckType { Registry, AuditPolicy };

struct CheckResult {
    std::wstring name;           // 检查项名称
    CheckType checkType;        // 检查类型（注册表或审核策略）
    //
    std::wstring regPath;        // 注册表路径（仅CheckType为Registry时有效）
    std::wstring valueName;      // 注册表值名称（仅CheckType为Registry时有效）
    //
    std::wstring auditCategory;  // 审核策略类别（仅CheckType为AuditPolicy时有效）
    std::wstring auditSubcategory; // 审核策略子类别（仅CheckType为AuditPolicy时有效）
    //
    ValueType valueType;        // 值类型（DWORD或STRING）
    DWORD standardDword;        // 标准DWORD值（字符串类型时无效）
    std::wstring standardString; // 标准字符串值（DWORD类型时无效）
    //
    std::wstring currentString;  // 当前值（字符串形式）
    bool isCompliant;           // 是否符合标准
    bool repairSuccess;         // 修复是否成功
};

class SecurityChecker {
public:
    SecurityChecker();// 构造函数
    std::vector<CheckResult> CheckAll();// 检查所有项并返回结果
    bool RepairAll(std::vector<CheckResult>& results);// 修复所有项并返回结果

    static CheckResult CheckOne(CheckResult rule); // 检查单个项并返回结果
    static bool RepairOne(CheckResult& result);// 修复单个项并返回结果

    bool GenerateReport(const std::vector<CheckResult>& results, const std::string& filePath);// 生成报告并保存到文件

private:
    std::vector<CheckResult> m_checkItems;
};