

#include"luaModel.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

lua_State* L = NULL;

//����lua�ļ�
void LoadLua(string luafile)
{
    L = luaL_newstate();
    luaL_openlibs(L);  // ����Luaͨ����չ��

    if (luaL_dofile(L, luafile.c_str()))
    {
        //��luaL_dofile(L,"test.lua")
        printf("error pcall!: %s\n", lua_tostring(L, -1));
        lua_close(L);
        L = NULL;
    }
}

//���غ�����
void LoadLuaFuntion(string funname, vector<string>& pushparam,ConfigClass& obj)
{
    if (NULL != L)
    {
        int inputParamCnt = 0;
        lua_getglobal(L, funname.c_str());

        lua_newtable(L); // ����һ��table

        //�����lua����
        for (vector<string>::iterator it = pushparam.begin(); it < pushparam.end(); it++)
        {
            lua_pushstring(L, (*it).c_str());  //keyΪintVal
            lua_pushstring(L, obj.getConfigValue(*it).c_str());      //ֵΪ1234
            lua_settable(L, -3);          //д��table
            inputParamCnt = 1;
        }

        //ִ��lua����
        lua_pcall(L, inputParamCnt, 1, 0);

        //��ȡ��������ֵ
        lua_pushnil(L);
        while (lua_next(L, -2)) {
            string key;
            string value;
            if (lua_isstring(L, -2))
            {
                const char* temp = lua_tostring(L, -2);
                key = string(temp);
            }

            if (lua_isstring(L, -1))
            {
                const char* temp = lua_tostring(L, -1);
                value = string(temp);
            }

            // cout << "key:" << key << "    value:" << value << endl;
            obj.setConfigValue(key, value);
            lua_pop(L, 1);
        }
    }
    else
    {
        //������
    }
}