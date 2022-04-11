#include "MemString.h"
#include "ConvertClass.h"

//���캯��
MemString::MemString(const string& pstr)
{
	str = pstr;
}

MemString::MemString(MemString& pstr)
{
	str = pstr.str;
}

//ת��Ϊ�޷����ַ�����
unsigned char MemString::asUChar(int base) const
{
	return (unsigned char)ConvertClass::asUChar(str, base);
}

//ת��Ϊlong long ����
long long MemString::asLLong(int base) const
{
	return ConvertClass::asLLong(str, base);
}

//ת��Ϊ�޷���short����
unsigned short MemString::asUShort(int base) const
{
	return (unsigned short)ConvertClass::asUShort(str, base);
}

//ת��Ϊ�޷���int����
unsigned int MemString::asUint(int base) const
{
	return (unsigned int)ConvertClass::asUint(str, base);
}

//ת��Ϊ�з�������
int MemString::asInt(int base) const
{
	return (int)ConvertClass::asInt(str, base);
}