#pragma once
#include "memMap.h"

typedef unsigned short UINT16;
typedef unsigned char UINT8;

struct Train_Info_struct
{
	UINT16 trainID;                                     /*�г�ID*/
	UINT8  chTrainType;                                 /*�г�����*/
	UINT8  chSuspectHead;                               /*ǰ�˿��ɱ�־*/
	UINT8  chSuspectTail;                               /*��˿��ɱ�־*/
};


class TrainMode :public memMapMode
{

public:
	TrainMode(string memType) {
		this->memType = memType;
	}



private:
	//ʵ��ӳ��ӿ�
	void MemMap(map<string, MemString*>& item, void* p)
	{
		Train_Info_struct* pTrain = (Train_Info_struct*)p;
		unsigned int trianID;
		trianID = item["�г�ID"]->asUint(16);

		pTrain[trianID].trainID = trianID;
		pTrain[trianID].chTrainType = item["�г�����"]->asUChar(16);
		pTrain[trianID].chSuspectHead = item["��˿��ɱ�־"]->asUChar(16);
		pTrain[trianID].chSuspectTail = item["ǰ�˿��ɱ�־"]->asUChar(16);

	}
};