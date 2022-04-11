#pragma once
#include<iostream>
using namespace std;

// �ڴ��ַ���ģ��
class MemString
{
private:
	string str;
public:
	//���캯��
	MemString() { ; }
	MemString(const string& pstr);
	MemString(MemString& pstr);

	//��������
	~MemString() { ; }

	string to_string() { return str; }    //�����ַ���
	unsigned char asUChar(int base) const;   //ת��Ϊ�޷����ַ�����
	long long asLLong(int base) const;	//ת��Ϊlong long ����
	unsigned short asUShort(int base) const;	//ת��Ϊ�޷���short����
	unsigned int asUint(int base) const;		//ת��Ϊ�޷���int����
	int asInt(int base) const;          //ת��Ϊ�з�������
};