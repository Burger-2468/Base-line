#include "DllFunctions.h"
#pragma warning( disable : 4996)

CheckResult_CSharp CheckRegistryRuleInternal(const char* registryPath, const char* itemName,
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
	if (strcmp(itemType,"DWORD")==0 ) { 
		rule.valueType = ValueType::DWORD;
		char *end;//ת������
		rule.standardDword = std::strtol(expectedValue,&end,10);
	}
	else if (strcmp(itemType,"STRING")==0 ){
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

extern "C" __declspec(dllexport) void CheckRegistryRule(const char* registryPath, const char* itemName,
	const char* itemType, const char* expectedValue, CheckResult_CSharp* result) {
	CheckResult_CSharp result_csharp = CheckRegistryRuleInternal(registryPath, itemName, itemType, expectedValue);
	// ��������Ƶ�����Ľṹ����
	result->status = result_csharp.status;
	strcpy(result->value, result_csharp.value);
}



char* outputAudit(std::wstring expectedValue) {
	char* output = new char[30];
	if (expectedValue == L"0") {
		return strcpy(output, "�����");
	}
	else if (expectedValue == L"1") {
		return strcpy(output, "�ɹ����");
	}
	else if (expectedValue == L"2") {
		return strcpy(output, "ʧ�����");
	}
	else if (expectedValue == L"3") {
		return strcpy(output, "�ɹ���ˣ�ʧ�����");
	}
	else {
		return strcpy(output, "���ʧ��");
	}
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
		strcpy(result.value , outputAudit(rule.currentString));//���ص�ǰֵ�ַ�����ʽ
	}
	else if (rule.isTrueDetect && rule.isCompliant) {
		result.status = 0;//������ȷ
		strcpy(result.value,outputAudit(rule.currentString));//���ص�ǰֵ�ַ�����ʽ
	}
	else {
		result.status = 2;//���ʧ��
		strcpy(result.value, "���ʧ��");
	}
	return result;
}

extern "C" __declspec(dllexport) CheckResult_CSharp FixRegistryRule(const char* registryPath, const char* itemName, const char* itemType, const char* expectedValue) {
	CheckResult_CSharp result;
	result.status = 2; // ��ʼ��Ϊ���ʧ��
	strncpy_s(result.value, sizeof(result.value), "��ʼ��ʧ��", _TRUNCATE);

	CheckResult rule;
	// ����Ϊע�������
	rule.checkType = CheckType::Registry;

	// ת��������ע���·��������
	std::string wpath(registryPath);
	rule.regPath = AnsiToWide(wpath);

	std::string wIName(itemName);
	rule.valueName = AnsiToWide(wIName);

	// ����ֵ���ͺ�����ֵ
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
		strncpy_s(result.value, sizeof(result.value), "��֧�ֵ����ͣ��޸�ʧ��", _TRUNCATE);
		return result;
	}

	// ִ���޸�����
	rule = SecurityChecker::RepairOne(rule);

	// ת��CheckResult��CheckResult_CSharp
	if (rule.repairSuccess) {
		if (rule.isCompliant) {
			result.status = 0; // �޸��ɹ��ҺϹ�
		}
		else {
			result.status = 1; // �޸��ɹ������Ϲ�
		}
	}
	else {
		result.status = 2; // �޸�ʧ��
	}

	// ת�����ַ���ǰֵ��ANSI������
	std::string ansiCurrent = WideToAnsi(rule.currentString);
	strncpy_s(result.value, sizeof(result.value), ansiCurrent.c_str(), _TRUNCATE);

	return result;
}

extern "C" __declspec(dllexport) CheckResult_CSharp FixAuditpolRule(const char* auditCategory, const char* auditSubcategory, const int expectedValue) {
	CheckResult_CSharp result;
	result.status = 2;
	strncpy_s(result.value, sizeof(result.value), "��ʼ��ʧ��", _TRUNCATE);

	CheckResult rule;
	// ����Ϊ��˲�������
	rule.checkType = CheckType::AuditPolicy;

	// ת����������˲������������
	std::string cat(auditCategory);
	rule.auditCategory = AnsiToWide(cat);

	std::string subcat(auditSubcategory);
	rule.auditSubcategory = AnsiToWide(subcat);

	// ����ֵ���ͺ�����ֵ
	rule.valueType = ValueType::DWORD;
	rule.standardDword = expectedValue;

	// ִ���޸�����
	rule = SecurityChecker::RepairOne(rule);

	// ת��CheckResult��CheckResult_CSharp
	if (rule.repairSuccess) {
		if (rule.isCompliant) {
			result.status = 0; // �޸��ɹ��ҺϹ�
		}
		else {
			result.status = 1; // �޸��ɹ������Ϲ�
		}
	}
	else {
		result.status = 2; // �޸�ʧ��
	}

	// ת�����ַ���ǰֵ��ANSI������
	std::string ansiCurrent = WideToAnsi(rule.currentString);
	strncpy_s(result.value, sizeof(result.value), ansiCurrent.c_str(), _TRUNCATE);

	return result;
}