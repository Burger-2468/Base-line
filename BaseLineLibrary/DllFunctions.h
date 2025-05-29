#pragma once

#include <windows.h>
#include "SecurityChecker.h"
#include <vector>
#include <string>

struct CheckResult_CSharp {
	// 0: 正确，1: 错误，2: 检测失败
	int status;
	// 检测到的值
	char value[256];
};

extern "C" __declspec(dllexport) CheckResult_CSharp CheckRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue);

extern "C" __declspec(dllexport) CheckResult_CSharp CheckAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue);

extern "C" __declspec(dllexport) CheckResult_CSharp FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue);

extern "C" __declspec(dllexport) CheckResult_CSharp FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue);
