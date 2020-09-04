#include "hw1.h"

void Init(){
	int i;

	pFreeListHead = NULL;
	pFreeListTail = NULL;
	
	for(i=0;i<HASH_TBL_SIZE;++i)
	{
		pHashTableEnt[i].elmtCount=0;
		pHashTableEnt[i].pHead = NULL;
		pHashTableEnt[i].pTail = NULL;
	}
}

void InsertObjectToTail(Object* pObj, int ObjNum){
    int index = ObjNum % HASH_TBL_SIZE;
    
    pObj->objnum = ObjNum;
	if (pHashTableEnt[index].pHead == NULL)
	{
		pHashTableEnt[index].pHead = pObj;
		pHashTableEnt[index].pTail = pObj;
	}
	else
	{
		pHashTableEnt[index].pTail->phNext = pObj;
		pObj->phPrev = pHashTableEnt[index].pTail;
		pHashTableEnt[index].pTail = pObj;
	}
	pHashTableEnt[index].elmtCount++;

}

void InsertObjectToHead(Object* pObj, int objNum){
    int index = objNum % HASH_TBL_SIZE;
	
	pObj->objnum = objNum;
	if (pHashTableEnt[index].pHead == NULL)
	{
		pHashTableEnt[index].pHead = pObj;
		pHashTableEnt[index].pTail = pObj;
	}
	else
	{
		pObj->phNext = pHashTableEnt[index].pHead;
		pHashTableEnt[index].pHead->phPrev = pObj;
		pHashTableEnt[index].pHead = pObj;
	}
	pHashTableEnt[index].elmtCount++;

}

Object* GetObjectByNum(int objnum){    
	int index = objnum % HASH_TBL_SIZE;
	Object* temp = pHashTableEnt[index].pHead;

	while (temp != NULL)
	{
		if (temp->objnum == objnum)
			return temp;
		temp = temp->phNext;
	}
	return NULL;
}

Object* GetObjectFromObjFreeList(){
	Object* temp = pFreeListTail;
	
	if (pFreeListTail != NULL)
	{
		if (pFreeListTail != pFreeListHead)
		{
			pFreeListTail = pFreeListTail->phPrev;
			pFreeListTail->phNext = NULL;
		}
		else
		{
			pFreeListHead = NULL;
			pFreeListTail = NULL;
		}
		temp->phNext = NULL;
		temp->phPrev = NULL;
		return temp;
	}
	return NULL;
}

BOOL DeleteObject(Object* pObj){
	int index = pObj->objnum % HASH_TBL_SIZE;
	Object* temp = pHashTableEnt[index].pHead;

	while (temp != NULL)
	{
		if (temp == pObj)
		{
			if (pObj == pHashTableEnt[index].pHead)
				pHashTableEnt[index].pHead = pObj->phNext;
			if (pObj == pHashTableEnt[index].pTail)
				pHashTableEnt[index].pTail = pObj->phPrev;
			if (pObj->phNext != NULL)
				pObj->phNext->phPrev = pObj->phPrev;
			if (pObj->phPrev != NULL)
				pObj->phPrev->phNext = pObj->phNext;
			pHashTableEnt[index].elmtCount--;
			return 1;
		}
		temp = temp->phNext;
	}
	return 0;
}

void InsertObjectIntoObjFreeList(Object* pObj){
	pObj->objnum = OBJ_INVALID;
	if (pFreeListHead == NULL)
	{
		pFreeListHead = pObj;
		pFreeListTail = pObj;
	}
	else
	{
		pObj->phNext = pFreeListHead;
		pFreeListHead = pObj;
		pObj->phNext->phPrev = pObj;
	}
}

