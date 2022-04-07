#include <iostream>
#include <dlfcn.h>
#include <funchook.h>
#include <unistd.h>

typedef void (*SetIsMultiplayer_t)(void* pThis, bool a2);
SetIsMultiplayer_t o_SetIsMultiplayer;

typedef void (*SetIsRelease_t)(void* pThis, bool a2);
SetIsRelease_t o_SetIsRelease;

typedef long long (*HandleInput_t)(uint8_t* pThis);
HandleInput_t o_HandleInput;

typedef long long (*RestoreDeviceObjects_t)(uint8_t* a1);
RestoreDeviceObjects_t o_RestoreDeviceObjects;

typedef long long (*CCountryViewShow_t)(long long a1);
CCountryViewShow_t o_CCountryViewShow;

typedef void (*WeWentOOS_t)(void** pThis, const char* a2);
WeWentOOS_t o_WeWentOOS;

bool bHasGameStarted = false;

void hk_SetIsMultiplayer(void* pThis, bool a2)
{
	o_SetIsMultiplayer(pThis, false);
}

void hk_SetIsRelease(void* pThis, bool a2)
{
	o_SetIsRelease(pThis, false);
}

long long hk_HandleInput(uint8_t* pThis)
{
	if (bHasGameStarted)
		*(uint8_t*)(pThis + 6873) = false;

	return o_HandleInput(pThis);
}

long long hk_RestoreDeviceObjects(uint8_t* a1)
{
    *(uint8_t*)(a1 + 1104) = false;

    return o_RestoreDeviceObjects(a1);
}

long long hk_CCountryViewShow(long long a1)
{
    bHasGameStarted = true;

    return o_CCountryViewShow(a1);
}

void hk_WeWentOOS(void** pThis, const char* a2)
{
    //o_WeWentOOS(pThis, a2);
}

int __attribute__ ((constructor)) Fentanyl_Main()
{
    funchook_t* funchook = funchook_create();

	o_SetIsMultiplayer = reinterpret_cast<SetIsMultiplayer_t>(dlsym(RTLD_DEFAULT, "_ZN18CConsoleCmdManager16SetIsMultiplayerEb"));
	o_SetIsRelease = reinterpret_cast<SetIsRelease_t>(dlsym(RTLD_DEFAULT, "_ZN18CConsoleCmdManager12SetIsReleaseEb"));
	o_HandleInput = reinterpret_cast<HandleInput_t>(dlsym(RTLD_DEFAULT, "_ZN12CInGameIdler11HandleInputEv"));
	o_RestoreDeviceObjects = reinterpret_cast<HandleInput_t>(dlsym(RTLD_DEFAULT, "_ZN12CInGameIdler20RestoreDeviceObjectsEv"));
	o_CCountryViewShow = reinterpret_cast<CCountryViewShow_t>(dlsym(RTLD_DEFAULT, "_ZN12CCountryView4HideEv"));
	o_WeWentOOS = reinterpret_cast<WeWentOOS_t>(dlsym(RTLD_DEFAULT, "_ZN11COOSHandler9WeWentOOSEPKc"));

	funchook_prepare(funchook, (void**)&o_SetIsMultiplayer, (void*)hk_SetIsMultiplayer);
	funchook_prepare(funchook, (void**)&o_SetIsRelease, (void*)hk_SetIsRelease);
	funchook_prepare(funchook, (void**)&o_HandleInput, (void*)hk_HandleInput);
	funchook_prepare(funchook, (void**)&o_RestoreDeviceObjects, (void*)hk_RestoreDeviceObjects);
	funchook_prepare(funchook, (void**)&o_CCountryViewShow, (void*)hk_CCountryViewShow);
	funchook_prepare(funchook, (void**)&o_WeWentOOS, (void*)hk_WeWentOOS);
	funchook_install(funchook, 0);

	return 0;
}
