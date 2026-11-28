#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;

//Colored test cube for reflection testing. Picks its material from a color
//keyword in the actor's name ("Red"/"Green"/"Blue"/"Orange"), falling back
//to the default (white) material otherwise.
class AColorBox : public FPActor
{
private:
	FPStaticMeshComponent* BoxMesh;

public:
	AColorBox() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};
