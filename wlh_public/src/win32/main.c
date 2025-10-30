#include "shared/main.h"
#include "notpublic/data_private.c"

extern DWORD WINAPI HookThread(HMODULE hModule);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hModule);
			if (AllocConsole()) {
				freopen("CONOUT$", "w", stdout);
				SetConsoleTitle("Console");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HookThread, hModule, 0, 0));
			return TRUE;

		case DLL_PROCESS_DETACH:
			break;
			return TRUE;
	}
	return TRUE;
}
