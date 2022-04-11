#ifndef BIG_FILE_MAP_H_
#define BIG_FILE_MAP_H_
#include <windows.h>
#include <iostream>
#include <thread>
#include <regex>

#define GET_HIGH(x) (DWORD)((x & 0xffffffff00000000) >> 32)
#define GET_LOW(x) (DWORD)(x & 0x00000000ffffffff)

using namespace std;

//每周期日志的起始地址和长度
struct cycleLogInfoStru
{
	string cycle;
	__int64 startIndex;
	__int64 len;
	cycleLogInfoStru()
	{
		cycle = "0";
		startIndex = 0;
		len = 0;
	}
	cycleLogInfoStru(const string &cycle, __int64 startIndex)
	{
		this->cycle = cycle;
		this->startIndex = startIndex;
		this->len = 0;
	}
};

//匹配成功的起始地址和匹配成功的字段
struct matchInfoStru
{
	vector<string> element;
	__int64 startIndex;

	matchInfoStru(std::smatch m, __int64 index)
	{
		for (auto& elem : m)
			element.push_back(elem.str());
		this->startIndex = index;
	}
};


//buf：从当前地址开始搜索。
//len: 存放该行的字符串长度，不包含回车换行符。
//返回值：下一次搜索开始的地址.如果到达文件尾，则返回NULL
static const char* _get_line(const char* buf, int* len, __int64 *curIndex, __int64 readLen)
{
	const char* tmp = buf;
	while ((*curIndex < readLen) && *tmp && (*tmp != 0x0d && *tmp != 0x0a && *tmp != '\n'))
	{
		++tmp;
		(*curIndex) += 1;
	}
	//while(*tmp && (*tmp != 0x0d || *tmp != 0x0a )) ++tmp;
	*len = tmp - buf; //

	if ((*curIndex) == readLen) return NULL;

	// skip New-Line char
	if (*tmp == 0x0d) { // Windows style New-Line 0x0d 0x0a
		tmp += 2;
		(*curIndex) += 2;
		//assert(*tmp == 0x0a);
	}//else Unix style New-Line 0x0a
	else {
		++tmp;
		(*curIndex) += 1;
	}

	return tmp;
}


//创建映射文件
static char * CreateMapHandle(HANDLE& hFile,HANDLE &hFileMap, __int64 startIndex, __int64 readLen)
{
	if (NULL == hFileMap)
		hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

	if (NULL == hFileMap)
		return NULL;
	
	return (char *)MapViewOfFile(hFileMap, FILE_MAP_READ, GET_HIGH(startIndex), GET_LOW(startIndex), readLen);

}

//关闭映射文件
static void destroyMapHandle(HANDLE& hFile, HANDLE &hFileMap)
{
	UnmapViewOfFile(hFile);
	CloseHandle(hFileMap);
};

//获取匹配上成功的地址信息
static bool SearchMatchInfo(const char * lpbMapAddress, __int64 readLen, const string &keyStr, const string &matchStr, vector<matchInfoStru> &matchInfo)
{
	const char* buf = lpbMapAddress;
	const char* start = lpbMapAddress;
	__int64 curIndex = 0;
	int len = 0;

	regex reg(matchStr);
	std::smatch m;

	while (curIndex < readLen) {
		__int64 tempIndex = curIndex;
		start = _get_line(buf, &len, &curIndex, readLen);
		string str(buf, len);

		if (strstr(str.c_str(), keyStr.c_str()))
		{
			auto pos = str.cbegin();
			auto end = str.cend();
			for (; std::regex_search(pos, end, m, reg); pos = m.suffix().first)
			{
				matchInfoStru temp(m, tempIndex);
				matchInfo.push_back(temp);
			}
		}

		buf = start; 
#if 0  //多线程时存在问题
		const char* temp = strchr(buf, '\n');
		len = temp - buf + 1;
		string str(buf, len);

		lineCnt++;
		if (strstr(str.c_str(), keystr))
		{
			cycles.push_back("find");
		}
		curIndex += len;
		buf = ++temp;
#endif
	}

	return curIndex == readLen;
}

//读取周期信息
static bool SplitToLines(HANDLE& hFile, __int64 fileSize, __int64 startIndex, __int64 readLen, vector<string> &cycleInfo)
{
	HANDLE hFileMap = NULL;
	char * lpbMapAddress = NULL;

	//创建映射文件
	lpbMapAddress = CreateMapHandle(hFile, hFileMap, 0, fileSize);

	if (lpbMapAddress == NULL)
		return false;

	const char* buf = lpbMapAddress + startIndex;
	const char* start = buf;
	__int64 curIndex = 0;
	int len = 0;

	while (curIndex < readLen) {
		__int64 tempIndex = curIndex;
		start = _get_line(buf, &len, &curIndex, readLen);
		string str(buf, len);
		cycleInfo.push_back(str);
		buf = start;
	}

	//关闭映射文件
	destroyMapHandle(hFile, hFileMap);

	return curIndex == readLen;
}

//拆分周期信息
static bool SplitCycleInfo(HANDLE& hFile, __int64 startIndex, __int64 readLen, const string &keyStr, const string &matchStr,
	vector<cycleLogInfoStru>& cycles)
{
	HANDLE hFileMap = NULL;
	char * lpbMapAddress = NULL;

	//创建映射文件
	lpbMapAddress = CreateMapHandle(hFile, hFileMap, startIndex, readLen);
	
	if (lpbMapAddress == NULL)
		return false;

	//获取匹配上成功的地址信息
	vector<matchInfoStru> matchInfo;
	bool flag = SearchMatchInfo(lpbMapAddress, readLen, keyStr, matchStr, matchInfo);

	if (flag)
	{
		//转换成cycleLogInfoStru
		for (vector<matchInfoStru>::iterator it = matchInfo.begin(); it != matchInfo.end();it++)
		{
			if (it == matchInfo.begin() && (*it).startIndex != 0)
			{
				cycles.push_back(cycleLogInfoStru("0", 0));
				cycles.push_back(cycleLogInfoStru((*it).element[1], (*it).startIndex));
			}
			else
			{
				cycles.push_back(cycleLogInfoStru((*it).element[1], (*it).startIndex));
			}
		}
	}

	//关闭映射文件
	destroyMapHandle(hFile, hFileMap);

	return flag;
}


class BigFileMap
{
	
public:
	//按周期获取行
	string SplitCycleInfoToLines(HANDLE &hFile,__int64 fileSize,cycleLogInfoStru &mapInfo, vector<string> &cycleInfo)    //获取周期数据
	{
		bool flag = SplitToLines(hFile, fileSize,mapInfo.startIndex, mapInfo.len, cycleInfo);

		return flag ? "" : "get cycle lines info error.";
	}

	//按周期拆分大文件
	string SplitCycles(HANDLE &hFile, const string &keystr, const string &matchStr,
		vector<cycleLogInfoStru> &cycles, __int64 qwFileSize)
	{
		__int64 start = 0;
		//按周期拆分
		bool flag = SplitCycleInfo(hFile, start, qwFileSize,keystr, matchStr,cycles);
		return flag ? "" : "split to cycle error.";

#if 0  //用线程方式读取--貌似没有快多少
		//每个线程读20M
		__int64 threadReadSize = 1024 * 1024 * 10;
		int threadNum = (qwFileSize % threadReadSize) == 0 ? (qwFileSize/ threadReadSize) : (qwFileSize / threadReadSize) + 1;

		for (int i = 0; i < threadNum; i++)
		{
			if (start + threadReadSize <= qwFileSize)
			{
				thread t(threadReadfun, hFile, i,start, threadReadSize);
				start += threadReadSize;
				t.detach();
			}
			else
			{
				thread t(threadReadfun, hFile, i, start, qwFileSize % threadReadSize);
				t.detach();
			}
		}
#endif
	}
};


#endif
