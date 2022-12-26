#include "includes.h"

int isUnitHostile(uint32_t* objectBase, uint32_t *unitToCompare){
	return fUnitReaction(objectBase, unitToCompare) < ReactionNeutral;
}

int isUnitFriendly(uint32_t* objectBase, uint32_t *unitToCompare){
	return fUnitReaction(objectBase, unitToCompare) > ReactionNeutral;
}