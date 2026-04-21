#ifndef EFFECT_GFF_LOCAL_HPP_INCLUDED
#define EFFECT_GFF_LOCAL_HPP_INCLUDED

#include <vector>

#include "vulkan_include.hpp"

#include "../effect_simple.hpp"
#include "config.hpp"

namespace vkBasalt
{
    // Local / spatial filter pass of the GameFiltersFlatpak chain — Sharpen,
    // Clarity, HDR Toning, Bloom. Reads Config for four gff.* float params
    // and bakes them into specialization constants for gff_local.frag.glsl.
    class GffLocalEffect : public SimpleEffect
    {
    public:
        GffLocalEffect(LogicalDevice*       pLogicalDevice,
                       VkFormat             format,
                       VkExtent2D           imageExtent,
                       std::vector<VkImage> inputImages,
                       std::vector<VkImage> outputImages,
                       Config*              pConfig);
        ~GffLocalEffect();
    };
} // namespace vkBasalt

#endif // EFFECT_GFF_LOCAL_HPP_INCLUDED
