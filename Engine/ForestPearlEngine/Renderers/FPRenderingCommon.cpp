#include "FPRenderingCommon.h"

Topology StringToTopology(std::string Topo)
{
	static const std::unordered_map<std::string, Topology> SamplerMap =
	{
		{ "TRIANGLELIST",               Topology::TRIANGLELIST },
		{ "TRIANGLESTRIP",                Topology::TRIANGLESTRIP },
		{ "LINELIST",              Topology::LINELIST },
	};

	auto it = SamplerMap.find(Topo);

	if (it != SamplerMap.end())
	{
		return it->second;
	}

	//기본값은 무조건 TriangleList
	return Topology::TRIANGLELIST;
}

FPSamplerType StringToSamplerType(const std::string& Name)
{
    static const std::unordered_map<std::string, FPSamplerType> SamplerMap =
    {
        // Point
        { "Filter_Point_Sampler_CLAMP",               Filter_Point_Sampler_CLAMP },
        { "Filter_Point_Sampler_Wrap",                Filter_Point_Sampler_Wrap },
        { "Filter_Point_Sampler_Mirror",              Filter_Point_Sampler_Mirror },
        { "Filter_Point_Sampler_Mirror_Once",         Filter_Point_Sampler_Mirror_Once },
        { "Filter_Point_Sampler_Border",              Filter_Point_Sampler_Border },
        { "Filter_Point_Sampler_BorderColor_RED",     Filter_Point_Sampler_BorderColor_RED },
        { "Filter_Point_Sampler_BorderColor_BLUE",    Filter_Point_Sampler_BorderColor_BLUE },
        { "Filter_Point_Sampler_BorderColor_GREEN",   Filter_Point_Sampler_BorderColor_GREEN },
        { "Filter_Point_Sampler_BorderColor_MAGENTA", Filter_Point_Sampler_BorderColor_MAGENTA },

        // Bi-Linear
        { "Filter_Bi_Linear_Sampler_CLAMP",               Filter_Bi_Linear_Sampler_CLAMP },
        { "Filter_Bi_Linear_Sampler_Wrap",                Filter_Bi_Linear_Sampler_Wrap },
        { "Filter_Bi_Linear_Sampler_Mirror",              Filter_Bi_Linear_Sampler_Mirror },
        { "Filter_Bi_Linear_Sampler_Mirror_Once",         Filter_Bi_Linear_Sampler_Mirror_Once },
        { "Filter_Bi_Linear_Sampler_Border",              Filter_Bi_Linear_Sampler_Border },
        { "Filter_Bi_Linear_Sampler_BorderColor_RED",     Filter_Bi_Linear_Sampler_BorderColor_RED },
        { "Filter_Bi_Linear_Sampler_BorderColor_BLUE",    Filter_Bi_Linear_Sampler_BorderColor_BLUE },
        { "Filter_Bi_Linear_Sampler_BorderColor_GREEN",   Filter_Bi_Linear_Sampler_BorderColor_GREEN },
        { "Filter_Bi_Linear_Sampler_BorderColor_MAGENTA", Filter_Bi_Linear_Sampler_BorderColor_MAGENTA },

        // Tri-Linear (Mip-Map On)
        { "Filter_Tri_Linear_Sampler_CLAMP",               Filter_Tri_Linear_Sampler_CLAMP },
        { "Filter_Tri_Linear_Sampler_Wrap",                Filter_Tri_Linear_Sampler_Wrap },
        { "Filter_Tri_Linear_Sampler_Mirror",              Filter_Tri_Linear_Sampler_Mirror },
        { "Filter_Tri_Linear_Sampler_Mirror_Once",         Filter_Tri_Linear_Sampler_Mirror_Once },
        { "Filter_Tri_Linear_Sampler_Border",              Filter_Tri_Linear_Sampler_Border },
        { "Filter_Tri_Linear_Sampler_BorderColor_RED",     Filter_Tri_Linear_Sampler_BorderColor_RED },
        { "Filter_Tri_Linear_Sampler_BorderColor_BLUE",    Filter_Tri_Linear_Sampler_BorderColor_BLUE },
        { "Filter_Tri_Linear_Sampler_BorderColor_GREEN",   Filter_Tri_Linear_Sampler_BorderColor_GREEN },
        { "Filter_Tri_Linear_Sampler_BorderColor_MAGENTA", Filter_Tri_Linear_Sampler_BorderColor_MAGENTA },

        // Anistropy
        { "Filter_Anistropy_Linear_Sampler_CLAMP",               Filter_Anistropy_Linear_Sampler_CLAMP },
        { "Filter_Anistropy_Linear_Sampler_Wrap",                Filter_Anistropy_Linear_Sampler_Wrap },
        { "Filter_Anistropy_Linear_Sampler_Mirror",              Filter_Anistropy_Linear_Sampler_Mirror },
        { "Filter_Anistropy_Linear_Sampler_Mirror_Once",         Filter_Anistropy_Linear_Sampler_Mirror_Once },
        { "Filter_Anistropy_Linear_Sampler_Border",              Filter_Anistropy_Linear_Sampler_Border },
        { "Filter_Anistropy_Linear_Sampler_BorderColor_RED",     Filter_Anistropy_Linear_Sampler_BorderColor_RED },
        { "Filter_Anistropy_Linear_Sampler_BorderColor_BLUE",    Filter_Anistropy_Linear_Sampler_BorderColor_BLUE },
        { "Filter_Anistropy_Linear_Sampler_BorderColor_GREEN",   Filter_Anistropy_Linear_Sampler_BorderColor_GREEN },
        { "Filter_Anistropy_Linear_Sampler_BorderColor_MAGENTA", Filter_Anistropy_Linear_Sampler_BorderColor_MAGENTA }
    };

	auto it = SamplerMap.find(Name);

	if (it != SamplerMap.end())
	{
		return it->second;
	}

	//기본값은 무조건 Filter_Tri_Linear_Sampler_Clamp
	return Filter_Tri_Linear_Sampler_CLAMP;
}

FPTextureType StringToTextureType(const std::string& Name)
{
	static const std::unordered_map<std::string, FPTextureType> TextureTypeMap =
	{
		{ "Texture2D",      Texture2D },
		{ "Texture2DArray", Texture2DArray },
	};

	auto it = TextureTypeMap.find(Name);

	if (it != TextureTypeMap.end())
	{
		return it->second;
	}

	return Texture2D;
}

FPShaderType StringToFPShaderType(const std::string& ShaderType)
{
    static const std::unordered_map<std::string, FPShaderType> FPShaderTypeMap =
    {
        { "VS",      FPShaderType::VS },
        { "PS", FPShaderType::PS },
    };

    auto it = FPShaderTypeMap.find(ShaderType);

    if (it != FPShaderTypeMap.end())
    {
        return it->second;
    }

    return FPShaderType::None;
}
