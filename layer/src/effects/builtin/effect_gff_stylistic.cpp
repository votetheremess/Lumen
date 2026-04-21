#include "effect_gff_stylistic.hpp"

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
        // gff_stylistic.frag.glsl.
        struct GffStylisticParams
        {
            float vignette;     // [0, 100]
            float bwIntensity;  // [0, 100]
        };

        GffStylisticParams readParams(Config* pConfig)
        {
            return GffStylisticParams{
                pConfig->getOption<float>("gff.vignette",    0.0f),
                pConfig->getOption<float>("gff.bwIntensity", 0.0f),
            };
        }
    } // namespace

    GffStylisticEffect::GffStylisticEffect(LogicalDevice*       pLogicalDevice,
                                           VkFormat             format,
                                           VkExtent2D           imageExtent,
                                           std::vector<VkImage> inputImages,
                                           std::vector<VkImage> outputImages,
                                           Config*              pConfig)
    {
        static GffStylisticParams params;
        params = readParams(pConfig);

        vertexCode   = full_screen_triangle_vert;
        fragmentCode = gff_stylistic_frag;

        constexpr size_t kParamCount = sizeof(GffStylisticParams) / sizeof(float);
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
        specInfo.dataSize      = sizeof(GffStylisticParams);
        specInfo.pData         = &params;

        pVertexSpecInfo   = nullptr;
        pFragmentSpecInfo = &specInfo;

        const VkFormat unormView = convertToUNORM(format);
        Logger::info("gff_stylistic: swapchain format=" + convertToString(format)
                     + " using view format=" + convertToString(unormView));

        init(pLogicalDevice, format, imageExtent, inputImages, outputImages, pConfig, unormView);
    }

    GffStylisticEffect::~GffStylisticEffect() = default;
} // namespace vkBasalt
