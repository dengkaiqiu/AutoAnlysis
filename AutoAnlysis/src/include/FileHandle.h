#pragma once
#include<iostream>
using namespace std;

namespace fileHandle
{
	static class FileHandle
	{
	public:
		//判断文件是否存在
		static bool exists_file(const std::string& name) {
			struct stat buffer;
			return (stat(name.c_str(), &buffer) == 0);
		}

		//判断是否路径
		static bool is_path(const string& path)
		{
			return path.find('\\') != string::npos;
		}

		//判读是否文件
		static bool is_file(const string& path)
		{
			return is_path(path) ? false : (path.find('.') != string::npos);
		}

		//获取路径的文件名
		static string getFullName(const string& path)
		{
			int pos1 = path.find_last_of('\\');
			return path.substr(pos1 + 1);
		}

		//获取文件的路径
		static string getFilePath(const string& path)
		{
			int pos1 = path.find_last_of('\\');
			return path.substr(0,pos1);
		}

		//获取文件的前缀
		static string getFilesuffix(const string& path)
		{
			string filename = getFullName(path);
			int pos1 = path.find('.');
			return filename.substr(0, pos1 + 1);
		}

		//获取文件的后缀
		static string getFilePrefix(const string& path)
		{
			string filename = getFullName(path);
			int pos1 = path.find('.');
			return filename.substr(pos1 + 1);
		}
	};
}
