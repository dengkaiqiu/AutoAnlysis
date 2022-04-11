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
	//转换成
	void ConvertTo(ItemInfo& itemInfo, map<string, MemString*>& item);

	//释放空间
	void releaseMemory(map<string, MemString*>& item);

	//获取待解析的内容
	void getMemInfo(ItemInfo& itemIn, map<string, vector<ItemInfo>>& itemOut);

	//虚函数
	virtual  void MemMap(map<string, MemString*>& item, void* p) = 0;

public:

	//映射到内存
	void Map(ItemInfo& itemInfo, void* p);
};