#pragma once

#include <reaper_plugin_functions.h>

namespace bp_actions
{
    int lx_CountMediaItems(int A, int B);
    double lx_GetItemRMS(MediaItem* item, int window);
}

