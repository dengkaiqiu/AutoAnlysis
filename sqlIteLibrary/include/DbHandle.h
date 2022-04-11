#pragma once
#include "sqlite3.h"
#include<iostream>
#include<list>
#include<vector>
#include<map>
#include <string>

using namespace std;

namespace sqlite
{
	//字段值
	struct keyValue
	{
		string key;
		string value;

		keyValue()
		{
			key = "";
			value = "";
		}

		keyValue(string pkey, string pvalue)
		{
			key = pkey;
			value = pvalue;
		}

		keyValue& operator=(const keyValue& b)
		{
			key = b.key;
			value = b.value;
			return *this;
		}

		bool operator<(const keyValue b) const
		{
			return this->key < b.key;
		}
	};

	//每行数据
	struct LineInfo
	{
		vector<keyValue> Element;
	};

	//每周期表数据
	struct TableInfo
	{
		map<string, string> item;  //类型
		map<string, vector<LineInfo>> Element;
	};

	//每周期内存信息
	struct MemInfo
	{
		map<string, string> item;   //类型
		map<string, vector<TableInfo>> Element; //每周期所有的表数据
	};


	class DbHandle
	{
	private:
		sqlite3 * pDB;		//据库指针
		string _DBPath;		//数据库路径
		int _runflag;       //数据库打开成功标志
		list<string> _tableList;  //数据库包含的表名列表
		string maxSeq;       //最大周期号
		string minSeq;       //最小周期号
		map<string, vector<string>> _tableHeader; //数据库表头
		  
		string QueryTableList();  //查询数据库包含的表名列表
		string QuerySeqLimit();	//查询数据库周期范围
		string QueryTableHeader();  //查询数据库表头

	public:
		DbHandle();  //构造函数
		~DbHandle(); //析构函数

		void openDB(const string &dbfile);  //打开数据库
		void closeDBConn();  //关闭数据库连接
		void BeginTransaction();  //开始事务
		void commit();            //提交
		int ExecuteNonQuery(string &sql);    //插入数据

		list<string>& getTableList(void); //获取数据库包含的列表
		string getCycleInfoFromDb(const string &cycle, const string &tablename, vector<LineInfo> &memItemInfo); //获取指定周期某个设备的内存日志
		string queryInfoFromDb(const string &tablename, vector<string> &tableHeader, vector<keyValue> &queryItem, vector<LineInfo> &memItemInfo);  //查询所需信息
	};
};