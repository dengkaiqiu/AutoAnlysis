#pragma once
#include <string>
#include <iostream>
#include <map>
#include <list>
#include "FileHandle.h"
#include "jsonHandleModel.h"


using namespace std;

//������
class FunClass
{
public:
    string _funName;
    string _requireParam;
    string _returnParam;

public:

    FunClass() { ; }

    //���캯��
    FunClass(const string& funName, const string& requireParam, const string& returnParam)
    {
        _funName = funName;
        _requireParam = requireParam;
        _returnParam = returnParam;
    }
};

//������
class ConfigClass
{
public:
    map<string, string> _cfgItem;

public:
    //���캯��
    ConfigClass() { ; }

    //��������
    ~ConfigClass() {
        _cfgItem.clear();
    }

    //��������ֵ
    void setConfigValue(string key, string value);

    //��ȡ����ֵ
    string getConfigValue(string key);

};

//����ִ����
class FunExecClass
{
    friend class errBranch;
    friend class errTree;

public:
    void funExec(FunClass& funobj, ConfigClass& cfgobj);    //����ִ����
};

//���Ϸ�֧
class errBranch : public ConfigClass
{
public:
    map<string, FunClass> _funs;
    FunExecClass _funExec;

public:
    errBranch() { ; }   //���캯��

    string Load(ItemInfo& item);   //���ع��Ϸ�֧

    string LoadExecFun(vector<ItemInfo>& Item);   //���ؿ�ִ�з�֧

    void execFun(int index);  //ִ�к���

    //��Ա����
    friend void split(const string& str, const string& delim, list<string>& listStr);

};


//��������
class errchildren : public ConfigClass
{
public:
    map<string, errBranch> _errBranchs;    //���Ϸ�֧�ֵ�

public:
    errchildren() { ; }   //���캯��

    string Load(ItemInfo& item);   //���ع�������
    string LoadErrBranchs(vector<ItemInfo>& Item);   //���ع��Ϸ�֧

    //��Ա����
    friend void split(const string& str, const string& delim, list<string>& listStr);
};


//������
class errTree : public ConfigClass
{
public:
    string _path;     //����������·��
    map<string, errchildren> _errChildren;
    map<string, FunClass> _funs;
    FunExecClass _funExec;

public:
    errTree(const string& path);   //���캯��
    ~errTree();  //��������

    string Load(const string& jsonFile);   //���ع�����

    void Show();    //��ʾ������

    string LoadCfg(ItemInfo& item); //��������

    string LoadScript(vector<ItemInfo>& Item);    //���ؽű�

    string LoadExecFun(vector<ItemInfo>& Item);    //���ؿ�ִ�к���

    string LoadErrChildren(vector<ItemInfo>& Item);   //���ع��Ϸ�֧
    
    void execFun(int index);  //ִ�к���

    void execChildIndex(int childIndex);  //ִ�й�������

    void execBranchIndex(int childIndex,int branchIndex);  //ִ�й��Ϸ�֧

    //��Ա����
    friend void split(const string& str, const string& delim, list<string>& listStr);
};


