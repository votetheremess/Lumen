#ifndef EFFECT_GFF_COLOR_HPP_INCLUDED
#define EFFECT_GFF_COLOR_HPP_INCLUDED

#include <vector>

#include "vulkan_include.hpp"

#include "../effect_simple.hpp"
#include "config.hpp"

namespace vkBasalt
{
    // Color / chroma pass of the GameFiltersFlatpak chain — Tint Color,
    // Tint Intensity, Temperature, Vibrance.
    class GffColorEffect : public SimpleEffect
    {
    public:
        GffColorEffect(LogicalDevice*       pLogicalDevice,
                       VkFormat             format,
                       VkExtent2D           imageExtent,
                       std::vector<VkImage> inputImages,
                       std::vector<VkImage> outputImages,
                       Config*              pConfig);
        ~GffColorEffect();
    };
} // namespace vkBasalt

#endif // EFFECT_GFF_COLOR_HPP_INCLUDED
