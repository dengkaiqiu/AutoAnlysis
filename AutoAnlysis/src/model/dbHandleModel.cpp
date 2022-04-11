#include "dbHandleModel.h"


//ת����·��
string DbHandleModel::ConvertToPath(const string& path)
{
	string filePath;
	if (FileHandle::is_path(path))
		filePath = path;
	else if (FileHandle::is_file(path))
		filePath = _path + path;
	else
		filePath = "";
	return filePath;
}

DbHandleModel::~DbHandleModel()
{
	CloseDBConn();
}

//�����ݿ�
string DbHandleModel::OpenDb(const string& dbfile)
{
	string err = "";
	string filePath = ConvertToPath(dbfile);
	if (FileHandle::exists_file(filePath))
		DBobj.openDB(filePath);
	else
		err = filePath + "not exit.";
	return err;
}

//�ر����ݿ�
void DbHandleModel::CloseDBConn(void)
{
	DBobj.closeDBConn();
}

//��������
void DbHandleModel::BeginTransaction()
{
	DBobj.BeginTransaction();
}

//�ύ
void DbHandleModel::Commit()
{
	DBobj.commit();
}

//������
int DbHandleModel::CreatTable(string tableName, vector<string>& Colums)
{
	string sql = "";
	sql.append("CREATE TABLE ");
	sql.append(tableName);
	sql.append(" (");

	for (vector<string>::iterator it = Colums.begin(); it != Colums.end(); it++)
	{
		sql.append(*it).append(" varchar").append(",");
	}
	sql.pop_back();
	sql.append(")");

	return DBobj.ExecuteNonQuery(sql);
}

//��������
int DbHandleModel::Insert(string& sql)
{
	return DBobj.ExecuteNonQuery(sql);
}

//��ȡָ������ĳ���豸���ڴ���־
string DbHandleModel::GetCycleInfoFromDb(const string& cycle, const string& tablename, vector<LineInfo>& lineInfos)
{
	return DBobj.getCycleInfoFromDb(cycle, tablename, lineInfos);
}

//��ȡָ�����������豸���ڴ���Ϣ
string DbHandleModel::GetAllTableCycleInfoFromDb(const string& cycle, vector<TableInfo>& tableInfos)
{
	std::string err;
	list<string>& tableList = DBobj.getTableList();

	for (list<string>::iterator it = tableList.begin(); it != tableList.end(); ++it) {
		TableInfo temp;
		temp.item.insert(pair<string, string>("memType", *it));

		vector<LineInfo> tempLineInfo;
		err = GetCycleInfoFromDb(cycle, *it, tempLineInfo);

		if (!err.empty())
			break;

		string str = *it + "Infos";
		temp.Element.insert(pair<string, vector<LineInfo>>(str, tempLineInfo));

		tableInfos.push_back(temp);
	}
	return err;

}

//��ȡĳһ���ڵ������豸�ڴ���Ϣ
string DbHandleModel::GetCycleMemsFromDb(list<string>& cyclelist, vector<MemInfo>& memInfos)
{
	string err;
	for (std::list<string>::iterator it = cyclelist.begin(); it != cyclelist.end(); ++it) {

		MemInfo temp;
		temp.item.insert(pair<string, string>("���к�", *it));

		vector<TableInfo> tableinfoTemp;
		err = GetAllTableCycleInfoFromDb(*it, tableinfoTemp);
		if (!err.empty())
			break;
		temp.Element.insert(pair<string, vector<TableInfo>>("memTypes", tableinfoTemp));
		memInfos.push_back(temp);
	}
	return err;
}

//���л�
string DbHandleModel::ConvertTo(MemInfo& memInfos, ItemInfo& itemInfo)
{
	string err;
	itemInfo._itemMapinfo.insert(memInfos.item.begin(), memInfos.item.end());

	for (auto it = memInfos.Element.begin(); it != memInfos.Element.end(); ++it)
	{
		pair<string, vector<TableInfo>> tempTableInfo = (*it);

		//��������table��Ϣ
		vector<ItemInfo> tablesInfo;
		for (vector<TableInfo>::iterator itt = tempTableInfo.second.begin(); itt != tempTableInfo.second.end(); ++itt)
		{
			//����table��Ϣ
			ItemInfo tableInfo;
			tableInfo._itemMapinfo.insert((*itt).item.begin(), (*itt).item.end());

			//�������е���
			for (auto ittt = (*itt).Element.begin(); ittt != (*itt).Element.end(); ++ittt)
			{
				pair<string, vector<LineInfo>> tempLineInfo = (*ittt);
				vector<ItemInfo> linesInfo;
				for (vector<LineInfo>::iterator itttt = tempLineInfo.second.begin(); itttt != tempLineInfo.second.end(); ++itttt)
				{
					ItemInfo lineInfo;
					for (vector<keyValue>::iterator ittttt = (*itttt).Element.begin(); ittttt != (*itttt).Element.end(); ++ittttt)
					{
						lineInfo._itemMapinfo.insert(pair<string, string>((*ittttt).key, (*ittttt).value));
					}
					linesInfo.push_back(lineInfo);
				}
				tableInfo._subItemMapinfo.insert(pair<string, vector<ItemInfo>>(tempLineInfo.first, linesInfo));
			}
			tablesInfo.push_back(tableInfo);
		}
		itemInfo._subItemMapinfo.insert(pair<string, vector<ItemInfo>>(tempTableInfo.first, tablesInfo));
	}
	return err;
}

//��ѯ������Ϣ
string DbHandleModel::QueryInfoFromDb(const string& tablename, vector<string>& tableHeader, vector<keyValue>& queryItem, vector<LineInfo>& memItemInfo)
{
	return DBobj.queryInfoFromDb(tablename, tableHeader, queryItem, memItemInfo);
}