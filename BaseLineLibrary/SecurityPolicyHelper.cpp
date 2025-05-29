#include "SecurityPolicyHelper.h"
#include <windows.h>
#include <shellapi.h>
#include <sstream>
#include <regex>

bool SecurityPolicyHelper::ExecuteAuditpolCommand(const std::string& command, std::string& output) {
    std::string fullCommand = "auditpol.exe " + command;

    // 创建匿名管道用于捕获命令输出
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        return false;
    }

    // 设置子进程启动信息
    STARTUPINFO si = { sizeof(STARTUPINFO) };
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // 创建进程信息结构体
    PROCESS_INFORMATION pi;
    std::wstring wCommand(fullCommand.begin(), fullCommand.end());

    // 创建子进程执行命令
    if (!CreateProcess(NULL, const_cast<LPWSTR>(wCommand.c_str()), NULL, NULL,
        TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return false;
    }

    // 关闭写入端，准备读取输出
    CloseHandle(hWritePipe);

    // 读取命令输出
    const int bufferSize = 4096;
    char buffer[bufferSize];
    DWORD bytesRead;
    std::stringstream ss;

    while (ReadFile(hReadPipe, buffer, bufferSize - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        ss << buffer;
    }

    output = ss.str();//把结果放到引用类型数据中

    // 清理资源
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
    // 使用正则表达式解析输出
    std::regex successRegex("Success:\\s+(\\S+)");  // TODO: 修改英文显示！ 修改英文显示！！ 修改英文显示！！
    std::regex failureRegex("Failure:\\s+(\\S+)");

    std::smatch successMatch, failureMatch;

    auditSuccess = false;
    auditFailure = false;

    // TODO: 修改英文显示！！
    // TODO: 修改英文显示！！
    // TODO: 修改英文显示！！
    if (std::regex_search(output, successMatch, successRegex) && successMatch.size() > 1) {
        auditSuccess = (successMatch[1] == "Enable");
    }

    if (std::regex_search(output, failureMatch, failureRegex) && failureMatch.size() > 1) {
        auditFailure = (failureMatch[1] == "Enable");
    }*/
    if(output.find("成功和失败") != std::string::npos) {
        auditSuccess = true;
        auditFailure = true;
    } else if(output.find("成功") != std::string::npos) {
        auditSuccess = true;
        auditFailure = false;
    } else if(output.find("失败") != std::string::npos) {
        auditSuccess = false;
        auditFailure = true;
    } else if(output.find("无审核") != std::string::npos) {
        auditSuccess = false;
        auditFailure = false;
    }
    else {
		return false; // 如果输出格式不符合预期，返回 false
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