#include "MemString.h"
#include "ConvertClass.h"

//构造函数
MemString::MemString(const string& pstr)
{
	str = pstr;
}

MemString::MemString(MemString& pstr)
{
	str = pstr.str;
}

//转换为无符号字符类型
unsigned char MemString::asUChar(int base) const
{
	return (unsigned char)ConvertClass::asUChar(str, base);
}

//转换为long long 类型
long long MemString::asLLong(int base) const
{
	return ConvertClass::asLLong(str, base);
}

//转换为无符号short类型
unsigned short MemString::asUShort(int base) const
{
	return (unsigned short)ConvertClass::asUShort(str, base);
}

//转换为无符号int类型
unsigned int MemString::asUint(int base) const
{
	return (unsigned int)ConvertClass::asUint(str, base);
}

//转换为有符号整型
int MemString::asInt(int base) const
{
	return (int)ConvertClass::asInt(str, base);
}