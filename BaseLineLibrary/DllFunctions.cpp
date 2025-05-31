#include "DllFunctions.h"
#include <string>
#include <iostream>
#include <sstream>

// ���ò���ȫ��������
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

// �����ַ���ת��ΪANSI�ַ���
std::string WideToAnsi(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
    
    return strTo;
}

// ��ANSI�ַ���ת��Ϊ���ַ���
std::wstring AnsiToWide(const std::string& str) {
    if (str.empty()) return std::wstring();
    
    int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    
    return wstrTo;
}

// ����ע���·������ȡ�������Ӽ�
bool ParseRegistryPath(const std::string& registryPath, HKEY& hRootKey, std::wstring& subKey) {
    // �ָ�ע���·������ȡ�������Ӽ�
    size_t pos = registryPath.find('\\');
    if (pos == std::string::npos) {
        return false;
    }
    
    std::string rootKey = registryPath.substr(0, pos);
    std::string subKeyStr = registryPath.substr(pos + 1);
    
    // ת���Ӽ�Ϊ���ַ���
    subKey = AnsiToWide(subKeyStr);
    
    // ȷ������
    if (rootKey == "HKEY_CURRENT_USER" || rootKey == "HKCU") {
        hRootKey = HKEY_CURRENT_USER;
    } else if (rootKey == "HKEY_LOCAL_MACHINE" || rootKey == "HKLM") {
        hRootKey = HKEY_LOCAL_MACHINE;
    } else if (rootKey == "HKEY_CLASSES_ROOT" || rootKey == "HKCR") {
        hRootKey = HKEY_CLASSES_ROOT;
    } else if (rootKey == "HKEY_USERS" || rootKey == "HKU") {
        hRootKey = HKEY_USERS;
    } else if (rootKey == "HKEY_CURRENT_CONFIG" || rootKey == "HKCC") {
        hRootKey = HKEY_CURRENT_CONFIG;
    } else {
        return false;
    }
    
    return true;
}

extern "C" __declspec(dllexport) void CheckRegistryRule(const char* registryPath, const char* itemName, 
                                                       const char* itemType, const char* expectedValue, 
                                                       CheckResult_CSharp* result) {
    // ��ʼ�����
    result->status = 4; // Ĭ��Ϊ��������
    strcpy(result->value, "");

	if (!result) return; // �����ָ���Ƿ���Ч
    
    // �������
    if (!registryPath || !itemName || !itemType || !expectedValue) {
        strcpy(result->value, "������Ч");
        return;
    }
    
    // ����ע���·��
    HKEY hRootKey;
    std::wstring subKey;
    if (!ParseRegistryPath(registryPath, hRootKey, subKey)) {
        result->status = 4;
        strcpy(result->value, "ע���·����ʽ����");
        return;
    }
    
    // ��ע����
    HKEY hKey;
    LONG lRes = RegOpenKeyExW(hRootKey, subKey.c_str(), 0, KEY_READ, &hKey);
    if (lRes != ERROR_SUCCESS) {
        result->status = 2; // ע���·��������
        strcpy(result->value, "ע���·��������");
        return;
    }
    
    // ת��������Ϊ���ַ�
    std::wstring wItemName = AnsiToWide(itemName);
    
    // ȷ��ֵ����
    DWORD dwType;
    if (strcmp(itemType, "REG_SZ") == 0) {
        dwType = REG_SZ;
    } else if (strcmp(itemType, "REG_DWORD") == 0) {
        dwType = REG_DWORD;
    } else {
        RegCloseKey(hKey);
        result->status = 4;
        strcpy(result->value, "��֧�ֵ�ע���ֵ����");
        return;
    }
    
    // ��ѯע���ֵ
    if (dwType == REG_SZ) {
        // ��ȡ�ַ���ֵ�Ĵ�С
        DWORD dataSize = 0;
        lRes = RegQueryValueExW(hKey, wItemName.c_str(), nullptr, &dwType, nullptr, &dataSize);
        if (lRes != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            result->status = 3; // ע���·�����ڵ�ע��������
            strcpy(result->value, "ע��������");
            return;
        }
        
        // ȷ������ƥ��
        if (dwType != REG_SZ && dwType != REG_EXPAND_SZ) {
            RegCloseKey(hKey);
            result->status = 4;
            strcpy(result->value, "ע���ֵ���Ͳ�ƥ��");
            return;
        }
        
        // ��ȡ�ַ���ֵ
        std::vector<wchar_t> data(dataSize / sizeof(wchar_t) + 1);
        lRes = RegQueryValueExW(hKey, wItemName.c_str(), nullptr, &dwType, 
                              reinterpret_cast<LPBYTE>(data.data()), &dataSize);
        RegCloseKey(hKey);
        
        if (lRes != ERROR_SUCCESS) {
            result->status = 4;
            strcpy(result->value, "��ȡע���ֵʧ��");
            return;
        }
        
        // ȷ���ַ�����null��β
        data[dataSize / sizeof(wchar_t)] = L'\0';
        
        // ת��ΪANSI�ַ������Ƚ�
        std::string currentValue = WideToAnsi(data.data());
        strcpy(result->value, currentValue.c_str());
        
        // �Ƚ�ֵ
        if (currentValue == expectedValue) {
            result->status = 0; // ֵƥ��
        } else {
            result->status = 1; // ֵ��ƥ��
        }
    } else if (dwType == REG_DWORD) {
        // ��ȡDWORDֵ
        DWORD data = 0;
        DWORD dataSize = sizeof(DWORD);
        lRes = RegQueryValueExW(hKey, wItemName.c_str(), nullptr, &dwType, 
                              reinterpret_cast<LPBYTE>(&data), &dataSize);
        
        if (lRes != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            result->status = 3; // ע���·�����ڵ�ע��������
            strcpy(result->value, "ע��������");
            return;
        }
        
        RegCloseKey(hKey);
        
        // ȷ������ƥ��
        if (dwType != REG_DWORD) {
            result->status = 4;
            strcpy(result->value, "ע���ֵ���Ͳ�ƥ��");
            return;
        }
        
        // ת������ֵΪDWORD
        DWORD expectedDword = 0;
        try {
            expectedDword = static_cast<DWORD>(std::stoul(expectedValue));
        } catch (...) {
            result->status = 4;
            strcpy(result->value, "����ֵ��ʽ����");
            return;
        }
        
        // ����ǰֵת��Ϊ�ַ���
        std::stringstream ss;
        ss << data;
        strcpy(result->value, ss.str().c_str());
        
        // �Ƚ�ֵ
        if (data == expectedDword) {
            result->status = 0; // ֵƥ��
        } else {
            result->status = 1; // ֵ��ƥ��
        }
    }
}


extern "C" __declspec(dllexport) void FixRegistryRule(const char* registryPath, const char* itemName, 
                                                    const char* itemType, const char* expectedValue, 
                                                    CheckResult_CSharp* result) {
    // ��ʼ�����
    result->status = 1; // Ĭ��Ϊ�޸�ʧ��
    strcpy(result->value, "");
    
    // �������
    if (!registryPath || !itemName || !itemType || !expectedValue || !result) {
        return;
    }
    
    // ����ע���·��
    HKEY hRootKey;
    std::wstring subKey;
    if (!ParseRegistryPath(registryPath, hRootKey, subKey)) {
        return;
    }
    
    // ��ע����������������򴴽�
    HKEY hKey;
    LONG lRes = RegCreateKeyExW(hRootKey, subKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, 
                              KEY_WRITE, nullptr, &hKey, nullptr);
    if (lRes != ERROR_SUCCESS) {
        return;
    }
    
    // ת��������Ϊ���ַ�
    std::wstring wItemName = AnsiToWide(itemName);
    
    // ��������д��ֵ
    bool writeSuccess = false;
    
    if (strcmp(itemType, "REG_SZ") == 0) {
        // ת������ֵΪ���ַ���
        std::wstring wExpectedValue = AnsiToWide(expectedValue);
        
        // д���ַ���ֵ
        lRes = RegSetValueExW(hKey, wItemName.c_str(), 0, REG_SZ, 
                            reinterpret_cast<const BYTE*>(wExpectedValue.c_str()), 
                            static_cast<DWORD>((wExpectedValue.length() + 1) * sizeof(wchar_t)));
        
        writeSuccess = (lRes == ERROR_SUCCESS);
    } 
    else if (strcmp(itemType, "REG_DWORD") == 0) {
        // ת������ֵΪDWORD
        DWORD dwValue = 0;
        try {
            dwValue = static_cast<DWORD>(std::stoul(expectedValue));
        } 
        catch (...) {
            RegCloseKey(hKey);
            return;
        }
        
        // д��DWORDֵ
        lRes = RegSetValueExW(hKey, wItemName.c_str(), 0, REG_DWORD, 
                            reinterpret_cast<const BYTE*>(&dwValue), sizeof(DWORD));
        
        writeSuccess = (lRes == ERROR_SUCCESS);
    }
    
    // �ر�ע����
    RegCloseKey(hKey);
    
    // ���÷���״̬
    if (writeSuccess) {
        result->status = 0; // �޸��ɹ�
    }
}


extern "C" __declspec(dllexport) void CheckAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result) {
    // ��ʼ�����
    result->status = 4; // Ĭ��Ϊ��������
    strcpy(result->value, "δʵ�ֵĹ���");
    
    // TODO: ʵ����Ʋ��Լ�鹦��
}

extern "C" __declspec(dllexport) void FixAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result) {
    // ��ʼ�����
    result->status = 1; // Ĭ��Ϊ�޸�ʧ��
    strcpy(result->value, "δʵ�ֵĹ���");
    
    // TODO: ʵ����Ʋ����޸�����
}