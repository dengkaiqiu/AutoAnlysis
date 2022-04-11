#ifndef BIG_FILE_MAP_MODEL_H_
#define BIG_FILE_MAP_MODEL_H_
#include <list>
#include <map>
#include "bigFileMap.h"

class BigFileMapModle
{
public:
	HANDLE _hFile;  //文件句柄
	__int64 _qwFileSize;   //文件大小
	BigFileMap _bigFileMapObj;     //大文件操作对象
	map<string, cycleLogInfoStru> _mapCycles;   //映射的周期信息
	list<string> _MapCycleSeq;    //映射的周期序列
	bool _readValid;

private:
	void SetFileSize();    //设置文件大小
	void ConvertTo(vector<cycleLogInfoStru>& cycles);  //转换成周期信息的map
	

public:

	BigFileMapModle();  //无参构造函数
	BigFileMapModle(const string& filePath);  //构造函数

	~BigFileMapModle();   //析构函数


	unsigned int GetFramsCnt();  //获取帧数量
	unsigned int GetFramsMinId();   //获取帧最小ID	
	unsigned int GetFramsMaxId();  //获取帧最大ID
	
	string GetCycleInfo(const string& cycle, vector<string>& cycleInfo);    //获取周期数据
	string SplitCycles(const string& keyStr, const string& matchStr);    //拆分周期信息

	string ResetBigFileMap(const string& filePath);      //重置大文件映射
	
};

#endif