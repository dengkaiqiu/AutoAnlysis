#include"JsonHandle.h"
#include"writer.h"
#include"reader.h"

using namespace Json;


typedef Json::Writer JsonWriter;
typedef Json::Reader JsonReader;
typedef Json::Value  JsonValue;
typedef JsonValue::Members JsonMembers;

//json序列化类
static class JsonSerialize
{
public:
	//序列化
	static string Serialize(JsonValue root, Item& item)
	{
		string err;
		JsonMembers mem = root.getMemberNames();
		for (auto iter = mem.begin(); iter != mem.end(); iter++)
		{
			if (root[*iter].type() == Json::stringValue)
			{
				KeyValueItem keyVale(*iter, root[*iter].asString());
				item._itemInfo.push_back(keyVale);
			}
			else if (root[*iter].type() == Json::arrayValue)
			{
				auto cnt = root[*iter].size();
				for (auto i = 0; i < cnt; i++)
				{
					Item subItem;
					subItem._arrayName = *iter;
					Serialize(root[*iter][i], subItem);
					item.push_back(subItem);
				}
			}
			else if (root[*iter].type() == Json::objectValue)
			{
				Item subItem;
				subItem._arrayName = *iter;
				Serialize(root[*iter], subItem);
				item.push_back(subItem);
			}
			else
			{
				err = "getMemItemFromJson error.\n";
				break;
			}
		}
		return err;
	}

	//解析所有的字段到MemTypeInfo
	static void analysisAll(Item& item)
	{
		if(item._arrayName.length() > 0)
			cout << item._arrayName << ":" << endl;

		for (vector<KeyValueItem>::iterator it = item._itemInfo.begin(); it < item._itemInfo.end(); it++)
		{
			cout << (*it).key << ":" << (*it).value << endl;
		}

		for (vector<Item>::iterator it = item.begin(); it < item.end(); it++)
		{
			analysisAll(*it);
		}
	}

	//转换信息
	static void SerializeInfo(Item &item, ItemInfo &itemInfo)
	{
		for (vector<KeyValueItem>::iterator it = item._itemInfo.begin(); it < item._itemInfo.end(); it++)
		{
			itemInfo._itemMapinfo.insert(pair<string, string>((*it).key, (*it).value));
		}

		if (item.size() > 0)
		{
			vector<Item>::iterator temp = item.begin();
			string str = (*temp)._arrayName;
			vector<ItemInfo> vectorItem;

			for (vector<Item>::iterator it = item.begin(); it < item.end(); it++)
			{
				ItemInfo tempitemInfo;
				SerializeInfo(*it, tempitemInfo); 
				string str = (*it)._arrayName;

				if (itemInfo._subItemMapinfo.count(str) == 0)
				{
					vectorItem.clear();
					vectorItem.push_back(tempitemInfo);
					itemInfo._subItemMapinfo.insert(pair<string, vector<ItemInfo>>(str, vectorItem));
				}
				else
				{
					itemInfo._subItemMapinfo[(*it)._arrayName].push_back(tempitemInfo);
				}
			}
		}
	}

	//反序列化
	static void Deserialization(ItemInfo &itemInf, JsonValue &root)
	{
		for (auto it = itemInf._itemMapinfo.begin(); it != itemInf._itemMapinfo.end(); it++)
		{
			pair<string, string> keyvlue = *it;
			root[keyvlue.first] = keyvlue.second;

		}

		for (auto it = itemInf._subItemMapinfo.begin(); it != itemInf._subItemMapinfo.end(); it++)
		{
			pair<string, vector<ItemInfo>> subItem = *it;

			for (vector<ItemInfo>::iterator itt = subItem.second.begin(); itt < subItem.second.end(); itt++)
			{
				JsonValue tempJsonVlue;
				Deserialization(*itt, tempJsonVlue);
				root[subItem.first].append(tempJsonVlue);

			}
		}
	}
};

//读取json文件
string JsonHandle::readDataFromJson(const string& path)
{
	string err;   //错误返回

	try {
		string filePath = path;
		JsonReader reader;// 解析json用Json::Reader   
		JsonValue root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array 
		std::ifstream jsReader;

		//读取json文件
		jsReader.open(filePath);
		if (!reader.parse(jsReader, root, false))
		{
			throw "read json file error!!!";
		}

		//序列化json文件
		err = JsonSerialize::Serialize(root, _mItem);
		jsReader.close();
	}
	catch (exception ex)
	{
		err = err.append(ex.what());
	}
	return err;
}

//显示序列化信息
void JsonHandle::show()
{
	JsonSerialize::analysisAll(_mItem);
}


//序列化信息
void JsonHandle::ConvertTo(ItemInfo &itemInfo)
{
	JsonSerialize::SerializeInfo(_mItem, itemInfo);
}

//写json文件
void JsonHandle::writeDataToJson(const string& path, ItemInfo &itemInf)
{
	JsonValue root;
	JsonSerialize::Deserialization(itemInf, root);

	//直接输出
	cout << "FastWriter:" << endl;
	Json::FastWriter fw;
	cout << fw.write(root) << endl << endl;

	//缩进输出
	cout << "StyledWriter:" << endl;
	Json::StyledWriter sw;
	cout << sw.write(root) << endl << endl;

	//输出到文件
	ofstream os;
	os.open(path);
	os << sw.write(root);
	os.close();
}