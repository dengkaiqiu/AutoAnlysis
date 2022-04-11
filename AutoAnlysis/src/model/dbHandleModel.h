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
	DbHandle DBobj;   //���ݿ����
	const string _path = ".\\files\\";		//Ĭ��·��

public:
	//ת����·��
	string ConvertToPath(const string& path);

	~DbHandleModel();

	//�����ݿ�
	string OpenDb(const string& dbfile);

	//�ر����ݿ�
	void CloseDBConn(void);

	//��������
	void BeginTransaction();

	//�ύ
	void Commit();

	//������
	int CreatTable(string tableName, vector<string>& Colums);

	//��������
	int Insert(string& sql);

	//��ȡָ������ĳ���豸���ڴ���־
	string GetCycleInfoFromDb(const string& cycle, const string& tablename, vector<LineInfo>& lineInfos);

	//��ȡָ�����������豸���ڴ���Ϣ
	string GetAllTableCycleInfoFromDb(const string& cycle, vector<TableInfo>& tableInfos);

	//��ȡĳһ���ڵ������豸�ڴ���Ϣ
	string GetCycleMemsFromDb(list<string>& cyclelist, vector<MemInfo>& memInfos);

	//���л�
	string ConvertTo(MemInfo& memInfos, ItemInfo& itemInfo);

	//��ѯ������Ϣ
	string QueryInfoFromDb(const string& tablename, vector<string>& tableHeader, vector<keyValue>& queryItem, vector<LineInfo>& memItemInfo);
};