#pragma once

//
void clientDB_Init();
// Returns pointer to client DB. have to be called after clientDB_Init()
uint32_t* GetDBPointer(ClientDB id);
//gets info about ID into buffer;
int32_t GetLocalizedRow(uint32_t* pDBTable, int32_t id, void* buffer);

void clientDB_test();