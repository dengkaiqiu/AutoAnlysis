#pragma once
#include<iostream>
using namespace std;

// 内存字符串模块
class MemString
{
private:
	string str;
public:
	//构造函数
	MemString() { ; }
	MemString(const string& pstr);
	MemString(MemString& pstr);

	//析构函数
	~MemString() { ; }

	string to_string() { return str; }    //返回字符串
	unsigned char asUChar(int base) const;   //转换为无符号字符类型
	long long asLLong(int base) const;	//转换为long long 类型
	unsigned short asUShort(int base) const;	//转换为无符号short类型
	unsigned int asUint(int base) const;		//转换为无符号int类型
	int asInt(int base) const;          //转换为有符号整型
};