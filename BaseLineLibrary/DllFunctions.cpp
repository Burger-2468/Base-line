#include "DllFunctions.h"
#include <string>
#include <iostream>
#include <sstream>

// 禁用不安全函数警告
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

// 将宽字符串转换为ANSI字符串
std::string WideToAnsi(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
    
    return strTo;
}

// 将ANSI字符串转换为宽字符串
std::wstring AnsiToWide(const std::string& str) {
    if (str.empty()) return std::wstring();
    
    int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    
    return wstrTo;
}

// 解析注册表路径，获取根键和子键
bool ParseRegistryPath(const std::string& registryPath, HKEY& hRootKey, std::wstring& subKey) {
    // 分割注册表路径，获取根键和子键
    size_t pos = registryPath.find('\\');
    if (pos == std::string::npos) {
        return false;
    }
    
    std::string rootKey = registryPath.substr(0, pos);
    std::string subKeyStr = registryPath.substr(pos + 1);
    
    // 转换子键为宽字符串
    subKey = AnsiToWide(subKeyStr);
    
    // 确定根键
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
    // 初始化结果
    result->status = 4; // 默认为其他错误
    strcpy(result->value, "");

	if (!result) return; // 检查结果指针是否有效
    
    // 参数检查
    if (!registryPath || !itemName || !itemType || !expectedValue) {
        strcpy(result->value, "参数无效");
        return;
    }
    
    // 解析注册表路径
    HKEY hRootKey;
    std::wstring subKey;
    if (!ParseRegistryPath(registryPath, hRootKey, subKey)) {
        result->status = 4;
        strcpy(result->value, "注册表路径格式错误");
        return;
    }
    
    // 打开注册表键
    HKEY hKey;
    LONG lRes = RegOpenKeyExW(hRootKey, subKey.c_str(), 0, KEY_READ, &hKey);
    if (lRes != ERROR_SUCCESS) {
        result->status = 2; // 注册表路径不存在
        strcpy(result->value, "注册表路径不存在");
        return;
    }
    
    // 转换项名称为宽字符
    std::wstring wItemName = AnsiToWide(itemName);
    
    // 确定值类型
    DWORD dwType;
    if (strcmp(itemType, "REG_SZ") == 0) {
        dwType = REG_SZ;
    } else if (strcmp(itemType, "REG_DWORD") == 0) {
        dwType = REG_DWORD;
    } else {
        RegCloseKey(hKey);
        result->status = 4;
        strcpy(result->value, "不支持的注册表值类型");
        return;
    }
    
    // 查询注册表值
    if (dwType == REG_SZ) {
        // 获取字符串值的大小
        DWORD dataSize = 0;
        lRes = RegQueryValueExW(hKey, wItemName.c_str(), nullptr, &dwType, nullptr, &dataSize);
        if (lRes != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            result->status = 3; // 注册表路径存在但注册表项不存在
            strcpy(result->value, "注册表项不存在");
            return;
        }
        
        // 确保类型匹配
        if (dwType != REG_SZ && dwType != REG_EXPAND_SZ) {
            RegCloseKey(hKey);
            result->status = 4;
            strcpy(result->value, "注册表值类型不匹配");
            return;
        }
        
        // 读取字符串值
        std::vector<wchar_t> data(dataSize / sizeof(wchar_t) + 1);
        lRes = RegQueryValueExW(hKey, wItemName.c_str(), nullptr, &dwType, 
                              reinterpret_cast<LPBYTE>(data.data()), &dataSize);
        RegCloseKey(hKey);
        
        if (lRes != ERROR_SUCCESS) {
            result->status = 4;
            strcpy(result->value, "读取注册表值失败");
            return;
        }
        
        // 确保字符串以null结尾
        data[dataSize / sizeof(wchar_t)] = L'\0';
        
        // 转换为ANSI字符串并比较
        std::string currentValue = WideToAnsi(data.data());
        strcpy(result->value, currentValue.c_str());
        
        // 比较值
        if (currentValue == expectedValue) {
            result->status = 0; // 值匹配
        } else {
            result->status = 1; // 值不匹配
        }
    } else if (dwType == REG_DWORD) {
        // 读取DWORD值
        DWORD data = 0;
        DWORD dataSize = sizeof(DWORD);
        lRes = RegQueryValueExW(hKey, wItemName.c_str(), nullptr, &dwType, 
                              reinterpret_cast<LPBYTE>(&data), &dataSize);
        
        if (lRes != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            result->status = 3; // 注册表路径存在但注册表项不存在
            strcpy(result->value, "注册表项不存在");
            return;
        }
        
        RegCloseKey(hKey);
        
        // 确保类型匹配
        if (dwType != REG_DWORD) {
            result->status = 4;
            strcpy(result->value, "注册表值类型不匹配");
            return;
        }
        
        // 转换期望值为DWORD
        DWORD expectedDword = 0;
        try {
            expectedDword = static_cast<DWORD>(std::stoul(expectedValue));
        } catch (...) {
            result->status = 4;
            strcpy(result->value, "期望值格式错误");
            return;
        }
        
        // 将当前值转换为字符串
        std::stringstream ss;
        ss << data;
        strcpy(result->value, ss.str().c_str());
        
        // 比较值
        if (data == expectedDword) {
            result->status = 0; // 值匹配
        } else {
            result->status = 1; // 值不匹配
        }
    }
}


extern "C" __declspec(dllexport) void FixRegistryRule(const char* registryPath, const char* itemName, 
                                                    const char* itemType, const char* expectedValue, 
                                                    CheckResult_CSharp* result) {
    // 初始化结果
    result->status = 1; // 默认为修复失败
    strcpy(result->value, "");
    
    // 参数检查
    if (!registryPath || !itemName || !itemType || !expectedValue || !result) {
        return;
    }
    
    // 解析注册表路径
    HKEY hRootKey;
    std::wstring subKey;
    if (!ParseRegistryPath(registryPath, hRootKey, subKey)) {
        return;
    }
    
    // 打开注册表键，如果不存在则创建
    HKEY hKey;
    LONG lRes = RegCreateKeyExW(hRootKey, subKey.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, 
                              KEY_WRITE, nullptr, &hKey, nullptr);
    if (lRes != ERROR_SUCCESS) {
        return;
    }
    
    // 转换项名称为宽字符
    std::wstring wItemName = AnsiToWide(itemName);
    
    // 根据类型写入值
    bool writeSuccess = false;
    
    if (strcmp(itemType, "REG_SZ") == 0) {
        // 转换期望值为宽字符串
        std::wstring wExpectedValue = AnsiToWide(expectedValue);
        
        // 写入字符串值
        lRes = RegSetValueExW(hKey, wItemName.c_str(), 0, REG_SZ, 
                            reinterpret_cast<const BYTE*>(wExpectedValue.c_str()), 
                            static_cast<DWORD>((wExpectedValue.length() + 1) * sizeof(wchar_t)));
        
        writeSuccess = (lRes == ERROR_SUCCESS);
    } 
    else if (strcmp(itemType, "REG_DWORD") == 0) {
        // 转换期望值为DWORD
        DWORD dwValue = 0;
        try {
            dwValue = static_cast<DWORD>(std::stoul(expectedValue));
        } 
        catch (...) {
            RegCloseKey(hKey);
            return;
        }
        
        // 写入DWORD值
        lRes = RegSetValueExW(hKey, wItemName.c_str(), 0, REG_DWORD, 
                            reinterpret_cast<const BYTE*>(&dwValue), sizeof(DWORD));
        
        writeSuccess = (lRes == ERROR_SUCCESS);
    }
    
    // 关闭注册表键
    RegCloseKey(hKey);
    
    // 设置返回状态
    if (writeSuccess) {
        result->status = 0; // 修复成功
    }
}


extern "C" __declspec(dllexport) void CheckAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result) {
    // 初始化结果
    result->status = 4; // 默认为其他错误
    strcpy(result->value, "未实现的功能");
    
    // TODO: 实现审计策略检查功能
}

extern "C" __declspec(dllexport) void FixAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result) {
    // 初始化结果
    result->status = 1; // 默认为修复失败
    strcpy(result->value, "未实现的功能");
    
    // TODO: 实现审计策略修复功能
}