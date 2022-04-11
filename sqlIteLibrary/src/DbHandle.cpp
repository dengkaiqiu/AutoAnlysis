#include "DbHandle.h"
#include <windows.h>
#include <locale>
#include <codecvt>

using namespace sqlite;

//#pragma comment(lib,"sqlite3.lib")

static class StringConvert
{
public:
	//wstringתstring
	static string WStringToString(const wstring& str)
	{
		int	iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		std::vector<char> vecText(iTextLen, '\0');
		::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &(vecText[0]), iTextLen, NULL, NULL);
		std::string strText = &(vecText[0]);

		return strText;
	}

	//stringתwstring
	static wstring StringToWsting(const string& str)
	{
		int iTextLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		std::vector<wchar_t> vecText(iTextLen, '\0');
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &(vecText[0]), iTextLen);
		std::wstring result = &(vecText[0]);
		return result;
	}

	//wstringתstring
	static string WStringToUTF8(const wstring& str)
	{
		int	iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		std::vector<char> vecText(iTextLen, '\0');
		::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, &(vecText[0]), iTextLen, NULL, NULL);
		std::string strText = &(vecText[0]);

		return strText;
	}

	//UTF8��ʽתstring��ʽ
	static string UTF8ToString(const string & utf8Data)
	{
		//�Ƚ�UTF-8ת����Unicode
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		std::wstring wString = conv.from_bytes(utf8Data);
		//��ת����string
		return WStringToString(wString);
	}

	//string��ʽתUTF8��ʽ
	static string StringToUTF8(const string & stringData)
	{
		//stringתwstring
		std::wstring wString = StringToWsting(stringData);

		//��ת����string
		return WStringToUTF8(wString);
	}

	//string��ʽתlong long 
	static long long StringToLLong(const string &string, int base)
	{
		return strtoll(string.c_str(), nullptr, base);
		//return _strtoi64(string.c_str(), nullptr, base);
	}

	//string��ʽתlong int
	static long int StringToLong(const string &string, int base)
	{
		return strtol(string.c_str(), nullptr, base);
	}
};

typedef StringConvert CmConvert;


//�ص�����
int callback(void*para, int nCount, char** pValue, char** pName)
{
	if (nullptr != para)
	{
		vector<LineInfo> *pTemp = (vector<LineInfo>*)para;
		LineInfo tempKeyValue;
		for (int i = 0; i < nCount; i++) {
			tempKeyValue.Element.push_back(keyValue(CmConvert::UTF8ToString(pName[i]), CmConvert::UTF8ToString(pValue[i])));
		}
		pTemp->push_back(tempKeyValue);
		
	}
	return SQLITE_OK;
}

//��ѯ���ݿ�����ı����б�
string DbHandle::QueryTableList()
{
	string err;
	sqlite3_stmt *stmt = NULL;
	string sql = "SELECT name FROM sqlite_sequence";
	int excflag = SQLITE_ERROR;

	if (_runflag == SQLITE_OK)
		excflag = sqlite3_prepare_v2(pDB, sql.c_str(), -1, &stmt, 0);

	if (excflag == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			_tableList.push_back((char *)sqlite3_column_text(stmt, 0));
		}
	}
	else
	{
		err = "query table name error!!!\n";
	}
	return err;
}

//��ѯ���ݿ����ڷ�Χ
string DbHandle::QuerySeqLimit()
{
	string err;
	sqlite3_stmt *stmt = NULL;
	int excflag = SQLITE_ERROR;

	string sql = "SELECT MIN(";
	sql.append(CmConvert::StringToUTF8("���к�"));
	sql.append(") as minseq, MAX(");
	sql.append(CmConvert::StringToUTF8("���к�"));
	sql.append(") as maxSeq FROM zcsetting");

	if (_runflag == SQLITE_OK)
		excflag = sqlite3_prepare_v2(pDB, sql.c_str(), -1, &stmt, 0);

	if (excflag == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			minSeq = (char *)sqlite3_column_text(stmt, 0);
			maxSeq = (char *)sqlite3_column_text(stmt, 1);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		err = "query maxmin Seq error!!!\n";
	}
	return err;
}

//��ȡ���ݿ��ͷ
string DbHandle::QueryTableHeader()
{
	string err;
	sqlite3_stmt *stmt = NULL;
	int excflag = SQLITE_ERROR;

	for (list<string>::iterator it = _tableList.begin(); it != _tableList.end();it++)
	{
		string sql = "PRAGMA table_info('";
		sql.append(*it);
		sql.append("')");

		if (_runflag == SQLITE_OK)
			excflag = sqlite3_prepare_v2(pDB, sql.c_str(), -1, &stmt, 0);

		if (excflag == SQLITE_OK)
		{
			vector<string> temp;
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				temp.push_back(CmConvert::UTF8ToString((char *)sqlite3_column_text(stmt, 1)));
			}
			_tableHeader.insert(pair<string, vector<string>>(*it, temp));
		}
		else
		{
			err = "query table Head error!!!\n";
		}
	}
	return err;
}


//���캯��
DbHandle::DbHandle()
{
	pDB = nullptr;
	_DBPath = "";
	_runflag = SQLITE_ERROR;
	_tableList.clear();
	_tableHeader.clear();
}

//��������
DbHandle::~DbHandle()
{
	closeDBConn();
	_tableList.clear();
	_tableHeader.clear();
}

//�����ݿ�
void DbHandle::openDB(const string &dbfile)
{
	if (_runflag == SQLITE_ERROR && !dbfile.empty())
		_runflag = sqlite3_open(CmConvert::StringToUTF8(dbfile.c_str()).c_str(), &this->pDB);

	_tableList.clear();
	if (_runflag == SQLITE_OK)
	{
		QueryTableList();
		QuerySeqLimit();
		QueryTableHeader();
		_DBPath = dbfile;
	}
}

//�ر����ݿ�����
void DbHandle::closeDBConn()
{
	if (_runflag == SQLITE_OK)
		sqlite3_close(this->pDB);
	_runflag = SQLITE_ERROR;
}

//��������
void DbHandle::BeginTransaction()
{
	if (_runflag == SQLITE_OK)
		sqlite3_exec(this->pDB, "begin;",0,0,0);
}

//�ύ
void DbHandle::commit()
{
	if (_runflag == SQLITE_OK)
		sqlite3_exec(this->pDB, "commit;", 0, 0, 0);
}

//��������
int DbHandle::ExecuteNonQuery(string &sql)
{
	char *errMsg = NULL;
	if (_runflag == SQLITE_OK)
		return sqlite3_exec(this->pDB, sql.c_str(), 0, 0, &errMsg);
}

//��ȡ���ݿ�������б�
list<string>& DbHandle::getTableList(void)
{
	return _tableList;
}

//��ȡָ������ĳ���豸���ڴ���־
string DbHandle::getCycleInfoFromDb(const string &cycle, const string &tablename, vector<LineInfo> &memItemInfo)
{
	string err;
	string sql = "SELECT * FROM '";
	sql.append(tablename);
	sql.append("' WHERE ");
	sql.append(CmConvert::StringToUTF8("���к�= '" + cycle + "'"));

	if (SQLITE_OK != sqlite3_exec(pDB, sql.c_str(), callback, &memItemInfo, nullptr))
	{
		err = sql.append(".exec error!!!.\n");
	}

	return err;
}

//��ѯ������Ϣ
string DbHandle::queryInfoFromDb(const string &tablename, vector<string> &tableHeader, vector<keyValue> &queryItem, vector<LineInfo> &memItemInfo)
{
	string err;

	if (queryItem.size() != 0)
	{
		string sql;
		if (tableHeader.size() == 0)
		{
			tableHeader = _tableHeader[tablename];
			sql = sql.append("SELECT * ");
		}
		else
		{
			sql = sql.append("SELECT ");
			for (vector<string>::iterator it = tableHeader.begin(); it != tableHeader.end(); it++)
			{
				sql.append(CmConvert::StringToUTF8((*it)+ ","));
			}
			sql.pop_back();
		}
		
		sql.append(" FROM '");
		sql.append(tablename);
		sql.append("' WHERE ");

		for (vector<keyValue>::iterator it = queryItem.begin(); it != queryItem.end(); it++)
		{
			sql.append(CmConvert::StringToUTF8((*it).key + "= '" + (*it).value + "',"));
		}

		sql.pop_back();

		cout << CmConvert::UTF8ToString(sql) << endl;

		sqlite3_stmt *stmt = NULL;
		int excflag = SQLITE_ERROR;
		int colnum = 0;

		if (_runflag == SQLITE_OK)
			excflag = sqlite3_prepare_v2(pDB, sql.c_str(), -1, &stmt, 0);

		if (excflag == SQLITE_OK)
		{
			while (sqlite3_step(stmt) == SQLITE_ROW)
			{
				LineInfo temp;
				colnum = sqlite3_column_count(stmt);
				if (tableHeader.size() == colnum)
				{
					for (int i = 0; i < colnum;i++)
					{
						string colValue = (char *)sqlite3_column_text(stmt, i);
						temp.Element.push_back(keyValue(tableHeader[i], CmConvert::UTF8ToString(colValue)));
					}
					memItemInfo.push_back(temp);
				}
				else
				{
					err = err.append("table colNum is error.");
					break;
				}
			}
			sqlite3_finalize(stmt);
		}
	}
	else
	{
		err = "queryItem is empty!";
	}
	return err;
}
