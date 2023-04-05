#include <reaper_plugin_functions.h>

#include "core.hpp"

void CORE::Add(structAPIdef apiDef)
{
    apiDefinitions.push_back(apiDef);
}

void CORE::Register()
{
    char temp[10000];
    for (structAPIdef& f : arrayAPIdefs)
    {
        
        snprintf(temp, 10000, "%s\n%s\n%s\n%s", f.ret_val, f.parm_types, f.parm_names, f.help);
        f.defstring = strdup(temp);
        int i = 0; 
        int countZeroes = 0; 
        while (countZeroes < 3) 
        { 
            if (f.defstring[i] == '\n') 
            { 
                f.defstring[i] = 0; 
                countZeroes++; 
            } 
            i++; 
        }
        plugin_register(f.regkey_def, (void*)f.defstring);
        plugin_register(f.regkey_func, f.func);
        plugin_register(f.regkey_vararg, f.func_vararg);
    }
}