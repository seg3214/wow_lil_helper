#include <stdint.h>
#include <stdlib.h>

void* buf;
uint32_t* dataStorePtr1;
uint32_t* fCDataStoreInitialize(uint32_t* dataStorePtr) {
	uint32_t* rr;
	__asm
	{
        mov ecx, dataStorePtr;
        mov eax, constCDataStoreInitialize;
        call eax;
        mov rr, eax;
	}
	return rr;
}
void fCDataStoreDestroy(uint32_t* dataStorePtr) {
	__asm {
        mov ecx, dataStorePtr;
        mov eax, constCDataStoreDestroy;
        call eax;
	}
}
void fCDataStoreFinalize(uint32_t* dataStorePtr) {
	__asm {
        mov ecx, dataStorePtr;
        mov eax, constCDataStoreFinalize;
        call eax;
	}
}

void CDataStore_Init() {
	buf = malloc(0x18);
	dataStorePtr1 = fCDataStoreInitialize(buf);
}
void CDataStore_Destroy() {
	fCDataStoreDestroy(dataStorePtr1);
	free(buf);
	dataStorePtr1 = 0;
}
void CDataStore_Finalize() {}

uint32_t* fGetInt8(uint32_t* dataStorePtr, uint8_t* buff) {
	uint32_t* rr;
	__asm {
        push buff;
        mov ecx, dataStorePtr;
        mov eax, constGetInt8;
        call eax;
        mov rr, eax;
	}
	return rr;
}
uint32_t* fGetInt16(uint32_t* dataStorePtr, uint16_t* buff) {
	uint32_t* rr;
	__asm {
        push buff;
        mov ecx, dataStorePtr;
        mov eax, constGetInt16;
        call eax;
        mov rr, eax;
	}
	return rr;
}
uint32_t* fGetIntArray(uint32_t* dataStorePtr, void* ar_buffer, uint32_t count) {  // gets Number of uint_32
	uint32_t* rr;
	__asm {
        push count;
        push ar_buffer;
        mov ecx, dataStorePtr;
        mov eax, constGetIntArray;
        call eax;
        mov rr, eax;
	}
	return rr;
}

uint32_t* fPutInt8(uint32_t* dataStorePtr, uint8_t value) {  //unsigned int
	uint32_t* rr;
	__asm {
        push value;
        mov ecx, dataStorePtr;
        mov eax, constPutInt8;
        call eax;
        mov rr, eax;
	}
	return rr;
}
uint32_t* fPutInt32(uint32_t* dataStorePtr, int32_t value) {  //signed int
	uint32_t* rr;
	__asm {
        push value;
        mov ecx, dataStorePtr;
        mov eax, constPutInt32;
        call eax;
        mov rr, eax;
	}
	return rr;
}
uint32_t* fPutInt64(uint32_t* dataStorePtr, int64_t value) {  //signed int
	uint32_t* rr;
	int32_t highword, lowword;
	highword = value >> 32;
	lowword = value;
	//printf("v %llX , %X, %X",value,highword,lowword);
	__asm {
        push highword;
        push lowword;

        mov ecx, dataStorePtr;
        mov eax, constPutInt64;
        call eax;
        mov rr, eax;
	}
	return rr;
}
uint32_t* fPutBytes(uint32_t* dataStorePtr, void* ar_buffer, uint32_t vsize) {	//
	uint32_t* rr;
	__asm {
        push vsize;
        push ar_buffer;
        mov ecx, dataStorePtr;
        mov eax, constPutBytes;
        call eax;
        mov rr, eax;
	}
	return rr;
}
