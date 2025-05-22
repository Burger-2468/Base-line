#include "SecurityChecker.h"
#include <fstream>
#include <regex>

bool ParseRegPath(const std::wstring& fullPath, HKEY& rootKey, std::wstring& keyPath) {
    // ����ע���·��
    size_t pos = fullPath.find_first_of(L"\\");
    if (pos == std::wstring::npos) return false;

    std::wstring rootStr = fullPath.substr(0, pos);
    keyPath = fullPath.substr(pos + 1);
    //��keyPath�еķ�б��תΪ��б��
    if (rootStr == L"HKEY_LOCAL_MACHINE") rootKey = HKEY_LOCAL_MACHINE;
    else if (rootStr == L"HKEY_CURRENT_USER") rootKey = HKEY_CURRENT_USER;
    else return false;
    return true;
}

// ANSI ת���ַ�
std::wstring AnsiToWide(const std::string& str) {
    int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], sizeNeeded);
    return wstr;
}

// ���ַ�ת ANSI
std::string WideToAnsi(const std::wstring& wstr) {
    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    return str;
}

//SecurityChecker::SecurityChecker() {
//    m_checkItems = {//����һ�����ݽṹ�������洢������Ҫ������
//        // ���ƣ����ͣ�·����ֵ���ƣ�ֵ���ݣ�ֵ���ͣ�����ֵ������ֵ���ݣ���ע���Ƿ����ã��Ƿ����
//        // �˻�����
//        { L"���Դ·������", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
//          L"DisableIPSourceRouting",L"",L"", ValueType::DWORD, 2,L"",L"", false, false },
//        { L"��������ʹ������", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"MaximumPasswordAge",L"",L"", ValueType::DWORD, 90,L"",L"", false, false },
//        { L"������볤����Сֵ", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"MinimumPasswordLength",L"",L"", ValueType::DWORD, 8,L"",L"", false, false },
//        { L"����Ƿ��������븴�Ӷ�Ҫ��", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"PasswordComplexity",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"���ǿ��������ʷ����", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"PasswordHistorySize", L"", L"", ValueType::DWORD, 2,L"",L"", false, false },
//        { L"����Ƿ������ʻ�������ֵ", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"LockoutBadCount", L"", L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"����Ƿ�����ȷ�����ʻ�����ʱ��", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"ResetLockoutCount", L"", L"", ValueType::DWORD, 5,L"",L"", false, false },
//        { L"����Ƿ�����ȷ���ø�λ�ʻ�����������ʱ��", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"LockoutDuration", L"", L"", ValueType::DWORD, 5,L"",L"", false, false },
//        { L"����Զ�̷��ʵ�ע���·������·��", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurePipeServers\\winreg\\AllowedPaths",
//          L"Machine", L"", L"", ValueType::STRING, 0,
//          L"System\\CurrentControlSet\\Control\\Print\\Printers,System\\CurrentControlSet\\Services\\Eventlog,Software\\Microsoft\\OLAP Server,Software\\Microsoft\\Windows NT\\CurrentVersion\\Print,Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows,System\\CurrentControlSet\\Control\\ContentIndex,System\\CurrentControlSet\\Control\\Terminal Server,System\\CurrentControlSet\\Control\\Terminal Server\\UserConfig,System\\CurrentControlSet\\Control\\Terminal Server\\DefaultUserConfiguration,Software\\Microsoft\\Windows NT\\CurrentVersion\\Perflib,System\\CurrentControlSet\\Services\\SysmonLog",
//          L"", false, false },
//        { L"����Ƿ���ɾ�����������ʵĹ���������ܵ�", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//          L"NullSessionPipes",L"",L"", ValueType::STRING, 0,L"",L"", false, false },
//        { L"����Ƿ���ɾ�����������ʵĹ���������ܵ�2", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//          L"NullSessionShares",L"",L"", ValueType::STRING, 0,L"",L"", false, false },
//        { L"����Ƿ�������SAM�����û�����", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa",
//          L"Restrictanonymous",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"����Ƿ�������SAM�����û�����2", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa",
//          L"Restrictanonymoussam",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"���ɹر�ϵͳ���ʻ�����", CheckType::Registry,
//          L"Privilege Rights", L"SeShutdownPrivilege",L"",L"", ValueType::STRING, 0,
//          L"*S-1-5-32-544",L"", false, false },
//        { L"����Ƿ������ƿɴ�Զ�˹ر�ϵͳ���ʻ�����", CheckType::Registry,
//          L"Privilege Rights", L"SeRemoteShutdownPrivilege",L"",L"", ValueType::STRING, 0,
//          L"*S-1-5-32-544",L"", false, false },
//        { L"����Ƿ�������ȡ���ļ����������������Ȩ���ʻ�����", CheckType::Registry,
//          L"Privilege Rights", L"SeProfileSingleProcessPrivilege",L"",L"", ValueType::STRING, 0,
//          L"*S-1-5-32-544",L"", false, false },
//        { L"����Ƿ������ò���ʾ�����û�������", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
//          L"Dontdisplaylastusername",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"����Ƿ�����ȷ������ʾ�û����������֮ǰ���и��Ĳ���", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
//          L"PasswordExpiryWarning",L"",L"", ValueType::DWORD, 14,L"",L"", false, false },
//        { L"����Ƿ�����ȷ���������Ựʱ��ʾ�û���Ϣ����", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
//          L"DontDisplayLockedUserId",L"",L"", ValueType::DWORD, 3,L"",L"", false, false },
//
//          // ��־
//          { L"����Ƿ������Ը������", CheckType::AuditPolicy,
//           L"",L"", L"System", L"Policy Change", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ�����¼�¼����", CheckType::AuditPolicy,
//           L"",L"", L"Logon/Logoff", L"Logon", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ�������������", CheckType::AuditPolicy,
//           L"",L"", L"Object Access", L"File System", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ������̸������", CheckType::AuditPolicy,
//           L"",L"", L"System", L"Process Tracking", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ���Ŀ¼����������", CheckType::AuditPolicy,
//           L"",L"", L"Directory Service Access", L"Directory Service Access", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ�����Ȩʹ�����", CheckType::AuditPolicy,
//           L"",L"", L"Privilege Use", L"Sensitive Privilege Use", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ���ϵͳ�¼����", CheckType::AuditPolicy,
//           L"",L"", L"System", L"System Events", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ����ʻ���¼�¼����", CheckType::AuditPolicy,
//           L"",L"", L"Account Logon", L"Account Logon", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"����Ƿ����ʻ��������", CheckType::AuditPolicy,
//           L"",L"", L"Account Management", L"Account Management", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"���ϵͳ��־�ļ��ﵽ����Сʱ�Ķ��������", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\System",
//            L"Retention",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//          { L"���ϵͳ��־����С", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\System",
//            L"MaxSize",L"",L"", ValueType::DWORD, 8388608,L"",L"", false, false },
//          { L"���Ӧ�ó�����־�ļ��ﵽ����Сʱ�Ķ��������", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Application",
//            L"Retention",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//          { L"���Ӧ�ó�����־����С", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Application",
//            L"MaxSize",L"",L"", ValueType::DWORD, 8388608,L"",L"", false, false },
//          { L"��鰲ȫ��־�ļ��ﵽ����Сʱ�Ķ��������", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Security",
//            L"Retention",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//          { L"��鰲ȫ��־����С", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Security",
//            L"MaxSize",L"",L"", ValueType::DWORD, 8388608,L"",L"", false, false },
//          { L"����Ƿ������ò���ȷ����Windows����ʱ��ͬ������(NTP)", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\W32Time\\TimeProviders\\NtpServer",
//            L"Enabled",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//
//            // ����ǽ+Σ�շ���+Σ�ն˿�
//            { L"���Զ������(RDP)����˿�", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp",
//              L"PortNumber",L"",L"", ValueType::DWORD, 3389, L"!3389",L"", false, false },
//            { L"����Ƿ��ѿ���Windows����ǽ", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile",
//              L"EnableFirewall",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"����Ƿ�������SYN��������", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"SynAttackProtect",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"���TCP����������ֵ", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxPortsExhausted",L"",L"", ValueType::DWORD, 5,L"",L"", false, false },
//            { L"���ȡ��������Ӧ SYN ����֮ǰҪ���´��� SYN-ACK �Ĵ���", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxConnectResponseRetransmissions",L"",L"", ValueType::DWORD, 2,L"",L"", false, false },
//            { L"��鴦��SYN_RCVD ״̬�µ� TCP ������ֵ", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxHalfOpen",L"",L"", ValueType::DWORD, 500,L"",L"", false, false },
//            { L"��鴦��SYN_RCVD ״̬��,�������Ѿ�������һ�����´����TCP������ֵ", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxHalfOpenRetried",L"",L"", ValueType::DWORD, 400,L"",L"", false, false },
//            { L"����Ƿ������ò���ȷ����ICMP��������", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"EnableICMPRedirect",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"����Ƿ��ѽ���ʧЧ���ؼ��", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"EnableDeadGWDetect",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"����Ƿ�����ȷ�����ش���������Ƭ�εĴ���", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxDataRetransmissions",L"",L"", ValueType::DWORD, 2,L"",L"", false, false },
//            { L"����Ƿ��ѽ���·�ɷ��ֹ���", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"PerformRouterDiscovery",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"����Ƿ�����ȷ����TCP���Ӵ��ʱ��", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"KeepAliveTime",L"",L"", ValueType::DWORD, 300000,L"",L"", false, false },
//            { L"����Ƿ������ò���ȷ����TCP��Ƭ��������", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"EnablePMTUDiscovery",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"����Ƿ��ѽ���WindowsӲ��Ĭ�Ϲ���", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Parameters",
//              L"AutoShareServer",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"����Ƿ��ѽ���WindowsӲ��Ĭ�Ϲ���2", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Parameters",
//              L"AutoShareWks",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"����Ƿ������ò���ȷ������Ļ��������", CheckType::Registry,
//              L"HKEY_CURRENT_USER\\Control Panel\\Desktop",
//              L"ScreenSaveActive",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"����Ƿ��ѹر�Windows�Զ�����", CheckType::Registry,
//              L"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\AutoplayHandlers",
//              L"DisableAutoplay",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"����Ƿ��ѽ�ֹWindows�Զ���¼", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
//              L"AutoAdminLogon",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"�򻷾�������Ƿ�����ȷ���ÿɱ����汣��ĵ�¼�ĸ�������", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
//              L"CachedLogonsCount",L"",L"", ValueType::DWORD, 5,L"",L"", false, false },
//
//              // �޹ؽ�Ҫ�ĸ��Ի�����
//              { L"�����Ļ��������ȴ�ʱ��", CheckType::Registry,
//                L"HKEY_CURRENT_USER\\Control Panel\\Desktop",
//                L"ScreenSaveTimeOut",L"",L"", ValueType::DWORD, 300,L"",L"", false, false },
//              { L"����Ƿ��������ڻָ�ʱ��ʾ��½����", CheckType::Registry,
//                L"HKEY_CURRENT_USER\\Control Panel\\Desktop",
//                L"ScreenSaverIsSecure",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//              { L"����Ƿ���ȷ���÷���������ͣ�Ựǰ����Ŀ���ʱ����", CheckType::Registry,
//                L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//                L"Autodisconnect",L"",L"", ValueType::DWORD, 15,L"",L"", false, false },
//              { L"����Ƿ������õ���¼ʱ������ʱ�Զ�ע���û�����", CheckType::Registry,
//                L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//                L"Enableforcedlogoff",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//              { L"����Ƿ��ѽ��õ�¼ʱ���밴 Ctrl+Alt+Del ����", CheckType::Registry,
//                L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
//                L"Disablecad",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//    };
//}

//// ����ע���·�������ظ�����·��
//std::vector<CheckResult> SecurityChecker::CheckAll() {
//    std::vector<CheckResult> results;
//
//    for (const auto& item : m_checkItems) {
//        CheckResult result = item;
//
//        if (item.checkType == CheckType::Registry) {
//            HKEY rootKey;
//            std::wstring keyPath;
//            //std::wstring wideRegPath = AnsiToWide(item.regPath);
//            if (!ParseRegPath(item.regPath, rootKey, keyPath)) {
//                // ·������ʧ��
//                result.currentString = L"·������ʧ��";
//                result.isCompliant = false;
//                results.push_back(result);
//                continue;
//            }
//            //std::wstring wideValueName = AnsiToWide(item.valueName);
//            RegistryHelper helper(rootKey, keyPath, item.valueName);
//            result.currentString = AnsiToWide(helper.ReadValue("", true)); // createIfMissing��Ϊtrue�Զ�����·��
//
//            if (item.valueType == ValueType::DWORD) {
//                try {
//                    DWORD currentValue = std::stoul(result.currentString);
//                    // ���������׼ֵ����Զ������˿�Ҫ�󲻵���3389��
//                    if (item.standardString == L"!3389") {
//                        result.isCompliant = (currentValue != 3389);
//                    }
//                    else {
//                        result.isCompliant = (currentValue == item.standardDword);
//                    }
//                }
//                catch (...) {
//                    result.isCompliant = false;
//                }
//            }
//            else {
//                // �ַ�������ֱ�ӱȽϣ�������·�����˻��б�ȣ�
//                result.isCompliant = (result.currentString == item.standardString);
//            }
//        }
//        else if (item.checkType == CheckType::AuditPolicy) {
//            bool auditSuccess = false;
//            bool auditFailure = false;
//
//            if (SecurityPolicyHelper::GetAuditPolicy(WideToAnsi(item.auditCategory), WideToAnsi(item.auditSubcategory),
//                auditSuccess, auditFailure)) {
//                DWORD currentValue = (auditSuccess ? 1 : 0) + (auditFailure ? 2 : 0);
//                result.currentString = AnsiToWide(std::to_string(currentValue));
//                result.isCompliant = (currentValue == item.standardDword);
//            }
//            else {
//                result.currentString = L"�޷���ȡ";
//                result.isCompliant = false;
//            }
//        }
//
//        results.push_back(result);
//    }
//
//    return results;
//}

// �޸����в����ϱ�׼��������Ƿ�ȫ���޸��ɹ�
bool SecurityChecker::RepairAll(std::vector<CheckResult>& results) {
    bool allSuccess = true;

    for (auto& result : results) {
        if (result.isCompliant) {
            result.repairSuccess = true;
            continue;
        }

        if (result.checkType == CheckType::Registry) {
            HKEY rootKey;
            std::wstring keyPath;
            //std::wstring wideRegPath = AnsiToWide(result.regPath);
            if (!ParseRegPath(result.regPath, rootKey, keyPath)) {
                result.repairSuccess = false;
                allSuccess = false;
                continue;
            }
            //std::wstring wideValueName = AnsiToWide(result.valueName);
            RegistryHelper helper(rootKey, keyPath, result.valueName);

            if (result.valueType == ValueType::DWORD) {
                // ���������׼ֵ����Զ������˿�Ҫ�󲻵���3389���˴����û��Զ����߼���ʾ�����ݲ�����
                if (result.standardString == L"!3389") {
                    result.repairSuccess = false; // ���ֶ����÷�3389�˿ڣ������޷��Զ���������˿�
                }
                else {
                    result.repairSuccess = helper.WriteDWORD(result.standardDword, true);
                }
                if (result.repairSuccess) {
                    result.currentString = AnsiToWide(helper.ReadValue());
                    try {
                        DWORD currentValue = std::stoul(result.currentString);
                        result.isCompliant = (currentValue == result.standardDword);
                    }
                    catch (...) {
                        result.isCompliant = false;
                    }
                }
            }
            else {
                // �ַ��������޸�����·�����˻��б��˴���Ϊ��֧���Զ��޸���
                result.repairSuccess = false;
                result.currentString = L"�ַ��������ݲ�֧���Զ��޸�";
            }
        }
        else if (result.checkType == CheckType::AuditPolicy) {
            bool auditSuccess = (result.standardDword & 1) != 0;
            bool auditFailure = (result.standardDword & 2) != 0;

            result.repairSuccess = SecurityPolicyHelper::SetAuditPolicy(
                WideToAnsi(result.auditCategory), WideToAnsi(result.auditSubcategory), auditSuccess, auditFailure);

            if (result.repairSuccess) {
                bool actualSuccess = false;
                bool actualFailure = false;
                if (SecurityPolicyHelper::GetAuditPolicy(
                    WideToAnsi(result.auditCategory), WideToAnsi(result.auditSubcategory), actualSuccess, actualFailure)) {
                    DWORD currentValue = (actualSuccess ? 1 : 0) + (actualFailure ? 2 : 0);
                    result.currentString = AnsiToWide(std::to_string(currentValue));
                    result.isCompliant = (currentValue == result.standardDword);
                }
                else {
                    result.isCompliant = false;
                }
            }
        }

        allSuccess = allSuccess && result.repairSuccess;
    }

    return allSuccess;
}

// ��ⵥ�������CheckResult
CheckResult SecurityChecker::CheckOne(CheckResult rule) {
    CheckResult result = rule;

    if (rule.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        //std::wstring wideRegPath = AnsiToWide(rule.regPath);
        if (!ParseRegPath(rule.regPath, rootKey, keyPath)) {
            // ·������ʧ��
            result.currentString = L"·������ʧ��";
            result.isTrueDetect = false;//û��⵽
            return result;
        }
        //std::wstring wideValueName = AnsiToWide(rule.valueName);
        RegistryHelper helper(rootKey, keyPath, rule.valueName);
        result.currentString = AnsiToWide(helper.ReadValue("", true)); // createIfMissing��Ϊtrue�Զ�����·��

        if (rule.valueType == ValueType::DWORD) {
            try {
                DWORD currentValue = std::stoul(result.currentString);
                // ���������׼ֵ����Զ������˿�Ҫ�󲻵���3389��
                if (rule.standardString == L"!3389") {
                    result.isCompliant = (currentValue != 3389);
                }
                else {
                    result.isCompliant = (currentValue == rule.standardDword);
                }
            }
            catch (...) {
                result.isTrueDetect = false;
            }
        }
        else {
            // �ַ�������ֱ�ӱȽϣ�������·�����˻��б�ȣ�
            result.isCompliant = (result.currentString == rule.standardString);
        }
    }
    else if (rule.checkType == CheckType::AuditPolicy) {
        bool auditSuccess = false;//�ɹ��¼�1:0
        bool auditFailure = false;//ʧ���¼�2:0

        if (SecurityPolicyHelper::GetAuditPolicy(WideToAnsi(rule.auditCategory), WideToAnsi(rule.auditSubcategory),
            auditSuccess, auditFailure)) {
            DWORD currentValue = (auditSuccess ? 1 : 0) + (auditFailure ? 2 : 0);
            result.currentString = AnsiToWide(std::to_string(currentValue));//�����ַ������͵�����
            result.isCompliant = (currentValue == rule.standardDword);
        }
        else {
            result.currentString = L"�޷���ȡ";
            result.isTrueDetect = false;
        }
    }

    return result;
}



// �޸�����������Ƿ��޸��ɹ�
CheckResult SecurityChecker::RepairOne(CheckResult& result) {
    // ִ���޸����������߼�
    bool repairActionSuccess = false;

    // ����ע��������޸�
    if (result.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        // ����ע���·��Ϊ��������·��
        if (!ParseRegPath(result.regPath, rootKey, keyPath)) {
            result.repairSuccess = false;
            result.currentString = L"ע���·������ʧ��";
            return result;
        }

        // ��ʼ��ע������������
        RegistryHelper helper(rootKey, keyPath, result.valueName);
        // ����ֵ����ִ�в�ͬ���޸�����
        if (result.valueType == ValueType::DWORD) {
            // ��������ֵ!3389���޷��Զ���������˿ڣ�
            repairActionSuccess = (result.standardString == L"!3389") ?
                false : helper.WriteDWORD(result.standardDword, true);
        }
        else if (result.valueType == ValueType::STRING) {
            // ת����׼�ַ���ΪANSI��ʽ��д��ע���
            std::string ansiValue = WideToAnsi(result.standardString);
            repairActionSuccess = helper.WriteString(ansiValue, true);
        }
        // ��¼�޸������Ƿ�ɹ�
        result.repairSuccess = repairActionSuccess;
    }
    // ������˲��������޸�
    else if (result.checkType == CheckType::AuditPolicy) {
        // ������׼DWORDֵΪ�ɹ�/ʧ����˱�־
        bool auditSuccess = (result.standardDword & 1) != 0;
        bool auditFailure = (result.standardDword & 2) != 0;
        // ���ð�ȫ��������������˲���
        repairActionSuccess = SecurityPolicyHelper::SetAuditPolicy(
            WideToAnsi(result.auditCategory), WideToAnsi(result.auditSubcategory), auditSuccess, auditFailure);
        result.repairSuccess = repairActionSuccess;
    }

    // �޸�����ʧ��ʱ���ش���״̬
    if (!repairActionSuccess) {
        result.currentString = L"�޸�����ִ��ʧ��";
        return result;
    }

    // �޸��ɹ�����ü�⺯����֤���
    CheckResult detectedResult = CheckOne(result);

    // �ϲ���������޸������
    // �����޸����ȡ��currentString�������ºϹ��Ժͼ��״̬
    result.isCompliant = detectedResult.isCompliant;
    result.isTrueDetect = detectedResult.isTrueDetect;

    // �����޸��ɹ��������޸������ɹ��Ҽ�����Ϲ�
    result.repairSuccess = repairActionSuccess && result.isCompliant;
    return result;
}






// ���ɱ���
bool SecurityChecker::GenerateReport(const std::vector<CheckResult>& results, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "���,���������,�������,ע���·��,�ֶ���,������,��������,ֵ����,��׼ֵ,��ǰֵ,�Ƿ����,�޸����\n";

    for (size_t i = 0; i < results.size(); i++) {
        const auto& result = results[i];
        std::string checkTypeStr = (result.checkType == CheckType::Registry) ? "ע���" : "��˲���";
        std::string valueTypeStr = (result.valueType == ValueType::DWORD) ? "DWORD" : "STRING";
        std::string standardValue = (result.valueType == ValueType::DWORD) ?
            (result.standardString.empty() ? std::to_string(result.standardDword) : WideToAnsi(result.standardString)) :
            WideToAnsi(result.standardString);

        // ���������׼ֵ��ʾ����!3389��
        if (result.valueType == ValueType::DWORD && !result.standardString.empty()) {
            standardValue = WideToAnsi(result.standardString);
        }

        file << i + 1 << ","
            << WideToAnsi(result.name) << ","
            << checkTypeStr << ","
            << WideToAnsi(result.regPath) << ","
            << WideToAnsi(result.valueName) << ","
            << WideToAnsi(result.auditCategory) << ","
            << WideToAnsi(result.auditSubcategory) << ","
            << valueTypeStr << ","
            << standardValue << ","
            << WideToAnsi(result.currentString) << ","
            << (result.isCompliant ? "��" : "��") << ","
            << (result.repairSuccess ? "�ɹ�" : "ʧ��") << "\n";
    }

    file << "\nͳ����Ϣ\n";
    file << "���������," << results.size() << "\n";
    file << "���ϱ�׼��," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.isCompliant; }) << "\n";
    file << "�޸��ɹ���," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.repairSuccess; }) << "\n";

    file.close();
    return true;
}