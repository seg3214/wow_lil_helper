#pragma once

//
void ReadKnownSpells();
//
void CastSpellById(uint32_t spellId);
//
int32_t CastSpellByName(char *spellName);
//Returns Channeled Spell Id; 0 if none
uint32_t GetChanneledCastingId();
//Returns Spell Name by Spell ID; 0 if cant find
char* GetSpellNameById(uint32_t spellId);
//Returns Spell ID by Spell name; 0 if cant find
uint32_t GetSpellIdBySpellName(char *spellName);

void spells_test();