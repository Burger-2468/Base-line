#include "SecurityChecker.h"
#include <fstream>
#include <regex>

bool ParseRegPath(const std::wstring& fullPath, HKEY& rootKey, std::wstring& keyPath) {
    // 解析注册表路径
    size_t pos = fullPath.find_first_of(L"\\");
    if (pos == std::wstring::npos) return false;

    std::wstring rootStr = fullPath.substr(0, pos);
    keyPath = fullPath.substr(pos + 1);
    //把keyPath中的反斜杠转为正斜杠
    if (rootStr == L"HKEY_LOCAL_MACHINE") rootKey = HKEY_LOCAL_MACHINE;
    else if (rootStr == L"HKEY_CURRENT_USER") rootKey = HKEY_CURRENT_USER;
    else return false;
    return true;
}

// ANSI 转宽字符
std::wstring AnsiToWide(const std::string& str) {
    int sizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(sizeNeeded, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], sizeNeeded);
    return wstr;
}

// 宽字符转 ANSI
std::string WideToAnsi(const std::wstring& wstr) {
    int sizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    return str;
}

//SecurityChecker::SecurityChecker() {
//    m_checkItems = {//这是一个数据结构，用来存储所有需要检查的项
//        // 名称，类型，路径，值名称，值数据，值类型，期望值，期望值数据，备注，是否启用，是否忽略
//        // 账户策略
//        { L"检查源路由配置", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters",
//          L"DisableIPSourceRouting",L"",L"", ValueType::DWORD, 2,L"",L"", false, false },
//        { L"检查密码最长使用期限", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"MaximumPasswordAge",L"",L"", ValueType::DWORD, 90,L"",L"", false, false },
//        { L"检查密码长度最小值", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"MinimumPasswordLength",L"",L"", ValueType::DWORD, 8,L"",L"", false, false },
//        { L"检查是否启用密码复杂度要求", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"PasswordComplexity",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"检查强制密码历史个数", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"PasswordHistorySize", L"", L"", ValueType::DWORD, 2,L"",L"", false, false },
//        { L"检查是否启用帐户锁定阈值", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"LockoutBadCount", L"", L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"检查是否已正确配置帐户锁定时间", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"ResetLockoutCount", L"", L"", ValueType::DWORD, 5,L"",L"", false, false },
//        { L"检查是否已正确配置复位帐户锁定计数器时间", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa\\MSV1_0",
//          L"LockoutDuration", L"", L"", ValueType::DWORD, 5,L"",L"", false, false },
//        { L"检查可远程访问的注册表路径和子路径", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SecurePipeServers\\winreg\\AllowedPaths",
//          L"Machine", L"", L"", ValueType::STRING, 0,
//          L"System\\CurrentControlSet\\Control\\Print\\Printers,System\\CurrentControlSet\\Services\\Eventlog,Software\\Microsoft\\OLAP Server,Software\\Microsoft\\Windows NT\\CurrentVersion\\Print,Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows,System\\CurrentControlSet\\Control\\ContentIndex,System\\CurrentControlSet\\Control\\Terminal Server,System\\CurrentControlSet\\Control\\Terminal Server\\UserConfig,System\\CurrentControlSet\\Control\\Terminal Server\\DefaultUserConfiguration,Software\\Microsoft\\Windows NT\\CurrentVersion\\Perflib,System\\CurrentControlSet\\Services\\SysmonLog",
//          L"", false, false },
//        { L"检查是否已删除可匿名访问的共享和命名管道", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//          L"NullSessionPipes",L"",L"", ValueType::STRING, 0,L"",L"", false, false },
//        { L"检查是否已删除可匿名访问的共享和命名管道2", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//          L"NullSessionShares",L"",L"", ValueType::STRING, 0,L"",L"", false, false },
//        { L"检查是否已限制SAM匿名用户连接", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa",
//          L"Restrictanonymous",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"检查是否已限制SAM匿名用户连接2", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Lsa",
//          L"Restrictanonymoussam",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"检查可关闭系统的帐户和组", CheckType::Registry,
//          L"Privilege Rights", L"SeShutdownPrivilege",L"",L"", ValueType::STRING, 0,
//          L"*S-1-5-32-544",L"", false, false },
//        { L"检查是否已限制可从远端关闭系统的帐户和组", CheckType::Registry,
//          L"Privilege Rights", L"SeRemoteShutdownPrivilege",L"",L"", ValueType::STRING, 0,
//          L"*S-1-5-32-544",L"", false, false },
//        { L"检查是否已限制取得文件或其他对象的所有权的帐户和组", CheckType::Registry,
//          L"Privilege Rights", L"SeProfileSingleProcessPrivilege",L"",L"", ValueType::STRING, 0,
//          L"*S-1-5-32-544",L"", false, false },
//        { L"检查是否已启用不显示最后的用户名策略", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
//          L"Dontdisplaylastusername",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//        { L"检查是否已正确配置提示用户在密码过期之前进行更改策略", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
//          L"PasswordExpiryWarning",L"",L"", ValueType::DWORD, 14,L"",L"", false, false },
//        { L"检查是否已正确配置锁定会话时显示用户信息策略", CheckType::Registry,
//          L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
//          L"DontDisplayLockedUserId",L"",L"", ValueType::DWORD, 3,L"",L"", false, false },
//
//          // 日志
//          { L"检查是否开启策略更改审核", CheckType::AuditPolicy,
//           L"",L"", L"System", L"Policy Change", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启登录事件审核", CheckType::AuditPolicy,
//           L"",L"", L"Logon/Logoff", L"Logon", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启对象访问审核", CheckType::AuditPolicy,
//           L"",L"", L"Object Access", L"File System", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启进程跟踪审核", CheckType::AuditPolicy,
//           L"",L"", L"System", L"Process Tracking", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启目录服务访问审核", CheckType::AuditPolicy,
//           L"",L"", L"Directory Service Access", L"Directory Service Access", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启特权使用审核", CheckType::AuditPolicy,
//           L"",L"", L"Privilege Use", L"Sensitive Privilege Use", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启系统事件审核", CheckType::AuditPolicy,
//           L"",L"", L"System", L"System Events", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启帐户登录事件审核", CheckType::AuditPolicy,
//           L"",L"", L"Account Logon", L"Account Logon", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查是否开启帐户管理审核", CheckType::AuditPolicy,
//           L"",L"", L"Account Management", L"Account Management", ValueType::DWORD, 3,L"",L"", false, false },
//          { L"检查系统日志文件达到最大大小时的动作的序号", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\System",
//            L"Retention",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//          { L"检查系统日志最大大小", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\System",
//            L"MaxSize",L"",L"", ValueType::DWORD, 8388608,L"",L"", false, false },
//          { L"检查应用程序日志文件达到最大大小时的动作的序号", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Application",
//            L"Retention",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//          { L"检查应用程序日志最大大小", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Application",
//            L"MaxSize",L"",L"", ValueType::DWORD, 8388608,L"",L"", false, false },
//          { L"检查安全日志文件达到最大大小时的动作的序号", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Security",
//            L"Retention",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//          { L"检查安全日志最大大小", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\eventlog\\Security",
//            L"MaxSize",L"",L"", ValueType::DWORD, 8388608,L"",L"", false, false },
//          { L"检查是否已启用并正确配置Windows网络时间同步服务(NTP)", CheckType::Registry,
//            L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\W32Time\\TimeProviders\\NtpServer",
//            L"Enabled",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//
//            // 防火墙+危险服务+危险端口
//            { L"检查远程桌面(RDP)服务端口", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp",
//              L"PortNumber",L"",L"", ValueType::DWORD, 3389, L"!3389",L"", false, false },
//            { L"检查是否已开启Windows防火墙", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile",
//              L"EnableFirewall",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"检查是否已启用SYN攻击保护", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"SynAttackProtect",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"检查TCP连接请求阈值", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxPortsExhausted",L"",L"", ValueType::DWORD, 5,L"",L"", false, false },
//            { L"检查取消尝试响应 SYN 请求之前要重新传输 SYN-ACK 的次数", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxConnectResponseRetransmissions",L"",L"", ValueType::DWORD, 2,L"",L"", false, false },
//            { L"检查处于SYN_RCVD 状态下的 TCP 连接阈值", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxHalfOpen",L"",L"", ValueType::DWORD, 500,L"",L"", false, false },
//            { L"检查处于SYN_RCVD 状态下,且至少已经进行了一次重新传输的TCP连接阈值", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxHalfOpenRetried",L"",L"", ValueType::DWORD, 400,L"",L"", false, false },
//            { L"检查是否已启用并正确配置ICMP攻击保护", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"EnableICMPRedirect",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"检查是否已禁用失效网关检测", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"EnableDeadGWDetect",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"检查是否已正确配置重传单独数据片段的次数", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"TcpMaxDataRetransmissions",L"",L"", ValueType::DWORD, 2,L"",L"", false, false },
//            { L"检查是否已禁用路由发现功能", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"PerformRouterDiscovery",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"检查是否已正确配置TCP连接存活时间", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"KeepAliveTime",L"",L"", ValueType::DWORD, 300000,L"",L"", false, false },
//            { L"检查是否已启用并正确配置TCP碎片攻击保护", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters",
//              L"EnablePMTUDiscovery",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"检查是否已禁用Windows硬盘默认共享", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Parameters",
//              L"AutoShareServer",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"检查是否已禁用Windows硬盘默认共享2", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\LanmanServer\\Parameters",
//              L"AutoShareWks",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"检查是否已启用并正确配置屏幕保护程序", CheckType::Registry,
//              L"HKEY_CURRENT_USER\\Control Panel\\Desktop",
//              L"ScreenSaveActive",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"检查是否已关闭Windows自动播放", CheckType::Registry,
//              L"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\AutoplayHandlers",
//              L"DisableAutoplay",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//            { L"检查是否已禁止Windows自动登录", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
//              L"AutoAdminLogon",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//            { L"域环境：检查是否已正确配置可被缓存保存的登录的个数策略", CheckType::Registry,
//              L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
//              L"CachedLogonsCount",L"",L"", ValueType::DWORD, 5,L"",L"", false, false },
//
//              // 无关紧要的个性化设置
//              { L"检查屏幕保护程序等待时间", CheckType::Registry,
//                L"HKEY_CURRENT_USER\\Control Panel\\Desktop",
//                L"ScreenSaveTimeOut",L"",L"", ValueType::DWORD, 300,L"",L"", false, false },
//              { L"检查是否已启用在恢复时显示登陆界面", CheckType::Registry,
//                L"HKEY_CURRENT_USER\\Control Panel\\Desktop",
//                L"ScreenSaverIsSecure",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//              { L"检查是否正确配置服务器在暂停会话前所需的空闲时间量", CheckType::Registry,
//                L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//                L"Autodisconnect",L"",L"", ValueType::DWORD, 15,L"",L"", false, false },
//              { L"检查是否已启用当登录时间用完时自动注销用户策略", CheckType::Registry,
//                L"HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\services\\LanmanServer\\Parameters",
//                L"Enableforcedlogoff",L"",L"", ValueType::DWORD, 1,L"",L"", false, false },
//              { L"检查是否已禁用登录时无须按 Ctrl+Alt+Del 策略", CheckType::Registry,
//                L"HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
//                L"Disablecad",L"",L"", ValueType::DWORD, 0,L"",L"", false, false },
//    };
//}

//// 解析注册表路径，返回根键和路径
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
//                // 路径解析失败
//                result.currentString = L"路径解析失败";
//                result.isCompliant = false;
//                results.push_back(result);
//                continue;
//            }
//            //std::wstring wideValueName = AnsiToWide(item.valueName);
//            RegistryHelper helper(rootKey, keyPath, item.valueName);
//            result.currentString = AnsiToWide(helper.ReadValue("", true)); // createIfMissing设为true自动创建路径
//
//            if (item.valueType == ValueType::DWORD) {
//                try {
//                    DWORD currentValue = std::stoul(result.currentString);
//                    // 处理特殊标准值（如远程桌面端口要求不等于3389）
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
//                // 字符串类型直接比较（适用于路径、账户列表等）
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
//                result.currentString = L"无法获取";
//                result.isCompliant = false;
//            }
//        }
//
//        results.push_back(result);
//    }
//
//    return results;
//}

// 修复所有不符合标准的项，返回是否全部修复成功
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
                // 处理特殊标准值（如远程桌面端口要求不等于3389，此处需用户自定义逻辑，示例中暂不处理）
                if (result.standardString == L"!3389") {
                    result.repairSuccess = false; // 需手动设置非3389端口，代码无法自动生成随机端口
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
                // 字符串类型修复（如路径、账户列表，此处简化为不支持自动修复）
                result.repairSuccess = false;
                result.currentString = L"字符串类型暂不支持自动修复";
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

// 检测单个项，返回CheckResult
CheckResult SecurityChecker::CheckOne(CheckResult rule) {
    CheckResult result = rule;

    if (rule.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        //std::wstring wideRegPath = AnsiToWide(rule.regPath);
        if (!ParseRegPath(rule.regPath, rootKey, keyPath)) {
            // 路径解析失败
            result.currentString = L"路径解析失败";
            result.isTrueDetect = false;//没检测到
            return result;
        }
        //std::wstring wideValueName = AnsiToWide(rule.valueName);
        RegistryHelper helper(rootKey, keyPath, rule.valueName);
        result.currentString = AnsiToWide(helper.ReadValue("", true)); // createIfMissing设为true自动创建路径

        if (rule.valueType == ValueType::DWORD) {
            try {
                DWORD currentValue = std::stoul(result.currentString);
                // 处理特殊标准值（如远程桌面端口要求不等于3389）
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
            // 字符串类型直接比较（适用于路径、账户列表等）
            result.isCompliant = (result.currentString == rule.standardString);
        }
    }
    else if (rule.checkType == CheckType::AuditPolicy) {
        bool auditSuccess = false;//成功事件1:0
        bool auditFailure = false;//失败事件2:0

        if (SecurityPolicyHelper::GetAuditPolicy(WideToAnsi(rule.auditCategory), WideToAnsi(rule.auditSubcategory),
            auditSuccess, auditFailure)) {
            DWORD currentValue = (auditSuccess ? 1 : 0) + (auditFailure ? 2 : 0);
            result.currentString = AnsiToWide(std::to_string(currentValue));//返回字符串类型的数字
            result.isCompliant = (currentValue == rule.standardDword);
        }
        else {
            result.currentString = L"无法获取";
            result.isTrueDetect = false;
        }
    }

    return result;
}



// 修复单个项，返回是否修复成功
CheckResult SecurityChecker::RepairOne(CheckResult& result) {
    // 执行修复操作主体逻辑
    bool repairActionSuccess = false;

    // 处理注册表类型修复
    if (result.checkType == CheckType::Registry) {
        HKEY rootKey;
        std::wstring keyPath;
        // 解析注册表路径为根键和子路径
        if (!ParseRegPath(result.regPath, rootKey, keyPath)) {
            result.repairSuccess = false;
            result.currentString = L"注册表路径解析失败";
            return result;
        }

        // 初始化注册表操作助手类
        RegistryHelper helper(rootKey, keyPath, result.valueName);
        // 根据值类型执行不同的修复操作
        if (result.valueType == ValueType::DWORD) {
            // 处理特殊值!3389（无法自动生成随机端口）
            repairActionSuccess = (result.standardString == L"!3389") ?
                false : helper.WriteDWORD(result.standardDword, true);
        }
        else if (result.valueType == ValueType::STRING) {
            // 转换标准字符串为ANSI格式并写入注册表
            std::string ansiValue = WideToAnsi(result.standardString);
            repairActionSuccess = helper.WriteString(ansiValue, true);
        }
        // 记录修复动作是否成功
        result.repairSuccess = repairActionSuccess;
    }
    // 处理审核策略类型修复
    else if (result.checkType == CheckType::AuditPolicy) {
        // 解析标准DWORD值为成功/失败审核标志
        bool auditSuccess = (result.standardDword & 1) != 0;
        bool auditFailure = (result.standardDword & 2) != 0;
        // 调用安全策略助手设置审核策略
        repairActionSuccess = SecurityPolicyHelper::SetAuditPolicy(
            WideToAnsi(result.auditCategory), WideToAnsi(result.auditSubcategory), auditSuccess, auditFailure);
        result.repairSuccess = repairActionSuccess;
    }

    // 修复动作失败时返回错误状态
    if (!repairActionSuccess) {
        result.currentString = L"修复动作执行失败";
        return result;
    }

    // 修复成功后调用检测函数验证结果
    CheckResult detectedResult = CheckOne(result);

    // 合并检测结果到修复结果中
    // 保留修复后读取的currentString，仅更新合规性和检测状态
    result.isCompliant = detectedResult.isCompliant;
    result.isTrueDetect = detectedResult.isTrueDetect;

    // 最终修复成功条件：修复动作成功且检测结果合规
    result.repairSuccess = repairActionSuccess && result.isCompliant;
    return result;
}






// 生成报告
bool SecurityChecker::GenerateReport(const std::vector<CheckResult>& results, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "序号,检测项名称,检测类型,注册表路径,字段名,审核类别,审核子类别,值类型,标准值,当前值,是否符合,修复结果\n";

    for (size_t i = 0; i < results.size(); i++) {
        const auto& result = results[i];
        std::string checkTypeStr = (result.checkType == CheckType::Registry) ? "注册表" : "审核策略";
        std::string valueTypeStr = (result.valueType == ValueType::DWORD) ? "DWORD" : "STRING";
        std::string standardValue = (result.valueType == ValueType::DWORD) ?
            (result.standardString.empty() ? std::to_string(result.standardDword) : WideToAnsi(result.standardString)) :
            WideToAnsi(result.standardString);

        // 处理特殊标准值显示（如!3389）
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
            << (result.isCompliant ? "是" : "否") << ","
            << (result.repairSuccess ? "成功" : "失败") << "\n";
    }

    file << "\n统计信息\n";
    file << "检测项总数," << results.size() << "\n";
    file << "符合标准项," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.isCompliant; }) << "\n";
    file << "修复成功项," << std::count_if(results.begin(), results.end(),
        [](const CheckResult& r) { return r.repairSuccess; }) << "\n";

    file.close();
    return true;
}