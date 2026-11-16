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
		{ "RenderTargetTexture2D",  RenderTargetTexture2D },
		{ "DepthStencilTexture2D",  DepthStencilTexture2D },
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

FPRenderTargetFormat StringToFPRenderTargetFormat(const std::string& Name)
{
    static const std::unordered_map<std::string, FPRenderTargetFormat> FPRenderTargetFormatMap =
    {
        { "R8",         FPRenderTargetFormat::R8 },
        { "RG8",        FPRenderTargetFormat::RG8 },
        { "RGBA8",      FPRenderTargetFormat::RGBA8 },
        { "RGBA8_SRGB", FPRenderTargetFormat::RGBA8_SRGB },
        { "R16F",       FPRenderTargetFormat::R16F },
        { "RGBA16F",    FPRenderTargetFormat::RGBA16F },
        { "R32F",       FPRenderTargetFormat::R32F },
        { "RGBA32F",    FPRenderTargetFormat::RGBA32F },
        { "RGBA10A2",   FPRenderTargetFormat::RGBA10A2 },
    };

    auto it = FPRenderTargetFormatMap.find(Name);

    if (it != FPRenderTargetFormatMap.end())
    {
        return it->second;
    }

    //기본값으로 설정된 RGBA8
    return FPRenderTargetFormat::RGBA8;
}

FPDepthStencilFormat StringToFPDepthStencilFormat(const std::string& Name)
{
    static const std::unordered_map<std::string, FPDepthStencilFormat> FPDepthStencilFormatMap =
    {
        { "D16",            FPDepthStencilFormat::D16 },
        { "D24_S8_UINT",     FPDepthStencilFormat::D24_S8_UINT },
        { "D32F",            FPDepthStencilFormat::D32F },
        { "D32F_S8X24_UINT", FPDepthStencilFormat::D32F_S8X24_UINT },
    };

    auto it = FPDepthStencilFormatMap.find(Name);

    if (it != FPDepthStencilFormatMap.end())
    {
        return it->second;
    }

    //기본값으로 설정된 D24_S8_UINT
    return FPDepthStencilFormat::D24_S8_UINT;
}

FPDepthWriteMask StringToFPDepthWriteMask(const std::string& Name)
{
    static const std::unordered_map<std::string, FPDepthWriteMask> FPDepthWriteMaskMap =
    {
        { "ZERO", FPDepthWriteMask::ZERO },
        { "ALL",  FPDepthWriteMask::ALL },
    };

    auto it = FPDepthWriteMaskMap.find(Name);

    if (it != FPDepthWriteMaskMap.end())
    {
        return it->second;
    }

    //기본값으로 설정된 ALL
    return FPDepthWriteMask::ALL;
}

FPCompraisonFunc StringToFPCompraisonFunc(const std::string& Name)
{
    static const std::unordered_map<std::string, FPCompraisonFunc> FPCompraisonFuncMap =
    {
        { "NERVER",        FPCompraisonFunc::NERVER },
        { "LESS",          FPCompraisonFunc::LESS },
        { "EQUAL",         FPCompraisonFunc::EQUAL },
        { "LESS_EQUAL",    FPCompraisonFunc::LESS_EQUAL },
        { "GREATER",       FPCompraisonFunc::GREATER },
        { "NOT_EQUAL",     FPCompraisonFunc::NOT_EQUAL },
        { "GREATER_EQUAL", FPCompraisonFunc::GREATER_EQUAL },
        { "ALWAYS",        FPCompraisonFunc::ALWAYS },
    };

    auto it = FPCompraisonFuncMap.find(Name);

    if (it != FPCompraisonFuncMap.end())
    {
        return it->second;
    }

    //기본값으로 설정된 LESS
    return FPCompraisonFunc::LESS;
}

FPStencilOP StringToFPStencilOP(const std::string& Name)
{
    static const std::unordered_map<std::string, FPStencilOP> FPStencilOPMap =
    {
        { "KEEP",     FPStencilOP::KEEP },
        { "ZERO",     FPStencilOP::ZERO },
        { "REPLACE",  FPStencilOP::REPLACE },
        { "INCR_SAT", FPStencilOP::INCR_SAT },
        { "DECR_SAT", FPStencilOP::DECR_SAT },
        { "INVERT",   FPStencilOP::INVERT },
        { "INCR",     FPStencilOP::INCR },
        { "DECR",     FPStencilOP::DECR },
    };

    auto it = FPStencilOPMap.find(Name);

    if (it != FPStencilOPMap.end())
    {
        return it->second;
    }

    //기본값으로 설정된 KEEP
    return FPStencilOP::KEEP;
}
