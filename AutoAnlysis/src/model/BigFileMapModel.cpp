#include "BigFileMapModel.h"
#include "MemString.h"

void BigFileMapModle::SetFileSize()    //�����ļ���С
{
	_qwFileSize = 0;
	if (_hFile != INVALID_HANDLE_VALUE) {
		DWORD dwFileSizeHigh;
		_qwFileSize = GetFileSize(_hFile, &dwFileSizeHigh);
		_qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
	}
}

void BigFileMapModle::ConvertTo(vector<cycleLogInfoStru>& cycles)  //ת����������Ϣ��map
{
	if (cycles.size() == 0)
		return;  //���账��

	if (cycles.size() == 1) {
		cycles[0].len = _qwFileSize;
		_mapCycles.insert(pair<string, cycleLogInfoStru>(cycles[0].cycle, cycles[0]));
		return;   //ֻ����ĳ���
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

//�޲ι��캯��
BigFileMapModle::BigFileMapModle()
{
	_hFile = INVALID_HANDLE_VALUE;  //�ļ����
	_mapCycles.clear();
	_MapCycleSeq.clear();
	_readValid = false;
}

BigFileMapModle::BigFileMapModle(const string& filePath)  //���캯��
{
	_hFile = INVALID_HANDLE_VALUE;  //�ļ����
	if (INVALID_HANDLE_VALUE == _hFile)
		_hFile = CreateFile(filePath.c_str(), GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	SetFileSize();  //�����ļ���С

	_readValid = true;
}

BigFileMapModle::~BigFileMapModle()   //��������
{
	if (_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(_hFile);
	_mapCycles.clear();
	_MapCycleSeq.clear();
	_readValid = false;
}

//��ȡ֡����
unsigned int BigFileMapModle::GetFramsCnt() 
{
	if (_readValid)
		return _MapCycleSeq.size();
	else
		return 0;
}

//��ȡ֡��СID
unsigned int BigFileMapModle::GetFramsMinId()
{
	if (_MapCycleSeq.size() == 0)
		return 0;
	return MemString(_MapCycleSeq.front()).asUint(10);
}

//��ȡ֡���ID
unsigned int BigFileMapModle::GetFramsMaxId()
{
	if (_MapCycleSeq.size() == 0)
		return 0;
	return MemString(_MapCycleSeq.back()).asUint(10);
}

//��ȡ��������
string BigFileMapModle::GetCycleInfo(const string& cycle, vector<string>& cycleInfo)    
{
	if (_mapCycles.count(cycle) != 0)
		return _bigFileMapObj.SplitCycleInfoToLines(_hFile, _qwFileSize, _mapCycles[cycle], cycleInfo);
	return "not exit cycle:" + cycle;
}

//���������Ϣ
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

//���ô��ļ�ӳ��
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

	SetFileSize();  //�����ļ���С

	if (INVALID_HANDLE_VALUE != _hFile)
		_readValid = true;
	else
		err.append("reset bigfile error");
	return err;
}
