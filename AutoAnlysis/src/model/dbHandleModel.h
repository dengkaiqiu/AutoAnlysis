#pragma once
#include"DbHandle.h"
#include"FileHandle.h"
#include"JsonHandle.h"


using namespace sqlite;
using namespace fileHandle;
using namespace Json;


class DbHandleModel
{
private:
	DbHandle DBobj;   //数据库对象
	const string _path = ".\\files\\";		//默认路径

public:
	//转换成路径
	string ConvertToPath(const string& path);

	~DbHandleModel();

	//打开数据库
	string OpenDb(const string& dbfile);

	//关闭数据库
	void CloseDBConn(void);

	//开启事务
	void BeginTransaction();

	//提交
	void Commit();

	//创建表
	int CreatTable(string tableName, vector<string>& Colums);

	//插入数据
	int Insert(string& sql);

	//获取指定周期某个设备的内存日志
	string GetCycleInfoFromDb(const string& cycle, const string& tablename, vector<LineInfo>& lineInfos);

	//获取指定周期所有设备的内存信息
	string GetAllTableCycleInfoFromDb(const string& cycle, vector<TableInfo>& tableInfos);

	//获取某一周期的所有设备内存信息
	string GetCycleMemsFromDb(list<string>& cyclelist, vector<MemInfo>& memInfos);

	//序列化
	string ConvertTo(MemInfo& memInfos, ItemInfo& itemInfo);

	//查询所需信息
	string QueryInfoFromDb(const string& tablename, vector<string>& tableHeader, vector<keyValue>& queryItem, vector<LineInfo>& memItemInfo);
};