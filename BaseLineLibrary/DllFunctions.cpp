#include "DllFunctions.h"


extern "C" __declspec(dllexport) CheckResult CheckRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue) {
	CheckResult result;

	SecurityChecker::CheckOne(registryPath, itemName, itemType, expectedValue, result);
	return result;
}

extern "C" __declspec(dllexport) CheckResult CheckAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue) {
	CheckResult result;


	return result;
}

extern "C" __declspec(dllexport) bool FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue) {


}

extern "C" __declspec(dllexport) bool FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue{


	}