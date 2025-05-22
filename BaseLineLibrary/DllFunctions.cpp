#include "DllFunctions.h"

extern "C" __declspec(dllexport) CheckResult_CSharp CheckRegistryRule(const char* registryPath, const char* itemName,
	const char* itemType, const char* expectedValue) {
	CheckResult_CSharp result;
	CheckResult rule;
	//����ע�������
	rule.checkType = CheckType::Registry;
	//����ע���·��
	std::string wpath(registryPath);
	rule.regPath = AnsiToWide(wpath);
	//����ע�������
	std::string wIName(registryPath);
	rule.valueName = AnsiToWide(wIName);
	//���÷���ֵ����
	if (itemType == "DWORD") { 
		rule.valueType = ValueType::DWORD;
		char *end;//ת������
		rule.standardDword = std::strtol(expectedValue,&end,10);
	}
	else if (itemType == "STRING"){
		rule.valueType = ValueType::STRING;
		std::string expV(expectedValue);
		rule.standardString = AnsiToWide(expV);
	}
	else {
		result.status = 2;//���ʧ��
		strcpy(result.value, "����ֵ�������ô��󣬼��ʧ��");		
		return result;
	};
		
	//ִ�м�⺯��
	rule = SecurityChecker::CheckOne(rule);

	//�����Ƿ���ϳɹ��߼�
	if(rule.isTrueDetect && !rule.isCompliant ){
		result.status = 1;//���ش���
		std::string ansiStr = WideToAnsi(rule.currentString);
		strcpy(result.value, ansiStr.c_str());
	}
	else if (rule.isTrueDetect && rule.isCompliant ) {
		result.status = 0;//������ȷ
		//���ص�ǰֵ
		std::string ansiStr = WideToAnsi(rule.currentString);
		strcpy(result.value, ansiStr.c_str());
	}
	else {
		result.status = 2;//���ʧ��
		strcpy(result.value, "���ʧ��");
	}

	return result;
}

char* outputAudit(const int expectedValue) {
	char* output = new char[100];
	if (expectedValue == 0) {
		return strcpy(output, "�����");
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
	//����Ϊ��˲�������
	rule.checkType = CheckType::AuditPolicy;
	//������˲������
	std::string Adp(auditCategory);
	rule.auditCategory = AnsiToWide(Adp);
	//������˲��������
	std::string Asup(auditSubcategory);
	rule.auditSubcategory = AnsiToWide(Asup);
	//����Ĭ��ֵ����
	rule.valueType = ValueType::DWORD;
	//��������ֵ
	rule.standardDword = expectedValue;

	rule = SecurityChecker::CheckOne(rule);

	//�����Ƿ���ϳɹ�
	if (rule.isTrueDetect && !rule.isCompliant) {
		result.status = 1;//���ش���
		if(expectedValue == 0)
		/*std::string ansiStr = WideToAnsi(rule.currentString);
		strcpy(result.value, ansiStr.c_str());*/
	}
	else if (rule.isTrueDetect && rule.isCompliant) {
		result.status = 0;//������ȷ
		if(exp)
		////���ص�ǰֵ
		//std::string ansiStr = WideToAnsi(rule.currentString);
		//strcpy(result.value, ansiStr.c_str());
	}
	else {
		result.status = 2;//���ʧ��
		strcpy(result.value, "���ʧ��");
	}
	return result;
}

extern "C" __declspec(dllexport) bool FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue) {


}

extern "C" __declspec(dllexport) bool FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue){


	}