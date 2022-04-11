

#include"luaModel.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

lua_State* L = NULL;

//加载lua文件
void LoadLua(string luafile)
{
    L = luaL_newstate();
    luaL_openlibs(L);  // 加载Lua通用扩展库

    if (luaL_dofile(L, luafile.c_str()))
    {
        //或luaL_dofile(L,"test.lua")
        printf("error pcall!: %s\n", lua_tostring(L, -1));
        lua_close(L);
        L = NULL;
    }
}

//加载函数名
void LoadLuaFuntion(string funname, vector<string>& pushparam,ConfigClass& obj)
{
    if (NULL != L)
    {
        int inputParamCnt = 0;
        lua_getglobal(L, funname.c_str());

        lua_newtable(L); // 创建一个table

        //传入给lua参数
        for (vector<string>::iterator it = pushparam.begin(); it < pushparam.end(); it++)
        {
            lua_pushstring(L, (*it).c_str());  //key为intVal
            lua_pushstring(L, obj.getConfigValue(*it).c_str());      //值为1234
            lua_settable(L, -3);          //写入table
            inputParamCnt = 1;
        }

        //执行lua函数
        lua_pcall(L, inputParamCnt, 1, 0);

        //获取函数返回值
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
        //不处理
    }
}