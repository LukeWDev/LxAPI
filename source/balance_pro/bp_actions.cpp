#include <balance_pro/bp_actions.hpp>
#include <reaper_plugin_functions.h>

#include <string>
#include <format>

constexpr ReaProject* THIS_PROJ = nullptr; 

namespace bp_actions
{
    int lx_CountMediaItems(int A, int B)
    {
        int TotalNumMediaItems = CountMediaItems(THIS_PROJ); 

        std::string log{std::format("total items: {}\n", TotalNumMediaItems)};

        ShowConsoleMsg(log.c_str());

        int Total = A + B;
        std::string log2{std::format("result = {}", Total)};
        ShowConsoleMsg(log2.c_str());
        return Total;
    }
}