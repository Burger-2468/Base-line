#include "DllFunctions.h"
#pragma warning( disable : 4996)

CheckResult_CSharp CheckRegistryRuleInternal(const char* registryPath, const char* itemName,
	const char* itemType, const char* expectedValue) {
	CheckResult_CSharp result;
	CheckResult rule;
	//设置注册表类型
	rule.checkType = CheckType::Registry;
	//设置注册表路径
	std::string wpath(registryPath);
	rule.regPath = AnsiToWide(wpath);
	//设置注册表名称
	std::string wIName(registryPath);
	rule.valueName = AnsiToWide(wIName);
	//设置返回值类型
	if (strcmp(itemType,"DWORD")==0 ) { 
		rule.valueType = ValueType::DWORD;
		char *end;//转成数字
		rule.standardDword = std::strtol(expectedValue,&end,10);
	}
	else if (strcmp(itemType,"STRING")==0 ){
		rule.valueType = ValueType::STRING;
		std::string expV(expectedValue);
		rule.standardString = AnsiToWide(expV);
	}
	else {
		result.status = 2;//检测失败
		strcpy(result.value, "返回值类型设置错误，检测失败");		
		return result;
	};
		
	//执行检测函数
	rule = SecurityChecker::CheckOne(rule);

	//返回是否符合成功逻辑
	if(rule.isTrueDetect && !rule.isCompliant ){
		result.status = 1;//返回错误
		std::string ansiStr = WideToAnsi(rule.currentString);
		strcpy(result.value, ansiStr.c_str());
	}
	else if (rule.isTrueDetect && rule.isCompliant ) {
		result.status = 0;//返回正确
		//返回当前值
		std::string ansiStr = WideToAnsi(rule.currentString);
		strcpy(result.value, ansiStr.c_str());
	}
	else {
		result.status = 2;//检测失败
		strcpy(result.value, "检测失败");
	}

	return result;
}

extern "C" __declspec(dllexport) void CheckRegistryRule(const char* registryPath, const char* itemName,
	const char* itemType, const char* expectedValue, CheckResult_CSharp* result) {
	CheckResult_CSharp result_csharp = CheckRegistryRuleInternal(registryPath, itemName, itemType, expectedValue);
	// 将结果复制到传入的结构体中
	result->status = result_csharp.status;
	strcpy(result->value, result_csharp.value);
}



char* outputAudit(std::wstring expectedValue) {
	char* output = new char[30];
	if (expectedValue == L"0") {
		return strcpy(output, "无审核");
	}
	else if (expectedValue == L"1") {
		return strcpy(output, "成功审核");
	}
	else if (expectedValue == L"2") {
		return strcpy(output, "失败审核");
	}
	else if (expectedValue == L"3") {
		return strcpy(output, "成功审核，失败审核");
	}
	else {
		return strcpy(output, "检测失败");
	}
}

extern "C" __declspec(dllexport) CheckResult_CSharp CheckAuditpolRule(
	const char* auditCategory, const char* auditSubcategory,const int expectedValue) {
	CheckResult_CSharp result;

	CheckResult rule;
	//设置为审核策略类型
	rule.checkType = CheckType::AuditPolicy;
	//设置审核策略类别
	std::string Adp(auditCategory);
	rule.auditCategory = AnsiToWide(Adp);
	//设置审核策略子类别
	std::string Asup(auditSubcategory);
	rule.auditSubcategory = AnsiToWide(Asup);
	//设置默认值类型
	rule.valueType = ValueType::DWORD;
	//设置期望值
	rule.standardDword = expectedValue;

	rule = SecurityChecker::CheckOne(rule);

	//返回是否符合成功
	if (rule.isTrueDetect && !rule.isCompliant) {
		result.status = 1;//返回错误
		strcpy(result.value , outputAudit(rule.currentString));//返回当前值字符串形式
	}
	else if (rule.isTrueDetect && rule.isCompliant) {
		result.status = 0;//返回正确
		strcpy(result.value,outputAudit(rule.currentString));//返回当前值字符串形式
	}
	else {
		result.status = 2;//检测失败
		strcpy(result.value, "检测失败");
	}
	return result;
}

extern "C" __declspec(dllexport) CheckResult_CSharp FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue) {
	CheckResult_CSharp result;
	result.status = 2; // 初始化为检测失败
	strncpy_s(result.value, sizeof(result.value), "初始化失败", _TRUNCATE);

	CheckResult rule;
	// 设置为注册表类型
	rule.checkType = CheckType::Registry;

	// 转换并设置注册表路径和名称
	std::string wpath(registryPath);
	rule.regPath = AnsiToWide(wpath);

	std::string wIName(itemName);
	rule.valueName = AnsiToWide(wIName);

	// 设置值类型和期望值
	if (strcmp(itemType, "DWORD") == 0) {
		rule.valueType = ValueType::DWORD;
		rule.standardDword = std::strtol(expectedValue, nullptr, 10);
	}
	else if (strcmp(itemType, "STRING") == 0) {
		rule.valueType = ValueType::STRING;
		std::string expV(expectedValue);
		rule.standardString = AnsiToWide(expV);
	}
	else {
		result.status = 2;
		strncpy_s(result.value, sizeof(result.value), "不支持的类型，修复失败", _TRUNCATE);
		return result;
	}

	// 执行修复函数
	rule = SecurityChecker::RepairOne(rule);

	// 转换CheckResult到CheckResult_CSharp
	if (rule.repairSuccess) {
		if (rule.isCompliant) {
			result.status = 0; // 修复成功且合规
		}
		else {
			result.status = 1; // 修复成功但不合规
		}
	}
	else {
		result.status = 2; // 修复失败
	}

	// 转换宽字符当前值到ANSI并复制
	std::string ansiCurrent = WideToAnsi(rule.currentString);
	strncpy_s(result.value, sizeof(result.value), ansiCurrent.c_str(), _TRUNCATE);

	return result;
}

extern "C" __declspec(dllexport) CheckResult_CSharp FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue) {
	CheckResult_CSharp result;
	result.status = 2;
	strncpy_s(result.value, sizeof(result.value), "初始化失败", _TRUNCATE);

	CheckResult rule;
	// 设置为审核策略类型
	rule.checkType = CheckType::AuditPolicy;

	// 转换并设置审核策略类别和子类别
	std::string cat(auditCategory);
	rule.auditCategory = AnsiToWide(cat);

	std::string subcat(auditSubcategory);
	rule.auditSubcategory = AnsiToWide(subcat);

	// 设置值类型和期望值
	rule.valueType = ValueType::DWORD;
	rule.standardDword = expectedValue;

	// 执行修复函数
	rule = SecurityChecker::RepairOne(rule);

	// 转换CheckResult到CheckResult_CSharp
	if (rule.repairSuccess) {
		if (rule.isCompliant) {
			result.status = 0; // 修复成功且合规
		}
		else {
			result.status = 1; // 修复成功但不合规
		}
	}
	else {
		result.status = 2; // 修复失败
	}

	// 转换宽字符当前值到ANSI并复制
	std::string ansiCurrent = WideToAnsi(rule.currentString);
	strncpy_s(result.value, sizeof(result.value), ansiCurrent.c_str(), _TRUNCATE);

	return result;
}