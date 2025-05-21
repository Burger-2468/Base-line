#pragma once

#include <windows.h>
#include "SecurityChecker.h"
#include <vector>
#include <string>
#include <memory>


struct CheckResultExport {
    char name[256];
    int checkType;
    char regPath[512];
    char valueName[256];
    char auditCategory[256];
    char auditSubcategory[256];
    int valueType;
    DWORD standardDword;
    char standardString[1024];
    char currentString[1024];
    BOOL isCompliant;
    BOOL repairSuccess;
};

extern "C" __declspec(dllexport) void* CreateSecurityChecker();
extern "C" __declspec(dllexport) void DestroySecurityChecker(void* checker);
extern "C" __declspec(dllexport) int CheckAll(void* checker, CheckResultExport* results, int maxCount);
