#ifndef MAIN_H
#define MAIN_H
//debug / release switch
#define _DEBUG
// #define DEBUG_OBJECTCACHE

//modules turned on or off
#define BGROLES
#define UNITBOXES

#if defined _DEBUG
#else
#define NDEBUG
#endif

// compiler : clang | cl.exe | clang-cl
#if (__clang__ && !_MSC_VER)
// clang
#define COMPILER_CLANG
#endif
#if (__clang__ && _MSC_VER)
// clang-cl
#define COMPILER_CLANG_CL
#endif
#if (!__clang__ && _MSC_VER)
// cl.exe
#define COMPILER_CL_EXE
#endif

// assuming we are on x86 ISA
#if (__x86_64__ || _WIN64)
#define x64BIT
#else
#define x32BIT
#endif

#if defined __gnu_linux__
#define LINUX
#elif defined _WIN32
#define WINDOWS
#if defined __MINGW32__
#define WINDOWS_MINGW
#elif defined _MSC_VER
#if defined __clang__
#define WINDOWS_MSVC_CLANG
#else
#define WINDOWS_MSVC
#endif
#endif
#endif

// Microsoft Visual C++ or clang-cl
#if (WINDOWS_MSVC || WINDOWS_MSVC_CLANG)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// mingw32 or mingw64
#elif defined WINDOWS_MINGW
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <mmsystem.h>
#include <direct.h>
#include <d3d9.h>
#include <d3dx9.h>

#elif defined LINUX
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef struct {
	float x;
	float y;
	float z;
} D3DXVECTOR3;
typedef struct {
	float x;
	float y;
} D3DXVECTOR2;

#endif

#include <stdio.h>
#include <time.h>
#include <assert.h>

///////////////////////////

#define szGroupMembers 39
#define szMemberName   13
#define szKnownSpells  200

//object list sizes
#define pf_size	  300
#define ph_size	  200
#define pph_size  150
#define ppf_size  150
#define vh_size	  100
#define vf_size	  100
#define th_size	  100
#define nf_size	  100
#define ch_size	  700
#define i_size	  300
#define go_size	  1500
#define mdkp_size 50
#define td_size	  20  //training dummy
#define dump_size pf_size + ph_size + vh_size + vf_size + go_size

#define PI 3.141592653589793
/////////////////////////////////
#include "utils/utils.h"
#include "../win32/etc/dx.h"
#include "../win32/etc/drawing/drawing.h"
#include "tests/logging/logging.h"
#include "../win32/notpublic/enums.h"
#include "../win32/notpublic/memory.h"
#include "structs.h"

#include "lua.h"
#include "clientDB.h"
#include "CDataStore.h"
#include "../win32/etc/objects/objects.h"
#include "packets.h"
#include "functions.h"
#include "z.scriptFunctions.h"
#include "tests/BGRoles/BGRoles.h"
#include "../win32/etc/objects/PlayerSpec/player_spec.h"
#include "tests/tests.h"

extern volatile uint64_t alive;
extern uint32_t timerflags;  //mask 0x10=30000 0x8=5000 0x4=2000 0x2=1000 0x1=500
extern int fishing_in_proggress;

extern struct stClientVars vars;
extern struct stObject2 stMyself, stTarget, stFocus, stPet;
extern const struct stObject2 *Myself, *Target, *Focus, *Pet;

extern struct SpellEntry* buffer_SpellEntry;
extern struct SkillLineEntry* buffer_SkillLineEntry;
extern struct stSpellIKnow arKnownSpells[szKnownSpells];

extern int DUMPREQUEST;
extern char* scriptName;
extern const char* varName;

extern int dump_count;
extern uint32_t arDumpObjects[dump_size];

extern int pf_count;
extern uint32_t arPlayersFriendly[pf_size];

extern int ph_count;
extern uint32_t arPlayersHostile[ph_size];

extern int pph_count;
extern uint32_t arPlayerPetHostile[pph_size];

extern int ppf_count;
extern uint32_t arPlayerPetFriendly[ppf_size];

extern int vh_count;
extern uint32_t arVehicleHostile[vh_size];

extern int vf_count;
extern uint32_t arVehicleFriendly[vf_size];

extern int th_count;
extern uint32_t arTotemHostile[th_size];

extern int nf_count;
extern uint32_t arNPCFriendly[nf_size];

extern int ch_count;
extern uint32_t arCreatureHostile[ch_size];

extern int i_count;
extern uint32_t arItems[i_size];

extern int go_count;
extern uint32_t arGOs[go_size];

extern int mdkp_count;
extern uint32_t arMyDKPets[mdkp_size];

extern int td_count;
extern uint32_t arTDs[td_size];

extern uint32_t* choose_DK();
extern uint32_t* choose_druid();
extern uint32_t* choose_mage();
extern uint32_t* choose_paladin();
extern uint32_t* choose_priest();
extern uint32_t* choose_shaman();
extern uint32_t* choose_warlock();
extern uint32_t* choose_warrior();
extern uint32_t* choose_buff();

extern void fishing();
extern void fishing_start();
extern void fishing_stop();

extern void Dump_players();
#endif
