#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <errno.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}


#ifdef MYLIB_EXPORTS
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif

extern "C"  __declspec(dllexport) int luaopen_bigfilemap(lua_State * L);
