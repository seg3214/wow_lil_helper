#pragma once
extern uint32_t* dataStorePtr1;

void CDataStore_Init();
void CDataStore_Destroy();
void fCDataStoreFinalize(uint32_t* dataStorePtr);

uint32_t* fGetInt8(uint32_t* dataStorePtr, uint8_t* buff);
uint32_t* fGetInt16(uint32_t* dataStorePtr, uint16_t* buff);
uint32_t* fGetIntArray(uint32_t* dataStorePtr, void* ar_buffer, uint32_t count);
uint32_t* fPutInt8(uint32_t* dataStorePtr, uint8_t value);
uint32_t* fPutInt32(uint32_t* dataStorePtr, int32_t value);
uint32_t* fPutInt64(uint32_t* dataStorePtr, int64_t value);
uint32_t* fPutBytes(uint32_t* dataStorePtr, void* ar_buffer, uint32_t vsize);
