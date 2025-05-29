#include "SecurityPolicyHelper.h"
#include <windows.h>
#include <shellapi.h>
#include <sstream>
#include <regex>

bool SecurityPolicyHelper::ExecuteAuditpolCommand(const std::string& command, std::string& output) {
    std::string fullCommand = "auditpol.exe " + command;

    // ���������ܵ����ڲ����������
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        return false;
    }

    // �����ӽ���������Ϣ
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // ����������Ϣ�ṹ��
    PROCESS_INFORMATION pi;
    std::wstring wCommand(fullCommand.begin(), fullCommand.end());

    // �����ӽ���ִ������
    if (!CreateProcess(NULL, const_cast<LPWSTR>(wCommand.c_str()), NULL, NULL,
        TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return false;
    }

    // �ر�д��ˣ�׼����ȡ���
    CloseHandle(hWritePipe);

    // ��ȡ�������
    const int bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead;
    std::stringstream ss;

    while (ReadFile(hReadPipe, buffer, bufferSize - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
    }

    output = ss.str();//�ѽ���ŵ���������������

    // ������Դ
    CloseHandle(hReadPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

bool SecurityPolicyHelper::GetAuditPolicy(
    const std::string& auditSubcategory,
    bool& auditSuccess, bool& auditFailure) {
    std::string command = "/get /subcategory:" + auditSubcategory ;
    std::string output;

    if (!ExecuteAuditpolCommand(command, output)) {
        return false;
    }
    /*
    // ʹ��������ʽ�������
    std::regex successRegex("Success:\\s+(\\S+)");  // TODO: �޸�Ӣ����ʾ�� �޸�Ӣ����ʾ���� �޸�Ӣ����ʾ����
    std::regex failureRegex("Failure:\\s+(\\S+)");

    std::smatch successMatch, failureMatch;

    auditSuccess = false;
    auditFailure = false;

    // TODO: �޸�Ӣ����ʾ����
    // TODO: �޸�Ӣ����ʾ����
    // TODO: �޸�Ӣ����ʾ����
    if (std::regex_search(output, successMatch, successRegex) && successMatch.size() > 1) {
        auditSuccess = (successMatch[1] == "Enable");
    }

    if (std::regex_search(output, failureMatch, failureRegex) && failureMatch.size() > 1) {
        auditFailure = (failureMatch[1] == "Enable");
    }*/
    if(output.find("�ɹ���ʧ��") != std::string::npos) {
        auditSuccess = true;
        auditFailure = true;
    } else if(output.find("�ɹ�") != std::string::npos) {
        auditSuccess = true;
        auditFailure = false;
    } else if(output.find("ʧ��") != std::string::npos) {
        auditSuccess = false;
        auditFailure = true;
    } else if(output.find("�����") != std::string::npos) {
        auditSuccess = false;
        auditFailure = false;
    }
    else {
		return false; // ��������ʽ������Ԥ�ڣ����� false
    }

    return true;
}

bool SecurityPolicyHelper::SetAuditPolicy(const std::string& auditCategory,
    const std::string& auditSubcategory,
    bool auditSuccess, bool auditFailure) {
    std::string command = "/set /subcategory:\"" + auditCategory + "\\" + auditSubcategory + "\"";

    if (auditSuccess && auditFailure) {
        command += " /success:enable /failure:enable";
    }
    else if (auditSuccess) {
        command += " /success:enable /failure:disable";
    }
    else if (auditFailure) {
        command += " /success:disable /failure:enable";
    }
    else {
        command += " /success:disable /failure:disable";
    }

    std::string output;
    return ExecuteAuditpolCommand(command, output);
}