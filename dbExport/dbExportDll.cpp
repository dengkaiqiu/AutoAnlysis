#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale>
#include <iostream>
#include "dbExportDll.h"
#include "jsonHandleModel.h"
#include "dbHandleModel.h"

using namespace std;

/* ����ע���Lua��C��������
* "typedef int (*lua_CFunction) (lua_State *L);"��ԭ�͡�
*/
extern "C" int write_toJson(lua_State * L)
{
	string errmsg = "";
	string cycle;
	string dbName;

	//��ȡ���ݿ�
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		dbName = string(temp);
	}

	//��ȡ����
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}
	lua_pop(L, 1);

	cout << dbName << cycle << endl;
	//�����ݿ�
	DbHandleModel dbObj;
	errmsg = dbObj.OpenDb(dbName);  //�����ݿ�

	if (errmsg.length() == 0) {
		cout << "=================��ʼ�������ݣ���ȴ�================" << endl;
		vector<MemInfo> memInfos;  //�����ڴ�
		list<string> cyclelist;      //��Ҫ��ѯ������
		cyclelist.push_back(cycle);
		dbObj.GetCycleMemsFromDb(cyclelist, memInfos);  //��ѯ���ݿ�

		cout << "=================��ѯ�ɹ�����д��================" << endl;
		//json�����������дjson
		JsonHandleModel jsonHandleObj;

		for (vector<MemInfo>::iterator it = memInfos.begin(); it != memInfos.end(); it++)
		{
			ItemInfo temp;
			dbObj.ConvertTo(*it, temp);
			jsonHandleObj.WriteDataToJson(cycle + ".json", temp);
		}
		cout << "=================�������ݳɹ�================" << endl;
	}

	lua_pushstring(L, errmsg.c_str());
	return 1;
}


//�ṩ�İ���˵��
extern "C" int help(lua_State * L)
{
	string help = "writeJson(cyclestr)����ָ�������ڴ���־������json�����ش�����Ϣ \n";
	lua_pushstring(L, help.c_str());
	return 1;
}


/* ��Ҫһ��"luaL_Reg"���͵Ľṹ�壬����ÿһ��Ԫ�ض�Ӧһ���ṩ��Lua�ĺ�����
* ÿһ��Ԫ���а����˺�����Lua�е����֣��Լ��ú�����C���еĺ���ָ�롣
* ���һ��Ԫ��Ϊ���ڱ�Ԫ�ء�������"NULL"�������ڸ���Luaû�������ĺ�����Ҫע�ᡣ
*/
static const struct luaL_Reg dbExport[] = {
	{ "help", help },
	{ "writeJson", write_toJson },
	{ NULL, NULL }
};


/* �˺���ΪC���еġ����⺯������
* ͨ��������ע������C���еĺ������������Ǵ洢���ʵ���λ�á�
* �˺�������������Ӧ��ѭ��
* 1��ʹ��"luaopen_"��Ϊǰ׺��
* 2��ǰ׺֮������ֽ���Ϊ"require"�Ĳ�����
*/
extern "C"  __declspec(dllexport)
int luaopen_dbExport(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* ����һ���µ�"table"������"l"�����г��ĺ���ע��Ϊ"table"����
	*/
	luaL_newlib(L, dbExport);

	return 1;
}