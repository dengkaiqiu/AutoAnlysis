// Copyright 2007-2010 Baptiste Lepilleur and The JsonCpp Authors
// Distributed under MIT license, or public domain if desired and
// recognized in your jurisdiction.
// See file LICENSE for detail or copy at http://jsoncpp.sourceforge.net/LICENSE

#ifndef JSON_JSONHANDLE_H_INCLUDED
#define JSON_JSONHANDLE_H_INCLUDED
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
using namespace std;

namespace Json
{
	//keyValueֵ
	class KeyValueItem
	{
	public:
		string key;
		string value;

		KeyValueItem(string pkey, string pValue)
		{
			key = pkey;
			value = pValue;
		}

		bool operator!=(KeyValueItem& p)
		{
			return key == p.key;
		}

		KeyValueItem& operator=(KeyValueItem& p)
		{
			if (*this != p)
			{
				key = p.key;
				value = p.value;
			}
			return *this;
		}
	};

	//json���л�
	class Item : public vector<Item>
	{
	public:
		vector<KeyValueItem> _itemInfo;
		string _arrayName;
	};

	//json�����л��ṹ
	class ItemInfo
	{
	public:
		map<string, string> _itemMapinfo;
		map<string, vector<ItemInfo>> _subItemMapinfo;

	};

	//json������
	class JsonHandle
	{

	private:
		Item _mItem;
	public:

		//��ȡjson�ļ�
		string readDataFromJson(const string& path);

		//��ʾ���л���Ϣ
		void show();

		//���л���Ϣ
		void ConvertTo(ItemInfo& itemInfo);

		//дjson�ļ�
		void writeDataToJson(const string& path, ItemInfo& itemInf);
	};
}

#endif
