#pragma once

#include <iostream>
#include "JsonHandle.h"
#include "MemString.h"

using namespace std;
using namespace Json;

class memMapMode
{
public:
	string memType;

private:
	//ת����
	void ConvertTo(ItemInfo& itemInfo, map<string, MemString*>& item);

	//�ͷſռ�
	void releaseMemory(map<string, MemString*>& item);

	//��ȡ������������
	void getMemInfo(ItemInfo& itemIn, map<string, vector<ItemInfo>>& itemOut);

	//�麯��
	virtual  void MemMap(map<string, MemString*>& item, void* p) = 0;

public:

	//ӳ�䵽�ڴ�
	void Map(ItemInfo& itemInfo, void* p);
};