#pragma once
#include "JsonHandle.h"
#include "FileHandle.h"


using namespace fileHandle;
using namespace Json;


//json操作扩展类
class JsonHandleModel
{
private:
	JsonHandle jsonObj;
	const string _path = ".\\files\\";		//默认路径
public:

	//转换成路径
	string ConvertToPath(const string& path);

	//读取json文件
	string readDataFromJson(const string& path);

	//显示读取的内容到控制台
	void show();

	//序列化信息
	void ConvertTo(ItemInfo& itemInfo);

	//写json文件
	string WriteDataToJson(const string& path, ItemInfo& itemInfo);
};
