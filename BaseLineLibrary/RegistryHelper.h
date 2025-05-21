#pragma once
#include <windows.h>
#include <string>

/**
 * ע������������
 * �ṩ��ȫ������Windowsע����д����
 */
class RegistryHelper {
public:
    RegistryHelper(HKEY rootKey, const std::wstring& keyPath, const std::wstring& valueName);
    ~RegistryHelper();

    std::string ReadValue(const std::string& defaultValue = "", bool createIfMissing = false);
    bool WriteDWORD(DWORD value, bool createIfMissing = false);
    bool WriteString(const std::string& value, bool createIfMissing = false);

private:
    HKEY m_rootKey;
    std::wstring m_keyPath;
    std::wstring m_valueName;
    HKEY m_hKey = nullptr;

    bool CreateKeyPath(HKEY root, const std::wstring& keyPath);
};