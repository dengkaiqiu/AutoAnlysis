#pragma once

#include<iostream>
#include<vector>
#include"errorTreeModel.h"

using namespace std;

//加载lua文件
void LoadLua(string luafile);

//加载函数名
void LoadLuaFuntion(string funname, vector<string>& pushparam, ConfigClass& obj);