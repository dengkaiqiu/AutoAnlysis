#pragma once
#include <string>
#include <iostream>
#include <map>
#include <list>
#include "FileHandle.h"
#include "jsonHandleModel.h"


using namespace std;

//函数类
class FunClass
{
public:
    string _funName;
    string _requireParam;
    string _returnParam;

public:

    FunClass() { ; }

    //构造函数
    FunClass(const string& funName, const string& requireParam, const string& returnParam)
    {
        _funName = funName;
        _requireParam = requireParam;
        _returnParam = returnParam;
    }
};

//配置类
class ConfigClass
{
public:
    map<string, string> _cfgItem;

public:
    //构造函数
    ConfigClass() { ; }

    //析构函数
    ~ConfigClass() {
        _cfgItem.clear();
    }

    //设置配置值
    void setConfigValue(string key, string value);

    //获取配置值
    string getConfigValue(string key);

};

//函数执行类
class FunExecClass
{
    friend class errBranch;
    friend class errTree;

public:
    void funExec(FunClass& funobj, ConfigClass& cfgobj);    //函数执行类
};

//故障分支
class errBranch : public ConfigClass
{
public:
    map<string, FunClass> _funs;
    FunExecClass _funExec;

public:
    errBranch() { ; }   //构造函数

    string Load(ItemInfo& item);   //加载故障分支

    string LoadExecFun(vector<ItemInfo>& Item);   //加载可执行分支

    void execFun(int index);  //执行函数

    //友员函数
    friend void split(const string& str, const string& delim, list<string>& listStr);

};


//故障子树
class errchildren : public ConfigClass
{
public:
    map<string, errBranch> _errBranchs;    //故障分支字典

public:
    errchildren() { ; }   //构造函数

    string Load(ItemInfo& item);   //加载故障子树
    string LoadErrBranchs(vector<ItemInfo>& Item);   //加载故障分支

    //友员函数
    friend void split(const string& str, const string& delim, list<string>& listStr);
};


//故障树
class errTree : public ConfigClass
{
public:
    string _path;     //故障树所在路径
    map<string, errchildren> _errChildren;
    map<string, FunClass> _funs;
    FunExecClass _funExec;

public:
    errTree(const string& path);   //构造函数
    ~errTree();  //析构函数

    string Load(const string& jsonFile);   //加载故障树

    void Show();    //显示故障树

    string LoadCfg(ItemInfo& item); //加载配置

    string LoadScript(vector<ItemInfo>& Item);    //加载脚本

    string LoadExecFun(vector<ItemInfo>& Item);    //加载可执行函数

    string LoadErrChildren(vector<ItemInfo>& Item);   //加载故障分支
    
    void execFun(int index);  //执行函数

    void execChildIndex(int childIndex);  //执行故障子树

    void execBranchIndex(int childIndex,int branchIndex);  //执行故障分支

    //友员函数
    friend void split(const string& str, const string& delim, list<string>& listStr);
};


