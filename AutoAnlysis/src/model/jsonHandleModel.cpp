#include "jsonHandleModel.h"


//转换成路径
string JsonHandleModel::ConvertToPath(const string& path)
{
	string filePath;
	if (FileHandle::is_path(path))
		filePath = path;
	else if (FileHandle::is_file(path))
		filePath = _path + path;
	else
		filePath = "";
	return filePath;
}

//读取json文件
string JsonHandleModel::readDataFromJson(const string& path)
{
	string err;
	string filePath = ConvertToPath(path);

	if (!filePath.empty() && FileHandle::exists_file(filePath))
		err = jsonObj.readDataFromJson(filePath);
	else
		err = path + " is error!!!";

	return err;
}

//显示读取的内容到控制台
void JsonHandleModel::show()
{
	jsonObj.show();
}

//序列化信息
void JsonHandleModel::ConvertTo(ItemInfo& itemInfo)
{
	jsonObj.ConvertTo(itemInfo);
}

//写json文件
string JsonHandleModel::WriteDataToJson(const string& path, ItemInfo& itemInfo)
{
	string err;
	string filePath = ConvertToPath(path);

	if (!filePath.empty())
		jsonObj.writeDataToJson(filePath, itemInfo);
	else
		err = path + " is error!!!";

	return err;
}