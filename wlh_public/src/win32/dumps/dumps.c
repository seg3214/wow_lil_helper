
#include "../shared/main.h"
#include "unit_fields_private.h"

char* dump_folder = "DUMP";
char* dump_binary_name = "players.bin";
char* dump_deref_binary_name = "deref_players.bin";
char* dump_binary_auras_name = "players_auras.bin";
char* dump_csv_name = "deref_players.csv";
char* dump_unitFields_name = "players_unitFields.csv";
char* dump_objects_csv_name = "deref_objects.csv";
char* dump_objects_unitFields_name = "objects_unitFields.csv";
struct stDerefObject2 {
	uint32_t objBase;
	uint64_t GUID;
	uint32_t Type;
	byte UnitClass;
	uint32_t Health;
	uint32_t MaxHealth;
	uint32_t HealthPercent;
	uint32_t Power;	    // not a pointer because needs correction for rage type
	uint32_t MaxPower;  // not a pointer because needs correction for rage type
	uint32_t PowerPercent;
	byte PowerType;	 //%d  -2 Health,  0 Mana, 1 Rage, 2 Focus, 3 Energy, 5 Runes, 6 Runic Power
	D3DXVECTOR3 pos;
	//float *pHeight;
	uint32_t CastingId;
	uint32_t ChanneledCastingId;
	float CombatReach;
	float BoundingRadius;
	float distanceToMe;
	struct stAura auras[100];
};

void _deref(uint32_t objBase, struct stDerefObject2* dof2) {
	struct stObject2 x = {0};
	initObject(objBase, &x);
	dof2->objBase = x.objBase;
	dof2->GUID = *x.pGUID;
	dof2->Type = *x.pType;
	dof2->UnitClass = *x.pUnitClass;
	dof2->Health = *x.pHealth;
	dof2->MaxHealth = *x.pMaxHealth;
	dof2->HealthPercent = x.HealthPercent;
	dof2->Power = x.Power;
	dof2->MaxPower = x.MaxPower;
	dof2->PowerPercent = x.PowerPercent;
	dof2->PowerType = *x.PowerType;
	dof2->pos = x.pos;
	dof2->CastingId = *x.CastingId;
	dof2->ChanneledCastingId = *x.ChanneledCastingId;
	dof2->CombatReach = *x.pCombatReach;
	dof2->BoundingRadius = *x.pBoundingRadius;
	dof2->distanceToMe = x.distanceToMe;
	memcpy(&dof2->auras, &x.auras, sizeof(struct stAura) * 100);
}
void _make_directory(const char* name) {
// Microsoft Visual C++ or clang-cl.exe or when the target tuple ends in -msvc
#if (_MSC_VER && !(__clang__ || __GNUC__)) || (_MSC_VER && __clang__)
	//--------------------------
	_mkdir(name);
//--------------------------
// mingw32 or mingw64
#elif __MINGW32__
	//--------------------------
	_mkdir(name);
//--------------------------
// clang. no microsoft.no mingw
#elif __clang__
	//--------------------------
	mkdir(name, 0755);
//--------------------------
#endif
}
void _makedirs() {
	_make_directory(dump_folder);
}
//dumps into csv format and binary
void Dump_players() {
	char cwd[1000] = {0};
	getcwd(cwd, sizeof(cwd));
	_makedirs();
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char s[1000] = {0};
	sprintf(&s, "%d-%02d-%02d_%02d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	// sprintf(&s, "%d-%02d-%02d_%02d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	chdir(dump_folder);
	_make_directory(&s);
	chdir(&s);
	FILE* pFile;
	//dump_binary_name
	if ((pFile = fopen(dump_binary_name, "wb")) == NULL) {
		printf("The file dump_binary_name file was not opened\n");
	} else {
		for (int i = 0; arPlayersFriendly[i] != 0; i++) {
			struct stObject2 x = {0};
			initObject(arPlayersFriendly[i], &x);
			fwrite(&x, sizeof(struct stObject2), 1, pFile);
		}
		for (int i = 0; arPlayersHostile[i] != 0; i++) {
			struct stObject2 x = {0};
			initObject(arPlayersHostile[i], &x);
			fwrite(&x, sizeof(struct stObject2), 1, pFile);
		}
	}
	fclose(pFile);
	pFile = 0;
	//dump_deref_binary_name
	if ((pFile = fopen(dump_deref_binary_name, "wb")) == NULL) {
		printf("The file dump_deref_binary_name file was not opened\n");
	} else {
		for (int i = 0; arPlayersFriendly[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			_deref(arPlayersFriendly[i], &y);
			fwrite(&y, sizeof(struct stDerefObject2), 1, pFile);
		}
		for (int i = 0; arPlayersHostile[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			_deref(arPlayersHostile[i], &y);
			fwrite(&y, sizeof(struct stDerefObject2), 1, pFile);
		}
	}
	fclose(pFile);
	pFile = 0;
	//dump_csv_name
	if ((pFile = fopen(dump_csv_name, "wb")) == NULL) {
		printf("The file dump_csv_name file was not opened\n");
	} else {
		char header1[] = {"name,uint32_t:objBase,uint64_t:GUID,uint32_t:Type,byte:UnitClass,uint32_t:Health,uint32_t:MaxHealth,uint32_t:HealthPercent,uint32_t:Power,uint32_t:MaxPower,uint32_t:PowerPercent,byte:PowerType,float:pos.x,float:pos.y,float:pos.z,uint32_t:CastingId,uint32_t:ChanneledCastingId,float:CombatReach,float:BoundingRadius,float:distanceToMe\n"};
		fwrite(&header1, strlen(header1), 1, pFile);
		for (int i = 0; arPlayersFriendly[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			char s[1000] = {0};
			_deref(arPlayersFriendly[i], &y);
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arPlayersFriendly[i]));
			length += sprintf(b + length, "%X,%llX,%X,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%f,%f,%f\n", y.objBase, y.GUID, y.Type, y.UnitClass, y.Health, y.MaxHealth, y.HealthPercent, y.Power, y.MaxPower, y.PowerPercent, y.PowerType, y.pos.x, y.pos.y, y.pos.z, y.CastingId, y.ChanneledCastingId, y.CombatReach, y.BoundingRadius, y.distanceToMe);
			fwrite(b, strlen(b), 1, pFile);
		}
		for (int i = 0; arPlayersHostile[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			char s[1000] = {0};
			_deref(arPlayersHostile[i], &y);
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arPlayersHostile[i]));
			length += sprintf(b + length, "%X,%llX,%X,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%f,%f,%f\n", y.objBase, y.GUID, y.Type, y.UnitClass, y.Health, y.MaxHealth, y.HealthPercent, y.Power, y.MaxPower, y.PowerPercent, y.PowerType, y.pos.x, y.pos.y, y.pos.z, y.CastingId, y.ChanneledCastingId, y.CombatReach, y.BoundingRadius, y.distanceToMe);
			fwrite(b, strlen(b), 1, pFile);
		}
		for (int i = 1; arVehicleHostile[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			char s[1000] = {0};
			_deref(arVehicleHostile[i], &y);
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arVehicleHostile[i]));
			length += sprintf(b + length, "%X,%llX,%X,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%f,%f,%f\n", y.objBase, y.GUID, y.Type, y.UnitClass, y.Health, y.MaxHealth, y.HealthPercent, y.Power, y.MaxPower, y.PowerPercent, y.PowerType, y.pos.x, y.pos.y, y.pos.z, y.CastingId, y.ChanneledCastingId, y.CombatReach, y.BoundingRadius, y.distanceToMe);
			fwrite(b, strlen(b), 1, pFile);
		}
		for (int i = 1; arVehicleFriendly[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			char s[1000] = {0};
			_deref(arVehicleFriendly[i], &y);
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arVehicleFriendly[i]));
			length += sprintf(b + length, "%X,%llX,%X,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%f,%f,%f\n", y.objBase, y.GUID, y.Type, y.UnitClass, y.Health, y.MaxHealth, y.HealthPercent, y.Power, y.MaxPower, y.PowerPercent, y.PowerType, y.pos.x, y.pos.y, y.pos.z, y.CastingId, y.ChanneledCastingId, y.CombatReach, y.BoundingRadius, y.distanceToMe);
			fwrite(b, strlen(b), 1, pFile);
		}
	}
	fclose(pFile);
	pFile = 0;
	//dump_binary_auras_name
	if ((pFile = fopen(dump_binary_auras_name, "wb")) == NULL) {
		printf("The file dump_binary_auras_name file was not opened\n");
	} else {
		for (int i = 0; arPlayersFriendly[i] != 0; i++) {
			struct stObject2 x = {0};
			initObject(arPlayersFriendly[i], &x);
			fwrite(&x.auras, sizeof(x.auras), 1, pFile);
		}
		for (int i = 0; arPlayersHostile[i] != 0; i++) {
			struct stObject2 x = {0};
			initObject(arPlayersHostile[i], &x);
			fwrite(&x.auras, sizeof(x.auras), 1, pFile);
		}
	}
	fclose(pFile);
	pFile = 0;
	//dump_unitFields_name
	if ((pFile = fopen(dump_unitFields_name, "wb")) == NULL) {
		printf("The file dump_unitFields_name file was not opened\n");
	} else {
		char header1[] = {
		    "name,CHARM:long,SUMMON:long,CRITTER:long,CHARMEDBY:long,SUMMONEDBY:long,CREATEDBY:long,TARGET:long,CHANNEL_OBJECT:long,CHANNEL_SPELL:int,BYTES_0:bytes,HEALTH:int,POWER1:int,POWER2:int,POWER3:int,POWER4:int,POWER5:int,POWER6:int,POWER7:int,MAXHEALTH:int,MAXPOWER1:int,MAXPOWER2:int,MAXPOWER3:int,MAXPOWER4:int,MAXPOWER5:int,MAXPOWER6:int,MAXPOWER7:int,LEVEL:int,FACTIONTEMPLATE:int,FLAGS:int,FLAGS_2:int,AURASTATE:int,RANGEDATTACKTIME:int,BOUNDINGRADIUS:float,COMBATREACH:float,"
		    "DISPLAYID:int,NATIVEDISPLAYID:int,MOUNTDISPLAYID:int,MINDAMAGE:float,MAXDAMAGE:float,MINOFFHANDDAMAGE:float,MAXOFFHANDDAMAGE:float,BYTES_1:bytes,PETNUMBER:int,PET_NAME_TIMESTAMP:int,PETEXPERIENCE:int,PETNEXTLEVELEXP:int,DYNAMIC_FLAGS:int,MOD_CAST_SPEED:float,CREATED_BY_SPELL:int,NPC_FLAGS:int,NPC_EMOTESTATE:int,STAT0:int,STAT1:int,STAT2:int,STAT3:int,STAT4:int,POSSTAT0:int,POSSTAT1:int,POSSTAT2:int,POSSTAT3:int,POSSTAT4:int,NEGSTAT0:int,NEGSTAT1:int,NEGSTAT2:int,"
		    "NEGSTAT3:int,NEGSTAT4:int,BASE_MANA:int,BASE_HEALTH:int,BYTES_2:bytes,ATTACK_POWER:int,ATTACK_POWER_MODS:two_short,ATTACK_POWER_MULTIPLIER:float,RANGED_ATTACK_POWER:int,RANGED_ATTACK_POWER_MODS:two_short,RANGED_ATTACK_POWER_MULTIPLIER:float,MINRANGEDDAMAGE:float,MAXRANGEDDAMAGE:float,MAXHEALTHMODIFIER:float,HOVERHEIGHT:float,PADDING:int\n"};
		fwrite(&header1, strlen(header1), 1, pFile);
		for (int i = 0; arPlayersFriendly[i] != 0; i++) {
			const int32_t* ObjectHeader = *(int32_t**)(arPlayersFriendly[i] + 0x8);
			uint32_t* uf_base = (uint32_t*)(ObjectHeader + UNIT_FIELD_CHARM);
			char s[1000] = {0};
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arPlayersFriendly[i]));

			for (int j = 0; j < 80; j++) {
				uint32_t offsetinINTs = arUnitFields[j].offsetinINTs;
				uint8_t data[50] = {0};
				//pointer arithmetics
				uint32_t* src = (uint32_t*)uf_base + offsetinINTs;
				memcpy(&data, src, arUnitFields[j].int32Size * 4);
				if (strcmp(arUnitFields[j].type, "LONG") == 0) {
					length += sprintf(b + length, "0x%llX,", *(uint64_t*)&data);
				} else if (strcmp(arUnitFields[j].type, "FLOAT") == 0) {
					length += sprintf(b + length, "%f,", *(float*)&data);
				} else if (strcmp(arUnitFields[j].type, "BYTES") == 0) {
					length += sprintf(b + length, "0x%02X 0x%02X 0x%02X 0x%02X,", data[0], data[1], data[2], data[3]);
				} else {
					length += sprintf(b + length, "0x%X,", *(uint32_t*)&data);
				}
			}
			length += sprintf(b + length, "\n");
			fwrite(b, strlen(b), 1, pFile);
		}
		for (int i = 0; arPlayersHostile[i] != 0; i++) {
			const int32_t* ObjectHeader = *(int32_t**)(arPlayersHostile[i] + 0x8);
			uint32_t* uf_base = (uint32_t*)(ObjectHeader + UNIT_FIELD_CHARM);
			char s[1000] = {0};
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arPlayersHostile[i]));

			for (int j = 0; j < 80; j++) {
				uint32_t offsetinINTs = arUnitFields[j].offsetinINTs;
				uint8_t data[50] = {0};
				//pointer arithmetics
				uint32_t* src = (uint32_t*)uf_base + offsetinINTs;
				memcpy(&data, src, arUnitFields[j].int32Size * 4);
				if (strcmp(arUnitFields[j].type, "LONG") == 0) {
					length += sprintf(b + length, "0x%llX,", *(uint64_t*)&data);
				} else if (strcmp(arUnitFields[j].type, "FLOAT") == 0) {
					length += sprintf(b + length, "%f,", *(float*)&data);
				} else if (strcmp(arUnitFields[j].type, "BYTES") == 0) {
					length += sprintf(b + length, "0x%02X 0x%02X 0x%02X 0x%02X,", data[0], data[1], data[2], data[3]);
				} else {
					length += sprintf(b + length, "0x%X,", *(uint32_t*)&data);
				}
			}
			length += sprintf(b + length, "\n");
			fwrite(b, strlen(b), 1, pFile);
		}
		for (int i = 1; arVehicleHostile[i] != 0; i++) {
			const int32_t* ObjectHeader = *(int32_t**)(arVehicleHostile[i] + 0x8);
			uint32_t* uf_base = (uint32_t*)(ObjectHeader + UNIT_FIELD_CHARM);
			char s[1000] = {0};
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arVehicleHostile[i]));

			for (int j = 0; j < 80; j++) {
				uint32_t offsetinINTs = arUnitFields[j].offsetinINTs;
				uint8_t data[50] = {0};
				//pointer arithmetics
				uint32_t* src = (uint32_t*)uf_base + offsetinINTs;
				memcpy(&data, src, arUnitFields[j].int32Size * 4);
				if (strcmp(arUnitFields[j].type, "LONG") == 0) {
					length += sprintf(b + length, "0x%llX,", *(uint64_t*)&data);
				} else if (strcmp(arUnitFields[j].type, "FLOAT") == 0) {
					length += sprintf(b + length, "%f,", *(float*)&data);
				} else if (strcmp(arUnitFields[j].type, "BYTES") == 0) {
					length += sprintf(b + length, "0x%02X 0x%02X 0x%02X 0x%02X,", data[0], data[1], data[2], data[3]);
				} else {
					length += sprintf(b + length, "0x%X,", *(uint32_t*)&data);
				}
			}
			length += sprintf(b + length, "\n");
			fwrite(b, strlen(b), 1, pFile);
		}
		for (int i = 1; arVehicleFriendly[i] != 0; i++) {
			const int32_t* ObjectHeader = *(int32_t**)(arVehicleFriendly[i] + 0x8);
			uint32_t* uf_base = (uint32_t*)(ObjectHeader + UNIT_FIELD_CHARM);
			char s[1000] = {0};
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,", fGetObjectName((uint32_t*)arVehicleFriendly[i]));

			for (int j = 0; j < 80; j++) {
				uint32_t offsetinINTs = arUnitFields[j].offsetinINTs;
				uint8_t data[50] = {0};
				//pointer arithmetics
				uint32_t* src = (uint32_t*)uf_base + offsetinINTs;
				memcpy(&data, src, arUnitFields[j].int32Size * 4);
				if (strcmp(arUnitFields[j].type, "LONG") == 0) {
					length += sprintf(b + length, "0x%llX,", *(uint64_t*)&data);
				} else if (strcmp(arUnitFields[j].type, "FLOAT") == 0) {
					length += sprintf(b + length, "%f,", *(float*)&data);
				} else if (strcmp(arUnitFields[j].type, "BYTES") == 0) {
					length += sprintf(b + length, "0x%02X 0x%02X 0x%02X 0x%02X,", data[0], data[1], data[2], data[3]);
				} else {
					length += sprintf(b + length, "0x%X,", *(uint32_t*)&data);
				}
			}
			length += sprintf(b + length, "\n");
			fwrite(b, strlen(b), 1, pFile);
		}
	}
	fclose(pFile);
	pFile = 0;
	//dump_objects_csv_name
	if ((pFile = fopen(dump_objects_csv_name, "wb")) == NULL) {
		printf("The file dump_objects_csv_name file was not opened\n");
	} else {
		char header1[] = {"name,hostile,uint32_t:objBase,uint64_t:GUID,uint32_t:Type,byte:UnitClass,uint32_t:Health,uint32_t:MaxHealth,uint32_t:HealthPercent,uint32_t:Power,uint32_t:MaxPower,uint32_t:PowerPercent,byte:PowerType,float:pos.x,float:pos.y,float:pos.z,uint32_t:CastingId,uint32_t:ChanneledCastingId,float:CombatReach,float:BoundingRadius,float:distanceToMe\n"};
		fwrite(&header1, strlen(header1), 1, pFile);
		for (int i = 0; arDumpObjects[i] != 0; i++) {
			struct stDerefObject2 y = {0};
			char s[1000] = {0};
			_deref(arDumpObjects[i], &y);
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,%d,", fGetObjectName((uint32_t*)arDumpObjects[i]), CanAttack(arDumpObjects[i]));
			length += sprintf(b + length, "%X,%llX,%X,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%f,%f,%f\n", y.objBase, y.GUID, y.Type, y.UnitClass, y.Health, y.MaxHealth, y.HealthPercent, y.Power, y.MaxPower, y.PowerPercent, y.PowerType, y.pos.x, y.pos.y, y.pos.z, y.CastingId, y.ChanneledCastingId, y.CombatReach, y.BoundingRadius, y.distanceToMe);
			fwrite(b, strlen(b), 1, pFile);
		}
	}

	fclose(pFile);
	pFile = 0;
	//dump_objects_unitFields_name
	if ((pFile = fopen(dump_objects_unitFields_name, "wb")) == NULL) {
		printf("The file dump_objects_unitFields_name file was not opened\n");
	} else {
		char header1[] = {
		    "name,hostile,CHARM:long,SUMMON:long,CRITTER:long,CHARMEDBY:long,SUMMONEDBY:long,CREATEDBY:long,TARGET:long,CHANNEL_OBJECT:long,CHANNEL_SPELL:int,BYTES_0:bytes,HEALTH:int,POWER1:int,POWER2:int,POWER3:int,POWER4:int,POWER5:int,POWER6:int,POWER7:int,MAXHEALTH:int,MAXPOWER1:int,MAXPOWER2:int,MAXPOWER3:int,MAXPOWER4:int,MAXPOWER5:int,MAXPOWER6:int,MAXPOWER7:int,LEVEL:int,FACTIONTEMPLATE:int,FLAGS:int,FLAGS_2:int,AURASTATE:int,RANGEDATTACKTIME:int,BOUNDINGRADIUS:float,COMBATREACH:float,"
		    "DISPLAYID:int,NATIVEDISPLAYID:int,MOUNTDISPLAYID:int,MINDAMAGE:float,MAXDAMAGE:float,MINOFFHANDDAMAGE:float,MAXOFFHANDDAMAGE:float,BYTES_1:bytes,PETNUMBER:int,PET_NAME_TIMESTAMP:int,PETEXPERIENCE:int,PETNEXTLEVELEXP:int,DYNAMIC_FLAGS:int,MOD_CAST_SPEED:float,CREATED_BY_SPELL:int,NPC_FLAGS:int,NPC_EMOTESTATE:int,STAT0:int,STAT1:int,STAT2:int,STAT3:int,STAT4:int,POSSTAT0:int,POSSTAT1:int,POSSTAT2:int,POSSTAT3:int,POSSTAT4:int,NEGSTAT0:int,NEGSTAT1:int,NEGSTAT2:int,"
		    "NEGSTAT3:int,NEGSTAT4:int,BASE_MANA:int,BASE_HEALTH:int,BYTES_2:bytes,ATTACK_POWER:int,ATTACK_POWER_MODS:two_short,ATTACK_POWER_MULTIPLIER:float,RANGED_ATTACK_POWER:int,RANGED_ATTACK_POWER_MODS:two_short,RANGED_ATTACK_POWER_MULTIPLIER:float,MINRANGEDDAMAGE:float,MAXRANGEDDAMAGE:float,MAXHEALTHMODIFIER:float,HOVERHEIGHT:float,PADDING:int\n"};
		fwrite(&header1, strlen(header1), 1, pFile);
		for (int i = 0; arDumpObjects[i] != 0; i++) {
			const int32_t* ObjectHeader = *(int32_t**)(arDumpObjects[i] + 0x8);
			uint32_t* uf_base = (uint32_t*)(ObjectHeader + UNIT_FIELD_CHARM);
			char s[1000] = {0};
			int length = 0;
			char* b = &s;
			length += sprintf(b + length, "%s,%d,", fGetObjectName((uint32_t*)arDumpObjects[i]), CanAttack(arDumpObjects[i]));

			for (int j = 0; j < 80; j++) {
				uint32_t offsetinINTs = arUnitFields[j].offsetinINTs;
				uint8_t data[50] = {0};
				//pointer arithmetics
				uint32_t* src = (uint32_t*)uf_base + offsetinINTs;
				memcpy(&data, src, arUnitFields[j].int32Size * 4);
				if (strcmp(arUnitFields[j].type, "LONG") == 0) {
					length += sprintf(b + length, "0x%llX,", *(uint64_t*)&data);
				} else if (strcmp(arUnitFields[j].type, "FLOAT") == 0) {
					length += sprintf(b + length, "%f,", *(float*)&data);
				} else if (strcmp(arUnitFields[j].type, "BYTES") == 0) {
					length += sprintf(b + length, "0x%02X 0x%02X 0x%02X 0x%02X,", data[0], data[1], data[2], data[3]);
				} else {
					length += sprintf(b + length, "0x%X,", *(uint32_t*)&data);
				}
			}
			length += sprintf(b + length, "\n");
			fwrite(b, strlen(b), 1, pFile);
		}
	}
	fclose(pFile);
	chdir(&cwd);
}
