#include "memMap.h"

#include "FileHandle.h"
#include "jsonHandleModel.h"

using namespace fileHandle;

//ת����
void memMapMode::ConvertTo(ItemInfo& itemInfo, map<string, MemString*>& item)
{
	for (auto it = itemInfo._itemMapinfo.begin(); it != itemInfo._itemMapinfo.end(); it++)
	{
		pair<string, string> temp = *it;
		item.insert(pair<string, MemString*>(temp.first, new MemString(temp.second)));
	}
}

//�ͷſռ�
void memMapMode::releaseMemory(map<string, MemString*>& item)
{
	for (auto it = item.begin(); it != item.end(); it++)
	{
		delete it->second;
	}
}


//��ȡ������������
void memMapMode::getMemInfo(ItemInfo& itemIn, map<string, vector<ItemInfo>>& itemOut)
{
	if (itemIn._itemMapinfo["memType"] == memType) {
		itemOut = itemIn._subItemMapinfo;
	}
	else
	{
		for (auto it = itemIn._subItemMapinfo.begin(); it != itemIn._subItemMapinfo.end(); it++)
		{
			pair<string, vector<ItemInfo>> temp = (*it);

			for (vector<ItemInfo>::iterator itt = temp.second.begin(); itt != temp.second.end(); itt++)
			{
				getMemInfo(*itt, itemOut);
			}
		}

	}
}


//ӳ�䵽�ڴ�
void memMapMode::Map(ItemInfo& itemInfo, void* p)
{
	map<string, vector<ItemInfo>> trainObj;
	string str = memType + "Infos";

	getMemInfo(itemInfo, trainObj);

	for (vector<ItemInfo>::iterator it = trainObj[str].begin(); it != trainObj[str].end(); it++)
	{
		map<string, MemString*> temp;
		ConvertTo(*it, temp);
		MemMap(temp, p);
		releaseMemory(temp);
	}

}
