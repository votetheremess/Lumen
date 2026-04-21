#ifndef EFFECT_GFF_TONAL_HPP_INCLUDED
#define EFFECT_GFF_TONAL_HPP_INCLUDED

#include <vector>

#include "vulkan_include.hpp"

#include "../effect_simple.hpp"
#include "config.hpp"

namespace vkBasalt
{
    // Tonal pass of the GameFiltersFlatpak chain — Exposure, Contrast,
    // Highlights, Shadows, Gamma. All five are bundled in one shader
    // because the math has no HDR float headroom to split across passes.
    class GffTonalEffect : public SimpleEffect
    {
    public:
        GffTonalEffect(LogicalDevice*       pLogicalDevice,
                       VkFormat             format,
                       VkExtent2D           imageExtent,
                       std::vector<VkImage> inputImages,
                       std::vector<VkImage> outputImages,
                       Config*              pConfig);
        ~GffTonalEffect();
    };
} // namespace vkBasalt

#endif // EFFECT_GFF_TONAL_HPP_INCLUDED
