#include "../../shared/main.h"
#include "drawing_device_related_private.c"
#include "drawing_cache_private.c"
#include "drawing_custom_figures_private.c"

void Drawing_Pulse() {
	if (!g_pFont)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont);
	if (!LineL) {
		D3DXCreateLine(pDevice, &LineL);
		LineL->lpVtbl->SetWidth(LineL, 1);
	}
	if (!LineL2) {
		D3DXCreateLine(pDevice, &LineL2);
		LineL2->lpVtbl->SetWidth(LineL2, 2);
	}

	Drawing_PrepareMatrices();

	if (drawing_cache_finalized) {
		Drawing_ProjectCache();
		Drawing_ProccessCache();
	}
	// CensorRect();
}
