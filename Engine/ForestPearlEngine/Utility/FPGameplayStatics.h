#pragma once
#include "../Object/Object.h"
#include <string>
#include <vector>

class FPGameplayStatics : public FPObject
{
public:

	virtual void Initialize() override final {};
	virtual void BeginPlay() override final {};
	virtual void Tick() override final {};

	virtual ~FPGameplayStatics() = default;

	static class FPActor* GetActorOfClass(FPWorld* World, std::string ClassName);

	static void GetAllActorsOfClass(FPWorld* World, std::string ClassName, std::vector<FPActor*>& OutActors);
};