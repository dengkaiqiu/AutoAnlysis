#include "errorTreeModel.h"
#include "luaModel.h"




//拆分条件
void split(const string& str, const string& delim, vector<string>& listStr)
{
    if ("" == str) return;
    //先将要切割的字符串从string类型转换为char*类型  
    char* strs = new char[str.length() + 1]; //不要忘了  
    strcpy(strs, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char* p = strtok(strs, d);
    while (p) {
        string s = p; //分割得到的字符串转换为string类型  
        listStr.push_back(s); //存入结果数组  
        p = strtok(NULL, d);
    }
}

void ConfigClass::setConfigValue(string key, string value)
{
    if (_cfgItem.count(key) == 0) {
        _cfgItem.insert(pair<string, string>(key, value));
    }
    else
    {
        _cfgItem[key] = value;
    }
}

string ConfigClass::getConfigValue(string key)
{
    if (_cfgItem.count(key) != 0) {
        return _cfgItem[key];
    }
    else {
        return "";
    }
}

void FunExecClass::funExec(FunClass& funobj, ConfigClass& cfgobj)
{
    vector<string> pushparam;

    split(funobj._requireParam, ",", pushparam);
    LoadLuaFuntion(funobj._funName, pushparam, cfgobj);
}

//构造函数
errTree::errTree(const string& path)
{
	_path = path;
}

//析构函数
errTree::~errTree()
{
    _errChildren.clear();
    _funs.clear();
}

/*==================================================故障分支====================================================*/
string errBranch::Load(ItemInfo& item)
{
    string errmsg = "";

    //加载stringValue配置值
    for (auto it = item._itemMapinfo.begin(); it != item._itemMapinfo.end(); it++)
    {
        pair<string, string> temp = (*it);

        if (temp.first == "staticConfigParam" || temp.first == "FlexibleConfigParam")
        {
            setConfigValue(temp.first, temp.second);

            vector<string> params;
            split(temp.second, ",", params);

            for (vector<string>::iterator itt = params.begin(); itt != params.end(); itt++)
            {
                setConfigValue((*itt), "");
            }
        }
        else
        {
            setConfigValue(temp.first, temp.second);
        }
    }

    //加载非stringValue配置值
    for (auto it = item._subItemMapinfo.begin(); it != item._subItemMapinfo.end(); it++)
    {
        pair<string, vector<ItemInfo>> temp = (*it);

        if (temp.first == "execFun")
        {
            errmsg = LoadExecFun(temp.second);
        }
        else
        {
            ;
        }

        if (errmsg.length() != 0) break;
    }

    return errmsg;

}

//加载可执行函数
string errBranch::LoadExecFun(vector<ItemInfo>& Item)
{
    string errmsg = "";
    for (auto it = Item.begin(); it != Item.end(); it++)
    {
        //加载配置值
        if ((*it)._itemMapinfo["index"].length() == 0) {
            errmsg = errmsg + "execFun:index not configured";
            break;
        }

        if ((*it)._itemMapinfo["funName"].length() == 0) {
            errmsg = errmsg + "execFun:funName not configured";
            break;
        }

        if ((*it)._itemMapinfo["requireParam"].length() == 0) {
            errmsg = errmsg + "execFun:requireParam not configured";
            break;
        }

        if ((*it)._itemMapinfo["returnParam"].length() == 0) {
            errmsg = errmsg + "execFun:returnParam not configured";
            break;
        }

        FunClass temp((*it)._itemMapinfo["funName"], (*it)._itemMapinfo["requireParam"], (*it)._itemMapinfo["returnParam"]);

        _funs.insert(pair<string, FunClass>((*it)._itemMapinfo["index"], temp));
    }

    return errmsg;
}

//执行函数
void errBranch::execFun(int index)
{
    string funIndex = to_string(index);

    if (_funs.count(funIndex) != 0)
        _funExec.funExec(_funs[funIndex], *this);
}

/*==================================================故障子树====================================================*/
//加载故障子树
string errchildren::Load(ItemInfo& item)
{
    string errmsg = "";

    //加载stringValue配置值
    for (auto it = item._itemMapinfo.begin(); it != item._itemMapinfo.end(); it++)
    {
        pair<string, string> temp = (*it);

        if (temp.first == "staticConfigParam" || temp.first == "FlexibleConfigParam")
        {
            setConfigValue(temp.first, temp.second);

            vector<string> params;
            split(temp.second, ",", params);

            for (vector<string>::iterator itt = params.begin(); itt != params.end(); itt++)
            {
                setConfigValue((*itt), "");
            }
        }
        else
        {
            setConfigValue(temp.first, temp.second);
        }
    }

    //加载非stringValue配置值
    for (auto it = item._subItemMapinfo.begin(); it != item._subItemMapinfo.end(); it++)
    {
        pair<string, vector<ItemInfo>> temp = (*it);

        if (temp.first == "errBranchs")
        {
            errmsg = LoadErrBranchs(temp.second);
        }
        else
        {
            ;
        }

        if (errmsg.length() != 0) break;
    }

    return errmsg;
}

//加载故障分支
string errchildren::LoadErrBranchs(vector<ItemInfo>& Item)
{
    string errmsg = "";
    for (auto it = Item.begin(); it != Item.end(); it++)
    {
        if ((*it)._itemMapinfo["index"].length() == 0) {
            errmsg = errmsg + "ErrBranchs:index not configured";
            break;
        }

        errBranch obj;
        errmsg = obj.Load((*it));

        if (errmsg.length() != 0) break;

        _errBranchs.insert(pair<string, errBranch>((*it)._itemMapinfo["index"], obj));
    }

    return errmsg;
}

/*==================================================故障树====================================================*/
//加载故障树
string errTree::Load(const string& jsonFile)
{
	string errmsg = "";
	string fileName;
	
	fileName.append(_path).append("\\files\\").append(jsonFile);

	if (!FileHandle::exists_file(fileName))     //判断Json文件是否存在
		return fileName + ",not exit.";

	JsonHandleModel jsonHandleObj;  //json操作对象

    errmsg = jsonHandleObj.readDataFromJson(fileName);   //读取

	if (errmsg.length() == 0) {
		//序列化
		ItemInfo ErrorTreeInfo;
		jsonHandleObj.ConvertTo(ErrorTreeInfo);

        //加载配置值
        errmsg = LoadCfg(ErrorTreeInfo);

	}

	return errmsg;
}

//显示故障树
void errTree::Show()
{
    cout << getConfigValue("Description") << endl;
    cout << "\t" << "|" << endl;

    for (auto it = _errChildren.begin(); it !=  _errChildren.end(); it++)
    {
        cout << "\t" << " _" << "故障子树" << (*it).second.getConfigValue("index")<< ":" << (*it).second.getConfigValue("Description")  << endl;
        cout << "\t" << "\t" << "|" << endl;
       
        errchildren& chiletemp = (*it).second;

        for (auto itt = chiletemp._errBranchs.begin(); itt != chiletemp._errBranchs.end(); itt++)
        {
            cout << "\t" << "\t"  << " _" << "故障分支" << (*itt).second.getConfigValue("index") << ":" << (*itt).second.getConfigValue("Description") << endl;
            cout << "\t" << "\t" << "|" << endl;
        }
        cout << "\t" << "|" << endl;
    }
    
}

//加载配置
string errTree::LoadCfg(ItemInfo& item)
{
    string errmsg;

    //加载stringValue配置值
    for (auto it = item._itemMapinfo.begin(); it != item._itemMapinfo.end(); it++)
    {
        pair<string, string> temp = (*it);

        if (temp.first == "staticConfigParam" || temp.first == "FlexibleConfigParam")
        {
            setConfigValue(temp.first, temp.second);

            vector<string> params;
            split(temp.second, ",", params);

            for (vector<string>::iterator itt = params.begin(); itt != params.end(); itt++)
            {
                setConfigValue((*itt), "");
            }
        }
        else
        {
            setConfigValue(temp.first, temp.second);
        }
    }

    //加载非stringValue配置值
    for (auto it = item._subItemMapinfo.begin(); it != item._subItemMapinfo.end(); it++)
    {
        pair<string, vector<ItemInfo>> temp = (*it);

        if (temp.first == "loadScript")
        {
            errmsg = LoadScript(temp.second);
        }
        else if (temp.first == "execFun")
        {
            errmsg = LoadExecFun(temp.second);
        }
        else if (temp.first == "errChildren")
        {
            errmsg = LoadErrChildren(temp.second);
        }
        else
        {
            ;
        }

        if (errmsg.length() != 0) break;
    }

    return errmsg;
}

//加载主脚本
string errTree::LoadScript(vector<ItemInfo>& Item)
{
    string errmsg;

    if (Item.size() == 1)
    {
        for (auto it = Item.begin(); it != Item.end(); it++)
        {
            //加载配置值
            for (auto itt = (*it)._itemMapinfo.begin(); itt != (*it)._itemMapinfo.end(); itt++)
            {
                pair<string, string> temp = (*itt);
                setConfigValue(temp.first, temp.second);
            }
        }

        if (getConfigValue("loadScriptType") == "Lua")
        {
            string scriptpath = _path + getConfigValue("loadpath") + getConfigValue("loadName");
            //加载脚本
            LoadLua(scriptpath);
        }
        else
        {
            errmsg = getConfigValue("loadScriptType") + " is not supported";
        }
    }
    else
    {
        errmsg = "The script is not configured or the configuration exceeds the limit";
    }
    return errmsg;
}

//加载可执行函数
string errTree::LoadExecFun(vector<ItemInfo>& Item)
{
    string errmsg = "";
    for (auto it = Item.begin(); it != Item.end(); it++)
    {
        //加载配置值
        if ((*it)._itemMapinfo["index"].length() == 0) {
            errmsg = errmsg + "execFun:index not configured";
            break;
        }

        if ((*it)._itemMapinfo["funName"].length() == 0) {
            errmsg = errmsg + "execFun:funName not configured";
            break;
        }

        if ((*it)._itemMapinfo["requireParam"].length() == 0) {
            errmsg = errmsg + "execFun:requireParam not configured";
            break;
        }

        if ((*it)._itemMapinfo["returnParam"].length() == 0) {
            errmsg = errmsg + "execFun:returnParam not configured";
            break;
        }

        FunClass temp((*it)._itemMapinfo["funName"], (*it)._itemMapinfo["requireParam"], (*it)._itemMapinfo["returnParam"]);

        _funs.insert(pair<string, FunClass>((*it)._itemMapinfo["index"], temp));
    }

    return errmsg;
}

//加载故障子树
string errTree::LoadErrChildren(vector<ItemInfo>& Item)
{
    string errmsg = "";
    for(auto it = Item.begin(); it != Item.end(); it++)
    {
        if ((*it)._itemMapinfo["index"].length() == 0) {
            errmsg = errmsg + "errChildren:index not configured";
            break;
        }

        errchildren obj;
        errmsg = obj.Load((*it));

        if (errmsg.length() != 0) break;

        _errChildren.insert(pair<string, errchildren>((*it)._itemMapinfo["index"], obj));
    }

	return errmsg;
}

//执行函数
void errTree::execFun(int index)
{
    string funIndex = to_string(index);

    if (_funs.count(funIndex) != 0)
        _funExec.funExec(_funs[funIndex], *this);
}

//执行故障子树
void errTree::execChildIndex(int index)
{
    string childstr = to_string(index);
    if (_errChildren.count(childstr) != 0)
    {
        vector<string> need;
        split(_errChildren[childstr].getConfigValue("FlexibleConfigParam"), ",", need);

        //故障子树需要的灵活变量进行赋值
        for (auto it = need.begin(); it != need.end(); it++)
        {
            _errChildren[childstr].setConfigValue((*it), getConfigValue(*it));
        }

        errchildren& childtemp = _errChildren[childstr];

        //遍历故障子树的故障分支
        for (auto it = childtemp._errBranchs.begin(); it != childtemp._errBranchs.end(); it++)
        {
            need.clear();
            split((*it).second.getConfigValue("FlexibleConfigParam"), ",", need);

            //故障分支需要的灵活变量进行赋值
            for (auto itt = need.begin(); itt != need.end(); itt++)
            {
                (*it).second.setConfigValue((*itt), getConfigValue(*itt));
            }

            errBranch& branchtemp = (*it).second;

            //遍历故障分支的所有处理函数
            for (auto itt = branchtemp._funs.begin(); itt != branchtemp._funs.end(); itt++)
            {
                //执行函数
                branchtemp._funExec.funExec((*itt).second, branchtemp);
            }
        }
    }
    else
    {
        cout << "not exit errchid index:" << childstr << endl;
    }
}

//执行故障分支
void errTree::execBranchIndex(int childIndex, int branchIndex)
{
    string childstr = to_string(childIndex);
    string branchstr = to_string(branchIndex);

    if (_errChildren.count(childstr) != 0)
    {
        vector<string> need;
        split(_errChildren[childstr].getConfigValue("FlexibleConfigParam"), ",", need);

        //故障子树需要的灵活变量进行赋值
        for (auto it = need.begin(); it != need.end(); it++)
        {
            _errChildren[childstr].setConfigValue((*it), getConfigValue(*it));
        }

        errchildren& childtemp = _errChildren[childstr];

        //遍历故障子树的故障分支
        if (childtemp._errBranchs.count(branchstr) != 0)
        {
            errBranch& branchtemp = childtemp._errBranchs[branchstr];
            need.clear();
            split(branchtemp.getConfigValue("FlexibleConfigParam"), ",", need);

            //故障分支需要的灵活变量进行赋值
            for (auto itt = need.begin(); itt != need.end(); itt++)
            {
                branchtemp.setConfigValue((*itt), getConfigValue(*itt));
            }

            //遍历故障分支的所有处理函数
            for (auto itt = branchtemp._funs.begin(); itt != branchtemp._funs.end(); itt++)
            {
                //执行函数
                branchtemp._funExec.funExec((*itt).second, branchtemp);
            }
        }
        else
        {
            cout << "not exit errbranch index:" << branchstr << endl;
        }
    }
    else
    {
        cout << "not exit errchid index:" << childstr << endl;
    }
}