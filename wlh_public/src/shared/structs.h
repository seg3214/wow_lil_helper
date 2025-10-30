#pragma once
//{{{  STRUCTS
struct stClientVars {
	int32_t ZoneID;
	char* ZoneText;
	char* ZoneMinimapText;
	int32_t IsIndoors;  // orgrimmar counts as indoors
	uint32_t MapID;

	//not used
	char* MapInternalName;
	uint32_t IsInInstance;	//IsInInstance - 1 if the player is in an instance(dungeon or battleground), otherwise 0
	uint32_t InstanceType;
};

struct stAura {
	uint32_t* pAuraId;
	uint64_t* pCreatorGuid;
	byte* pFlags;  //enum AURA_FLAGS
	byte* pStackCount;
	char* pAuraName;
	uint32_t RemainingSec;
	int32_t Mechanic;
	uint32_t DispelType;
};
struct AuraRec {
	uint64_t CreatorGuid;
	uint32_t AuraId;
	byte Flags;
	byte Level;
	//unsigned short StackCount;
	byte StackCount;
	byte unk0;
	uint32_t Duration;
	uint32_t EndTime;
};
struct stObject2 {
	uint32_t objBase;
	uint64_t* pGUID;
	uint32_t* pType;
	byte* pUnitClass;
	uint32_t* pHealth;
	uint32_t* pMaxHealth;
	uint32_t HealthPercent;
	uint32_t Power;	    // not a pointer because needs correction for rage type
	uint32_t MaxPower;  // not a pointer because needs correction for rage type
	uint32_t PowerPercent;
	byte* PowerType;  //%d  -2 Health,  0 Mana, 1 Rage, 2 Focus, 3 Energy, 5 Runes, 6 Runic Power
	D3DXVECTOR3 pos;
	//float *pHeight;
	uint32_t* CastingId;
	uint32_t* ChanneledCastingId;
	float* pCombatReach;
	float* pBoundingRadius;
	float distanceToMe;
	struct stAura auras[100];
};

struct ClientDBHeader {
	void* funcTable;
	int isLoaded;
	int numRows;
	int maxIndex;
	int minIndex;
	int stringTable;
	void* funcTable2;
	int FirstRow;
	DWORD** Rows;
};

struct clientDBTables {
	uint32_t* pBase;
	uint32_t id;
	//struct WoWClientDB header;
};

struct SkillLineEntry {
	uint32_t ID;
	enum SkillCategory CategoryID;
	uint32_t unk0;
	char* DisplayName;
	char* Description;
	short SpellIconID;
	short unk1;
	char* ActionName;
	uint32_t CanLink;
};

struct stSpellIKnow {
	uint32_t spellId;
	char* spellName;
	char* spellRank;
	uint32_t Attributes;
	uint32_t AttributesEx;
	uint32_t DmgClass;
	uint32_t PowerCost;
	uint32_t IsHealthFunnel;
	uint32_t CastTime;
	uint32_t MinRange;
	uint32_t MaxRange;
	int32_t Mechanic;
	int32_t TriggersGCD;
};

#pragma region SpellEntry
struct Flag96 {
	uint32_t flag1;
	uint32_t flag2;
	uint32_t flag3;
};
struct SpellEntry {
	uint32_t Id;
	uint32_t Category;
	uint32_t Dispel;  //%d looks like dispel type 1 for fear 2 for curses
	int32_t Mechanic;

	uint32_t Attributes;  //group of flags
	uint32_t AttributesEx;
	uint32_t AttributesEx2;
	uint32_t AttributesEx3;
	uint32_t AttributesEx4;
	uint32_t AttributesEx5;
	uint32_t AttributesEx6;
	uint32_t AttributesEx7;

	uint32_t Stances;     //std::array<uint32, 2> ShapeshiftMask;                               // 12-13
	uint32_t unk_320_2;   // std::array<uint32, 2> ShapeshiftMask;                               // 12-13
	uint32_t StancesNot;  // std::array<uint32, 2> ShapeshiftExclude;                            // 14-15
	uint32_t unk_320_3;   //std::array<uint32, 2> ShapeshiftExclude;                            // 14-15
	uint32_t Targets;
	uint32_t TargetCreatureType;
	uint32_t RequiresSpellFocus;
	uint32_t FacingCasterFlags;
	uint32_t CasterAuraState;
	uint32_t TargetAuraState;
	uint32_t CasterAuraStateNot;
	uint32_t TargetAuraStateNot;
	uint32_t casterAuraSpell;
	uint32_t targetAuraSpell;
	uint32_t excludeCasterAuraSpell;
	uint32_t excludeTargetAuraSpell;
	uint32_t CastingTimeIndex;
	uint32_t RecoveryTime;		//%d cooldown in milliseconds.
	uint32_t CategoryRecoveryTime;	//%d cooldown in milliseconds. shares cooldown with other similar items i.e "Will of the Forsaken". regular cooldwon is here if spell have that shared cooldown whatever that is.
	uint32_t InterruptFlags;
	uint32_t AuraInterruptFlags;
	uint32_t ChannelInterruptFlags;
	uint32_t procFlags;
	uint32_t procChance;
	uint32_t procCharges;
	uint32_t maxLevel;
	uint32_t baseLevel;
	uint32_t spellLevel;  //%d required character level to learn this spell
	uint32_t DurationIndex;
	int32_t powerType;  //%d  -2 Health,  0 Mana, 1 Rage, 2 Focus, 3 Energy, 5 Runes, 6 Runic Power
	uint32_t manaCost;
	uint32_t manaCostPerlevel;
	uint32_t manaPerSecond;
	uint32_t manaPerSecondPerLevel;
	uint32_t rangeIndex;
	float speed;
	uint32_t modalNextSpell;
	uint32_t StackAmount;
	uint32_t Totem[2];
	int32_t Reagent[8];
	uint32_t ReagentCount[8];
	int32_t EquippedItemClass;
	int32_t EquippedItemSubClassMask;
	int32_t EquippedItemInventoryTypeMask;

	uint32_t Effect[3];
	int32_t EffectDieSides[3];
	float EffectRealPointsPerLevel[3];
	int32_t EffectBasePoints[3];
	uint32_t EffectMechanic[3];
	uint32_t EffectImplicitTargetA[3];
	uint32_t EffectImplicitTargetB[3];
	uint32_t EffectRadiusIndex[3];
	uint32_t EffectApplyAuraName[3];
	uint32_t EffectAmplitude[3];
	float EffectMultipleValue[3];
	uint32_t EffectChainTarget[3];
	uint32_t EffectItemType[3];
	int32_t EffectMiscValue[3];
	int32_t EffectMiscValueB[3];
	uint32_t EffectTriggerSpell[3];
	float EffectPointsPerComboPoint[3];
	struct Flag96 EffectSpellClassMask[3];
	uint32_t SpellVisual[2];

	uint32_t SpellIconID;
	uint32_t activeIconID;
	uint32_t spellPriority;

	char* SpellName;    //pointer to string
	char* Rank;	    //pointer to string
	char* Description;  //pointer to string
	char* ToolTip;	    //pointer to string

	uint32_t ManaCostPercentage;
	uint32_t StartRecoveryCategory;
	uint32_t StartRecoveryTime;
	uint32_t MaxTargetLevel;
	uint32_t SpellFamilyName;
	struct Flag96 SpellFamilyFlags;
	uint32_t MaxAffectedTargets;
	uint32_t DmgClass;
	uint32_t PreventionType;
	uint32_t StanceBarOrder;

	float DmgMultiplier[3];

	uint32_t MinFactionId;
	uint32_t MinReputation;
	uint32_t RequiredAuraVision;

	uint32_t TotemCategory[2];

	int32_t AreaGroupId;
	int32_t SchoolMask;
	uint32_t runeCostID;
	uint32_t spellMissileID;
	uint32_t PowerDisplayId;
	float unk_320_4[3];
	uint32_t spellDescriptionVariableID;
	uint32_t SpellDifficultyId;
};
#pragma endregion

#pragma pack(push, 1)
struct stGroupMember {
	uint8_t Name[szMemberName];  //null-terminated
	uint64_t uGUID;
	uint8_t GroupMemberOnlineStatus;  //enum enGroupMemberOnlineStatus
	uint8_t SubGroup;
	uint8_t GroupMemberFlags;  //enum enGroupMemberFlags
	uint8_t LfgRoles;	   //enum enLfgRoles
};
struct stGroupInfo {
	enum enGroupType groupType;
	uint8_t mySubGroup;
	enum enGroupMemberFlags myGroupFlags;
	enum enLfgRoles myLfgRoles;
	uint64_t groupGUID;
	uint32_t packetCount;
	uint64_t leaderGUID;
	uint8_t membersCount;
	struct stGroupMember members[szGroupMembers];
	//loot method
	//master loot guid
	//loot threshold
	//dungeon difficulty
	//raid difficulty
	//dynraiddiff 0 normal/1 heroic
};
struct realm_data {
	char realname[50];
	float distancefix;
};
struct GlyphSocketInfo {
	int enabled;
	int glyphType;
	int glyphSpell;
};
#pragma pack(pop)

struct SCameraInfo  //for wotlk . after wotlk zNearPlane,zFarPlane, aspect not working?
{
	uint32_t dwFoo1[2];  //00
	//CVec3 vPos;
	float fPos[3];	// 02
	//CMat3 matView;
	float fViewMat[3][3];  // 05
	float zNearPlane;      // 14
	float zFarPlane;       // 15
	float fFov;	       //16
	float aspect;	       // 17
};

struct ContainerItemInfo {
	char name[100];	 //item name without suffix
	int32_t itemID;	 //https://db.rising-gods.de/?item=6948
	//int32_t count;//Number of items in the slot (number)
	//int32_t locked;// 1 if the item is locked; otherwise nil. Items become locked while being moved, split, or placed into other UI elements (such as the mail, trade, and auction windows). (1nil)
	enum ItemQuality quality;  // Quality (or rarity) of the item (number, itemQuality)
				   //int32_t readable;// 1 if the item is readable; otherwise nil. This value is used by the default UI to show a special cursor over items such as books and scrolls which can be read by right-clicking. (1nil)
				   //int32_t lootable; // 1 if the item is a temporary container containing items that can be looted; otherwise nil.Examples include the Bag of Fishing Treasures and Small Spice Bag rewarded by daily quests, lockboxes(once unlocked), and the trunks occasionally found while fishing. (1nil)
				   //	link // A hyperlink for the item (itemLink)
};
//}}}
