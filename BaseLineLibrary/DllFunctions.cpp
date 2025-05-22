#include "DllFunctions.h"

extern "C" __declspec(dllexport) CheckResult_CSharp CheckRegistryRule(const char* registryPath, const char* itemName,
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
		strcpy(result.value = outputAudit(rule.currentString));//返回当前值字符串形式
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

extern "C" __declspec(dllexport) bool FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue) {


}

extern "C" __declspec(dllexport) bool FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue){


	}