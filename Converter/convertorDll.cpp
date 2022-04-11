// mylib.cpp : 定义 DLL 应用程序的导出函数。

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale>
#include "ConvertorDll.h"
#include "ConvertClass.h"

//重定义
typedef ConvertClass CmConvert;

/* 所有注册给Lua的C函数具有
* "typedef int (*lua_CFunction) (lua_State *L);"的原型。
*/

//将字符串转换成utf-8
extern "C" int to_utf8(lua_State * L)
{
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		std::string luastring = string(numberB);

		string convertstr = CmConvert::StringToUTF8(luastring);

		lua_pushstring(L,convertstr.c_str());
	}

	return 1;
}

//将utf-8转换成string
extern "C" int to_string(lua_State * L)
{
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		std::string luastring = string(numberB);
		
		string convertstr = CmConvert::UTF8ToString(luastring);

		lua_pushstring(L, convertstr.c_str());
	}

	return 1;
}

//将数字字符串转换成对应的数值，其中base为字符串的进制
extern "C" int as_Uchar(lua_State * L)
{
	std::string luastring;
	int base = 0;;

	//获取字符串
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		luastring = string(numberB);
	}

	if (lua_isinteger(L, 2))
	{
		base = lua_tointeger(L, 2);
	}

	if (luastring.length() > 0)
	{
		lua_pushinteger(L, CmConvert::asUChar(luastring,base));
		return 1;
	}
	return 0;
}

extern "C" int as_LLong(lua_State * L)
{
	std::string luastring;
	int base = 0;;

	//获取字符串
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		luastring = string(numberB);
	}

	if (lua_isinteger(L, 2))
	{
		base = lua_tointeger(L, 2);
	}

	if (luastring.length() > 0)
	{
		lua_pushnumber(L, CmConvert::asLLong(luastring, base));
		return 1;
	}
	return 0;
}



extern "C" int as_UShort(lua_State * L)
{
	std::string luastring;
	int base = 0;;

	//获取字符串
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		luastring = string(numberB);
	}

	if (lua_isinteger(L, 2))
	{
		base = lua_tointeger(L, 2);
	}

	if (luastring.length() > 0)
	{
		lua_pushinteger(L, CmConvert::asUShort(luastring, base));
		return 1;
	}
	return 0;
}

extern "C" int as_Uint(lua_State * L)
{
	std::string luastring;
	int base = 0;;

	//获取字符串
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		luastring = string(numberB);
	}

	if (lua_isinteger(L, 2))
	{
		base = lua_tointeger(L, 2);
	}

	if (luastring.length() > 0)
	{
		lua_pushinteger(L, CmConvert::asUint(luastring, base));
		return 1;
	}
	return 0;
}


extern "C" int as_Int(lua_State * L)
{
	std::string luastring;
	int base = 0;;

	//获取字符串
	if (lua_isstring(L, 1))
	{
		const char* numberB = lua_tostring(L, 1);
		luastring = string(numberB);
	}

	if (lua_isinteger(L, 2))
	{
		base = lua_tointeger(L, 2);
	}

	if (luastring.length() > 0)
	{
		lua_pushinteger(L, CmConvert::asInt(luastring, base));
		return 1;
	}
	return 0;
}


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
static const struct luaL_Reg convertor[] = {
	{ "help", help },
	{ "to_utf8", to_utf8 },
	{ "to_string", to_string },
	{ "as_Uchar", as_Uchar },
	{ "as_LLong", as_LLong },
	{ "as_UShort", as_UShort },
	{ "as_Uint", as_Uint },
	{ "as_Int", as_Int },
	{ NULL, NULL }
};


/* 此函数为C库中的“特殊函数”。
* 通过调用它注册所有C库中的函数，并将它们存储在适当的位置。
* 此函数的命名规则应遵循：
* 1、使用"luaopen_"作为前缀。
* 2、前缀之后的名字将作为"require"的参数。
*/
extern "C"  __declspec(dllexport)
int luaopen_convertor(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* 创建一个新的"table"，并将"l"中所列出的函数注册为"table"的域。
	*/
	luaL_newlib(L, convertor);

	return 1;
}