#ifndef EFFECT_GFF_STYLISTIC_HPP_INCLUDED
#define EFFECT_GFF_STYLISTIC_HPP_INCLUDED

#include <vector>

#include "vulkan_include.hpp"

#include "../effect_simple.hpp"
#include "config.hpp"

namespace vkBasalt
{
    // Stylistic / mood pass of the GameFiltersFlatpak chain — Black & White
    // desaturation and radial Vignette.
    class GffStylisticEffect : public SimpleEffect
    {
    public:
        GffStylisticEffect(LogicalDevice*       pLogicalDevice,
                           VkFormat             format,
                           VkExtent2D           imageExtent,
                           std::vector<VkImage> inputImages,
                           std::vector<VkImage> outputImages,
                           Config*              pConfig);
        ~GffStylisticEffect();
    };
} // namespace vkBasalt

#endif // EFFECT_GFF_STYLISTIC_HPP_INCLUDED
