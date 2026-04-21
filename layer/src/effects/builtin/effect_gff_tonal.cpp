#include "effect_gff_tonal.hpp"

#include <array>
#include <cstring>

#include "format.hpp"
#include "logger.hpp"
#include "shader_sources.hpp"
#include "util.hpp"

namespace vkBasalt
{
    namespace
    {
        // Order must match the `constant_id = N` declarations in
        // gff_tonal.frag.glsl.
        struct GffTonalParams
        {
            float exposure;    // [-100, 100]
            float contrast;    // [-100, 100]
            float highlights;  // [-100, 100]
            float shadows;     // [-100, 100]
            float gamma;       // [-100, 100]
        };

        GffTonalParams readParams(Config* pConfig)
        {
            return GffTonalParams{
                pConfig->getOption<float>("gff.exposure",   0.0f),
                pConfig->getOption<float>("gff.contrast",   0.0f),
                pConfig->getOption<float>("gff.highlights", 0.0f),
                pConfig->getOption<float>("gff.shadows",    0.0f),
                pConfig->getOption<float>("gff.gamma",      0.0f),
            };
        }
    } // namespace

    GffTonalEffect::GffTonalEffect(LogicalDevice*       pLogicalDevice,
                                   VkFormat             format,
                                   VkExtent2D           imageExtent,
                                   std::vector<VkImage> inputImages,
                                   std::vector<VkImage> outputImages,
                                   Config*              pConfig)
    {
        static GffTonalParams params;
        params = readParams(pConfig);

        vertexCode   = full_screen_triangle_vert;
        fragmentCode = gff_tonal_frag;

        constexpr size_t kParamCount = sizeof(GffTonalParams) / sizeof(float);
        static std::array<VkSpecializationMapEntry, kParamCount> entries = []() {
            std::array<VkSpecializationMapEntry, kParamCount> e{};
            for (uint32_t i = 0; i < e.size(); ++i)
            {
                e[i].constantID = i;
                e[i].offset     = i * sizeof(float);
                e[i].size       = sizeof(float);
            }
            return e;
        }();

        static VkSpecializationInfo specInfo{};
        specInfo.mapEntryCount = static_cast<uint32_t>(entries.size());
        specInfo.pMapEntries   = entries.data();
        specInfo.dataSize      = sizeof(GffTonalParams);
        specInfo.pData         = &params;

        pVertexSpecInfo   = nullptr;
        pFragmentSpecInfo = &specInfo;

        const VkFormat unormView = convertToUNORM(format);
        Logger::info("gff_tonal: swapchain format=" + convertToString(format)
                     + " using view format=" + convertToString(unormView));

        init(pLogicalDevice, format, imageExtent, inputImages, outputImages, pConfig, unormView);
    }

    GffTonalEffect::~GffTonalEffect() = default;
} // namespace vkBasalt
