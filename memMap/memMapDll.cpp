#include "memMapDll.h"
#include "jsonHandleModel.h"
#include "FileHandle.h"

using namespace fileHandle;



//����json�ļ�
extern "C" MYLIB_API
string LoadJson(const string & path, ItemInfo & item)
{
	string errmsg = "";

	//�ж��ļ��Ƿ����
	if (!FileHandle::exists_file(path)) {
		errmsg.append(path + " not exit!!!");
		return errmsg;
	}

	//�����ļ�
	JsonHandleModel jsonHandleObj;

	//��ȡ
	errmsg = jsonHandleObj.readDataFromJson(path);
	if (errmsg.length() != 0)
		return errmsg;

	//���л�
	jsonHandleObj.ConvertTo(item);

	return errmsg;
}