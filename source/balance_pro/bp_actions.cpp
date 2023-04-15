#include <balance_pro/bp_actions.hpp>
#include <utilities/AudioSource.hpp>

#include <string>
#include <format>
#include <memory>

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

    double lx_GetItemRMS(MediaItem* item, int window)
    {
        if (!item)
            return -150;

        auto itemTake = GetActiveTake(item);
        std::unique_ptr<AudioSource> itemSource = std::make_unique<AudioSource>(GetMediaItemTake_Source(itemTake));
        itemSource->GetSourceRMS(512, window);
        return -150;
    }
}