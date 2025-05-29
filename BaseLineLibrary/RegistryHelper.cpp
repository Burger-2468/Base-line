#include "RegistryHelper.h"
#include <sstream>
#include <vector>

RegistryHelper::RegistryHelper(HKEY rootKey, const std::wstring& keyPath, const std::wstring& valueName)
    : m_rootKey(rootKey), m_keyPath(keyPath), m_valueName(valueName) {
    LONG status = RegOpenKeyExW(m_rootKey, m_keyPath.c_str(), 0, KEY_READ, &m_hKey);
    if (status != ERROR_SUCCESS) m_hKey = nullptr;
}

RegistryHelper::~RegistryHelper() {
    if (m_hKey) RegCloseKey(m_hKey);
}

bool RegistryHelper::CreateKeyPath(HKEY root, const std::wstring& keyPath) {
    if (keyPath.empty()) return true;

    size_t pos = keyPath.find(L'\\');
    std::wstring currentKey = (pos != std::wstring::npos) ? keyPath.substr(0, pos) : keyPath;
    std::wstring remainingPath = (pos != std::wstring::npos) ? keyPath.substr(pos + 1) : L"";

    HKEY hCurrentKey;
    LONG status = RegCreateKeyExW(root, currentKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, nullptr, &hCurrentKey, nullptr);

    if (status != ERROR_SUCCESS) return false;

    bool result = true;
    if (!remainingPath.empty()) {
        result = CreateKeyPath(hCurrentKey, remainingPath);
    }

    RegCloseKey(hCurrentKey);
    return result;
}

std::string RegistryHelper::ReadValue(const std::string& defaultValue, bool createIfMissing) {
    if (!m_hKey && createIfMissing) {
        if (!CreateKeyPath(m_rootKey, m_keyPath)) {
            return defaultValue;
        }

        LONG status = RegOpenKeyExW(m_rootKey, m_keyPath.c_str(), 0,
            KEY_READ | KEY_WRITE, &m_hKey);
        if (status != ERROR_SUCCESS) return defaultValue;
    }

    if (!m_hKey) return defaultValue;

    DWORD type = 0;
    DWORD size = 0;
    LONG status = RegQueryValueExW(m_hKey, m_valueName.c_str(), nullptr, &type, nullptr, &size);
    //m_valueName.c_str()��const char*���ͣ�m_valueName��std::string���ͣ�c_str()����const char*���ͣ����Կ���ֱ����,RegQueryValueExA�ĵڶ���������

    if (status != ERROR_SUCCESS) return defaultValue;

    switch (type) {
    case REG_DWORD: {
        DWORD value = 0;
        status = RegQueryValueExW(m_hKey, m_valueName.c_str(), nullptr,
            &type, reinterpret_cast<LPBYTE>(&value), &size);
        return status == ERROR_SUCCESS ? std::to_string(value) : defaultValue;
    }

    case REG_SZ:
    case REG_EXPAND_SZ: {
        std::vector<char> buffer(size);
        status = RegQueryValueExW(m_hKey, m_valueName.c_str(), nullptr,
            &type, reinterpret_cast<LPBYTE>(buffer.data()), &size);
        return status == ERROR_SUCCESS ? std::string(buffer.data(), size - 1) : defaultValue;
    }

    default:
        return defaultValue;
    }
}

bool RegistryHelper::WriteDWORD(DWORD value, bool createIfMissing) {
    if (!m_hKey && createIfMissing) {
        if (!CreateKeyPath(m_rootKey, m_keyPath)) {
            return false;
        }

        LONG status = RegOpenKeyExW(m_rootKey, m_keyPath.c_str(), 0,
            KEY_READ | KEY_WRITE, &m_hKey);
        if (status != ERROR_SUCCESS) return false;
    }

    if (!m_hKey) return false;

    return RegSetValueExW(m_hKey, m_valueName.c_str(), 0, REG_DWORD,
        reinterpret_cast<const BYTE*>(&value), sizeof(DWORD)) == ERROR_SUCCESS;
}

bool RegistryHelper::WriteString(const std::string& value, bool createIfMissing) {
    if (!m_hKey && createIfMissing) {
        if (!CreateKeyPath(m_rootKey, m_keyPath)) {
            return false;
        }

        LONG status = RegOpenKeyExW(m_rootKey, m_keyPath.c_str(), 0,
            KEY_READ | KEY_WRITE, &m_hKey);
        if (status != ERROR_SUCCESS) return false;
    }

    if (!m_hKey) return false;

    // ת��Ϊ���ַ�
    int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, nullptr, 0);
    if (sizeNeeded <= 0) return false;

    std::vector<wchar_t> wideStr(sizeNeeded);
    MultiByteToWideChar(CP_UTF8, 0, value.c_str(), -1, wideStr.data(), sizeNeeded);

    int status = RegSetValueExW(m_hKey, m_valueName.c_str(), 0, REG_SZ,
        reinterpret_cast<const BYTE*>(wideStr.data()), sizeNeeded * sizeof(wchar_t));
	return status == ERROR_SUCCESS;
    //m_hKey��HKEY���ͣ�RegSetValueExA��RegSetValueExW����������͵Ĳ��������Կ���ֱ����
    //m_valueName.c_str()��const char*���ͣ����Կ���ֱ����

}