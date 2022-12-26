#include "includes.h"




SCameraInfo camera;

D3DVIEWPORT9 viewport;
D3DXMATRIX proj, mview, Identity;

ID3DXFont* g_pFont;
ID3DXLine* LineL;

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color){
	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(Line, 2, color);
}

void DrawFont(int X, int Y, D3DCOLOR Color, char *format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	RECT FontRect = { X, Y, X + 120, Y + 16 };
	g_pFont->DrawText(NULL, buffer, -1, &FontRect, DT_NOCLIP, Color);
	va_end(args);
}

bool WorldToScreen(D3DXVECTOR3* dest, D3DXVECTOR3* src){
	D3DXVec3Project(dest, src, &viewport, &proj, &mview, &Identity);
	return (dest->z < 1.f);//what is this? out of camera vision?
	//return (dest->z);
}

float distanceTwoVectors(D3DXVECTOR3* v1, D3DXVECTOR3* v2){
	return sqrt(pow((float)(v1->x - v2->x), 2) + pow((float)(v1->y - v2->y), 2) + pow((float)(v1->z - v2->z), 2));

}

void DrawESPBox(D3DXVECTOR3 dest,float v3){

	DrawLine(dest.x - 10, dest.y, dest.x + 10, dest.y, 1, D3DCOLOR_ARGB(255, 255, 000, 000));
	DrawLine(
		dest.x - 10,
		v3,
		dest.x + 10,
		v3,
		1, D3DCOLOR_ARGB(255, 255, 000, 000));
	DrawLine(dest.x - 10, v3, dest.x - 10, dest.y, 1, D3DCOLOR_ARGB(255, 255, 000, 000));
	DrawLine(dest.x + 10, dest.y, dest.x + 10, v3, 1, D3DCOLOR_ARGB(255, 255, 000, 000));
}

void drawPlayerBoxes(){
	
	//beforeWtS();
	uint32_t camera1 = *(uint32_t*)WorldFrame_x86;
	uint32_t cameraptr = *(uint32_t*)(camera1 + WorldFrame_ActiveCamera_x86);
	memcpy(&camera, (uint32_t*)cameraptr, sizeof(SCameraInfo));

	//readobjects();//should be moved out. to reading objects handler.
	D3DXMATRIX projMat;
	D3DXMatrixOrthoOffCenterLH(&projMat, 0, windowWidth, windowHeight, 0, 0, 1); //width and height ??? also did pass projMat as parameter

	D3DXVECTOR3 vEyePt = { camera.fPos[0], camera.fPos[1], camera.fPos[2] };
	D3DXVECTOR3 vLookatPt = {
		vEyePt.x + camera.fViewMat[0][0],
		vEyePt.y + camera.fViewMat[0][1],
		vEyePt.z + camera.fViewMat[0][2]
	};
	D3DXVECTOR3 vUpVec = { 0.0f, 0.0f, 1.0f };

	pDevice->GetViewport(&viewport);

	D3DXMatrixPerspectiveFovRH(&proj, camera.fFov* 0.6f, camera.aspect, camera.zNearPlane, camera.zFarPlane);

	D3DXMatrixLookAtRH(&mview, &vEyePt, &vLookatPt, &vUpVec);

	D3DXMatrixIdentity(&Identity);


	D3DXVECTOR3 mySelf3D,mySelf2D;
	mySelf3D.x = pMyselfstPlayerObject->pstClientObject->posx;
	mySelf3D.y = pMyselfstPlayerObject->pstClientObject->posy;
	mySelf3D.z = pMyselfstPlayerObject->pstClientObject->posz;
	

	bool myr = WorldToScreen(&mySelf2D, &mySelf3D);
	
	for (int i = 0; ar_Players[i].pstClientObject != 0; i++){
	
		if (!ar_Players[i].hostile) continue;
		D3DXVECTOR3 src = { ar_Players[i].pstClientObject->posx, ar_Players[i].pstClientObject->posy, ar_Players[i].pstClientObject->posz };
		D3DXVECTOR3 dest;
		bool rr = WorldToScreen(&dest, &src);
		
		if (rr){
			float dist = distanceTwoVectors(&mySelf3D, &src);
			
			DrawLine(mySelf2D.x, mySelf2D.y, dest.x, dest.y, 1, D3DCOLOR_ARGB(255, 255, 000, 000));
			
			float v0 = 20;
			float v1 = (dest.y - ar_Players[i].pstClientObject->height * v0);
			float v2 = sqrt(dist*3);
			float v3 = v1+ v2;

			DrawESPBox(dest,v3);
			
		}
	}
}

void drawing_Init(){
	D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont);
	D3DXCreateLine(pDevice, &LineL);
}

void drawing_Pulse(){
	drawPlayerBoxes();
	//DrawFont(50, 50, D3DCOLOR_ARGB(255, 255, 0, 255), "%f", 0.5f);
}

void drawing_cleanup(){
	if (g_pFont != NULL)
		g_pFont->Release();
	if (LineL != NULL)
		LineL->Release();
}
