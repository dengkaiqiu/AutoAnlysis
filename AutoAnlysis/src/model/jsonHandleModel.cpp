#include "jsonHandleModel.h"


//ת����·��
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

//��ȡjson�ļ�
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

//��ʾ��ȡ�����ݵ�����̨
void JsonHandleModel::show()
{
	jsonObj.show();
}

//���л���Ϣ
void JsonHandleModel::ConvertTo(ItemInfo& itemInfo)
{
	jsonObj.ConvertTo(itemInfo);
}

//дjson�ļ�
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