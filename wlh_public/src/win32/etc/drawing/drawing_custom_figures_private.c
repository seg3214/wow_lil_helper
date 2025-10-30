#include "../../shared/main.h"
struct stBoxType {
	D3DXVECTOR2 start_offset;
	D3DXVECTOR2 end_offset;
};
enum eBoxValues {
	MAIN_BOX_OFFSET_X2 = 10,
	MAIN_BOX_OFFSET_Y2 = -14,
	MAIN_BOX_OFFSET_X1 = -MAIN_BOX_OFFSET_X2 / 2,
	MAIN_BOX_OFFSET_Y1 = 0,

	INNER_BOX_OFFSET_PIXELS = 2,

	INNER_BOX_OFFSET_X2 = MAIN_BOX_OFFSET_X2 - INNER_BOX_OFFSET_PIXELS * 2,
	INNER_BOX_OFFSET_Y2 = MAIN_BOX_OFFSET_Y2 + INNER_BOX_OFFSET_PIXELS * 2,
	INNER_BOX_OFFSET_X1 = MAIN_BOX_OFFSET_X1 + INNER_BOX_OFFSET_PIXELS,
	INNER_BOX_OFFSET_Y1 = MAIN_BOX_OFFSET_Y1 - INNER_BOX_OFFSET_PIXELS,

	CENTER_BOX_OFFSET_PIXELS = 2,

	CENTER_BOX_OFFSET_X2 = INNER_BOX_OFFSET_X2 - CENTER_BOX_OFFSET_PIXELS * 2,
	CENTER_BOX_OFFSET_Y2 = INNER_BOX_OFFSET_Y2 + CENTER_BOX_OFFSET_PIXELS * 2,
	CENTER_BOX_OFFSET_X1 = INNER_BOX_OFFSET_X1 + CENTER_BOX_OFFSET_PIXELS,
	CENTER_BOX_OFFSET_Y1 = INNER_BOX_OFFSET_Y1 - CENTER_BOX_OFFSET_PIXELS,

};
struct stBoxType MainBox = {{MAIN_BOX_OFFSET_X1, MAIN_BOX_OFFSET_Y1}, {MAIN_BOX_OFFSET_X2, MAIN_BOX_OFFSET_Y2}};

struct stBoxType InnerBox = {{INNER_BOX_OFFSET_X1, INNER_BOX_OFFSET_Y1}, {INNER_BOX_OFFSET_X2, INNER_BOX_OFFSET_Y2}};
struct stBoxType CenterBox = {{CENTER_BOX_OFFSET_X1, CENTER_BOX_OFFSET_Y1}, {CENTER_BOX_OFFSET_X2, CENTER_BOX_OFFSET_Y2}};
RECT FontRect = {MAIN_BOX_OFFSET_X1, MAIN_BOX_OFFSET_Y1, MAIN_BOX_OFFSET_X2, MAIN_BOX_OFFSET_Y2};

void _Drawing_Custom_prologue() {
	if (COUNT_Store_Vertices >= MAX_StoreVertices) {
		printf("Store_Vertices is full \n");
		return;
	}
}
void _Drawing_Custom_epilogue(struct stObjectBox* pbox, D3DCOLOR color, uint32_t count) {
	D3DXVECTOR2* pvdest = &Store_Vertices[COUNT_Store_Vertices];
	COUNT_Store_Vertices += count;
	pbox->GOVCount++;
	struct stGroupOfVertices* pgov = Drawing_AddGroupOfVertices(pvdest, count, color);
	if (!pgov)
		return;
	if (!pbox->pGOVStart)
		pbox->pGOVStart = pgov;
}
void make_line_offset(D3DXVECTOR2* start, float x2, float y2, D3DXVECTOR2* dest) {
	dest[0] = dest[1] = start[0];
	dest[1].x += x2;
	dest[1].y += y2;
}

void make_box_offset(D3DXVECTOR2* start, float x2, float y2, D3DXVECTOR2* dest) {
	dest[0] = dest[1] = dest[2] = dest[3] = dest[4] = start[0];
	// dest[1].y=dest[0].

	make_line_offset(&dest[0], 0, y2, &dest[0]);
	make_line_offset(&dest[1], x2, 0, &dest[1]);
	make_line_offset(&dest[2], 0, -y2, &dest[2]);
}

void make_box(struct stBoxType* pbt, D3DXVECTOR2* start, D3DXVECTOR2* dest) {
	dest[0] = start[0];
	dest[0].x += pbt->start_offset.x;
	dest[0].y += pbt->start_offset.y;
	dest[1] = dest[2] = dest[3] = dest[4] = dest[0];

	make_line_offset(&dest[0], 0, pbt->end_offset.y, &dest[0]);
	make_line_offset(&dest[1], pbt->end_offset.x, 0, &dest[1]);
	make_line_offset(&dest[2], 0, -pbt->end_offset.y, &dest[2]);
}
void make_line_1(struct stBoxType* pbt, D3DXVECTOR2* start, D3DXVECTOR2* dest) {
	dest[0] = start[0];
	dest[0].x += pbt->start_offset.x;
	dest[0].y += pbt->start_offset.y;
	// dest[1] = dest[0];

	make_line_offset(&dest[0], pbt->end_offset.x, pbt->end_offset.y, &dest[0]);
}
void Drawing_CustomInnerBox(struct stObjectBox* pbox, D3DCOLOR color) {
	_Drawing_Custom_prologue();
	D3DXVECTOR2* pvdest = &Store_Vertices[COUNT_Store_Vertices];
	//>>>>>>>>>>>>>>>>>
	uint32_t vCount = 5;
	make_box(&InnerBox, &pbox->tpos, pvdest);
	//<<<<<<<<<<<<<<<<<
	_Drawing_Custom_epilogue(pbox, color, vCount);
}
// void Drawing_CustomClassColorBox(struct stObjectBox* pbox, D3DCOLOR color) {
// 	_Drawing_Custom_prologue();
// 	D3DXVECTOR2* pvdest = &Store_Vertices[COUNT_Store_Vertices];
// 	//>>>square red 5 lines
// 	uint32_t vCount = 2;
// 	memcpy(pvdest, &pbox->tpos, sizeof(struct D3DXVECTOR2));
// 	pvdest[1] = pvdest[0];
// 	pvdest[0].y -= 11.0f;
// 	pvdest[0].x += 10.0f;
// 	pvdest[1].y -= 11.0f;
// 	pvdest[1].x += 10.0f;
//
// 	//<<<<<<<<<<<<<<<<<
// 	_Drawing_Custom_epilogue(pbox, color, vCount);
// }
void Drawing_CustomCenterBox(struct stObjectBox* pbox, D3DCOLOR color) {
	_Drawing_Custom_prologue();
	D3DXVECTOR2* pvdest = &Store_Vertices[COUNT_Store_Vertices];
	//>>>>>>>>>>>>>>>>>
	uint32_t vCount = 5;
	make_box(&CenterBox, &pbox->tpos, pvdest);
	//<<<<<<<<<<<<<<<<<
	_Drawing_Custom_epilogue(pbox, color, vCount);
}
void Drawing_CustomMainBox(struct stObjectBox* pbox, D3DCOLOR color) {
	_Drawing_Custom_prologue();
	D3DXVECTOR2* pvdest = &Store_Vertices[COUNT_Store_Vertices];
	//>>>>>>>>>>>>>>>>>
	uint32_t vCount = 5;
	make_box(&MainBox, &pbox->tpos, pvdest);
	//<<<<<<<<<<<<<<<<<
	_Drawing_Custom_epilogue(pbox, color, vCount);
}
void Drawing_CustomClassBox(struct stObjectBox* pbox, D3DCOLOR color) {
	_Drawing_Custom_prologue();
	D3DXVECTOR2* pvdest = &Store_Vertices[COUNT_Store_Vertices];
	//>>>>>>>>>>>>>>>>>
	uint32_t vCount = 2;
	// make_box(&ClassBox, &pbox->tpos, pvdest);
	// make_line_1(&ClassBox, &pbox->tpos, pvdest);
	memcpy(pvdest, &pbox->tpos, sizeof(struct D3DXVECTOR2));
	pvdest[1] = pvdest[0];
	pvdest[0].x += MAIN_BOX_OFFSET_X1;
	pvdest[0].y += MAIN_BOX_OFFSET_Y2 - 2;
	float imax = MAIN_BOX_OFFSET_X2;
	float val = imax * pbox->pCacheObject->HP_Percent / 100;
	// printf("imax=%f val=%f\n", imax, val);

	pvdest[1].x = pvdest[0].x;
	pvdest[1].x += val;
	pvdest[1].y += MAIN_BOX_OFFSET_Y2 - 2;

	//<<<<<<<<<<<<<<<<<
	_Drawing_Custom_epilogue(pbox, color, vCount);
}
void CensorRect() {
	LineL2->lpVtbl->SetWidth(LineL2, 20);
	LineL2->lpVtbl->Begin(LineL2);
	D3DXVECTOR2 vrr[2];
	vrr[0].x = 100.0f;
	vrr[0].y = 35.0f;
	vrr[1].x = vrr[0].x + 300;
	vrr[1].y = vrr[0].y;

	LineL2->lpVtbl->Draw(LineL2, vrr, 2, D3DCOLOR_ARGB(255, 155, 155, 255));

	LineL2->lpVtbl->End(LineL2);
	///
	LineL2->lpVtbl->SetWidth(LineL2, 80);
	LineL2->lpVtbl->Begin(LineL2);

	vrr[0].x = 1200.0f;
	vrr[0].y = 770.0f;
	vrr[1].x = vrr[0].x + 300;
	vrr[1].y = vrr[0].y;

	LineL2->lpVtbl->Draw(LineL2, vrr, 2, D3DCOLOR_ARGB(255, 155, 155, 255));

	LineL2->lpVtbl->End(LineL2);
	///
	LineL2->lpVtbl->SetWidth(LineL2, 80);
	LineL2->lpVtbl->Begin(LineL2);

	vrr[0].x = 950.0f;
	vrr[0].y = 50.0f;
	vrr[1].x = vrr[0].x + 350;
	vrr[1].y = vrr[0].y;

	LineL2->lpVtbl->Draw(LineL2, vrr, 2, D3DCOLOR_ARGB(255, 155, 155, 255));
	LineL2->lpVtbl->End(LineL2);
	///
	LineL2->lpVtbl->SetWidth(LineL2, 160);
	LineL2->lpVtbl->Begin(LineL2);

	vrr[0].x = 40.0f;
	vrr[0].y = 655.0f;
	vrr[1].x = vrr[0].x + 300;
	vrr[1].y = vrr[0].y;

	LineL2->lpVtbl->Draw(LineL2, vrr, 2, D3DCOLOR_ARGB(255, 155, 155, 255));
	LineL2->lpVtbl->End(LineL2);
}
