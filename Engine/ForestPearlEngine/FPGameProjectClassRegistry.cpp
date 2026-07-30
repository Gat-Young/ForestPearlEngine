#include "FPGameProjectClassRegistry.h"


std::unique_ptr<FPObject> FPGameProjectClassRegistry::Create(const std::string& className) const
{
	auto it = factories.find(className);

	if (it == factories.end())
	{
		return nullptr;
	}

	return it->second();
}

bool FPGameProjectClassRegistry::HasFactory(std::string ClassName)
{
	if (factories.count(ClassName) > 0)
	{
		return true;
	}
	return false;
}
