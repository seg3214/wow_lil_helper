D3DVIEWPORT9 viewport;
D3DXMATRIX proj, mview, Identity;
ID3DXLine* LineL;
ID3DXLine* LineL2;
ID3DXFont* g_pFont;

void DrawingRelease() {
	if (g_pFont)
		g_pFont->lpVtbl->Release(g_pFont);
	if (LineL)
		LineL->lpVtbl->Release(LineL);
	if (LineL2)
		LineL2->lpVtbl->Release(LineL2);
}
void DrawingLostDevice() {
	if (LineL)
		LineL->lpVtbl->OnLostDevice(LineL);
	if (LineL2)
		LineL2->lpVtbl->OnLostDevice(LineL2);

	if (g_pFont)
		g_pFont->lpVtbl->OnLostDevice(g_pFont);
}
void DrawingResetDevice() {
	if (LineL)
		LineL->lpVtbl->OnResetDevice(LineL);
	if (LineL2)
		LineL2->lpVtbl->OnResetDevice(LineL2);

	if (g_pFont)
		g_pFont->lpVtbl->OnResetDevice(g_pFont);
}
void Drawing_PrepareMatrices() {
	const struct SCameraInfo* pCamera = *(uint32_t*)(WorldFrame_x86 + WorldFrame_ActiveCamera_x86);

	const D3DXVECTOR3 vEyePt = {pCamera->fPos[0], pCamera->fPos[1], pCamera->fPos[2]};
	const D3DXVECTOR3 vLookatPt = {vEyePt.x + pCamera->fViewMat[0][0], vEyePt.y + pCamera->fViewMat[0][1], vEyePt.z + pCamera->fViewMat[0][2]};
	const D3DXVECTOR3 vUpVec = {0.0f, 0.0f, 1.0f};

	pDevice->lpVtbl->GetViewport(pDevice, &viewport);
	D3DXMatrixPerspectiveFovRH(&proj, pCamera->fFov * 0.6f, pCamera->aspect, pCamera->zNearPlane, pCamera->zFarPlane);

	D3DXMatrixLookAtRH(&mview, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixIdentity(&Identity);
}
