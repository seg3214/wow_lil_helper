#include "../../shared/main.h"

D3DXVECTOR2 Store_Vertices[MAX_StoreVertices] = {0};
uint32_t COUNT_Store_Vertices = 0;
struct stGroupOfVertices Store_GroupOfVertices[MAX_StoreGroupOfVertices] = {0};
uint32_t COUNT_Store_GroupOfVertices = 0;

struct stObjectBox Store_ObjectBox[MAX_StoreObjectBox] = {0};
uint32_t COUNT_Store_ObjectBox = 0;
int drawing_cache_finalized = 0;

void Drawing_Cache_Init() {}
void Drawing_Cache_Wipe() {
	memset(&Store_ObjectBox, 0, sizeof(Store_ObjectBox));
	COUNT_Store_ObjectBox = 0;
	memset(&Store_GroupOfVertices, 0, sizeof(Store_GroupOfVertices));
	COUNT_Store_GroupOfVertices = 0;
	memset(&Store_Vertices, 0, sizeof(Store_Vertices));
	COUNT_Store_Vertices = 0;

	drawing_cache_finalized = 0;
}

void Drawing_Cache_Reset() {
	Drawing_Cache_Wipe();
	Drawing_Cache_Init();
}
struct stGroupOfVertices* Drawing_AddGroupOfVertices(D3DXVECTOR2* pstore_vertices, uint32_t count, D3DCOLOR color) {
	if (COUNT_Store_GroupOfVertices >= MAX_StoreGroupOfVertices) {
		printf("Store_GroupOfVertices is full \n");
		return 0;
	}

	struct stGroupOfVertices* pNewGOV = &Store_GroupOfVertices[COUNT_Store_GroupOfVertices];
	pNewGOV->pVertStart = pstore_vertices;
	pNewGOV->VertCount = count;
	pNewGOV->color = color;
	COUNT_Store_GroupOfVertices++;
	return pNewGOV;
}
void Drawing_ProjectCache() {
	//if empty
	if (!COUNT_Store_ObjectBox)
		return;

	D3DXVECTOR3* pos = &Store_ObjectBox[0].pos;
	D3DXVECTOR3* tpos = &Store_ObjectBox[0].tpos;
	uint32_t vstride = sizeof(struct stObjectBox);
	D3DXVec3ProjectArray(tpos, vstride, pos, vstride, &viewport, &proj, &mview, &Identity, COUNT_Store_ObjectBox);
	// log_line(bg_id, "govstart %X ,govcout %d\n", Store_ObjectBox[0].pGOVStart, Store_ObjectBox[0].GOVCount);
	// log_dump(bg_id, "uni1", 0, &Store_ObjectBox[0], sizeof(struct stObjectBox));
}

void Drawing_Cache_AddEntry(struct stBGPlayer* pCacheObject) {
	if (!pCacheObject)
		return;
	if (COUNT_Store_ObjectBox >= MAX_StoreObjectBox) {
		printf("Store_ObjectBox is full \n");
		return;
	}
	// log_line(bg_id, "adding UB type=%d count=%d x=%f %s\n", type, COUNT_Store_ObjectBox, pos->x, &buffer);
	struct stObjectBox* box = &Store_ObjectBox[COUNT_Store_ObjectBox];
	COUNT_Store_ObjectBox++;
	box->pos = pCacheObject->pos;
	box->pCacheObject = pCacheObject;
}
void Drawing_DrawObjectCaptions(struct stObjectBox* pbox) {
	RECT FontRect2 = {pbox->tpos.x + FontRect.left, pbox->tpos.y + FontRect.top, pbox->tpos.x + FontRect.right, pbox->tpos.y + FontRect.bottom};
	g_pFont->lpVtbl->DrawText(g_pFont, NULL, &pbox->caption, -1, &FontRect2, DT_NOCLIP, pbox->color_caption);
}

void Drawing_AppendCaptionText(struct stObjectBox* pbox, char* format, ...) {
	va_list args;
	char buffer[256];
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	int ln = strlen(&pbox->caption) + strlen(&buffer);
	if (ln >= MAX_ObjectBoxCaption) {
		printf("error Drawing_AppendCaptionText caption size\n");
		return;
	}
	strcat(&pbox->caption, &buffer);
}
D3DCOLOR GetClassColor(uint32_t unitClass) {
	static const enum eCOLORS_CLASS classcolor_map[] = {
	    COLOR_NONE, COLOR_WARRIOR, COLOR_PALADIN, COLOR_HUNTER, COLOR_ROGUE, COLOR_PRIEST, COLOR_DEATHKNIGHT, COLOR_SHAMAN, COLOR_MAGE, COLOR_WARLOCK, COLOR_MONK, COLOR_DRUID,
	};
	return classcolor_map[unitClass];
}
void Drawing_AddObjectFigures(struct stObjectBox* pbox) {
	// return;
	assert(pbox->pCacheObject);
	switch (pbox->pCacheObject->objType) {
		case CACHE_OBJECT_PLAYER: {
			// Drawing_CustomMainBox(pbox, COLOR_BLUE1);
			// Drawing_CustomInnerBox(pbox, COLOR_GREEN);
			if (pbox->pCacheObject->tags_player & CACHE_PLAYER_DEAD)
				return;
			pbox->color_caption = COLOR_WHITE;
			if (pbox->pCacheObject->tags_player & CACHE_PLAYER_GEAR_PVE) {
				Drawing_AppendCaptionText(pbox, "PVE ");
			}
			if (pbox->pCacheObject->tags_player & CACHE_PLAYER_SPEC_TANK) {
				Drawing_AppendCaptionText(pbox, "TNK");
				Drawing_CustomInnerBox(pbox, COLOR_BROWN);
			} else if (pbox->pCacheObject->tags_player & CACHE_PLAYER_SPEC_HEALER) {
				Drawing_AppendCaptionText(pbox, "HLR");
				Drawing_CustomInnerBox(pbox, COLOR_GREEN);
			}

			if (pbox->pCacheObject->tags_player & CACHE_PLAYER_ATTACKABLE) {
				pbox->color_caption = COLOR_RED;
				Drawing_CustomMainBox(pbox, COLOR_RED);

			} else if (pbox->pCacheObject->tags_player & CACHE_PLAYER_ASSISTABLE) {
				pbox->color_caption = COLOR_BLUE1;
				Drawing_CustomMainBox(pbox, COLOR_BLUE1);
			}
			Drawing_CustomClassBox(pbox, GetClassColor(pbox->pCacheObject->unitClass));
			break;
		}
		case CACHE_OBJECT_NPC: {
			break;
		}
	}
}
void Drawing_ProccessCache() {
	// LineL->lpVtbl->SetWidth(LineL, 1);
	LineL->lpVtbl->Begin(LineL);
	for (int i = 0; i < COUNT_Store_ObjectBox; i++) {
		struct stObjectBox* pbox = &Store_ObjectBox[i];
		//>>Store_ObjectBox
		if (pbox->tpos.z < 1.f) {
			Drawing_AddObjectFigures(pbox);
			Drawing_DrawObjectCaptions(pbox);
		}
		//<<Store_ObjectBox
		uint32_t GOV_Count = pbox->GOVCount;
		for (int j = 0; j < GOV_Count; j++) {
			struct stGroupOfVertices* pGOV = &pbox->pGOVStart[j];
			uint32_t v_Count = pGOV->VertCount;
			//>>Store_GroupOfVertices

			//<<Store_GroupOfVertices
			for (int v = 0; v < v_Count; v++) {
				D3DXVECTOR2* pv2 = pGOV->pVertStart;
				//>>Store_Vertices
				//<<Store_Vertices
			}
			//
			LineL->lpVtbl->Draw(LineL, pGOV->pVertStart, pGOV->VertCount, pGOV->color);
		}
	}
	LineL->lpVtbl->End(LineL);
}
