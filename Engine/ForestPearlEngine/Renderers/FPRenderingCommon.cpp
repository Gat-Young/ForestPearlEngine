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

FPSamplerFilterType StringToFPSamplerFilterType(const std::string& Name)
{
	static const std::unordered_map<std::string, FPSamplerFilterType> FilterMap =
	{
		{ "Point",              FPSamplerFilterType::Point },
		{ "BiLinear",           FPSamplerFilterType::BiLinear },
		{ "TriLinear",          FPSamplerFilterType::TriLinear },
		{ "Anisotropic",        FPSamplerFilterType::Anisotropic },
		{ "ComparisonBiLinear", FPSamplerFilterType::ComparisonBiLinear },
	};

	auto it = FilterMap.find(Name);

	if (it != FilterMap.end())
	{
		return it->second;
	}

	//기본값은 TriLinear
	return FPSamplerFilterType::TriLinear;
}

FPTextureAddressMode StringToFPTextureAddressMode(const std::string& Name)
{
	static const std::unordered_map<std::string, FPTextureAddressMode> AddressMap =
	{
		{ "CLAMP",       FPTextureAddressMode::CLAMP },
		{ "WRAP",        FPTextureAddressMode::WRAP },
		{ "MIRROR",      FPTextureAddressMode::MIRROR },
		{ "MIRROR_ONCE", FPTextureAddressMode::MIRROR_ONCE },
		{ "BORDER",      FPTextureAddressMode::BORDER },
	};

	auto it = AddressMap.find(Name);

	if (it != AddressMap.end())
	{
		return it->second;
	}

	//기본값은 CLAMP
	return FPTextureAddressMode::CLAMP;
}

FPTextureType StringToTextureType(const std::string& Name)
{
	static const std::unordered_map<std::string, FPTextureType> TextureTypeMap =
	{
		{ "Texture2D",      Texture2D },
		{ "Texture2DArray", Texture2DArray },
		{ "RenderTargetTexture2D",  RenderTargetTexture2D },
		{ "DepthStencilTexture2D",  DepthStencilTexture2D },
		{ "StencilTexture2D",       StencilTexture2D },
		{ "CubeMap",        CubeMap },
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

FPFillMode StringToFPFillMode(const std::string& Name)
{
	static const std::unordered_map<std::string, FPFillMode> FPFillModeMap =
	{
		{ "SOLID",     FPFillMode::SOLID },
		{ "WIREFRAME", FPFillMode::WIREFRAME },
	};

	auto it = FPFillModeMap.find(Name);

	if (it != FPFillModeMap.end())
	{
		return it->second;
	}

	//Default : SOLID
	return FPFillMode::SOLID;
}

FPCullMode StringToFPCullMode(const std::string& Name)
{
	static const std::unordered_map<std::string, FPCullMode> FPCullModeMap =
	{
		{ "NONE",  FPCullMode::NONE },
		{ "FRONT", FPCullMode::FRONT },
		{ "BACK",  FPCullMode::BACK },
	};

	auto it = FPCullModeMap.find(Name);

	if (it != FPCullModeMap.end())
	{
		return it->second;
	}

	//Default : NONE
	return FPCullMode::NONE;
}

FPBlend StringToFPBlend(const std::string& Name)
{
	static const std::unordered_map<std::string, FPBlend> FPBlendMap =
	{
		{ "ZERO",             FPBlend::ZERO },
		{ "ONE",              FPBlend::ONE },
		{ "SRC_COLOR",        FPBlend::SRC_COLOR },
		{ "INV_SRC_COLOR",    FPBlend::INV_SRC_COLOR },
		{ "SRC_ALPHA",        FPBlend::SRC_ALPHA },
		{ "INV_SRC_ALPHA",    FPBlend::INV_SRC_ALPHA },
		{ "DEST_ALPHA",       FPBlend::DEST_ALPHA },
		{ "INV_DEST_ALPHA",   FPBlend::INV_DEST_ALPHA },
		{ "DEST_COLOR",       FPBlend::DEST_COLOR },
		{ "INV_DEST_COLOR",   FPBlend::INV_DEST_COLOR },
		{ "SRC_ALPHA_SAT",    FPBlend::SRC_ALPHA_SAT },
		{ "BLEND_FACTOR",     FPBlend::BLEND_FACTOR },
		{ "INV_BLEND_FACTOR", FPBlend::INV_BLEND_FACTOR },
		{ "SRC1_COLOR",       FPBlend::SRC1_COLOR },
		{ "INV_SRC1_COLOR",   FPBlend::INV_SRC1_COLOR },
		{ "SRC1_ALPHA",       FPBlend::SRC1_ALPHA },
		{ "INV_SRC1_ALPHA",   FPBlend::INV_SRC1_ALPHA },
	};

	auto it = FPBlendMap.find(Name);

	if (it != FPBlendMap.end())
	{
		return it->second;
	}

	//기본값으로 설정된 ONE
	return FPBlend::ONE;
}

FPBlendOp StringToFPBlendOp(const std::string& Name)
{
	static const std::unordered_map<std::string, FPBlendOp> FPBlendOpMap =
	{
		{ "ADD",          FPBlendOp::ADD },
		{ "SUBTRACT",     FPBlendOp::SUBTRACT },
		{ "REV_SUBTRACT", FPBlendOp::REV_SUBTRACT },
		{ "MIN",          FPBlendOp::MIN },
		{ "MAX",          FPBlendOp::MAX },
	};

	auto it = FPBlendOpMap.find(Name);

	if (it != FPBlendOpMap.end())
	{
		return it->second;
	}

	//기본값으로 설정된 ADD
	return FPBlendOp::ADD;
}

FPColorWriteMask StringToFPColorWriteMask(const std::string& Name)
{
	static const std::unordered_map<std::string, FPColorWriteMask> FPColorWriteMaskMap =
	{
		{ "RED",   FPColorWriteMask::RED },
		{ "GREEN", FPColorWriteMask::GREEN },
		{ "BLUE",  FPColorWriteMask::BLUE },
		{ "ALPHA", FPColorWriteMask::ALPHA },
	};

	auto it = FPColorWriteMaskMap.find(Name);

	if (it != FPColorWriteMaskMap.end())
	{
		return it->second;
	}

	//기본값으로 설정된 ALPHA
	return FPColorWriteMask::ALPHA;
}

FPMaterialDetail StringToFPMaterialDetail(const std::string& Name)
{
	static const std::unordered_map<std::string, FPMaterialDetail> FPMaterialDetailMap =
	{
		{ "Opaque",      FPMaterialDetail::Opaque },
		{ "Masked",      FPMaterialDetail::Masked },
		{ "Translucent", FPMaterialDetail::Translucent },
	};

	auto it = FPMaterialDetailMap.find(Name);

	if (it != FPMaterialDetailMap.end())
	{
		return it->second;
	}

	return FPMaterialDetail::Opaque;
}
