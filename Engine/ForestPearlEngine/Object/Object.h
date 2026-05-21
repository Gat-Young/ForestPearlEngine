#pragma once

class FPObject
{
	public:
		virtual void BeginPlay() = 0;
		virtual void Tick() = 0;
		
		virtual ~FPObject() = default;
};