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



/* ����ע���Lua��C��������
* "typedef int (*lua_CFunction) (lua_State *L);"��ԭ�͡�
*/
//��ʼ��ӳ���
extern "C" int init_objBigFile(lua_State * L)
{
	//��ȡ����
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

//����־�ļ�
extern "C" int open_objBigFile(lua_State * L)
{
	string errmsg = "";
	//��ȡ����
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

//�����־
extern "C" int split_objBigFile(lua_State * L)
{
	string errmsg = "";
	string logtype;
	string keyStr;
	string matchStr;

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//��ȡ�ؼ���
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		keyStr = string(temp);
	}

	//��ȡƥ����
	if (lua_isstring(L, 3)) {
		const char* temp = lua_tostring(L, 3);
		matchStr = string(temp);
	}
	lua_pop(L, 1);

	//ִ�к���
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

//��ȡ�������֡����
extern "C" int get_framsCnt(lua_State * L)
{
	string errmsg = "";
	string logtype;
	int cnt = 0;

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}
	lua_pop(L, 1);

	//ִ�к���
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

//��ȡ�������֡��Χ
extern "C" int get_frameLimit(lua_State * L)
{
	string errmsg = "";
	string logtype;
	int minid = 0;
	int maxid = 0;

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}
	lua_pop(L, 1);

	//ִ�к���
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

//��ȡָ�����ڵ���־����
extern "C" int get_cycleInfo(lua_State * L)
{
	string errmsg = "";
	string logmsg = "";
	string logtype;
	string cycle;
	vector<string> cycleInfo;

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//��ȡָ������
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}
	lua_pop(L, 1);

	//ִ�к���
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

//��ȡָ������ƫ�ƺ����־�ڴ�
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

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//��ȡָ������
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}

	//��ȡָ�����ڵ�ƫ��ֵ
	if (lua_isinteger(L, 3)) {
		offset = lua_tointeger(L, 3);
		int temp = ConvertClass::asInt(cycle, 10);
		sprintf_s(tempchar, "%d", temp+offset);
		cycleoffset = string(tempchar);
	}


	lua_pop(L, 1);

	//ִ�к���
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


//���л�ȡָ�����ڵ���־����
extern "C" int get_cycleRowInfo(lua_State * L)
{
	static int index = 0;
	static vector<string> cycleInfo;
	string errmsg = "";
	string logmsg = "";
	string logtype;
	string cycle;

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//��ȡָ������
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}
	lua_pop(L, 1);

	//ִ�к���
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

//���л�ȡָ�����ڵ���־����
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

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}

	//��ȡָ������
	if (lua_isstring(L, 2)) {
		const char* temp = lua_tostring(L, 2);
		cycle = string(temp);
	}

	//��ȡָ�����ڵ�ƫ��ֵ
	if (lua_isinteger(L, 3)) {
		offset = lua_tointeger(L, 3);
		int temp = ConvertClass::asInt(cycle, 10);
		sprintf_s(tempchar, "%d", temp + offset);
		cycleoffset = string(tempchar);
	}

	lua_pop(L, 1);

	//ִ�к���
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


//�رմ��ļ�����
extern "C" int close_objBigFile(lua_State * L)
{
	string errmsg = "";
	string logtype;

	//��ȡ��־����
	if (lua_isstring(L, 1)) {
		const char* temp = lua_tostring(L, 1);
		logtype = string(temp);
	}
	lua_pop(L, 1);

	//ִ�к���
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


//�ṩ�İ���˵��
extern "C" int help(lua_State * L)
{
	string help = "new({value1,...,valuen})����ʼ���ı��ļ���־���ͣ��޷���ֵ \n";
	help.append("openfile({value1=filepath1,...valuen=filepathn}): �򿪶�Ӧ���ı��ļ���־�����ش�����Ϣ\n");
	help.append("splitfile(valuen,keystr,matchstr):�������ֶ�Ӧ���ı��ļ���־�����ش�����Ϣ\n");
	help.append("getFrameCnt(valuen):��ȡ�ı��ļ���־����֡���������ش�����Ϣ������֡����\n");
	help.append("getFrameLimit(valuen):��ȡ�ı��ļ���־����֡ID��Χ����������֡��Χ\n");
	help.append("getCycleInfo(valuen,cyclestr):��ȡ�ı��ļ���־ָ�����ڵ���־��Ϣ�����ش�����Ϣ����־��Ϣ\n");
	help.append("getCycleOffestInfo(valuen,cyclestr,offest):��ȡ�ı��ļ���־ָ������ƫ�ƺ����־��Ϣ�����ش�����Ϣ����־��Ϣ\n");
	help.append("getCycleRowInfo(valuen,cyclestr):�������λ�ȡ�ı��ļ���־ָ�����ڵ���־��Ϣ��������־��Ϣ��������Ϣ\n");
	help.append("getCycleRowOffestInfo(valuen,cyclestr,offest):�������λ�ȡ�ı��ļ���־ָ������ƫ�ƺ����־��Ϣ��������־��Ϣ��������Ϣ\n");
	help.append("close(valuen):�ر��ļ����󣬷��ش�����Ϣ\n");
	lua_pushstring(L, help.c_str());
	return 1;
}


/* ��Ҫһ��"luaL_Reg"���͵Ľṹ�壬����ÿһ��Ԫ�ض�Ӧһ���ṩ��Lua�ĺ�����
* ÿһ��Ԫ���а����˺�����Lua�е����֣��Լ��ú�����C���еĺ���ָ�롣
* ���һ��Ԫ��Ϊ���ڱ�Ԫ�ء�������"NULL"�������ڸ���Luaû�������ĺ�����Ҫע�ᡣ
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


/* �˺���ΪC���еġ����⺯������
* ͨ��������ע������C���еĺ������������Ǵ洢���ʵ���λ�á�
* �˺�������������Ӧ��ѭ��
* 1��ʹ��"luaopen_"��Ϊǰ׺��
* 2��ǰ׺֮������ֽ���Ϊ"require"�Ĳ�����
*/
extern "C"  __declspec(dllexport)
int luaopen_bigfilemap(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* ����һ���µ�"table"������"l"�����г��ĺ���ע��Ϊ"table"����
	*/
	luaL_newlib(L, bigfilemap);

	return 1;
}



#if 0  //���ݽṹ��д��

extern "C" struct StudentTag
{
	char* strName; // ѧ������
	char* strNum; // ѧ��
	int iSex; // ѧ���Ա�
	int iAge; // ѧ������
};



/* ����ע���Lua��C��������
* "typedef int (*lua_CFunction) (lua_State *L);"��ԭ�͡�
*/


//lua ��ͨ����������ӿ����õ� pStudentָ��
extern "C" int Student(lua_State* L)
{
	size_t iBytes = sizeof(struct StudentTag);
	struct StudentTag* pStudent;
	pStudent = (struct StudentTag*)lua_newuserdata(L, iBytes);
	return 1; // �µ�userdata�Ѿ���ջ����

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
	// ��һ��������userdata
	struct StudentTag* pStudent = (struct StudentTag*)lua_touserdata(L, 1);

	luaL_argcheck(L, pStudent != NULL, 1, "Wrong Parameter");

	// �ڶ���������һ���ַ���
	const char* pName = luaL_checkstring(L, 2);//����2�������ǲ���string,�����ز���ֵ
	luaL_argcheck(L, pName != NULL && pName != "", 2, "Wrong Parameter");
	pStudent->strName = (char*)pName;
	return 0;
}


//�ṩ�İ���˵��
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
static const struct luaL_Reg bigfilemap[] = {
	{"new", Student},//ע���ΪStudent�Ľӿ�
	{"getName", GetName},
	{"setName", SetName},
	{ "help", help },
	{ NULL, NULL }
};


/* �˺���ΪC���еġ����⺯������
* ͨ��������ע������C���еĺ������������Ǵ洢���ʵ���λ�á�
* �˺�������������Ӧ��ѭ��
* 1��ʹ��"luaopen_"��Ϊǰ׺��
* 2��ǰ׺֮������ֽ���Ϊ"require"�Ĳ�����
*/
extern "C"  __declspec(dllexport)
int luaopen_bigfilemap(lua_State * L) {
	/* void luaL_newlib (lua_State *L, const luaL_Reg l[]);
	* ����һ���µ�"table"������"l"�����г��ĺ���ע��Ϊ"table"����
	*/
	luaL_newlib(L, bigfilemap);

	return 1;
}
#endif