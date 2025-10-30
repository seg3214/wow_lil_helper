#include "../shared/main.h"
#ifndef LINUX

#pragma region SendPacket
uint32_t*(__cdecl* fClientServices_GetCurrent)() = constClientServices_GetCurrent;

void fSendPacket(uint32_t* dataStorePtr) {
	//fCDataStoreFinalize(dataStorePtr);
	//static uint32_t* pcur=0;

	//if (!pcur)
	uint32_t* pcur = fClientServices_GetCurrent();
	{
		__asm {
        push dataStorePtr;
        mov ecx, pcur;
        mov eax, constSendPacket;
        call eax;
		}
	}
}
void packGUID(int64_t* guid, uint8_t(*buffer), int32_t* buffsize) {
	uint8_t (*ar)[8] = guid;
	uint8_t mask1 = 0;
	int c = 1;
	for (int i = 0; i < 8; i++) {
		if ((*ar)[i] != 0x0) {
			//printf("not zero %d\n", i);
			buffer[c] = (*ar)[i];
			mask1 = (1 << i) | mask1;
			c++;
		}
	}
	buffer[0] = mask1;  //0 byte for mask
	*buffsize = c;	    //number of written bytes including mask
	return;
}

void Send_CMSG_CAST_SPELL(uint32_t* dataStorePtr, uint64_t* guid, int32_t SpellID, uint8_t CastCount, uint8_t unkFlags) {
	///Has uint BytesRead at Pointer + 0x14 keeps tracks of read position. -1 when not finalized
	///Has uint BytesWritten at Pointer + 0x10 keeps tracks of write position
	///
	byte* wr = ((uint32_t)dataStorePtr + 0x10);
	//byte *re = ((uint32_t)dataStorePtr + 0x14);
	//printf("*wr %X\n",*wr);
	*wr = 0;
	//*re = 0;
	//CDataStore_Init();//make it once
	//dataStorePtr1 = fCDataStoreInitialize(buf);
	//uint32_t *bufd =malloc(0x18);
	//uint32_t *dataStorePtr1 =fCDataStoreInitialize(bufd);
	//printf("%X %X\n",buf,dataStorePtr1);

	fPutInt32(dataStorePtr, (int32_t)CMSG_CAST_SPELL);
	//CastCount = 0xAA;
	fPutInt8(dataStorePtr, CastCount);
	fPutInt32(dataStorePtr, (int32_t)SpellID);
	fPutInt8(dataStorePtr, unkFlags);
	fPutInt32(dataStorePtr, (int32_t)TARGET_FLAG_UNIT);
	//fPutInt32(dataStorePtr1,(int32_t)0x22222222);
	printf("p %p\n", dataStorePtr);
	//fPutInt64(dataStorePtr1,(int64_t)*Myself->pGUID);//guid converting to int?
	int64_t guid1 = (int64_t)*guid;

	//uint8_t buffer[9] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99};
	uint8_t buffer[9] = {0};
	int32_t buffsize = 0;
	packGUID(&guid1, &buffer, &buffsize);

	//fPutInt64(dataStorePtr1,guid1);//guid converting to int?
	fPutBytes(dataStorePtr, &buffer, buffsize);
	//printf("putting %d bytes\n",buffsize);
	fCDataStoreFinalize(dataStorePtr);
	fSendPacket(dataStorePtr);

	//printf("sent ok\n");
	//CDataStore_Destroy();//make it once
	//fCDataStoreDestroy(dataStorePtr1);
	//free(dataStorePtr1);
	//dataStorePtr1 = 0;
}
void Send_CMSG_BATTLEFIELD_PORT(uint32_t* dataStorePtr, uint8_t accept) {
	///Has uint BytesRead at Pointer + 0x14 keeps tracks of read position. -1 when not finalized
	///Has uint BytesWritten at Pointer + 0x10 keeps tracks of write position
	///
	//int32_t OPCODE
	//uint8 type;                                             // arenatype if arena
	//uint8 unk2;                                             // unk, can be 0x0 (may be if was invited?) and 0x1
	//uint32 bgTypeId_;                                       // type id from dbc
	//uint16 unk;                                             // 0x1F90 constant?
	//uint8 action;                                           // enter battle 0x1, leave queue 0x0
	byte* wr = ((uint32_t)dataStorePtr + 0x10);
	*wr = 0;
	fPutInt32(dataStorePtr, (int32_t)CMSG_BATTLEFIELD_PORT);
	fPutInt8(dataStorePtr, 0);
	fPutInt8(dataStorePtr, 0);
	fPutInt32(dataStorePtr, (int32_t)0x20);	 //hardcoded random bg
	fPutInt8(dataStorePtr, 0x90);		 //..
	fPutInt8(dataStorePtr, 0x1F);		 //2 bytes instead of 1 short because TODO implement putInt16
	fPutInt8(dataStorePtr, accept);
	//printf("p %X\n",dataStorePtr);
	fCDataStoreFinalize(dataStorePtr);
	fSendPacket(dataStorePtr);
}
void Send_Join_RBG(uint32_t* dataStorePtr) {
	//uint64 guid;                                      // battlemaster guid
	//uint32 bgTypeId_;                                 // battleground type id (DBC id)
	//uint32 instanceId;                                // instance id, 0 if First Available selected
	//uint8  joinAsGroup;                               // join as group
	byte* wr = ((uint32_t)dataStorePtr + 0x10);
	*wr = 0;
	fPutInt32(dataStorePtr, (int32_t)CMSG_BATTLEMASTER_JOIN);
	fPutInt64(dataStorePtr, 0);
	fPutInt32(dataStorePtr, 0x20);	//hardcoded random bg
	fPutInt32(dataStorePtr, 0);
	fPutInt8(dataStorePtr, 0);
	fCDataStoreFinalize(dataStorePtr);
	fSendPacket(dataStorePtr);
}
void Send_CMSG_INSPECT(uint32_t* dataStorePtr, uint64_t* guid) {  //sends out CMSG_INSPECT with unpacked GUID (is it always has to be unpacked?)
	byte* wr = ((uint32_t)dataStorePtr + 0x10);
	*wr = 0;
	//uint8_t buffer[9] = { 0 };
	//int32_t buffsize = 0;
	//packGUID(guid,&buffer,&buffsize); //client message doesnt pack the GUID?

	fPutInt32(dataStorePtr, (int32_t)CMSG_INSPECT);
	//fPutInt64(dataStorePtr,*(int64_t*)buffer);
	fPutInt64(dataStorePtr, *guid);

	fCDataStoreFinalize(dataStorePtr);
	fSendPacket(dataStorePtr);
}
#pragma endregion

#pragma region GetPacket
void(__cdecl* fSetMessageHandler)(enum NetMessage msgId, uint32_t* handler, uint32_t* param) = constSetMessageHandler;

int32_t (*fPH_SMSG_MESSAGECHAT)(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) = constPH_SMSG_MESSAGECHAT;
int32_t (*fPH_SMSG_BATTLEFIELD_STATUS)(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) = constPH_SMSG_BATTLEFIELD_STATUS;
int32_t (*fPH_SMSG_SPELL_START)(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) = constPH_SMSG_SPELL_START;
int32_t (*fPH_SMSG_CAST_FAILED)(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) = constPH_SMSG_CAST_FAILED;
int32_t (*fPH_SMSG_GROUP_LIST)(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) = constPH_SMSG_GROUP_LIST;
int32_t (*fPH_SMSG_LFG_PROPOSAL_UPDATE)(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) = constPH_SMSG_LFG_PROPOSAL_UPDATE;

//****SMSG_MESSAGECHAT or SMSG_GM_MESSAGECHAT*****
//data << uint8(chatType);
//data << int32(language);
//data << uint64(senderGUID);
//data << uint32(0);  // some flags
//	switch (chatType)
//	if (gmMessage)
//  if (chatType == CHAT_MSG_CHANNEL)
//data << uint64(receiverGUID);
//data << uint32(message.length() + 1);
//data << message;
//data << uint8(chatTag);
//	if (chatType == CHAT_MSG_ACHIEVEMENT || chatType == CHAT_MSG_GUILD_ACHIEVEMENT)
//**************************************************
int32_t cbPH_SMSG_MESSAGECHAT(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) {
	uint8_t b = 0;
	byte* wr = ((uint32_t)pData + 0x10);
	byte* re = ((uint32_t)pData + 0x14);
#define cdd 500
	uint8_t bar[cdd] = {0};
	byte bck = *re;
	//fGetInt8(pData,&b);
	//fReadBytes(pData,&bar,cdd);
	//fCDataStoreFinalize(pData);
	for (int i = 0; i < cdd; i++) {
		fGetInt8(pData, &b);
		bar[i] = b;
	}

	*re = bck;
	printf("wr %X re %X %X\n", *wr, *re, bar[0]);

	//	printf("%X %X\n",*pData,b);
	//dump_to_file(&bar, cdd, "d:\chat.bin");

	return fPH_SMSG_MESSAGECHAT(param, msgId, time, pData);
}
int32_t cbPH_SMSG_BATTLEFIELD_STATUS(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) {
	//23	bytes offset to map id
	//19 statusID
	byte* re = ((uint32_t)pData + 0x14);
	byte* wr = ((uint32_t)pData + 0x10);
	byte bck = *re;

	*re = 19 + 2;
	uint8_t statusID = 0;
	fGetInt8(pData, &statusID);

	*re = 23 + 2;  //+2 is already read bytes? byte *re = ((uint32_t)pData + 0x14);?
	uint16_t mapid = 0;
	fGetInt16(pData, &mapid);
	static uint32_t BG_invite_active = 0;
	if (statusID == 2)
		BG_invite_active = 1;
	else if (statusID == 3 || !statusID)
		BG_invite_active = 0;
	//printf("b %X\n", b);
	static int qwecount = 0;
	uint32_t* pdata = ((uint32_t*)pData + 0x1);
	//printf("pdata %X %X\n",pData,*pdata);
	if (mapid) {
		//printf("mapid !=0 %X %X\n", mapid,statusID);
		//char  bb[30] = {0};
		//sprintf(&bb, "batstatus%d.bin", qwecount);
		//dump_to_file(*pdata,*wr,bb);
		//qwecount++;
	}
	//if (BG_invite_active)
	if (mapid == 0x24F) {  //turned off . accepting all BGs
		Send_CMSG_BATTLEFIELD_PORT(dataStorePtr1, 0);
		printf("BG denied %X\n", mapid);
		//dump_to_file(*pdata,41,"batstatus.bin");
		Send_Join_RBG(dataStorePtr1);
	} else {
		if (statusID == 2) {
			Send_CMSG_BATTLEFIELD_PORT(dataStorePtr1, 1);
			printf("BG accepted mapid:%X\n", mapid);

			int x = PlaySound("BG_accepted.wav", NULL, SND_FILENAME | SND_ASYNC);
			//Send_CMSG_BATTLEFIELD_PORT(dataStorePtr1, 0);
		}
		/*printf("*wr %d file:%d statusID:%d ",*wr,qwecount,statusID);
        if (*wr > 40)
        {

            *re = 37;
            uint16_t someint16=0;
            fGetInt16(pData, &someint16);
            printf("someint16 1 %X \n",someint16);

        }
        else
            printf("\n");
        Send_CMSG_BATTLEFIELD_PORT(dataStorePtr1,1);
        printf("BG accepted mapid:%X\n",mapid);
        char  bb[30] = {0};
        sprintf(&bb, "batstatus%d.bin", qwecount);
        dump_to_file(*pdata,*wr,bb);
        //dump_to_file(*pdata,41,"batstatus.bin");
        qwecount++;*/
	}

	*re = bck;
	//printf("---------done\n");
	return fPH_SMSG_BATTLEFIELD_STATUS(param, msgId, time, pData);
}
int32_t cbPH_SMSG_SPELL_START(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) {
	byte* re = ((uint32_t)pData + 0x14);
	byte bck = *re;

	*re = 2;
	uint8_t tmpv = 0;
	fGetInt8(pData, &tmpv);	 //guid #1 length
	uint8_t buffer[9] = {0};
	//uint8_t buffer[9] = { 0x11,0x22,0x33 };
	*re += tmpv;
	fGetInt8(pData, &tmpv);	 //guid #2 length
	*re += tmpv;
	*re += 1 + 4 + 4 + 4 + 4;
	fGetInt8(pData, &tmpv);	 //target guid length
	//fGetIntArray(pData,&buffer,2);
	*re += 1;
	fGetInt8(pData, &buffer);  //del
	printf("size %X GUID #1 %llX \n", tmpv, *(uint64_t*)&buffer);

	*re = bck;
	return fPH_SMSG_SPELL_START(param, msgId, time, pData);
}
int32_t cbPH_SMSG_CAST_FAILED(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) {
	return fPH_SMSG_CAST_FAILED(param, msgId, time, pData);
}
int32_t cbPH_SMSG_GROUP_LIST(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) {	//crashes on av
	//
	// dump packet into file
	//uint32_t *pdata = ((uint32_t*)pData + 0x1);
	//dump_to_file(*pdata,100,"cbPH_SMSG_INSPECT_RESULTS.bin");
	// first 2 bytes is the opcode
	//
	byte* re = ((uint32_t)pData + 0x14);  // pointer to position where to read from
	// *re==2 from the start. *re=x sets an absolute position from the start of packet.      F4 03 01 11 3B000000 01000565
	//																			   position>  1  2  3  4
	//byte *wr = ((uint32_t)pData + 0x10);// pointer to position where to write
	byte bck = *re;	 // backup the original read position
	// every read through fGetInt8 etc., will increment *re position on number of bytes read

	//>>>

	//<<<
	*re = bck;  // revert to original
	return fPH_SMSG_GROUP_LIST(param, msgId, time, pData);
}
int32_t cbPH_SMSG_LFG_PROPOSAL_UPDATE(uint32_t* param, enum NetMessage msgId, uint32_t time, uint32_t* pData) {
	//
	// dump packet into file
	//uint32_t *pdata = ((uint32_t*)pData + 0x1);
	//dump_to_file(*pdata,100,"cbPH_SMSG_INSPECT_RESULTS.bin");
	// first 2 bytes is the opcode
	//
	byte* re = ((uint32_t)pData + 0x14);  // pointer to position where to read from
	// *re==2 from the start. *re=x sets an absolute position from the start of packet.      F4 03 01 11 3B000000 01000565
	//																			   position>  1  2  3  4
	//byte *wr = ((uint32_t)pData + 0x10);// pointer to position where to write
	byte bck = *re;	 // backup the original read position
	// every read through fGetInt8 etc., will increment *re position on number of bytes read

	//>>>

	*re = 6;
	uint8_t prstate = 0;
	fGetInt8(pData, &prstate);  //proposal state

	if (prstate == LFG_PROPOSAL_INITIATING) {
		printf("LFG proposal\n");
		int x = PlaySound("RD_proposal.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
	//<<<
	*re = bck;  // revert to original
	return fPH_SMSG_LFG_PROPOSAL_UPDATE(param, msgId, time, pData);
}

void SetPHs() {
	//	fSetMessageHandler(SMSG_MESSAGECHAT,cbPH_SMSG_MESSAGECHAT,0);
	fSetMessageHandler(SMSG_BATTLEFIELD_STATUS, cbPH_SMSG_BATTLEFIELD_STATUS, 0);
	fSetMessageHandler(SMSG_GROUP_LIST, cbPH_SMSG_GROUP_LIST, 0);  //crashed on AV
	//fSetMessageHandler(SMSG_SPELL_START,cbPH_SMSG_SPELL_START,0);
	//fSetMessageHandler(SMSG_CAST_FAILED,cbPH_SMSG_CAST_FAILED,0);
	fSetMessageHandler(SMSG_LFG_PROPOSAL_UPDATE, cbPH_SMSG_LFG_PROPOSAL_UPDATE, 0);
}
void ResetPHs() {
	//	fSetMessageHandler(SMSG_MESSAGECHAT,fPH_SMSG_MESSAGECHAT,0);
	fSetMessageHandler(SMSG_BATTLEFIELD_STATUS, fPH_SMSG_BATTLEFIELD_STATUS, 0);
	fSetMessageHandler(SMSG_GROUP_LIST, fPH_SMSG_GROUP_LIST, 0);
	//fSetMessageHandler(SMSG_SPELL_START,fPH_SMSG_SPELL_START,0);
	//fSetMessageHandler(SMSG_CAST_FAILED,fPH_SMSG_CAST_FAILED,0);
	fSetMessageHandler(SMSG_LFG_PROPOSAL_UPDATE, fPH_SMSG_LFG_PROPOSAL_UPDATE, 0);
}
#pragma endregion
#endif
