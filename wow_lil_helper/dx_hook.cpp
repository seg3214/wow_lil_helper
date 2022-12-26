#include "includes.h"

HWND hwnd_wow;
int windowHeight, windowWidth;

BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD procId;
	char buf[20] = {};
	const char* ww = "World of Warcraft";
	char * bp = buf;

	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() == procId){
		GetWindowTextA(handle, buf, sizeof(buf));
		if (strcmp(buf, ww) == 0) {
			//printf("= %s %p\n", buf, handle);
			hwnd_wow = handle;
			return FALSE;
		}
	}

	return TRUE;
}

HWND GetProcessWindow() {
	hwnd_wow = NULL;
	RECT clientRectSize;
	EnumWindows(enumWind, NULL);
	//if (!::GetClientRect(hwndWoW, &rc))
	//	return FALSE;
	//RECT size;
	GetWindowRect(hwnd_wow, &clientRectSize);
	//GetClientRect(hwnd_wow, &clientRectSize);
	windowWidth = clientRectSize.right - clientRectSize.left;
	windowHeight = clientRectSize.bottom - clientRectSize.top;

	windowHeight -= 29;
	windowWidth -= 5;
	//printf("window %p\n", window);
	return hwnd_wow;
}

bool GetD3D9Device(void** pTable, size_t size) {
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) {
			pD3D->Release();
			return false;
		}
	}
	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}

void Patch(BYTE* dst, BYTE* src, unsigned int size) {
	DWORD oProc;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oProc);
	memcpy(dst, src, size);
	VirtualProtect(dst, size, oProc, &oProc);
}

bool Hook(char* src, char* dst, int len) {
	if (len < 5) return false;
	DWORD oProc;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oProc);
	memset(src, 0x90, len);
	uintptr_t relAddy = (uintptr_t)(dst - src - 5);
	*src = (char)0xE9;
	*(uintptr_t*)(src + 1) = (uintptr_t)relAddy;
	VirtualProtect(src, len, oProc, &oProc);
}

char* TrampHook(char* src, char* dst, unsigned int len) {
	if (len < 5) return 0;
	char* gateway = (char*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	memcpy(gateway, src, len);
	uintptr_t jumpAddy = (uintptr_t)(src - gateway - 5);
	*(gateway + len) = (char)0xE9;
	*(uintptr_t*)(gateway + len + 1) = jumpAddy;
	if (Hook(src, dst, len)) {
		return gateway;
	}
	else return nullptr;
}