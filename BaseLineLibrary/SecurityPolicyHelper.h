#pragma once
#include <windows.h>
#include <string>

/**
 * 安全策略助手类
 * 使用auditpol.exe命令行工具管理Windows审核策略
 */
class SecurityPolicyHelper {
public:
    static bool GetAuditPolicy(
        const std::string& auditSubcategory,
        bool& auditSuccess, bool& auditFailure);
    //这个函数是用来获取Windows审核策略的
    static bool SetAuditPolicy(
        const std::string& auditSubcategory,
        bool auditSuccess, bool auditFailure);
    //这个函数是用来设置Windows审核策略的
    static bool ExecuteAuditpolCommand(const std::string& command, std::string& output);
    //这个函数是用来执行auditpol.exe命令的
};

std::wstring AnsiToWide(const std::string& str);
std::string WideToAnsi(const std::wstring& wstr);