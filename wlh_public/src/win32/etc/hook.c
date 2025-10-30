#include "../shared/main.h"
#ifndef LINUX

static HWND window;
int windowHeight, windowWidth;
extern void Capture_D3D9Device();
extern void Loop_HookThread();

BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() != procId)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND GetProcessWindow() {
	window = NULL;
	EnumWindows(enumWind, (LPARAM)NULL);

	RECT size;
	GetWindowRect(window, &size);
	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;
	windowHeight -= 29;
	windowWidth -= 5;
	return window;
}

void PatchBack(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oProc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oProc, &oProc);
}

int Hook(char* src, char* dst, int len) {
	if (len < 5)
		return 0;
	DWORD oProc;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
	memset(src, 0x90, len);
	uintptr_t relAddy = (uintptr_t)(dst - src - 5);
	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
	VirtualProtect(src, len, oProc, &oProc);
	return 1;
}

char* TrampHook(char* src, char* dst, unsigned int len) {
	if (len < 5)
		return 0;
	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xE9;
	*(uintptr_t*)(gateway + len + 1) = jumpAddy;
	if (Hook(src, dst, len)) {
		return gateway;
	} else
		return 0;
}
DWORD WINAPI HookThread(HMODULE hModule) {
	Capture_D3D9Device();
	Loop_HookThread();
	PostMessage(GetConsoleWindow(), WM_QUIT, 0, 0);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
}

#endif
