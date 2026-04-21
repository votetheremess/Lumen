#include "effect_gff_local.hpp"

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
        // gff_local.frag.glsl.
        struct GffLocalParams
        {
            float sharpen;    // [   0, 100]
            float clarity;    // [   0, 100]
            float hdrToning;  // [   0, 100]
            float bloom;      // [   0, 100]
        };

        GffLocalParams readParams(Config* pConfig)
        {
            return GffLocalParams{
                pConfig->getOption<float>("gff.sharpen",   0.0f),
                pConfig->getOption<float>("gff.clarity",   0.0f),
                pConfig->getOption<float>("gff.hdrToning", 0.0f),
                pConfig->getOption<float>("gff.bloom",     0.0f),
            };
        }
    } // namespace

    GffLocalEffect::GffLocalEffect(LogicalDevice*       pLogicalDevice,
                                   VkFormat             format,
                                   VkExtent2D           imageExtent,
                                   std::vector<VkImage> inputImages,
                                   std::vector<VkImage> outputImages,
                                   Config*              pConfig)
    {
        static GffLocalParams params;
        params = readParams(pConfig);

        vertexCode   = full_screen_triangle_vert;
        fragmentCode = gff_local_frag;

        constexpr size_t kParamCount = sizeof(GffLocalParams) / sizeof(float);
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
        specInfo.dataSize      = sizeof(GffLocalParams);
        specInfo.pData         = &params;

        pVertexSpecInfo   = nullptr;
        pFragmentSpecInfo = &specInfo;

        // Pin the image view to the UNORM alias so the hardware sampler
        // never auto-linearizes or sRGB-encodes. Matches the upstream
        // Freestyle math which runs in gamma-encoded sRGB directly.
        const VkFormat unormView = convertToUNORM(format);
        Logger::info("gff_local: swapchain format=" + convertToString(format)
                     + " using view format=" + convertToString(unormView));

        init(pLogicalDevice, format, imageExtent, inputImages, outputImages, pConfig, unormView);
    }

    GffLocalEffect::~GffLocalEffect() = default;
} // namespace vkBasalt
