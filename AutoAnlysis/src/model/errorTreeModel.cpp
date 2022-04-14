#include "errorTreeModel.h"
#include "luaModel.h"




//�������
void split(const string& str, const string& delim, vector<string>& listStr)
{
    if ("" == str) return;
    //�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
    char* strs = new char[str.length() + 1]; //��Ҫ����  
    strcpy(strs, str.c_str());

    char* d = new char[delim.length() + 1];
    strcpy(d, delim.c_str());

    char* p = strtok(strs, d);
    while (p) {
        string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
        listStr.push_back(s); //����������  
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

//���캯��
errTree::errTree(const string& path)
{
	_path = path;
}

//��������
errTree::~errTree()
{
    _errChildren.clear();
    _funs.clear();
}

/*==================================================���Ϸ�֧====================================================*/
string errBranch::Load(ItemInfo& item)
{
    string errmsg = "";

    //����stringValue����ֵ
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

    //���ط�stringValue����ֵ
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

//���ؿ�ִ�к���
string errBranch::LoadExecFun(vector<ItemInfo>& Item)
{
    string errmsg = "";
    for (auto it = Item.begin(); it != Item.end(); it++)
    {
        //��������ֵ
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

//ִ�к���
void errBranch::execFun(int index)
{
    string funIndex = to_string(index);

    if (_funs.count(funIndex) != 0)
        _funExec.funExec(_funs[funIndex], *this);
}

/*==================================================��������====================================================*/
//���ع�������
string errchildren::Load(ItemInfo& item)
{
    string errmsg = "";

    //����stringValue����ֵ
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

    //���ط�stringValue����ֵ
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

//���ع��Ϸ�֧
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

/*==================================================������====================================================*/
//���ع�����
string errTree::Load(const string& jsonFile)
{
	string errmsg = "";
	string fileName;
	
	fileName.append(_path).append("\\files\\").append(jsonFile);

	if (!FileHandle::exists_file(fileName))     //�ж�Json�ļ��Ƿ����
		return fileName + ",not exit.";

	JsonHandleModel jsonHandleObj;  //json��������

    errmsg = jsonHandleObj.readDataFromJson(fileName);   //��ȡ

	if (errmsg.length() == 0) {
		//���л�
		ItemInfo ErrorTreeInfo;
		jsonHandleObj.ConvertTo(ErrorTreeInfo);

        //��������ֵ
        errmsg = LoadCfg(ErrorTreeInfo);

	}

	return errmsg;
}

//��ʾ������
void errTree::Show()
{
    cout << getConfigValue("Description") << endl;
    cout << "\t" << "|" << endl;

    for (auto it = _errChildren.begin(); it !=  _errChildren.end(); it++)
    {
        cout << "\t" << " _" << "��������" << (*it).second.getConfigValue("index")<< ":" << (*it).second.getConfigValue("Description")  << endl;
        cout << "\t" << "\t" << "|" << endl;
       
        errchildren& chiletemp = (*it).second;

        for (auto itt = chiletemp._errBranchs.begin(); itt != chiletemp._errBranchs.end(); itt++)
        {
            cout << "\t" << "\t"  << " _" << "���Ϸ�֧" << (*itt).second.getConfigValue("index") << ":" << (*itt).second.getConfigValue("Description") << endl;
            cout << "\t" << "\t" << "|" << endl;
        }
        cout << "\t" << "|" << endl;
    }
    
}

//��������
string errTree::LoadCfg(ItemInfo& item)
{
    string errmsg;

    //����stringValue����ֵ
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

    //���ط�stringValue����ֵ
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

//�������ű�
string errTree::LoadScript(vector<ItemInfo>& Item)
{
    string errmsg;

    if (Item.size() == 1)
    {
        for (auto it = Item.begin(); it != Item.end(); it++)
        {
            //��������ֵ
            for (auto itt = (*it)._itemMapinfo.begin(); itt != (*it)._itemMapinfo.end(); itt++)
            {
                pair<string, string> temp = (*itt);
                setConfigValue(temp.first, temp.second);
            }
        }

        if (getConfigValue("loadScriptType") == "Lua")
        {
            string scriptpath = _path + getConfigValue("loadpath") + getConfigValue("loadName");
            //���ؽű�
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

//���ؿ�ִ�к���
string errTree::LoadExecFun(vector<ItemInfo>& Item)
{
    string errmsg = "";
    for (auto it = Item.begin(); it != Item.end(); it++)
    {
        //��������ֵ
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

//���ع�������
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

//ִ�к���
void errTree::execFun(int index)
{
    string funIndex = to_string(index);

    if (_funs.count(funIndex) != 0)
        _funExec.funExec(_funs[funIndex], *this);
}

//ִ�й�������
void errTree::execChildIndex(int index)
{
    string childstr = to_string(index);
    if (_errChildren.count(childstr) != 0)
    {
        vector<string> need;
        split(_errChildren[childstr].getConfigValue("FlexibleConfigParam"), ",", need);

        //����������Ҫ�����������и�ֵ
        for (auto it = need.begin(); it != need.end(); it++)
        {
            _errChildren[childstr].setConfigValue((*it), getConfigValue(*it));
        }

        errchildren& childtemp = _errChildren[childstr];

        //�������������Ĺ��Ϸ�֧
        for (auto it = childtemp._errBranchs.begin(); it != childtemp._errBranchs.end(); it++)
        {
            need.clear();
            split((*it).second.getConfigValue("FlexibleConfigParam"), ",", need);

            //���Ϸ�֧��Ҫ�����������и�ֵ
            for (auto itt = need.begin(); itt != need.end(); itt++)
            {
                (*it).second.setConfigValue((*itt), getConfigValue(*itt));
            }

            errBranch& branchtemp = (*it).second;

            //�������Ϸ�֧�����д�����
            for (auto itt = branchtemp._funs.begin(); itt != branchtemp._funs.end(); itt++)
            {
                //ִ�к���
                branchtemp._funExec.funExec((*itt).second, branchtemp);
            }
        }
    }
    else
    {
        cout << "not exit errchid index:" << childstr << endl;
    }
}

//ִ�й��Ϸ�֧
void errTree::execBranchIndex(int childIndex, int branchIndex)
{
    string childstr = to_string(childIndex);
    string branchstr = to_string(branchIndex);

    if (_errChildren.count(childstr) != 0)
    {
        vector<string> need;
        split(_errChildren[childstr].getConfigValue("FlexibleConfigParam"), ",", need);

        //����������Ҫ�����������и�ֵ
        for (auto it = need.begin(); it != need.end(); it++)
        {
            _errChildren[childstr].setConfigValue((*it), getConfigValue(*it));
        }

        errchildren& childtemp = _errChildren[childstr];

        //�������������Ĺ��Ϸ�֧
        if (childtemp._errBranchs.count(branchstr) != 0)
        {
            errBranch& branchtemp = childtemp._errBranchs[branchstr];
            need.clear();
            split(branchtemp.getConfigValue("FlexibleConfigParam"), ",", need);

            //���Ϸ�֧��Ҫ�����������и�ֵ
            for (auto itt = need.begin(); itt != need.end(); itt++)
            {
                branchtemp.setConfigValue((*itt), getConfigValue(*itt));
            }

            //�������Ϸ�֧�����д�����
            for (auto itt = branchtemp._funs.begin(); itt != branchtemp._funs.end(); itt++)
            {
                //ִ�к���
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