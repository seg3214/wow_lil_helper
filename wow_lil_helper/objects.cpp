#include "includes.h"




struct stClientObject *ar_stClientObject=0 ;
struct stPlayerObject ar_Players[ar_PlayerObjectIndex_length]; 

struct stPlayerObject *pMyselfstPlayerObject = 0;




void ObjectsPostProcess(int ocount){
	ar_Players[0].pstClientObject = 0; //clear the array;
	uint32_t PlayerCount = 0;
	int i ;

	G_bobberObjectBase = 0;
	uint32_t boober1 = 0, boober2 = 0;
	int bc = 0;

	for (i = 0; i < ocount; i++){
		if ((ar_stClientObject[i].type & Player) == Player){ //if human player
			
			ar_Players[PlayerCount].pstClientObject = (struct stClientObject*)&ar_stClientObject[i];
			ar_Players[PlayerCount].hostile = isUnitHostile((uint32_t*)ar_stClientObject[i].pBase, (uint32_t*)G_PlayerBase);

			if (ar_stClientObject[i].guid == G_PlayerGUID) //if myself
				pMyselfstPlayerObject = &ar_Players[PlayerCount];
			PlayerCount++;
		}
		else //if not human player
			if ((ar_stClientObject[i].type & GameObject) == GameObject){
				//fishbot finding the boober
				if (G_fishbotEnabled)
					if (ar_stClientObject[i].createdby == G_PlayerGUID){
						if (strcmp(ar_stClientObject[i].ObjectName, "Fishing Bobber") == 0) {

							bc++;
							if (bc == 1){
								boober1 = ar_stClientObject[i].pBase;
							}
							else
								if (bc == 2){
									boober2 = ar_stClientObject[i].pBase;
								}
						}
					}
				//fishbot finding the boober end
			}
	}

	ar_Players[PlayerCount].pstClientObject = 0;//mark the end of array;
	
	if (G_fishbotEnabled)
		if (boober2){
			G_bobberObjectBase = boober2;
		}
		else
			if (boober1){
				G_bobberObjectBase = boober1;
			}

	//objects_test();
}



void ReadGameObjects(){
	
	//allocating arObjects
	if (ar_stClientObject == 0)
		ar_stClientObject = (struct stClientObject*)malloc(sizeof(stClientObject)*ar_stClientObject_length);
	
	memset(ar_stClientObject, 0, sizeof(stClientObject)*ar_stClientObject_length);

	//etc params
	int ocount = 0;

	//code
	uint32_t* CurrentObject = (uint32_t*)(*G_ObjectManagerBase + constfirstObjectOffset);
	uint32_t* nextObject = (uint32_t*)(*CurrentObject + constnextObjectOffset);

		while (*CurrentObject) {

			uint32_t CurrObjType = *(uint32_t*)(*CurrentObject + 0xxx);
			int32_t* ObjectHeader = *(int32_t**)(*CurrentObject + 0xxx);

			if (CurrObjType > 0xx)
			{
				break;
			}
			//for every object
			struct stClientObject go;
			memset(&go, 0, sizeof(struct stClientObject));
			go.pBase = *CurrentObject;
			go.guid = *(uint64_t*)(ObjectHeader + OBJECT_FIELD_GUID);
			go.createdby = *(uint64_t*)(ObjectHeader + OBJECT_FIELD_CREATED_BY);
			go.type = *(uint32_t*)(ObjectHeader + OBJECT_FIELD_TYPE);
			go.ObjectName = fGetObjectName((uint32_t*)*CurrentObject);
			
			
			//for every object end

			if ((go.type & Player) == Player){ //if human player
				if (go.guid == G_PlayerGUID) G_PlayerBase = go.pBase;
				go.posx = *(float*)(go.pBase + 0xxx);
				go.posy = *(float*)(go.pBase + 0xxx);
				go.posz = *(float*)(go.pBase + 0xxx);
				go.height = *(float*)(go.pBase + 0xxx);
			}
			
			ar_stClientObject[ocount] = go;
			ocount++;
			//next object
			CurrentObject = (uint32_t*)(*CurrentObject + constnextObjectOffset);
			if (!*CurrentObject)
			{
				printf("!*CurrentObject %X", *CurrentObject);
				break;
			}
		}
		
		ObjectsPostProcess(ocount);// ocount: 1-ocount
		
}

void objects_cleanup(){
	free(ar_stClientObject);
}

//////////////////////////////////////////////////////////////
void DEBUG_print_arObjects(){
	for (int i = 0; i < ar_stClientObject_length; i++){
		if (ar_stClientObject[i].pBase == 0) break;
		printf("%d: %X %I64X %X %s %I64X\n", i, ar_stClientObject[i].pBase, ar_stClientObject[i].guid, ar_stClientObject[i].type, ar_stClientObject[i].ObjectName, ar_stClientObject[i].createdby);
	}
}

void DEBUG_print_playerArrayNames(){
	for (int i = 0; ar_Players[i].pstClientObject != 0; i++){
		//if (ar_PlayerObjectIndex[i] == -1) break;
		log_this("%i: %s %d\n", i, ar_Players[i].pstClientObject->ObjectName, ar_Players[i].hostile );
	}
}

void objects_test(){
	printf(" !!!!!!!!!!!!objects_test() \n");

	static int once = 0;
	//ReadGameObjects();
	//dump_to_file(ar_stClientObject, sizeof(stClientObject)*ar_stClientObject_length);
	if (!once){
		dump_to_file(ar_Players, sizeof(struct stPlayerObject)*ar_PlayerObjectIndex_length);
		//DEBUG_print_arObjects();
		DEBUG_print_playerArrayNames();
		once = 1;
	}
}
