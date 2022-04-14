#pragma once
#include<iostream>
using namespace std;

namespace fileHandle
{
	static class FileHandle
	{
	public:
		//�ж��ļ��Ƿ����
		static bool exists_file(const std::string& name) {
			struct stat buffer;
			return (stat(name.c_str(), &buffer) == 0);
		}

		//�ж��Ƿ�·��
		static bool is_path(const string& path)
		{
			return path.find('\\') != string::npos;
		}

		//�ж��Ƿ��ļ�
		static bool is_file(const string& path)
		{
			return is_path(path) ? false : (path.find('.') != string::npos);
		}

		//��ȡ·�����ļ���
		static string getFullName(const string& path)
		{
			int pos1 = path.find_last_of('\\');
			return path.substr(pos1 + 1);
		}

		//��ȡ�ļ���·��
		static string getFilePath(const string& path)
		{
			int pos1 = path.find_last_of('\\');
			return path.substr(0,pos1);
		}

		//��ȡ�ļ���ǰ׺
		static string getFilesuffix(const string& path)
		{
			string filename = getFullName(path);
			int pos1 = path.find('.');
			return filename.substr(0, pos1 + 1);
		}

		//��ȡ�ļ��ĺ�׺
		static string getFilePrefix(const string& path)
		{
			string filename = getFullName(path);
			int pos1 = path.find('.');
			return filename.substr(pos1 + 1);
		}
	};
}
