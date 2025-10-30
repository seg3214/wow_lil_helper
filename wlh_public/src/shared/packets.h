#pragma once

void SetPHs();
void ResetPHs();
void Send_CMSG_CAST_SPELL(uint32_t* dataStorePtr, uint64_t* guid, int32_t SpellID, uint8_t CastCount, uint8_t unkFlags);
