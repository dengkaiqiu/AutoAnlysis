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
	//keyValue值
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

	//json序列化
	class Item : public vector<Item>
	{
	public:
		vector<KeyValueItem> _itemInfo;
		string _arrayName;
	};

	//json反序列化结构
	class ItemInfo
	{
	public:
		map<string, string> _itemMapinfo;
		map<string, vector<ItemInfo>> _subItemMapinfo;

	};

	//json操作类
	class JsonHandle
	{

	private:
		Item _mItem;
	public:

		//读取json文件
		string readDataFromJson(const string& path);

		//显示序列化信息
		void show();

		//序列化信息
		void ConvertTo(ItemInfo& itemInfo);

		//写json文件
		void writeDataToJson(const string& path, ItemInfo& itemInf);
	};
}

#endif
