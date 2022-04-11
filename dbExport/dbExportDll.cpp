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

/* 所有注册给Lua的C函数具有
* "typedef int (*lua_CFunction) (lua_State *L);"的原型。
*/
extern "C" int write_toJson(lua_State * L)
{
	string errmsg = "";
	string cycle;
	string dbName;

	//获取数据库
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		dbName = string(temp);
	}

	//获取周期
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}
	lua_pop(L, 1);

	cout << dbName << cycle << endl;
	//打开数据库
	DbHandleModel dbObj;
	errmsg = dbObj.OpenDb(dbName);  //打开数据库

	if (errmsg.length() == 0) {
		cout << "=================开始导出数据，请等待================" << endl;
		vector<MemInfo> memInfos;  //周期内存
		list<string> cyclelist;      //需要查询的周期
		cyclelist.push_back(cycle);
		dbObj.GetCycleMemsFromDb(cyclelist, memInfos);  //查询数据库

		cout << "=================查询成功，待写入================" << endl;
		//json操作对象进行写json
		JsonHandleModel jsonHandleObj;

		for (vector<MemInfo>::iterator it = memInfos.begin(); it != memInfos.end(); it++)
		{
			ItemInfo temp;
			dbObj.ConvertTo(*it, temp);
			jsonHandleObj.WriteDataToJson(cycle + ".json", temp);
		}
		cout << "=================导出数据成功================" << endl;
	}

	lua_pushstring(L, errmsg.c_str());
	return 1;
}


//提供的帮助说明
extern "C" int help(lua_State * L)
{
	string help = "writeJson(cyclestr)：将指定周期内存日志导出到json，返回错误信息 \n";
	lua_pushstring(L, help.c_str());
	return 1;
}


/* 需要一个"luaL_Reg"类型的结构体，其中每一个元素对应一个提供给Lua的函数。
* 每一个元素中包含此函数在Lua中的名字，以及该函数在C库中的函数指针。
* 最后一个元素为“哨兵元素”（两个"NULL"），用于告诉Lua没有其他的函数需要注册。
*/
static const struct luaL_Reg dbExport[] = {
	{ "help", help },
	{ "writeJson", write_toJson },
	{ NULL, NULL }
};


/* 此函数为C库中的“特殊函数”。
* 通过调用它注册所有C库中的函数，并将它们存储在适当的位置。
* 此函数的命名规则应遵循：
* 1、使用"luaopen_"作为前缀。
* 2、前缀之后的名字将作为"require"的参数。
*/
extern "C"  __declspec(dllexport)
int luaopen_dbExport(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* 创建一个新的"table"，并将"l"中所列出的函数注册为"table"的域。
	*/
	luaL_newlib(L, dbExport);

	return 1;
}