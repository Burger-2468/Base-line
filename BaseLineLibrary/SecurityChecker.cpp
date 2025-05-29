#include "SecurityChecker.h"
#include <fstream>
#include <regex>

bool ParseRegPath(const std::wstring& fullPath /* fullPath��regPath������ע������� */, HKEY& rootKey, std::wstring& keyPath) {
    // ����ע���·��
    size_t pos = fullPath.find_first_of(L"\\");
    if (pos == std::wstring::npos) pos = fullPath.size() - 1;

    std::wstring rootStr = fullPath.substr(0, pos);
    keyPath = fullPath.substr(pos + 1);
    //��keyPath�еķ�б��תΪ��б��
    if (rootStr == L"HKEY_LOCAL_MACHINE") rootKey = HKEY_LOCAL_MACHINE;
    else if (rootStr == L"HKEY_CURRENT_USER") rootKey = HKEY_CURRENT_USER;
    else return false;
    return true;
}

// ANSI ת���ַ�
std::wstring AnsiToWide(const std::string& str) {
    int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], sizeNeeded);
    return wstr;
}

// ���ַ�ת ANSI
std::string WideToAnsi(const std::wstring& wstr) {
    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    return str;
}


// ��ⵥ�������CheckResult
CheckResult SecurityChecker::CheckOne(CheckResult rule) {
    CheckResult result = rule;

    if (rule.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        //std::wstring wideRegPath = AnsiToWide(rule.regPath);
        if (!ParseRegPath(rule.regPath, rootKey, keyPath)) {
            // ·������ʧ��
            result.currentString = L"·������ʧ��";
            result.isTrueDetect = false;//û��⵽
            return result;
        }
        //std::wstring wideValueName = AnsiToWide(rule.valueName);
        RegistryHelper helper(rootKey, keyPath, rule.valueName);
        result.currentString = AnsiToWide(helper.ReadValue("", true)); // createIfMissing��Ϊtrue�Զ�����·��

        if (rule.valueType == ValueType::DWORD) {
            try {
                DWORD currentValue = std::stoul(result.currentString);
                // ���������׼ֵ����Զ������˿�Ҫ�󲻵���3389��
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
            // �ַ�������ֱ�ӱȽϣ�������·�����˻��б�ȣ�
            result.isCompliant = (result.currentString == rule.standardString);
        }
    }
    else if (rule.checkType == CheckType::AuditPolicy) {
        bool auditSuccess = false;//�ɹ��¼�1:0
        bool auditFailure = false;//ʧ���¼�2:0

        if (SecurityPolicyHelper::GetAuditPolicy(WideToAnsi(rule.auditSubcategory),
            auditSuccess, auditFailure)) {
            DWORD currentValue = (auditSuccess ? 1 : 0) + (auditFailure ? 2 : 0);
            result.currentString = AnsiToWide(std::to_string(currentValue));//�����ַ������͵�����
            result.isCompliant = (currentValue == rule.standardDword);
        }
        else {
            result.currentString = L"�޷���ȡ";
            result.isTrueDetect = false;
        }
    }

    return result;
}



// �޸�����������Ƿ��޸��ɹ�
CheckResult SecurityChecker::RepairOne(CheckResult& result) {
    // ִ���޸����������߼�
    bool repairActionSuccess = false;

    // ����ע��������޸�
    if (result.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        // ����ע���·��Ϊ��������·��
        if (!ParseRegPath(result.regPath, rootKey, keyPath)) {
            result.repairSuccess = false;
            result.currentString = L"ע���·������ʧ��";
            return result;
        }

        // ��ʼ��ע������������
        RegistryHelper helper(rootKey, keyPath, result.valueName);
        // ����ֵ����ִ�в�ͬ���޸�����
        if (result.valueType == ValueType::DWORD) {
            // ��������ֵ!3389���޷��Զ���������˿ڣ�
            repairActionSuccess = (result.standardString == L"!3389") ?
                false : helper.WriteDWORD(result.standardDword, true);
        }
        else if (result.valueType == ValueType::STRING) {
            // ת����׼�ַ���ΪANSI��ʽ��д��ע���
            std::string ansiValue = WideToAnsi(result.standardString);
            repairActionSuccess = helper.WriteString(ansiValue, true);
        }
        // ��¼�޸������Ƿ�ɹ�
        result.repairSuccess = repairActionSuccess;
    }
    // ������˲��������޸�
    else if (result.checkType == CheckType::AuditPolicy) {
        // ������׼DWORDֵΪ�ɹ�/ʧ����˱�־
        bool auditSuccess = (result.standardDword & 1) != 0;
        bool auditFailure = (result.standardDword & 2) != 0;
        // ���ð�ȫ��������������˲���
        repairActionSuccess = SecurityPolicyHelper::SetAuditPolicy(
            WideToAnsi(result.auditCategory), WideToAnsi(result.auditSubcategory), auditSuccess, auditFailure);
        result.repairSuccess = repairActionSuccess;
    }

    // �޸�����ʧ��ʱ���ش���״̬
    if (!repairActionSuccess) {
        result.currentString = L"�޸�����ִ��ʧ��";
        return result;
    }

    // �޸��ɹ�����ü�⺯����֤���
    CheckResult detectedResult = CheckOne(result);

    // �ϲ���������޸������
    // �����޸����ȡ��currentString�������ºϹ��Ժͼ��״̬
    result.isCompliant = detectedResult.isCompliant;
    result.isTrueDetect = detectedResult.isTrueDetect;

    // �����޸��ɹ��������޸������ɹ��Ҽ�����Ϲ�
    result.repairSuccess = repairActionSuccess && result.isCompliant;
    return result;
}






// ���ɱ���
bool SecurityChecker::GenerateReport(const std::vector<CheckResult>& results, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "���,���������,�������,ע���·��,�ֶ���,������,��������,ֵ����,��׼ֵ,��ǰֵ,�Ƿ����,�޸����\n";

    for (size_t i = 0; i < results.size(); i++) {
        const auto& result = results[i];
        std::string checkTypeStr = (result.checkType == CheckType::Registry) ? "ע���" : "��˲���";
        std::string valueTypeStr = (result.valueType == ValueType::DWORD) ? "DWORD" : "STRING";
        std::string standardValue = (result.valueType == ValueType::DWORD) ?
            (result.standardString.empty() ? std::to_string(result.standardDword) : WideToAnsi(result.standardString)) :
            WideToAnsi(result.standardString);

        // ���������׼ֵ��ʾ����!3389��
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
            << (result.isCompliant ? "��" : "��") << ","
            << (result.repairSuccess ? "�ɹ�" : "ʧ��") << "\n";
    }

    file << "\nͳ����Ϣ\n";
    file << "���������," << results.size() << "\n";
    file << "���ϱ�׼��," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.isCompliant; }) << "\n";
    file << "�޸��ɹ���," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.repairSuccess; }) << "\n";

    file.close();
    return true;
}