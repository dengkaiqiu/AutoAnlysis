#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include <codecvt>
using namespace std;

static class ConvertClass
{
public:
	//wstring转string
	static string WStringToString(const wstring& str)
	{
		int	iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		std::vector<char> vecText(iTextLen, '\0');
		::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, &(vecText[0]), iTextLen, NULL, NULL);
		std::string strText = &(vecText[0]);

		return strText;
	}

	//string转wstring
	static wstring StringToWsting(const string& str)
	{
		int iTextLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		std::vector<wchar_t> vecText(iTextLen, '\0');
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &(vecText[0]), iTextLen);
		std::wstring result = &(vecText[0]);
		return result;
	}

	//wstring转string
	static string WStringToUTF8(const wstring& str)
	{
		int	iTextLen = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
		std::vector<char> vecText(iTextLen, '\0');
		::WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, &(vecText[0]), iTextLen, NULL, NULL);
		std::string strText = &(vecText[0]);

		return strText;
	}

	//UTF8格式转string格式
	static string UTF8ToString(const string& utf8Data)
	{
		//先将UTF-8转换成Unicode
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		std::wstring wString = conv.from_bytes(utf8Data);

		//在转换成string
		return WStringToString(wString);
	}

	//string格式转UTF8格式
	static string StringToUTF8(const string& stringData)
	{
		//string转wstring
		std::wstring wString = StringToWsting(stringData);

		//在转换成string
		return WStringToUTF8(wString);
	}

	//string格式转long long 
	static long long StringToLLong(const string& string, int base)
	{
		return strtoll(string.c_str(), nullptr, base);
		//return _strtoi64(string.c_str(), nullptr, base);
	}

	//string格式转long int
	static long int StringToLong(const string& string, int base)
	{
		return strtol(string.c_str(), nullptr, base);
	}


	//转换为无符号字符类型
	static unsigned char asUChar(const string& str, int base)
	{
		return (unsigned char)StringToLong(str, base);
	}

	//转换为long long 类型
	static long long asLLong(const string& str, int base)
	{
		return StringToLLong(str, base);
	}

	//转换为无符号short类型
	static unsigned short asUShort(const string& str, int base)
	{
		return (unsigned short)StringToLLong(str, base);
	}

	//转换为无符号int类型
	static unsigned int asUint(const string& str, int base)
	{
		return (unsigned int)StringToLLong(str, base);
	}

	//转换为有符号整型
	static int asInt(const string& str, int base)
	{
		return (int)StringToLLong(str, base);
	}
};

