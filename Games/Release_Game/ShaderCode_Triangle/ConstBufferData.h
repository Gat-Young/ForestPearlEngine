#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Define/FPMath.h"

class ConstBufferData : public FPActor
{
public :
	//Object Const Buffer
	FPMatrix Location;
	FPMatrix Rotation;
	FPMatrix Scale;
	FPMatrix World;

	//VertexShaderConstBuffer
	float r, g, b, a;	//color
	float per;			//Percentage
	float x;			//offset

	//VertexViewPortConstBuffer
	float AniOn;
	float BlendOn;

	//PixelShaderConstBuffer
	float r, g, b, a;	//color
	float per;			//Percentage

	//PixelViewPortConstBuffer
	float AniOn;
	float BlendOn;

public:
	ConstBufferData() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void SetObjectConstBuffer(FPMatrix Location, FPMatrix Rotation, FPMatrix Scale, FPMatrix World);
	void SetVertexShaderConstBuffer(float r, float g, float b, float a, float per, float x);
	void SetVertexViewPortConstBuffer(float AniOn, float BlendOn);
	void SetPixelShaderConstBuffer(float r, float g, float b, float a, float per, float x);
	void SetPixelViewPortConstBuffer(float AniOn; float BlendOn);
};