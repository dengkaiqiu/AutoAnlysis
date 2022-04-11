#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include <codecvt>
using namespace std;

static class ConvertClass
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
	static string UTF8ToString(const string& utf8Data)
	{
		//�Ƚ�UTF-8ת����Unicode
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

		std::wstring wString = conv.from_bytes(utf8Data);

		//��ת����string
		return WStringToString(wString);
	}

	//string��ʽתUTF8��ʽ
	static string StringToUTF8(const string& stringData)
	{
		//stringתwstring
		std::wstring wString = StringToWsting(stringData);

		//��ת����string
		return WStringToUTF8(wString);
	}

	//string��ʽתlong long 
	static long long StringToLLong(const string& string, int base)
	{
		return strtoll(string.c_str(), nullptr, base);
		//return _strtoi64(string.c_str(), nullptr, base);
	}

	//string��ʽתlong int
	static long int StringToLong(const string& string, int base)
	{
		return strtol(string.c_str(), nullptr, base);
	}


	//ת��Ϊ�޷����ַ�����
	static unsigned char asUChar(const string& str, int base)
	{
		return (unsigned char)StringToLong(str, base);
	}

	//ת��Ϊlong long ����
	static long long asLLong(const string& str, int base)
	{
		return StringToLLong(str, base);
	}

	//ת��Ϊ�޷���short����
	static unsigned short asUShort(const string& str, int base)
	{
		return (unsigned short)StringToLLong(str, base);
	}

	//ת��Ϊ�޷���int����
	static unsigned int asUint(const string& str, int base)
	{
		return (unsigned int)StringToLLong(str, base);
	}

	//ת��Ϊ�з�������
	static int asInt(const string& str, int base)
	{
		return (int)StringToLLong(str, base);
	}
};

