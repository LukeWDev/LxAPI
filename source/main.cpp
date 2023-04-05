// declarations for registering and unregistering my plugin
#include <core.hpp>

#define REAPERAPI_IMPLEMENT
#include <reaper_plugin_functions.h>

REAPER_PLUGIN_HINSTANCE g_hInst; // used for dialogs, if any

// enter the plugin
extern "C" 
{
    REAPER_PLUGIN_DLL_EXPORT int REAPER_PLUGIN_ENTRYPOINT(REAPER_PLUGIN_HINSTANCE hInstance, reaper_plugin_info_t* rec)
    {
        g_hInst = hInstance;
        if (rec && REAPERAPI_LoadAPI(rec->GetFunc) == 0) 
        {
            Register();
            return 1;
        }
        else 
        {
            return 0;
        }
    }
}
