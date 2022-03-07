#pragma once

#include <psapi.h>

MODULEINFO GetModuleInfo(char* szModule)
{
    MODULEINFO ModInfo = {0};
    HMODULE hModule = GetModuleHandle(szModule);

    GetModuleInformation(GetCurrentProcess(), hModule, &ModInfo, sizeof(MODULEINFO));
    
    return ModInfo;
}

uintptr_t FindPattern(char* module, char* pattern, char* mask)
{
    uintptr_t base = (uintptr_t)GetModuleHandleA(module);
    uintptr_t size = GetModuleInfo(module).SizeOfImage;

    uintptr_t patternLength = (uintptr_t)strlen(mask);

    for (uintptr_t i = 0; i < size - patternLength; i++)
    {
        bool found = true;

        for (uintptr_t j = 0; j < patternLength; j++)
        {
            found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
        }

        if (found)
            return base + i;
    }

    return 0xDEADBEEF;
}