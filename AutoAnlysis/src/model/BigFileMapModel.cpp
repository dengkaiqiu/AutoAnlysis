#include "BigFileMapModel.h"
#include "MemString.h"

void BigFileMapModle::SetFileSize()    //设置文件大小
{
	_qwFileSize = 0;
	if (_hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSizeHigh;
		_qwFileSize = GetFileSize(_hFile, &dwFileSizeHigh);
		_qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
	}
}

void BigFileMapModle::ConvertTo(vector<cycleLogInfoStru>& cycles)  //转换成周期信息的map
{
	if (cycles.size() == 0)
		return;  //无需处理

	if (cycles.size() == 1) {
		cycles[0].len = _qwFileSize;
		_mapCycles.insert(pair<string, cycleLogInfoStru>(cycles[0].cycle, cycles[0]));
		return;   //只需更改长度
	}

	if (cycles.size() > 1) {
		__int64 lastStartIndex;
		long long dwLastCycle;
		for (vector<cycleLogInfoStru>::reverse_iterator it = cycles.rbegin(); it != cycles.rend(); it++)
		{
			if (it == cycles.rbegin()) {
				(*it).len = _qwFileSize - (*it).startIndex;
				string convertCycle = to_string(MemString((*it).cycle).asLLong(10));
				_mapCycles.insert(pair<string, cycleLogInfoStru>(convertCycle, (*it)));
				_MapCycleSeq.emplace_front(convertCycle);
			}
			else if (it == cycles.rend() - 1) {
				if ((*it).cycle == "0") {
					(*it).len = lastStartIndex - (*it).startIndex;
					_mapCycles.insert(pair<string, cycleLogInfoStru>(to_string(dwLastCycle - 1), (*it)));
					_MapCycleSeq.emplace_front(to_string(dwLastCycle - 1));
				}
				else {
					(*it).len = lastStartIndex - (*it).startIndex;
					string convertCycle = to_string(MemString((*it).cycle).asLLong(10));
					_mapCycles.insert(pair<string, cycleLogInfoStru>(convertCycle, (*it)));
					_MapCycleSeq.emplace_front(convertCycle);
				}
			}
			else {
				(*it).len = lastStartIndex - (*it).startIndex;
				string convertCycle = to_string(MemString((*it).cycle).asLLong(10));
				_mapCycles.insert(pair<string, cycleLogInfoStru>(convertCycle, (*it)));
				_MapCycleSeq.emplace_front(convertCycle);
			}
			lastStartIndex = (*it).startIndex;
			dwLastCycle = MemString((*it).cycle).asLLong(10);
		}
	}

}

//无参构造函数
BigFileMapModle::BigFileMapModle()
{
	_hFile = INVALID_HANDLE_VALUE;  //文件句柄
	_mapCycles.clear();
	_MapCycleSeq.clear();
	_readValid = false;
}

BigFileMapModle::BigFileMapModle(const string& filePath)  //构造函数
{
	_hFile = INVALID_HANDLE_VALUE;  //文件句柄
	if (INVALID_HANDLE_VALUE == _hFile)
		_hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	SetFileSize();  //设置文件大小

	_readValid = true;
}

BigFileMapModle::~BigFileMapModle()   //析构函数
{
	if (_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(_hFile);
	_mapCycles.clear();
	_MapCycleSeq.clear();
	_readValid = false;
}

//获取帧数量
unsigned int BigFileMapModle::GetFramsCnt() 
{
	if (_readValid)
		return _MapCycleSeq.size();
	else
		return 0;
}

//获取帧最小ID
unsigned int BigFileMapModle::GetFramsMinId()
{
	if (_MapCycleSeq.size() == 0)
		return 0;
	return MemString(_MapCycleSeq.front()).asUint(10);
}

//获取帧最大ID
unsigned int BigFileMapModle::GetFramsMaxId()
{
	if (_MapCycleSeq.size() == 0)
		return 0;
	return MemString(_MapCycleSeq.back()).asUint(10);
}

//获取周期数据
string BigFileMapModle::GetCycleInfo(const string& cycle, vector<string>& cycleInfo)    
{
	if (_mapCycles.count(cycle) != 0)
		return _bigFileMapObj.SplitCycleInfoToLines(_hFile, _qwFileSize, _mapCycles[cycle], cycleInfo);
	return "not exit cycle:" + cycle;
}

//拆分周期信息
string BigFileMapModle::SplitCycles(const string& keyStr, const string& matchStr)    
{
	vector<cycleLogInfoStru> cycles;

	if (_hFile == INVALID_HANDLE_VALUE)
		return "file open error.";

	string err = _bigFileMapObj.SplitCycles(_hFile, keyStr, matchStr, cycles, _qwFileSize);

	if (err.length() == 0)
		ConvertTo(cycles);

	return err;
}

//重置大文件映射
string BigFileMapModle::ResetBigFileMap(const string& filePath)
{
	string err = "";
	if (_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(_hFile);
	_mapCycles.clear();
	_MapCycleSeq.clear();
	_readValid = false;

	if (INVALID_HANDLE_VALUE == _hFile)
		_hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	SetFileSize();  //设置文件大小

	if (INVALID_HANDLE_VALUE != _hFile)
		_readValid = true;
	else
		err.append("reset bigfile error");
	return err;
}
