#pragma once
struct stMyCastingInfo {
	int casting;
	int neededToWait;
	char SpellName[50];
	int64_t startTime;
	int64_t endTime;
	int64_t castTime;
	int64_t lastCastStartedTime;
};
struct stPlayerState {
	struct stMyCastingInfo castingInfo;
	int32_t latencyHome;
};
extern struct stPlayerState PlayerState;
extern uint32_t autoLengthMS;

#pragma region script functions
int _errorcheck(struct stObject2* pstObj, char* fname, int line);
void CastSlow(char* spellName, struct stObject2* pstObj);
uint32_t GetCursesState(struct stObject2* pstObj);

struct stSpellIKnow* GetMySpellByMySpellName(char* spellName);
int IsSpellInRange(char* spellName, struct stObject2* pstObj);
void _LookAtAndCast(char* spellName, struct stObject2* pstObj);
int8_t fUnitCanAssist(uint32_t* thiss, uint32_t* obj2, uint8_t zero);
void StartAttack();
void StopAttack();
void SpellStopCasting();
int HasAura(char* name, struct stObject2* pstObj);
int HasMyAura(char* name, struct stObject2* pstObj);
int HasAuraByID(uint32_t spellId, struct stObject2* pstObj);
int AuraSeconds(char* spellName, struct stObject2* pstObj);
int AuraMySeconds(char* spellName, struct stObject2* pstObj);
int AuraStackCount(char* spellName, struct stObject2* pstObj);
int ifUnitPet(uint32_t objBase);
int ifUnitMyPet(uint32_t objBase);
int ifUnitTotem(uint32_t objBase);
int GetUnitTypeFromGUID(uint64_t guid);
int CanAttack(uint32_t objBase);
float GetSpellCooldownById(uint32_t spellId);
int IsGCD();
float GetSpellCooldownByName(char* spellName);
void TargetNearestEnemy();
void TargetNearestEnemyPlayer();
int IsUsableSpell(char* spellName);
int IsSpellKnown(char* spellName);
int IsSpellReady(char* spellName);
int IsCasting(struct stObject2* pstObj);
void CastSpellByName(char* spellName, struct stObject2* pstObj);
byte GetShapeshiftForm(uint32_t ObjBase);
void SetShapeshiftForm(enum ShapeshiftForm stance);
void UseItemByName(char* itemName, struct stObject2* pstObj);
int PlayerHasItem(char* itemName);
int HasMechanic(int32_t mechanic, struct stObject2* pstObj);
int HasDispellableBuff(uint32_t dispelType, struct stObject2* pstObj);
int HasDispellableDebuff(uint32_t dispelType, struct stObject2* pstObj);
int IsLostControl(struct stObject2* pstObj);
int IsImmune(struct stObject2* pstObj);
int IsDisarmed(struct stObject2* pstObj);
int IsSilenced(struct stObject2* pstObj);
int IsFeared(struct stObject2* pstObj);
int IsCharmed(struct stObject2* pstObj);
int IsStunned(struct stObject2* pstObj);
int IsSleeping(struct stObject2* pstObj);
int IsPoisoned(struct stObject2* pstObj);
int GetSpellPowerCost(char* spellName);
void silence(char* spell);
int IsCurrentSpell(char* spellName);
uint32_t GetMySpellIdByMySpellName(char* spellName);
int IsObjectMelee(char* Expansion, struct stObject2* pstObj);
int IsObjectRanged(char* Expansion, struct stObject2* pstObj);
int IsObjectCaster(char* Expansion, struct stObject2* pstObj);
void CancelBuffByID(uint32_t spellId);
enum RuneType RunesGetType(int index);
int RunesGetCooldown(int index);
int RunesReady(enum RuneType type);
uint32_t* DK_GetMyPetObjectBase();
void PetAttack();
int InMyMeleeRange(struct stObject2* pstObj);
int MySpec();
int IsInPartyOrRaid();
int TargetIsElite();
int TargetIsPlayer();
int IsInCombat(struct stObject2* pstObj);
float GetDistance(struct stObject2* pstObj);
void GetGlyphSocketInfo(int index, struct GlyphSocketInfo* data);
int IsGlyphOn(int id);
void DK_RaiseDead();
void DK_UseDeathPact();

void UseHealthStones();
void UseHealthPotions();
void UseManaPotions();
void DefenseTrinkets();
void AttackTrinkets();
void DispelTrinkets();

int IsEquippedItem(char* itemName);
int IsEquippedItemByID(int itemID);
int IsEquippableItemByName(char* itemName);
int IsEquippableItemByID(int itemID);

int IfDKUnholy(struct stObject2* pstObj);
void BloodTap();
void focc();

int GetContainerItemID(int32_t containerID, uint32_t containerSlotID);
void GetItemInfo(int itemID, struct ContainerItemInfo* x);
void GetContainerItemInfo(int32_t containerID, uint32_t containerSlotID, struct ContainerItemInfo* x);
int ItemInfo(int32_t containerID, uint32_t containerSlotID, struct ContainerItemInfo* x);
int CastObjectEnemy(char* spellName, struct stObject2* pstObj);
#pragma endregion

void AutoScriptPulse(uint64_t nowframe);
void scripting_pulse();
