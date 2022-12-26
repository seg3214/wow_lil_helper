#pragma once
typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

bool GetD3D9Device(void** pTable, size_t size);
void Patch(BYTE* dst, BYTE* src, unsigned int size);
bool Hook(char* src, char* dst, int len);
char * TrampHook(char* src, char* dst, unsigned int len);



extern HWND hwnd_wow;
extern int windowHeight, windowWidth;