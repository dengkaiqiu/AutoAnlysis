#include"JsonHandle.h"
#include"writer.h"
#include"reader.h"

using namespace Json;


typedef Json::Writer JsonWriter;
typedef Json::Reader JsonReader;
typedef Json::Value  JsonValue;
typedef JsonValue::Members JsonMembers;

//json���л���
static class JsonSerialize
{
public:
	//���л�
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

	//�������е��ֶε�MemTypeInfo
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

	//ת����Ϣ
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

	//�����л�
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

//��ȡjson�ļ�
string JsonHandle::readDataFromJson(const string& path)
{
	string err;   //���󷵻�

	try {
		string filePath = path;
		JsonReader reader;// ����json��Json::Reader   
		JsonValue root; // Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array 
		std::ifstream jsReader;

		//��ȡjson�ļ�
		jsReader.open(filePath);
		if (!reader.parse(jsReader, root, false))
		{
			throw "read json file error!!!";
		}

		//���л�json�ļ�
		err = JsonSerialize::Serialize(root, _mItem);
		jsReader.close();
	}
	catch (exception ex)
	{
		err = err.append(ex.what());
	}
	return err;
}

//��ʾ���л���Ϣ
void JsonHandle::show()
{
	JsonSerialize::analysisAll(_mItem);
}


//���л���Ϣ
void JsonHandle::ConvertTo(ItemInfo &itemInfo)
{
	JsonSerialize::SerializeInfo(_mItem, itemInfo);
}

//дjson�ļ�
void JsonHandle::writeDataToJson(const string& path, ItemInfo &itemInf)
{
	JsonValue root;
	JsonSerialize::Deserialization(itemInf, root);

	//ֱ�����
	cout << "FastWriter:" << endl;
	Json::FastWriter fw;
	cout << fw.write(root) << endl << endl;

	//�������
	cout << "StyledWriter:" << endl;
	Json::StyledWriter sw;
	cout << sw.write(root) << endl << endl;

	//������ļ�
	ofstream os;
	os.open(path);
	os << sw.write(root);
	os.close();
}