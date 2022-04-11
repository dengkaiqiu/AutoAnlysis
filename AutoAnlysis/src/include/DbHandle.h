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
	//�ֶ�ֵ
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

	//ÿ������
	struct LineInfo
	{
		vector<keyValue> Element;
	};

	//ÿ���ڱ�����
	struct TableInfo
	{
		map<string, string> item;  //����
		map<string, vector<LineInfo>> Element;
	};

	//ÿ�����ڴ���Ϣ
	struct MemInfo
	{
		map<string, string> item;   //����
		map<string, vector<TableInfo>> Element; //ÿ�������еı�����
	};


	class DbHandle
	{
	private:
		sqlite3 * pDB;		//�ݿ�ָ��
		string _DBPath;		//���ݿ�·��
		int _runflag;       //���ݿ�򿪳ɹ���־
		list<string> _tableList;  //���ݿ�����ı����б�
		string maxSeq;       //������ں�
		string minSeq;       //��С���ں�
		map<string, vector<string>> _tableHeader; //���ݿ��ͷ
		  
		string QueryTableList();  //��ѯ���ݿ�����ı����б�
		string QuerySeqLimit();	//��ѯ���ݿ����ڷ�Χ
		string QueryTableHeader();  //��ѯ���ݿ��ͷ

	public:
		DbHandle();  //���캯��
		~DbHandle(); //��������

		void openDB(const string &dbfile);  //�����ݿ�
		void closeDBConn();  //�ر����ݿ�����
		void BeginTransaction();  //��ʼ����
		void commit();            //�ύ
		int ExecuteNonQuery(string &sql);    //��������

		list<string>& getTableList(void); //��ȡ���ݿ�������б�
		string getCycleInfoFromDb(const string &cycle, const string &tablename, vector<LineInfo> &memItemInfo); //��ȡָ������ĳ���豸���ڴ���־
		string queryInfoFromDb(const string &tablename, vector<string> &tableHeader, vector<keyValue> &queryItem, vector<LineInfo> &memItemInfo);  //��ѯ������Ϣ
	};
};