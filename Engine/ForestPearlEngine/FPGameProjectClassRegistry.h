#pragma once
#include <functional>
#include <typeindex>
#include <memory>
#include <string>
#include <unordered_map>
#include "./Object/Object.h"
#include "FPGameInstanceSubSystem.h"

class FPGameProjectClassRegistry : public FPGameInstanceSubSystem
{
public:
	using CreateFunc = std::function<std::unique_ptr<FPObject>()>;

	template <typename T>
	void Register(const std::string& ClassName)
	{
		static_assert(std::is_base_of_v<FPObject, T>);

		factories[ClassName] = []()
			{
				return std::make_unique<T>();
			};

		ClassTypeMap.insert_or_assign(
			ClassName,
			std::type_index(typeid(T))
		);
	};

	std::unique_ptr<FPObject> Create(const std::string& ClassName) const;
	const std::type_index* GetClassType(const std::string& ClassName) const;

	bool HasFactory(std::string ClassName);

	FPGameProjectClassRegistry() = default;
	~FPGameProjectClassRegistry() = default;

private:
	std::unordered_map<std::string, CreateFunc> factories;
	std::unordered_map<std::string, std::type_index> ClassTypeMap;

	FPGameProjectClassRegistry(const FPGameProjectClassRegistry&) = delete;
	FPGameProjectClassRegistry& operator=(const FPGameProjectClassRegistry&) = delete;
};
