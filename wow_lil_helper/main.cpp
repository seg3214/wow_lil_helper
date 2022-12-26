#include "includes.h"

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
LPDIRECT3DDEVICE9 pDevice = nullptr;

int SHUTINGDOWN = 0;

void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;
	if (!SHUTINGDOWN){
		CheckUpOnTimers();
		CheckUpOnWorldLoaded();
		//PulseWorldLoaded();
		PulseEndScene();
		//drawing_Pulse();
	}
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	//hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);

		program_started();
	}

	while (!GetAsyncKeyState(VK_END)) {
		Sleep(1);
	}
	SHUTINGDOWN = 1;
	Sleep(1000);
	//cleanup
	cleanup();

	//unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	//uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		if (AllocConsole()) {
			freopen("CONOUT$", "w", stdout);
			SetConsoleTitle("Console");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
		return TRUE;

	case DLL_PROCESS_DETACH:
		printf("exiting.");
		FreeConsole();
		break;
	}
}

