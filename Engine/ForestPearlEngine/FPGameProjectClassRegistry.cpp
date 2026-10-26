#include "FPGameProjectClassRegistry.h"


std::unique_ptr<FPObject> FPGameProjectClassRegistry::Create(const std::string& ClassName) const
{
	auto it = factories.find(ClassName);

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

const std::type_index* FPGameProjectClassRegistry::GetClassType(const std::string& ClassName) const
{
	auto It = ClassTypeMap.find(ClassName);

	if (It == ClassTypeMap.end())
		return nullptr;

	return &It->second;
}