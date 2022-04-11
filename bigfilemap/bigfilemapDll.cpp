#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale>
#include <iostream>
#include <map>
#include "bigfilemapDll.h"
#include "BigFileMapModel.h"
#include "ConvertClass.h"

using namespace std;


static map<string, BigFileMapModle *> objBigFile;



/* 所有注册给Lua的C函数具有
* "typedef int (*lua_CFunction) (lua_State *L);"的原型。
*/
//初始化映射表
extern "C" int init_objBigFile(lua_State * L)
{
	//获取传参
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		string value;
		if (lua_isstring(L, -1))
		{
			const char* temp = lua_tostring(L, -1);
			value = string(temp);
			cout << value << endl;
			if (objBigFile.count(value) == 0)
				objBigFile.insert(pair<string, BigFileMapModle *>(value, new BigFileMapModle()));
		}
		lua_pop(L, 1);
	}
	return 0;
}

//打开日志文件
extern "C" int open_objBigFile(lua_State * L)
{
	string errmsg = "";
	//获取传参
	lua_pushnil(L);
	while (lua_next(L, -2)) {
		string logtype;
		string logpath;
		if (lua_isstring(L, -1))
		{
			const char* temp = lua_tostring(L, -1);
			logpath = string(temp);
		}

		if (lua_isstring(L, -2))
		{
			const char* temp = lua_tostring(L, -2);
			logtype = string(temp);
		}
		cout << logtype << ":" << logpath << endl;

		if (objBigFile.count(logtype) == 0) {
			errmsg.append("not exit key:").append(logtype);
			cout << "error:"  << errmsg << endl;
			break;
		}
		else
		{
			string ret = objBigFile[logtype]->ResetBigFileMap(logpath);
			errmsg.append(ret);
			if (ret.length() != 0) {
				cout << "error:"  << errmsg << endl;
				break;
			}
		}	

		lua_pop(L, 1);
	}

	lua_pushstring(L, errmsg.c_str());
	return 1;
}

//拆分日志
extern "C" int split_objBigFile(lua_State * L)
{
	string errmsg = "";
	string logtype;
	string keyStr;
	string matchStr;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//获取关键字
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		keyStr = string(temp);
	}

	//获取匹配字
	if (lua_isstring(L, 3)) {
		const char* temp = lua_tostring(L, 3);
		matchStr = string(temp);
	}
	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
	}
	else
	{
		string ret = objBigFile[logtype]->SplitCycles(keyStr, matchStr);
		errmsg.append(ret);
		if (ret.length() != 0) {
			cout << "error:" << errmsg << endl;
		}
	}
	
	lua_pushstring(L, errmsg.c_str());
	return 1;
}

//获取拆分数据帧数量
extern "C" int get_framsCnt(lua_State * L)
{
	string errmsg = "";
	string logtype;
	int cnt = 0;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}
	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
	}
	else
	{
		cnt = objBigFile[logtype]->GetFramsCnt();
	}


	lua_pushstring(L, errmsg.c_str());
	lua_pushinteger(L, cnt);
	return 2;
}

//获取拆分数据帧范围
extern "C" int get_frameLimit(lua_State * L)
{
	string errmsg = "";
	string logtype;
	int minid = 0;
	int maxid = 0;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}
	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
	}
	else
	{
		minid = objBigFile[logtype]->GetFramsMinId();
		maxid = objBigFile[logtype]->GetFramsMaxId();
	}


	lua_pushinteger(L, minid);
	lua_pushinteger(L, maxid);
	return 2;
}

//获取指定周期的日志内容
extern "C" int get_cycleInfo(lua_State * L)
{
	string errmsg = "";
	string logmsg = "";
	string logtype;
	string cycle;
	vector<string> cycleInfo;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//获取指定周期
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}
	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
	}
	else
	{
		string ret = objBigFile[logtype]->GetCycleInfo(cycle, cycleInfo);
		errmsg.append(ret);
		if (ret.length() != 0) {
			cout << "error:" << errmsg << endl;
		}
		else
		{
			for (vector<string>::iterator it = cycleInfo.begin(); it != cycleInfo.end(); it++)
			{
				logmsg.append((*it)).append("\n");
				//cout << (*it) << endl;
			}
		}
	}

	lua_pushstring(L, errmsg.c_str());
	lua_pushstring(L, logmsg.c_str());
	return 2;
}

//获取指定周期偏移后的日志内存
extern "C" int get_cycleOffestInfo(lua_State * L)
{
	string errmsg = "";
	string logmsg = "";
	string logtype;
	string cycle;
	string cycleoffset;
	char tempchar[100] = { '\0' };
	int offset = 0;
	vector<string> cycleInfo;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//获取指定周期
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}

	//获取指定周期的偏移值
	if (lua_isinteger(L, 3)) {
		offset = lua_tointeger(L, 3);
		int temp = ConvertClass::asInt(cycle, 10);
		sprintf_s(tempchar, "%d", temp+offset);
		cycleoffset = string(tempchar);
	}


	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
	}
	else
	{
		string ret = objBigFile[logtype]->GetCycleInfo(cycleoffset, cycleInfo);
		errmsg.append(ret);
		if (ret.length() != 0) {
			cout << "error:" << errmsg << endl;
		}
		else
		{
			for (vector<string>::iterator it = cycleInfo.begin(); it != cycleInfo.end(); it++)
			{
				logmsg.append((*it)).append("\n");
				//cout << (*it) << endl;
			}
		}
	}

	lua_pushstring(L, errmsg.c_str());
	lua_pushstring(L, logmsg.c_str());
	return 2;
}


//按行获取指定周期的日志内容
extern "C" int get_cycleRowInfo(lua_State * L)
{
	static int index = 0;
	static vector<string> cycleInfo;
	string errmsg = "";
	string logmsg = "";
	string logtype;
	string cycle;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//获取指定周期
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}
	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
		lua_pushnil(L);
	}
	else
	{
		if (0 == index)
		{
			string ret = objBigFile[logtype]->GetCycleInfo(cycle, cycleInfo);
			errmsg.append(ret);
		}

		if (errmsg.length() != 0) {
			cout << "error:" << errmsg << endl;
			lua_pushnil(L);
		}
		else if(index < cycleInfo.size())
		{
			string temp = "";
			temp.append(cycleInfo[index++]);
			lua_pushstring(L, temp.c_str());
		}
		else
		{
			lua_pushnil(L);
		}
	}

	lua_pushstring(L, errmsg.c_str());
	return 2;
}

//按行获取指定周期的日志内容
extern "C" int get_cycleRowOffestInfo(lua_State * L)
{
	static int index = 0;
	static vector<string> cycleInfo;
	string errmsg = "";
	string logmsg = "";
	string logtype;
	string cycle;
	string cycleoffset;
	char tempchar[100] = { '\0' };
	int offset = 0;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//获取指定周期
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}

	//获取指定周期的偏移值
	if (lua_isinteger(L, 3)) {
		offset = lua_tointeger(L, 3);
		int temp = ConvertClass::asInt(cycle, 10);
		sprintf_s(tempchar, "%d", temp + offset);
		cycleoffset = string(tempchar);
	}

	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
		lua_pushnil(L);
	}
	else
	{
		if (0 == index)
		{
			string ret = objBigFile[logtype]->GetCycleInfo(cycleoffset, cycleInfo);
			errmsg.append(ret);
		}

		if (errmsg.length() != 0) {
			cout << "error:" << errmsg << endl;
			lua_pushnil(L);
		}
		else if (index < cycleInfo.size())
		{
			string temp = "";
			temp.append(cycleInfo[index++]);
			lua_pushstring(L, temp.c_str());
		}
		else
		{
			lua_pushnil(L);
		}
	}

	lua_pushstring(L, errmsg.c_str());
	return 2;
}


//关闭大文件对象
extern "C" int close_objBigFile(lua_State * L)
{
	string errmsg = "";
	string logtype;

	//获取日志类型
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}
	lua_pop(L, 1);

	//执行函数
	if (objBigFile.count(logtype) == 0) {
		errmsg.append("not exit key:").append(logtype);
		cout << "error:" << errmsg << endl;
	}
	else
	{
		delete objBigFile[logtype];
		map<string, BigFileMapModle*> ::iterator iter = objBigFile.find(logtype);
		objBigFile.erase(iter);
	}

	lua_pushstring(L, errmsg.c_str());
	return 1;
}


//提供的帮助说明
extern "C" int help(lua_State * L)
{
	string help = "new({value1,...,valuen})：初始化文本文件日志类型，无返回值 \n";
	help.append("openfile({value1=filepath1,...valuen=filepathn}): 打开对应的文本文件日志，返回错误信息\n");
	help.append("splitfile(valuen,keystr,matchstr):按规则拆分对应的文本文件日志，返回错误信息\n");
	help.append("getFrameCnt(valuen):获取文本文件日志数据帧数量，返回错误信息及数据帧数量\n");
	help.append("getFrameLimit(valuen):获取文本文件日志数据帧ID范围，返回数据帧范围\n");
	help.append("getCycleInfo(valuen,cyclestr):获取文本文件日志指定周期的日志信息，返回错误信息及日志信息\n");
	help.append("getCycleOffestInfo(valuen,cyclestr,offest):获取文本文件日志指定周期偏移后的日志信息，返回错误信息及日志信息\n");
	help.append("getCycleRowInfo(valuen,cyclestr):按行依次获取文本文件日志指定周期的日志信息，返回日志信息及错误信息\n");
	help.append("getCycleRowOffestInfo(valuen,cyclestr,offest):按行依次获取文本文件日志指定周期偏移后的日志信息，返回日志信息及错误信息\n");
	help.append("close(valuen):关闭文件对象，返回错误信息\n");
	lua_pushstring(L, help.c_str());
	return 1;
}


/* 需要一个"luaL_Reg"类型的结构体，其中每一个元素对应一个提供给Lua的函数。
* 每一个元素中包含此函数在Lua中的名字，以及该函数在C库中的函数指针。
* 最后一个元素为“哨兵元素”（两个"NULL"），用于告诉Lua没有其他的函数需要注册。
*/
static const struct luaL_Reg bigfilemap[] = {
	{ "help", help },
	{ "new", init_objBigFile },
	{ "openfile", open_objBigFile },
	{ "splitfile", split_objBigFile },
	{ "getFrameCnt", get_framsCnt },
	{ "getFrameLimit", get_frameLimit },
	{ "getCycleInfo", get_cycleInfo },
	{ "getCycleOffestInfo", get_cycleOffestInfo },
	{ "getCycleRowInfo", get_cycleRowInfo },
	{ "getCycleRowOffestInfo", get_cycleRowOffestInfo },
	{ "close", close_objBigFile },
	{ NULL, NULL }
};


/* 此函数为C库中的“特殊函数”。
* 通过调用它注册所有C库中的函数，并将它们存储在适当的位置。
* 此函数的命名规则应遵循：
* 1、使用"luaopen_"作为前缀。
* 2、前缀之后的名字将作为"require"的参数。
*/
extern "C"  __declspec(dllexport)
int luaopen_bigfilemap(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* 创建一个新的"table"，并将"l"中所列出的函数注册为"table"的域。
	*/
	luaL_newlib(L, bigfilemap);

	return 1;
}



#if 0  //传递结构体写法

extern "C" struct StudentTag
{
	char* strName; // 学生姓名
	char* strNum; // 学号
	int iSex; // 学生性别
	int iAge; // 学生年龄
};



/* 所有注册给Lua的C函数具有
* "typedef int (*lua_CFunction) (lua_State *L);"的原型。
*/


//lua 中通过调用这个接口来得到 pStudent指针
extern "C" int Student(lua_State* L)
{
	size_t iBytes = sizeof(struct StudentTag);
	struct StudentTag* pStudent;
	pStudent = (struct StudentTag*)lua_newuserdata(L, iBytes);
	return 1; // 新的userdata已经在栈上了

}

extern "C" int GetName(lua_State* L)
{
	struct StudentTag* pStudent = (struct StudentTag*)lua_touserdata(L, 1);
	luaL_argcheck(L, pStudent != NULL, 1, "Wrong Parameter");
	lua_pushstring(L, pStudent->strName);
	return 1;

}

extern "C" int SetName(lua_State* L)
{
	// 第一个参数是userdata
	struct StudentTag* pStudent = (struct StudentTag*)lua_touserdata(L, 1);

	luaL_argcheck(L, pStudent != NULL, 1, "Wrong Parameter");

	// 第二个参数是一个字符串
	const char* pName = luaL_checkstring(L, 2);//检查第2个参数是不是string,并返回参数值
	luaL_argcheck(L, pName != NULL && pName != "", 2, "Wrong Parameter");
	pStudent->strName = (char*)pName;
	return 0;
}


//提供的帮助说明
extern "C" int help(lua_State * L)
{
	string help = "to_utf8(string str)：utf8转string\n";
	help.append("to_string(string str):string转utf8\n");
	help.append("as_Uchar(string str,int base):string转无符号数值\n");
	help.append("as_LLong(string str,int base):string转long long数值\n");
	help.append("as_UShort(string str,int base):string转ushort数值\n");
	help.append("as_Uint(string str,int base):string转uint数值\n");
	help.append("as_Int(string str,int base):string转int数值\n");
	lua_pushstring(L, help.c_str());
	return 1;
}


/* 需要一个"luaL_Reg"类型的结构体，其中每一个元素对应一个提供给Lua的函数。
* 每一个元素中包含此函数在Lua中的名字，以及该函数在C库中的函数指针。
* 最后一个元素为“哨兵元素”（两个"NULL"），用于告诉Lua没有其他的函数需要注册。
*/
static const struct luaL_Reg bigfilemap[] = {
	{"new", Student},//注册成为Student的接口
	{"getName", GetName},
	{"setName", SetName},
	{ "help", help },
	{ NULL, NULL }
};


/* 此函数为C库中的“特殊函数”。
* 通过调用它注册所有C库中的函数，并将它们存储在适当的位置。
* 此函数的命名规则应遵循：
* 1、使用"luaopen_"作为前缀。
* 2、前缀之后的名字将作为"require"的参数。
*/
extern "C"  __declspec(dllexport)
int luaopen_bigfilemap(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* 创建一个新的"table"，并将"l"中所列出的函数注册为"table"的域。
	*/
	luaL_newlib(L, bigfilemap);

	return 1;
}
#endif