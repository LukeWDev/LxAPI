#include <plugin.hpp>

#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>
#include <balance_pro/bp_actions.hpp>

REAPER_PLUGIN_HINSTANCE g_hInst; 

using namespace bp_actions;

extern "C" 
{
    REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t* rec)
    {
        PLUGIN plugin;
        g_hInst = hInstance;
        if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0) 
        {
            plugin.Add(structAPIdef{APIFUNC(lxu_CountMediaItems), "int", "int,int", "A,B", "test"});
            plugin.Register();
            return 1;
        }
        else 
        {
            plugin.Unregister();
            return 0;
        }
    }
}
