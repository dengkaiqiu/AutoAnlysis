// AutoAnlysis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "BigFileMapModel.h"
#include "luaModel.h"
#include "errorTreeModel.h"
#include "jsonHandleModel.h"
#include "dbHandleModel.h"
#include "memMap.h"
#include "memMapTest.h"

using namespace std;


int main()
{
    BigFileMapModle bigfile;
    vector<string> pushparam;
    string TrainID;
    char temp[100] = { '\0' };
    sprintf_s(temp, "0x%04x", 10);
    TrainID = string(temp);
    errTree obj(".\\files\\");


    //加载故障树
    string errmsg = obj.Load("故障树.json");

    if (errmsg.length() == 0)
    {
        obj.Show();

        //设置配置值
        obj.setConfigValue("ret", "0xaa");
        obj.setConfigValue("errmsg", "");
        obj.setConfigValue("dbName", "E:\\study\\JsonTest_lib\\AutoAnlysis\\AutoAnlysis\\log\\DBLog\\20211225_22.13.20.db");
        obj.setConfigValue("applog", "E:\\study\\JsonTest_lib\\AutoAnlysis\\AutoAnlysis\\log\\zcAppDebugChar\\20211225_22.13.20.txt");
        obj.setConfigValue("platlog", "E:\\study\\JsonTest_lib\\AutoAnlysis\\AutoAnlysis\\log\\hostPlaDebug\\20211225_22.13.20.txt");
        obj.setConfigValue("devID", TrainID);
        obj.setConfigValue("appSplitKeystr", "=====period");
        obj.setConfigValue("appSplitMatchstr", "=====period (.*) ZC App Start===== at (.*)");
        obj.setConfigValue("platSplitKeystr", "@@@@@@@@@period");
        obj.setConfigValue("platSplitMatchstr", "@@@@@@@@@period  (.*)  start at  (.*)  @@@@@@@@");

        int childIndex;
        int banchIndex;
        cout << "请输入故障子树序号：";
        cin >> childIndex;
        cout << endl;


        cout << "请输入故障分支序号：";
        cin >> banchIndex;
        cout << endl;

        if (banchIndex == 0)
        {
            obj.execFun(1);
            obj.execChildIndex(childIndex);
        }
        else
        {
            obj.execFun(1);
            obj.execBranchIndex(childIndex, banchIndex);
        }
    }
    else
    {
        cout << "Error Tree init error,info:" << errmsg << endl;
    }

    system("pause");
}


