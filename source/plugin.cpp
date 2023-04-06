#include <reaper_plugin_functions.h>

#include <plugin.hpp>

void PLUGIN::Add(const structAPIdef apiDef)
{
    apiDefinitions.push_back(apiDef);
}

void PLUGIN::Register()
{
    char temp[10000];
    for (structAPIdef& apiDef : apiDefinitions)
    {
        snprintf(temp, 10000, "%s\n%s\n%s\n%s", apiDef.ret_val, apiDef.parm_types, apiDef.parm_names, apiDef.help);
        apiDef.defstring = _strdup(temp);
        int i = 0; 
        int countZeroes = 0; 
        while (countZeroes < 3) 
        { 
            if (apiDef.defstring[i] == '\n') 
            { 
                apiDef.defstring[i] = 0; 
                countZeroes++; 
            } 
            i++; 
        }
        plugin_register(apiDef.regkey_def, (void*)apiDef.defstring);
        plugin_register(apiDef.regkey_func, apiDef.func);
        plugin_register(apiDef.regkey_vararg, apiDef.func_vararg);
    }
}

void PLUGIN::Unregister()
{
    char temp[10000];
    for (structAPIdef& apiDef : apiDefinitions)
    {
        snprintf(temp, sizeof(temp), "-%s", apiDef.regkey_func);
		plugin_register(temp, apiDef.func);
    }
}