#pragma once
#include "JsonHandle.h"
#include "FileHandle.h"


using namespace fileHandle;
using namespace Json;


//json������չ��
class JsonHandleModel
{
private:
	JsonHandle jsonObj;
	const string _path = ".\\files\\";		//Ĭ��·��
public:

	//ת����·��
	string ConvertToPath(const string& path);

	//��ȡjson�ļ�
	string readDataFromJson(const string& path);

	//��ʾ��ȡ�����ݵ�����̨
	void show();

	//���л���Ϣ
	void ConvertTo(ItemInfo& itemInfo);

	//дjson�ļ�
	string WriteDataToJson(const string& path, ItemInfo& itemInfo);
};
