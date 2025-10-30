struct stUnitFields {
	char* fieldName;
	uint32_t offsetinINTs;
	uint32_t int32Size;
	char* type;
};

const struct stUnitFields arUnitFields[] = {
    {"CHARM", 0x0000, 2, "LONG"},			     //1
    {"SUMMON", 0x0002, 2, "LONG"},			     //2
    {"CRITTER", 0x0004, 2, "LONG"},			     //3
    {"CHARMEDBY", 0x0006, 2, "LONG"},			     //4
    {"SUMMONEDBY", 0x0008, 2, "LONG"},			     //5
    {"CREATEDBY", 0x000A, 2, "LONG"},			     //6
    {"TARGET", 0x000C, 2, "LONG"},			     //7
    {"CHANNEL_OBJECT", 0x000E, 2, "LONG"},		     //8
    {"CHANNEL_SPELL", 0x0010, 1, "INT"},		     //9
    {"BYTES_0", 0x0011, 1, "BYTES"},			     //10
    {"HEALTH", 0x0012, 1, "INT"},			     //11
    {"POWER1", 0x0013, 1, "INT"},			     //12
    {"POWER2", 0x0014, 1, "INT"},			     //13
    {"POWER3", 0x0015, 1, "INT"},			     //14
    {"POWER4", 0x0016, 1, "INT"},			     //15
    {"POWER5", 0x0017, 1, "INT"},			     //16
    {"POWER6", 0x0018, 1, "INT"},			     //17
    {"POWER7", 0x0019, 1, "INT"},			     //18
    {"MAXHEALTH", 0x001A, 1, "INT"},			     //19
    {"MAXPOWER1", 0x001B, 1, "INT"},			     //20
    {"MAXPOWER2", 0x001C, 1, "INT"},			     //21
    {"MAXPOWER3", 0x001D, 1, "INT"},			     //22
    {"MAXPOWER4", 0x001E, 1, "INT"},			     //23
    {"MAXPOWER5", 0x001F, 1, "INT"},			     //24
    {"MAXPOWER6", 0x0020, 1, "INT"},			     //25
    {"MAXPOWER7", 0x0021, 1, "INT"},			     //26
    {"LEVEL", 0x0030, 1, "INT"},			     //27
    {"FACTIONTEMPLATE", 0x0031, 1, "INT"},		     //28
    {"FLAGS", 0x0035, 1, "INT"},			     //29
    {"FLAGS_2", 0x0036, 1, "INT"},			     //30
    {"AURASTATE", 0x0037, 1, "INT"},			     //31
    {"RANGEDATTACKTIME", 0x003A, 1, "INT"},		     //32
    {"BOUNDINGRADIUS", 0x003B, 1, "FLOAT"},		     //33
    {"COMBATREACH", 0x003C, 1, "FLOAT"},		     //34
    {"DISPLAYID", 0x003D, 1, "INT"},			     //35
    {"NATIVEDISPLAYID", 0x003E, 1, "INT"},		     //36
    {"MOUNTDISPLAYID", 0x003F, 1, "INT"},		     //37
    {"MINDAMAGE", 0x0040, 1, "FLOAT"},			     //38
    {"MAXDAMAGE", 0x0041, 1, "FLOAT"},			     //39
    {"MINOFFHANDDAMAGE", 0x0042, 1, "FLOAT"},		     //40
    {"MAXOFFHANDDAMAGE", 0x0043, 1, "FLOAT"},		     //41
    {"BYTES_1", 0x0044, 1, "BYTES"},			     //42
    {"PETNUMBER", 0x0045, 1, "INT"},			     //43
    {"PET_NAME_TIMESTAMP", 0x0046, 1, "INT"},		     //44
    {"PETEXPERIENCE", 0x0047, 1, "INT"},		     //45
    {"PETNEXTLEVELEXP", 0x0048, 1, "INT"},		     //46
    {"DYNAMIC_FLAGS", 0x0049, 1, "INT"},		     //47
    {"MOD_CAST_SPEED", 0x004A, 1, "FLOAT"},		     //48
    {"CREATED_BY_SPELL", 0x004B, 1, "INT"},		     //49
    {"NPC_FLAGS", 0x004C, 1, "INT"},			     //50
    {"NPC_EMOTESTATE", 0x004D, 1, "INT"},		     //51
    {"STAT0", 0x004E, 1, "INT"},			     //52
    {"STAT1", 0x004F, 1, "INT"},			     //53
    {"STAT2", 0x0050, 1, "INT"},			     //54
    {"STAT3", 0x0051, 1, "INT"},			     //55
    {"STAT4", 0x0052, 1, "INT"},			     //56
    {"POSSTAT0", 0x0053, 1, "INT"},			     //57
    {"POSSTAT1", 0x0054, 1, "INT"},			     //58
    {"POSSTAT2", 0x0055, 1, "INT"},			     //59
    {"POSSTAT3", 0x0056, 1, "INT"},			     //60
    {"POSSTAT4", 0x0057, 1, "INT"},			     //61
    {"NEGSTAT0", 0x0058, 1, "INT"},			     //62
    {"NEGSTAT1", 0x0059, 1, "INT"},			     //63
    {"NEGSTAT2", 0x005A, 1, "INT"},			     //64
    {"NEGSTAT3", 0x005B, 1, "INT"},			     //65
    {"NEGSTAT4", 0x005C, 1, "INT"},			     //66
    {"BASE_MANA", 0x0072, 1, "INT"},			     //67
    {"BASE_HEALTH", 0x0073, 1, "INT"},			     //68
    {"BYTES_2", 0x0074, 1, "BYTES"},			     //69
    {"ATTACK_POWER", 0x0075, 1, "INT"},			     //70
    {"ATTACK_POWER_MODS", 0x0076, 1, "TWO_SHORT"},	     //71
    {"ATTACK_POWER_MULTIPLIER", 0x0077, 1, "FLOAT"},	     //72
    {"RANGED_ATTACK_POWER", 0x0078, 1, "INT"},		     //73
    {"RANGED_ATTACK_POWER_MODS", 0x0079, 1, "TWO_SHORT"},    //74
    {"RANGED_ATTACK_POWER_MULTIPLIER", 0x007A, 1, "FLOAT"},  //75
    {"MINRANGEDDAMAGE", 0x007B, 1, "FLOAT"},		     //76
    {"MAXRANGEDDAMAGE", 0x007C, 1, "FLOAT"},		     //77
    {"MAXHEALTHMODIFIER", 0x008B, 1, "FLOAT"},		     //78
    {"HOVERHEIGHT", 0x008C, 1, "FLOAT"},		     //79
    {"PADDING", 0x008D, 1, "INT"},			     //80
};
