#pragma once

#include<iostream>
#include<vector>
#include"errorTreeModel.h"

using namespace std;

//����lua�ļ�
void LoadLua(string luafile);

//���غ�����
void LoadLuaFuntion(string funname, vector<string>& pushparam, ConfigClass& obj);