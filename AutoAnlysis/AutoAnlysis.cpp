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
#include <windows.h>
#include "FileHandle.h"

using namespace std;
using namespace fileHandle;


//离线解析
void OfflineParsing(string path)
{
    DWORD    dwExitCode = -1;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(STARTUPINFO)); //初始化si在内存块中的值（详见memset函数）
    memset(&pi, 0, sizeof(PROCESS_INFORMATION)); //初始化si在内存块中的值（详见memset函数）
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    //所要执行的命令
    string execmd = path + "\\OfflineParsing\\OfflineParsing.exe";
    string cmdLine = " -logpath C:\\Users\\DKQ\\Desktop\\aa";


    char charexecmd[_MAX_PATH] = {'\0'};
    char charcmdLine[_MAX_PATH] = { '\0' };
    strcpy(charexecmd, execmd.c_str());
    strcpy(charcmdLine, cmdLine.c_str());

    // Start the child process.创建子进程   
    if (!CreateProcess(charexecmd, // an exe file.   
        charcmdLine,        // parameter for your exe file.   
        NULL,             // Process handle not inheritable.   
        NULL,             // Thread handle not inheritable.   
        FALSE,            // Set handle inheritance to FALSE.   
        0,                // No creation flags.   
        NULL,             // Use parent's environment block.   
        NULL,             // Use parent's starting directory.  
                          // 传null默认为使用父目录启动子进程，如要改变子进程运行目录可以传相应路径
        &si,              // Pointer to STARTUPINFO structure.  
        &pi)             // Pointer to PROCESS_INFORMATION structure.  
        )
    {
        //return MYRESULT::CREATE_PROCESS_ERROR;
    }

    // Wait until child process exits.  等待子进程执行结束返回
    WaitForSingleObject(pi.hProcess, INFINITE);
    GetExitCodeProcess(pi.hProcess, &dwExitCode);

    cout << dwExitCode << endl;

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


//测试自动分析
void autoAnlysis(string path)
{
    string TrainID;
    char temp[100] = { '\0' };
    sprintf_s(temp, "0x%04x", 10);
    TrainID = string(temp);

    //故障树对象
    errTree obj(path);

    //加载故障树
    string errmsg = obj.Load("故障树.json");

    if (errmsg.length() == 0)
    {
        obj.Show();

        //设置配置值
        obj.setConfigValue("ret", "0xaa");
        obj.setConfigValue("errmsg", "");
        obj.setConfigValue("dbName", "E:\\study\\JsonTest_lib\\AutoAnlysis\\log\\DBLog\\20211225_22.13.20.db");
        obj.setConfigValue("applog", "E:\\study\\JsonTest_lib\\AutoAnlysis\\log\\zcAppDebugChar\\20211225_22.13.20.txt");
        obj.setConfigValue("platlog", "E:\\study\\JsonTest_lib\\AutoAnlysis\\log\\hostPlaDebug\\20211225_22.13.20.txt");
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

}


int main()
{
    //主程序路径
    char szPath[_MAX_PATH] = { 0 };

    //获取程序的路径
    GetModuleFileName(NULL, szPath, sizeof(szPath));
    string path = FileHandle::getFilePath(szPath);

    //离线解析
   // OfflineParsing(path);

    //自动分析
    autoAnlysis(path);

    system("pause");
}


