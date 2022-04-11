#pragma once
#include "memMap.h"

typedef unsigned short UINT16;
typedef unsigned char UINT8;

struct Train_Info_struct
{
	UINT16 trainID;                                     /*列车ID*/
	UINT8  chTrainType;                                 /*列车类型*/
	UINT8  chSuspectHead;                               /*前端可疑标志*/
	UINT8  chSuspectTail;                               /*后端可疑标志*/
};


class TrainMode :public memMapMode
{

public:
	TrainMode(string memType) {
		this->memType = memType;
	}



private:
	//实现映射接口
	void MemMap(map<string, MemString*>& item, void* p)
	{
		Train_Info_struct* pTrain = (Train_Info_struct*)p;
		unsigned int trianID;
		trianID = item["列车ID"]->asUint(16);

		pTrain[trianID].trainID = trianID;
		pTrain[trianID].chTrainType = item["列车类型"]->asUChar(16);
		pTrain[trianID].chSuspectHead = item["后端可疑标志"]->asUChar(16);
		pTrain[trianID].chSuspectTail = item["前端可疑标志"]->asUChar(16);

	}
};