#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "./Object/Object.h"

class GameProjectClassRegistry
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

	//Single Tone
	static GameProjectClassRegistry& Get()
	{
		static GameProjectClassRegistry Instance;
		return Instance;
	}

private:
	std::unordered_map<std::string, CreateFunc> factories;

	GameProjectClassRegistry() = default;
	~GameProjectClassRegistry() = default;

	GameProjectClassRegistry(const GameProjectClassRegistry&) = delete;
	GameProjectClassRegistry& operator=(const GameProjectClassRegistry&) = delete;
};
