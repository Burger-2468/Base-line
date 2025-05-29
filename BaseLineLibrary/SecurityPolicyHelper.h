#pragma once
#include <windows.h>
#include <string>

/**
 * ��ȫ����������
 * ʹ��auditpol.exe�����й��߹���Windows��˲���
 */
class SecurityPolicyHelper {
public:
    static bool GetAuditPolicy(
        const std::string& auditSubcategory,
        bool& auditSuccess, bool& auditFailure);
    //���������������ȡWindows��˲��Ե�
    static bool SetAuditPolicy(
        const std::string& auditSubcategory,
        bool auditSuccess, bool auditFailure);
    //�����������������Windows��˲��Ե�
    static bool ExecuteAuditpolCommand(const std::string& command, std::string& output);
    //�������������ִ��auditpol.exe�����
};

std::wstring AnsiToWide(const std::string& str);
std::string WideToAnsi(const std::wstring& wstr);