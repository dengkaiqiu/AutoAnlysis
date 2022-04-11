#pragma once
#include "JsonHandle.h"
#include "memMap.h"

using namespace Json;


#ifdef MYLIB_EXPORTS
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif


//¼ÓÔØjsonÎÄ¼þ
extern "C" MYLIB_API
string LoadJson(const string & path, ItemInfo & item);