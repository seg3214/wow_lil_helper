#include "../shared/main.h"
#ifndef LINUX
// #include <stdint.h>
// #include <d3d9.h>
// #include <d3dx9.h>

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
typedef HRESULT(APIENTRY* tReset)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* params);
void* d3d9Device[119];
BYTE EndSceneBytes[7] = {0};
BYTE ResetBytes[5] = {0};
void* d3d9Device[119];
LPDIRECT3DDEVICE9 pDevice = 0;
tEndScene oEndScene = 0;
tReset oReset = 0;
volatile uint64_t alive;
extern HWND GetProcessWindow();
extern void Loop_EndScene();
extern char* TrampHook(char* src, char* dst, unsigned int len);
extern void PatchBack(BYTE* dst, BYTE* src, unsigned int size);

int GetD3D9Device(void** pTable, size_t size) {
	if (!pTable)
		return 0;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return 0;

	IDirect3DDevice9* pDummyDevice = 0;

	D3DPRESENT_PARAMETERS d3dpp = {0};
	d3dpp.Windowed = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->lpVtbl->CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->lpVtbl->CreateDevice(pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) {
			pD3D->lpVtbl->Release(pD3D);
			return 0;
		}
	}
	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->lpVtbl->Release(pDummyDevice);
	pD3D->lpVtbl->Release(pD3D);
	return 1;
}

HRESULT APIENTRY hkReset(LPDIRECT3DDEVICE9 o_pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
	// printf("hkReset.\n");
	DrawingLostDevice();
	HRESULT hRet = oReset(o_pDevice, pPresentationParameters);
	if (hRet == D3D_OK) {
		// printf("oReset OK\n");
		DrawingResetDevice();
	} else {
		// printf("oReset !!!!OK\n");
	}
	return hRet;

	//return oReset(o_pDevice,pPresentationParameters);
}

HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;
	Loop_EndScene();
	return oEndScene(o_pDevice);
}
void Capture_D3D9Device() {
	alive = GetTickCount64();

	int devres = GetD3D9Device(d3d9Device, sizeof(d3d9Device));
	while (!devres) {
		devres = GetD3D9Device(d3d9Device, sizeof(d3d9Device));
		// printf("waiting for application to be restored...\n");
		Sleep(500);
		alive = GetTickCount64();
	}
}
void InstallHooks() {
	//oReset = Hook((uint8_t*)d3d9Device[16], (uint8_t*)hkReset, &backup_Reset, 5);
	//oEndScene = Hook((uint8_t*)d3d9Device[42], (uint8_t*)hkEndScene, &backup_EndScene, 5);//hook endscene last because it fires before other hooks are done
	memcpy(ResetBytes, (char*)d3d9Device[16], 5);
	oReset = (tReset)TrampHook((char*)d3d9Device[16], (char*)hkReset, 5);
	memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
	oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);	//hook endscene last because it fires before other hooks are done
}
void UninstallHooks() {
	//unhook
	PatchBack((BYTE*)d3d9Device[42], EndSceneBytes, 7);
	PatchBack((BYTE*)d3d9Device[16], ResetBytes, 5);
}
#endif
