#include <Windows.h>
#include <iostream>
#include "MinHook.h"
#include "sigscanner.h"

#pragma comment(lib, "libMinHook.x64.lib")

typedef long long (__fastcall* Execute_t)(uint8_t* pThis, long long a2, long long* a3);
Execute_t o_Execute;
Execute_t Execute_hook;

typedef long long(__fastcall* HandleInput_t)(uint8_t* a1, long long* a2);
HandleInput_t o_HandleInput;
HandleInput_t HandleInput_hook;

typedef long long (__fastcall* RestoreDeviceObjects_t)(uint8_t* a1);
RestoreDeviceObjects_t o_RestoreDeviceObjects;
RestoreDeviceObjects_t RestoreDeviceObjects_hook;

bool bExecutingCommand = false;

long long __fastcall hk_Execute(uint8_t* pThis, long long a2, long long* a3)
{
    fputs("[N-Phenethyl-4-piperidinone] CConsoleCmdManager::Execute called.\n", stdout);

    *(uint8_t*)(pThis + 24) = false;
    *(uint8_t*)(pThis + 25) = false;

    return o_Execute(pThis, a2, a3);
}

long long __fastcall hk_HandleInput(uint8_t* a1, long long* a2)
{
    //fputs("[N-Phenethyl-4-piperidinone] CInGameIdler::HandleInput called.\n", stdout);

    if (GetAsyncKeyState(VK_OEM_3))
    {
        *(uint8_t*)(a1 + 6601) = false;
    }
    else
    {
        *(uint8_t*)(a1 + 6601) = true;
    }

    return o_HandleInput(a1, a2);
}

long long __fastcall hk_RestoreDeviceObjects(uint8_t* a1)
{
    fputs("[N-Phenethyl-4-piperidinone] CInGameIdler::RestoreDeviceObjects called.\n", stdout);

    *(uint8_t*)(a1 + 1120) = false;

    return o_RestoreDeviceObjects(a1);
}

void InputThread(HMODULE hModule)
{
    while (true)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            fclose(stdin); 
            fclose(stdout);
            fclose(stderr);
            FreeConsole();
            MH_RemoveHook(Execute_hook);
            MH_RemoveHook(HandleInput_hook);
            MH_RemoveHook(RestoreDeviceObjects_hook);
            MH_Uninitialize();
            FreeLibraryAndExitThread(hModule, NULL);
           
            break;
        }

        Sleep(50);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        MH_Initialize();
        AllocConsole();
        FILE* fDummy;
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
        fputs("[N-Phenethyl-4-piperidinone] EU4 Console Enabler for ironman & multiplayer by Mastoid loaded.\n[N-Phenethyl-4-piperidinone] Please wait and do not interact with the game while searching for offsets...\n", stdout);

        //my sigmaker is a bit off. i shouldn't have to make sigs this ridiculously large, but sometimes the generated smaller ones don't work for some reason.
        Execute_hook = Execute_t(FindPattern(const_cast<char*>("eu4.exe"), const_cast<char*>("\x48\x8B\xC4\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x48\x89\x58\x08\x48\x89\x70\x10\x48\x89\x78\x18\x49\x8B\xF8\x48\x8B\xF2\x4C\x8B\xF9\x45\x33\xE4\x41\x8B\xDC\x89\x5C\x24\x20\x88\x5C\x24\x28\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x4C\x89\x64\x24\x00\x88\x5C\x24\x30\x41\xB0\x01\x48\x8B\x17\xE8\x00\x00\x00\x00\x4C\x8B\xE8\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x44\x38\x20\x75\x7B\x45\x38\x67\x18\x74\x75\x44\x88\x64\x24\x00\x45\x8D\x44\x24\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x0F\xB6\x44\x24\x00\x88\x06\x48\x8D\x4E\x08\x48\xC7\x41\x00\x00\x00\x00\x00\x4C\x89\x61\x10\x48\x83\x79\x00\x00\x72\x05\x48\x8B\x01\xEB\x03\x48\x8B\xC1\xC6\x00\x00\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x90\x4C\x8B\x44\x24\x00\x49\x83\xF8\x10\x0F\x82\x00\x00\x00\x00\x49\xFF\xC0\x48\x8B\x54\x24\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x4C\x89\x64\x24\x00\xC6\x44\x24\x00\x00\x0F\xB6\x05\x00\x00\x00\x00\xF6\xD8\x4D\x1B\xC0\x41\x83\xE0\x05\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x8B\x0F\xE8\x00\x00\x00\x00\x44\x8B\xF0\x4C\x8B\x44\x24\x00\x49\x83\xF8\x10\x72\x12\x49\xFF\xC0\x48\x8B\x54\x24\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x4C\x89\x64\x24\x00\xC6\x44\x24\x00\x00\x45\x85\xF6\x74\x05\x41\xC6\x47\x00\x00\x41\x80\x7F\x00\x00\x0F\x84\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x4C\x89\x65\xA0\xC6\x45\x90\x00\x0F\xB6\x05\x00\x00\x00\x00\xF6\xD8\x4D\x1B\xC0\x41\x83\xE0\x13\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4D\x90\xE8\x00\x00\x00\x00\x90\xBB\x00\x00\x00\x00\x89\x5C\x24\x20\x48\x8D\x55\x90\x48\x8B\x0F\xE8\x00\x00\x00\x00\x85\xC0\x74\x51\x48\xC7\x45\x00\x00\x00\x00\x00\x4C\x89\x65\x80\xC6\x44\x24\x00\x00\x0F\xB6\x05\x00\x00\x00\x00\xF6\xD8\x4D\x1B\xC0\x41\x83\xE0\x05\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\xBB\x00\x00\x00\x00\x89\x5C\x24\x20\x48\x8D\x54\x24\x00\x48\x8B\x0F\xE8\x00\x00\x00\x00\x85\xC0\x74\x05\x41\xB6\x01\xEB\x03\x45\x32\xF6\xF6\xC3\x04\x74\x34\x83\xE3\xFB\x89\x5C\x24\x20\x4C\x8B\x45\x88\x49\x83\xF8\x10\x72\x12\x49\xFF\xC0\x48\x8B\x54\x24\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x4C\x89\x65\x80\xC6\x44\x24\x00\x00\xF6\xC3\x02\x74\x31\x83\xE3\xFD\x89\x5C\x24\x20\x4C\x8B\x45\xA8\x49\x83\xF8\x10\x72\x10\x49\xFF\xC0\x48\x8B\x55\x90\x48\x8D\x4D\x90\xE8\x00\x00\x00\x00\x48\xC7\x45\x00\x00\x00\x00\x00\x4C\x89\x65\xA0\xC6\x45\x90\x00\x45\x84\xF6\x74\x5F\xC6\x44\x24\x00\x00\x41\xB8\x00\x00\x00\x00\x48\x8D\x15\x00\x00\x00\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x0F\xB6\x44\x24\x00\x88\x06\x48\x8D\x4E\x08\x48\xC7\x41\x00\x00\x00\x00\x00\x4C\x89\x61\x10"), const_cast<char*>("xxxxxxxxxxxxxxx????xxx????xxx?????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?????xxxx?xxxxxxxxxxx????xxxxxxxx????xxxxxxxxxxxxxxx?xxxx?xxx????xxxx?x????xxxx?xxxxxxxxx?????xxxxxxx??xxxxxxxxxxxxxxxxx?x????xxxxx?xxxxxx????xxxxxxx?xxxx?x????x????xxxx?????xxxx?xxx??xxx????xxxxxxxxxxxx????xxxx?x????xxxx?xxxx????xxxxxxx?xxxxxxxxxxxxx?xxxx?x????xxxx?????xxxx?xxx??xxxxxxxx??xxx??xx????xxx?????xxxxxxxxxxx????xxxxxxxxxxxx????xxxxx????xx????xxxxxxxxxxxx????xxxxxxx?????xxxxxxx??xxx????xxxxxxxxxxxx????xxxx?x????x????xxxxxxxx?xxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xxxx?x????xxx?????xxxxxxx??xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx?????xxxxxxxxxxxxxxxx??xx????xxx????xxxx?x????xxxx?xxxxxxxxx?????xxxx")));
        HandleInput_hook = HandleInput_t(FindPattern(const_cast<char*>("eu4.exe"), const_cast<char*>("\xE8\x00\x00\x00\x00\x48\x8B\x8F\x00\x00\x00\x00\x80\x79\x18\x00"), const_cast<char*>("x????xxx????xxxx")));
        RestoreDeviceObjects_hook = RestoreDeviceObjects_t(FindPattern(const_cast<char*>("eu4.exe"), const_cast<char*>("\x48\x89\x81\x00\x00\x00\x00\x33\xD2\x49\x8B\xCE"), const_cast<char*>("xxx????xxxxx")));

        printf("[N-Phenethyl-4-piperidinone] CConsoleCmdManager::Execute %p\n", Execute_hook);
        printf("[N-Phenethyl-4-piperidinone] CInGameIdler::HandleInput %p\n", HandleInput_hook);
        printf("[N-Phenethyl-4-piperidinone] CInGameIdler::RestoreDeviceObjects %p\n", RestoreDeviceObjects_hook);
        fputs("[N-Phenethyl-4-piperidinone] Offsets found.\n", stdout);

        MH_CreateHook(Execute_hook, &hk_Execute, (LPVOID*)&o_Execute);
        MH_EnableHook(Execute_hook);

        MH_CreateHook(HandleInput_hook, &hk_HandleInput, (LPVOID*)&o_HandleInput);
        MH_EnableHook(HandleInput_hook);

        MH_CreateHook(RestoreDeviceObjects_hook, &hk_RestoreDeviceObjects, (LPVOID*)&o_RestoreDeviceObjects);
        MH_EnableHook(RestoreDeviceObjects_hook);

        CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)InputThread, hModule, NULL, nullptr);

        fputs("[N-Phenethyl-4-piperidinone] Functions hooked.\n[N-Phenethyl-4-piperidinone] Cheat is ready to use. Press <Insert> to unload.\n", stdout);
    }

    return TRUE;
}
