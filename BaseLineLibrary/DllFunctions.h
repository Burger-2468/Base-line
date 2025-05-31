#pragma once

#include <windows.h>
// #include "SecurityChecker.h"
#include <vector>
#include <string>

struct CheckResult_CSharp {
	// ����CheckRegistryRule��0=��ȷ��1=����2=ע���·�������ڣ�3=ע�������ڣ�4=��������
	// ����FixRegistryRule��0=�ɹ���1=ʧ��
	int status;
	// ��⵽��ֵ
	char value[1024];
};

extern "C" __declspec(dllexport) void CheckRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue, CheckResult_CSharp* result);

extern "C" __declspec(dllexport) void CheckAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result);

extern "C" __declspec(dllexport) void FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue, CheckResult_CSharp* result);

extern "C" __declspec(dllexport) void FixAuditpolRule(const char* auditSubcategory, const int expectedValue, CheckResult_CSharp* result);
