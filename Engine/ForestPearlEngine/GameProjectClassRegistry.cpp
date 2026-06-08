#include "GameProjectClassRegistry.h"


std::unique_ptr<FPObject> GameProjectClassRegistry::Create(const std::string& className) const
{
	auto it = factories.find(className);

	if (it == factories.end())
	{
		return nullptr;
	}

	return it->second();
}

bool GameProjectClassRegistry::HasFactory(std::string ClassName)
{
	if (factories.count(ClassName) > 0)
	{
		return true;
	}
	return false;
}
