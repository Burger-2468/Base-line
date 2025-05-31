#pragma once

#include <windows.h>
// #include "SecurityChecker.h"
#include <vector>
#include <string>

struct CheckResult_CSharp {
	// 对于CheckRegistryRule：0=正确，1=错误，2=注册表路径不存在，3=注册表项不存在，4=其他错误
	// 对于FixRegistryRule：0=成功，1=失败
	int status;
	// 检测到的值
	char value[1024];
};

extern "C" __declspec(dllexport) void CheckRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue, CheckResult_CSharp* result);

extern "C" __declspec(dllexport) void CheckAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result);

extern "C" __declspec(dllexport) void FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue, CheckResult_CSharp* result);

extern "C" __declspec(dllexport) void FixAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result);
