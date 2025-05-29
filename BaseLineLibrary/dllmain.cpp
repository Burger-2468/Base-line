//// DllMain.cpp
//#include "DllMain.h"
//
//extern "C" __declspec(dllexport) void* CreateSecurityChecker() {//该函数返回一个SecurityChecker的实例
//    return new SecurityChecker();
//}
//
//extern "C" __declspec(dllexport) void DestroySecurityChecker(void* checker) {
//    if (checker) {
//        delete static_cast<SecurityChecker*>(checker);
//    }
//}
//
//extern "C" __declspec(dllexport) int CheckAll(void* checker, CheckResultExport* results, int maxCount) {
//    if (!checker || !results || maxCount <= 0) return 0;
//
//    SecurityChecker* sc = static_cast<SecurityChecker*>(checker);
//    std::vector<CheckResult> checkResults = sc->CheckAll();
//
//    int count = (std::min)(static_cast<int>(checkResults.size()), maxCount);
//    for (int i = 0; i < count; i++) {
//        const auto& src = checkResults[i];
//        auto& dst = results[i];
//
//        // 宽字符串转ANSI并复制到固定缓冲区
//        auto SafeCopy = [](char* dest, size_t destSize, const std::wstring& src) {
//            std::string narrow = WideToAnsi(src);
//            strncpy_s(dest, destSize, narrow.c_str(), _TRUNCATE);
//            };
//
//        SafeCopy(dst.name, sizeof(dst.name), src.name);
//        dst.checkType = static_cast<int>(src.checkType);
//        SafeCopy(dst.regPath, sizeof(dst.regPath), src.regPath);
//        SafeCopy(dst.valueName, sizeof(dst.valueName), src.valueName);
//        SafeCopy(dst.auditCategory, sizeof(dst.auditCategory), src.auditCategory);
//        SafeCopy(dst.auditSubcategory, sizeof(dst.auditSubcategory), src.auditSubcategory);
//        dst.valueType = static_cast<int>(src.valueType);
//        dst.standardDword = src.standardDword;
//        SafeCopy(dst.standardString, sizeof(dst.standardString), src.standardString);
//        SafeCopy(dst.currentString, sizeof(dst.currentString), src.currentString);
//        dst.isCompliant = src.isCompliant ? TRUE : FALSE;
//        dst.repairSuccess = FALSE;
//    }
//    return count;
//}
//
//extern "C" __declspec(dllexport) BOOL RepairAll(void* checker, CheckResultExport* results, int count) {
//    if (!checker || !results || count <= 0) return FALSE;
//
//    SecurityChecker* sc = static_cast<SecurityChecker*>(checker);
//    std::vector<CheckResult> checkResults(count);
//
//    // 转换CheckResultExport到CheckResult
//    for (int i = 0; i < count; i++) {
//        const auto& src = results[i];
//        auto& dst = checkResults[i];
//
//        auto SafeConvert = [](const char* src) -> std::wstring {
//            return AnsiToWide(src);
//            };
//
//        dst.name = SafeConvert(src.name);
//        dst.checkType = static_cast<CheckType>(src.checkType);
//        dst.regPath = SafeConvert(src.regPath);
//        dst.valueName = SafeConvert(src.valueName);
//        dst.auditCategory = SafeConvert(src.auditCategory);
//        dst.auditSubcategory = SafeConvert(src.auditSubcategory);
//        dst.valueType = static_cast<ValueType>(src.valueType);
//        dst.standardDword = src.standardDword;
//        dst.standardString = SafeConvert(src.standardString);
//        dst.currentString = SafeConvert(src.currentString);
//        dst.isCompliant = (src.isCompliant == TRUE);
//        dst.repairSuccess = (src.repairSuccess == TRUE);
//    }
//
//    BOOL success = sc->RepairAll(checkResults) ? TRUE : FALSE;
//
//    // 回写修复结果
//    for (int i = 0; i < count; i++) {
//        const auto& src = checkResults[i];
//        auto& dst = results[i];
//
//        std::string current = WideToAnsi(src.currentString);
//        strncpy_s(dst.currentString, sizeof(dst.currentString), current.c_str(), _TRUNCATE);
//        dst.isCompliant = src.isCompliant ? TRUE : FALSE;
//        dst.repairSuccess = src.repairSuccess ? TRUE : FALSE;
//    }
//
//    return success;
//}
//
//extern "C" __declspec(dllexport) BOOL GenerateReport(void* checker, const char* filePath, CheckResultExport* results, int count) {
//    if (!checker || !filePath || !results || count <= 0) return FALSE;
//
//    SecurityChecker* sc = static_cast<SecurityChecker*>(checker);
//    std::vector<CheckResult> checkResults(count);
//
//    for (int i = 0; i < count; i++) {
//        const auto& src = results[i];
//        auto& dst = checkResults[i];
//
//        dst.name = AnsiToWide(src.name);
//        dst.checkType = static_cast<CheckType>(src.checkType);
//        dst.regPath = AnsiToWide(src.regPath);
//        dst.valueName = AnsiToWide(src.valueName);
//        dst.auditCategory = AnsiToWide(src.auditCategory);
//        dst.auditSubcategory = AnsiToWide(src.auditSubcategory);
//        dst.valueType = static_cast<ValueType>(src.valueType);
//        dst.standardDword = src.standardDword;
//        dst.standardString = AnsiToWide(src.standardString);
//        dst.currentString = AnsiToWide(src.currentString);
//        dst.isCompliant = (src.isCompliant == TRUE);
//        dst.repairSuccess = (src.repairSuccess == TRUE);
//    }
//
//    return sc->GenerateReport(checkResults, filePath) ? TRUE : FALSE;
//}
//
//BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
//    switch (ul_reason_for_call) {
//    case DLL_PROCESS_ATTACH:
//    case DLL_THREAD_ATTACH:
//    case DLL_THREAD_DETACH:
//    case DLL_PROCESS_DETACH:
//        break;
//    }
//    return TRUE;
//}