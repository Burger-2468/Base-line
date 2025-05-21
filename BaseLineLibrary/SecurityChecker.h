#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include "RegistryHelper.h"
#include "SecurityPolicyHelper.h"

enum class ValueType { DWORD, STRING };
enum class CheckType { Registry, AuditPolicy };

struct CheckResult {
    std::wstring name;           // ���������
    CheckType checkType;        // ������ͣ�ע������˲��ԣ�
    //
    std::wstring regPath;        // ע���·������CheckTypeΪRegistryʱ��Ч��
    std::wstring valueName;      // ע���ֵ���ƣ���CheckTypeΪRegistryʱ��Ч��
    //
    std::wstring auditCategory;  // ��˲�����𣨽�CheckTypeΪAuditPolicyʱ��Ч��
    std::wstring auditSubcategory; // ��˲�������𣨽�CheckTypeΪAuditPolicyʱ��Ч��
    //
    ValueType valueType;        // ֵ���ͣ�DWORD��STRING��
    DWORD standardDword;        // ��׼DWORDֵ���ַ�������ʱ��Ч��
    std::wstring standardString; // ��׼�ַ���ֵ��DWORD����ʱ��Ч��
    //
    std::wstring currentString;  // ��ǰֵ���ַ�����ʽ��
    bool isCompliant;           // �Ƿ���ϱ�׼
    bool repairSuccess;         // �޸��Ƿ�ɹ�
};

class SecurityChecker {
public:
    SecurityChecker();// ���캯��
    std::vector<CheckResult> CheckAll();// �����������ؽ��
    bool RepairAll(std::vector<CheckResult>& results);// �޸���������ؽ��

    static CheckResult CheckOne(CheckResult rule); // ��鵥������ؽ��
    static bool RepairOne(CheckResult& result);// �޸���������ؽ��

    bool GenerateReport(const std::vector<CheckResult>& results, const std::string& filePath);// ���ɱ��沢���浽�ļ�

private:
    std::vector<CheckResult> m_checkItems;
};