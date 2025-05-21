#pragma once

#include <windows.h>
#include "SecurityChecker.h"
#include <vector>
#include <string>

struct CheckResult_CSharp {
	// 0: ��ȷ��1: ����2: ���ʧ��
	int status;
	// ��⵽��ֵ
	char value[1024];
};

extern "C" __declspec(dllexport) CheckResult_CSharp CheckRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue);

extern "C" __declspec(dllexport) CheckResult_CSharp CheckAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue);

extern "C" __declspec(dllexport) bool FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue);

extern "C" __declspec(dllexport) bool FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue);