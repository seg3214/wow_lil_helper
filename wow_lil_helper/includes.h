#define WIN32_LEAN_AND_MEAN
#pragma once
#include <Windows.h>
#include<stdint.h>
#include<iostream>
#include<time.h>
#include<d3d9.h>
#include<d3dx9.h>

//#pragma optimize( "", off )
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

extern "C"{
#include "../../includes/utils.h"
}
#include "descriptors.h"
#include "constants.h"
#include "pointers.h"

#include "clientDB.h"
#include "globals.h"
#include "clients_lua.h"
#include "internal_functions.h"
#include "infrastructure.h"
#include "objects.h"
#include "dx_hook.h"
#include "spells.h"
#include "fishing.h"
#include "drawing2.h"
#include "units.h"
#include "auras.h"
#include "we_do_alil_scripting.h"






