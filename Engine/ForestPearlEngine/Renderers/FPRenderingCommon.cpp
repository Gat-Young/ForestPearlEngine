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
		{ "Sampler_CLAMP",               Sampler_CLAMP },
		{ "Sampler_Wrap",                Sampler_Wrap },
		{ "Sampler_Mirror",              Sampler_Mirror },
		{ "Sampler_Mirror_Once",         Sampler_Mirror_Once },
		{ "Sampler_BorderColor_RED",     Sampler_BorderColor_RED },
		{ "Sampler_BorderColor_BLUE",    Sampler_BorderColor_BLUE },
		{ "Sampler_BorderColor_GREEN",   Sampler_BorderColor_GREEN },
		{ "Sampler_BorderColor_MAGENTA", Sampler_BorderColor_MAGENTA }
	};

	auto it = SamplerMap.find(Name);

	if (it != SamplerMap.end())
	{
		return it->second;
	}

	//기본값은 무조건 Clamp
	return Sampler_CLAMP;
}