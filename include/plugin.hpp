#pragma once

#include "reascript_vararg.hpp"

#include <vector>

struct structAPIdef
{
	void* func; // pointer to the function that other extensions use
	const char* func_name;
	void* func_vararg; // pointer to the wrapper function that ReaScript API calls
	const char* regkey_vararg; // "APIvararg_funcName" - for
	const char* regkey_func; // "API_funcName"
	const char* regkey_def; // "APIdef_funcName"
	const char* ret_val; // return value type, as string
	const char* parm_types;
	const char* parm_names;
	const char* help;
	char* defstring; // \0-separated string for APIdef... Will be concatenated and assigned while registering function

};
#define APIFUNC(x) (void*)x, #x, reinterpret_cast<void*>(&InvokeReaScriptAPI<&x>), "APIvararg_" #x "", "API_" #x "", "APIdef_" #x ""

class PLUGIN
{
public:
	void Add(structAPIdef apiDef);
	void Register();
	void Unregister();
private:
	std::vector<structAPIdef> apiDefinitions{};
};
