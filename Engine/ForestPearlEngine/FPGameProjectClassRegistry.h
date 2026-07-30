#pragma once
#include <functional>
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
	void Register(const std::string& className)
	{
		static_assert(std::is_base_of_v<FPObject, T>);

		factories[className] = []()
			{
				return std::make_unique<T>();
			};
	};

	std::unique_ptr<FPObject> Create(const std::string& className) const;

	bool HasFactory(std::string ClassName);

	FPGameProjectClassRegistry() = default;
	~FPGameProjectClassRegistry() = default;

private:
	std::unordered_map<std::string, CreateFunc> factories;

	FPGameProjectClassRegistry(const FPGameProjectClassRegistry&) = delete;
	FPGameProjectClassRegistry& operator=(const FPGameProjectClassRegistry&) = delete;
};
