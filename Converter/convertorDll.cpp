// mylib.cpp : ���� DLL Ӧ�ó���ĵ���������

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <locale>
#include "ConvertorDll.h"
#include "ConvertClass.h"

//�ض���
typedef ConvertClass CmConvert;

/* ����ע���Lua��C��������
* "typedef int (*lua_CFunction) (lua_State *L);"��ԭ�͡�
*/

//���ַ���ת����utf-8
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

//��utf-8ת����string
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

//�������ַ���ת���ɶ�Ӧ����ֵ������baseΪ�ַ����Ľ���
extern "C" int as_Uchar(lua_State * L)
{
	std::string luastring;
	int base = 0;;

	//��ȡ�ַ���
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

	//��ȡ�ַ���
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

	//��ȡ�ַ���
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

	//��ȡ�ַ���
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

	//��ȡ�ַ���
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
	string help = "to_utf8(string str)��utf8תstring\n";
	help.append("to_string(string str):stringתutf8\n");
	help.append("as_Uchar(string str,int base):stringת�޷�����ֵ\n");
	help.append("as_LLong(string str,int base):stringתlong long��ֵ\n");
	help.append("as_UShort(string str,int base):stringתushort��ֵ\n");
	help.append("as_Uint(string str,int base):stringתuint��ֵ\n");
	help.append("as_Int(string str,int base):stringתint��ֵ\n");
	lua_pushstring(L, help.c_str());
	return 1;
}


/* ��Ҫһ��"luaL_Reg"���͵Ľṹ�壬����ÿһ��Ԫ�ض�Ӧһ���ṩ��Lua�ĺ�����
* ÿһ��Ԫ���а����˺�����Lua�е����֣��Լ��ú�����C���еĺ���ָ�롣
* ���һ��Ԫ��Ϊ���ڱ�Ԫ�ء�������"NULL"�������ڸ���Luaû�������ĺ�����Ҫע�ᡣ
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


/* �˺���ΪC���еġ����⺯������
* ͨ��������ע������C���еĺ������������Ǵ洢���ʵ���λ�á�
* �˺�������������Ӧ��ѭ��
* 1��ʹ��"luaopen_"��Ϊǰ׺��
* 2��ǰ׺֮������ֽ���Ϊ"require"�Ĳ�����
*/
extern "C"  __declspec(dllexport)
int luaopen_convertor(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* ����һ���µ�"table"������"l"�����г��ĺ���ע��Ϊ"table"����
	*/
	luaL_newlib(L, convertor);

	return 1;
}