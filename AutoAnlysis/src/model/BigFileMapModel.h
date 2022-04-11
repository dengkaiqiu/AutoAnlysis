#ifndef BIG_FILE_MAP_MODEL_H_
#define BIG_FILE_MAP_MODEL_H_
#include <list>
#include <map>
#include "bigFileMap.h"

class BigFileMapModle
{
public:
	HANDLE _hFile;  //�ļ����
	__int64 _qwFileSize;   //�ļ���С
	BigFileMap _bigFileMapObj;     //���ļ���������
	map<string, cycleLogInfoStru> _mapCycles;   //ӳ���������Ϣ
	list<string> _MapCycleSeq;    //ӳ�����������
	bool _readValid;

private:
	void SetFileSize();    //�����ļ���С
	void ConvertTo(vector<cycleLogInfoStru>& cycles);  //ת����������Ϣ��map
	

public:

	BigFileMapModle();  //�޲ι��캯��
	BigFileMapModle(const string& filePath);  //���캯��

	~BigFileMapModle();   //��������


	unsigned int GetFramsCnt();  //��ȡ֡����
	unsigned int GetFramsMinId();   //��ȡ֡��СID	
	unsigned int GetFramsMaxId();  //��ȡ֡���ID
	
	string GetCycleInfo(const string& cycle, vector<string>& cycleInfo);    //��ȡ��������
	string SplitCycles(const string& keyStr, const string& matchStr);    //���������Ϣ

	string ResetBigFileMap(const string& filePath);      //���ô��ļ�ӳ��
	
};

#endif