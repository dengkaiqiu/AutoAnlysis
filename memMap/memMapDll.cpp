#include "memMapDll.h"
#include "jsonHandleModel.h"
#include "FileHandle.h"

using namespace fileHandle;



//加载json文件
extern "C" MYLIB_API
string LoadJson(const string & path, ItemInfo & item)
{
	string errmsg = "";

	//判断文件是否存在
	if (!FileHandle::exists_file(path)) {
		errmsg.append(path + " not exit!!!");
		return errmsg;
	}

	//加载文件
	JsonHandleModel jsonHandleObj;

	//读取
	errmsg = jsonHandleObj.readDataFromJson(path);
	if (errmsg.length() != 0)
		return errmsg;

	//序列化
	jsonHandleObj.ConvertTo(item);

	return errmsg;
}