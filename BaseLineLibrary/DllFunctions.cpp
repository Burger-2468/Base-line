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
	if (itemType == "DWORD") { 
		rule.valueType = ValueType::DWORD;
		char *end;//转成数字
		rule.standardDword = std::strtol(expectedValue,&end,10);
	}
	else if (itemType == "STRING"){
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

char* outputAudit(const int expectedValue) {
	char* output = new char[100];
	if (expectedValue == 0) {
		return strcpy(output, "无审核");
	}
	else if (expectedValue == 1) {
	
	}
	else if (expectedValue == 2) {}
	else if (expectedValue == 3) {}

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
		if(expectedValue == 0)
		/*std::string ansiStr = WideToAnsi(rule.currentString);
		strcpy(result.value, ansiStr.c_str());*/
	}
	else if (rule.isTrueDetect && rule.isCompliant) {
		result.status = 0;//返回正确
		if(exp)
		////返回当前值
		//std::string ansiStr = WideToAnsi(rule.currentString);
		//strcpy(result.value, ansiStr.c_str());
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