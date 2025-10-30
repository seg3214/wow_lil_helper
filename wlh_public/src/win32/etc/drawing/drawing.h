
extern D3DVIEWPORT9 viewport;
extern D3DXMATRIX proj, mview, Identity;
extern ID3DXLine* LineL;
extern ID3DXLine* LineL2;
extern ID3DXFont* g_pFont;

extern RECT FontRect;
#define MAX_ObjectBoxCaption	 22
#define MAX_StoreVertices	 755
#define MAX_StoreGroupOfVertices 255
#define MAX_StoreObjectBox	 255

enum eCOLORS {
	COLOR_BLACK = D3DCOLOR_ARGB(255, 0, 0, 0),
	COLOR_WHITE = D3DCOLOR_ARGB(255, 255, 255, 255),
	COLOR_RED = D3DCOLOR_ARGB(255, 255, 0, 0),
	COLOR_GREEN = D3DCOLOR_ARGB(255, 0, 255, 0),
	COLOR_BLUE = D3DCOLOR_ARGB(255, 0, 0, 255),
	COLOR_BLUE1 = D3DCOLOR_ARGB(255, 0, 125, 255),
	COLOR_YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0),
	COLOR_CYAN = D3DCOLOR_ARGB(255, 0, 255, 255),
	COLOR_MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255),
	COLOR_BROWN = D3DCOLOR_ARGB(255, 240, 134, 29),
};

enum eCOLORS_CLASS {
	COLOR_NONE = D3DCOLOR_ARGB(255, 0, 0, 0),	      //
	COLOR_WARRIOR = D3DCOLOR_ARGB(255, 198, 155, 109),    //Tan
	COLOR_PALADIN = D3DCOLOR_ARGB(255, 244, 140, 186),    //Pink
	COLOR_HUNTER = D3DCOLOR_ARGB(255, 170, 211, 114),     //Pistachio
	COLOR_ROGUE = D3DCOLOR_ARGB(255, 255, 244, 104),      //Yellow*
	COLOR_PRIEST = D3DCOLOR_ARGB(255, 255, 255, 255),     //White*
	COLOR_DEATHKNIGHT = D3DCOLOR_ARGB(255, 196, 30, 58),  //Red
	COLOR_SHAMAN = D3DCOLOR_ARGB(255, 0, 112, 221),	      //Blue
	COLOR_MAGE = D3DCOLOR_ARGB(255, 63, 199, 235),	      //Light Blue
	COLOR_WARLOCK = D3DCOLOR_ARGB(255, 135, 136, 238),    //Purple
	COLOR_MONK = D3DCOLOR_ARGB(255, 0, 255, 152),	      //Spring Green
	COLOR_DRUID = D3DCOLOR_ARGB(255, 255, 124, 10),	      //Orange
};
extern D3DXVECTOR2 Store_Vertices[MAX_StoreVertices];
extern uint32_t COUNT_Store_Vertices;

struct stGroupOfVertices {
	D3DXVECTOR2* pVertStart;
	uint32_t VertCount;
	D3DCOLOR color;
};
extern struct stGroupOfVertices Store_GroupOfVertices[MAX_StoreGroupOfVertices];
extern uint32_t COUNT_Store_GroupOfVertices;

struct stObjectBox {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 tpos;

	struct stBGPlayer* pCacheObject;
	char caption[MAX_ObjectBoxCaption];
	D3DCOLOR color_caption;

	struct stGroupOfVertices* pGOVStart;
	uint32_t GOVCount;
};
extern struct stObjectBox Store_ObjectBox[MAX_StoreObjectBox];
extern uint32_t COUNT_Store_ObjectBox;

extern int drawing_cache_finalized;

extern void DrawingLostDevice();
extern void DrawingResetDevice();
extern void DrawingRelease();
extern void Drawing_Pulse();

extern void Drawing_Cache_AddEntry(struct stBGPlayer* pCacheObject);
extern void Drawing_CustomMainBox(struct stObjectBox* pbox, D3DCOLOR color);
extern void Drawing_CustomInnerBox(struct stObjectBox* pbox, D3DCOLOR color);
extern void Drawing_CustomCenterBox(struct stObjectBox* pbox, D3DCOLOR color);
extern void Drawing_CustomClassBox(struct stObjectBox* pbox, D3DCOLOR color);
// extern void Drawing_CustomBrownTankSquare(struct stObjectBox* pbox, D3DCOLOR color);
// extern void Drawing_CustomGreenHealerSquare(struct stObjectBox* pbox, D3DCOLOR color);
// extern void Drawing_CustomClassColorBox(struct stObjectBox* pbox, D3DCOLOR color);

struct stGroupOfVertices* Drawing_AddGroupOfVertices(D3DXVECTOR2* pstore_vertices, uint32_t count, D3DCOLOR color);
extern void Drawing_Cache_Init();
extern void Drawing_Cache_Wipe();
extern void Drawing_Cache_Reset();
extern void CensorRect();
