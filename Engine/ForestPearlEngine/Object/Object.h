#pragma once
//Object는 World 코드를 어떻게 알지?

class FPObject
{
	protected:
		FPObject* Outer = nullptr;

	public:
		virtual void Initialize() = 0;
		virtual void BeginPlay() = 0;
		virtual void Tick() = 0;

		virtual ~FPObject() = default;

		//GetWorld용
		void SetOuter(FPObject* Outer) { this->Outer = Outer; }

		virtual class FPWorld* GetWorld()
		{
			if (FPObject* Outer = GetOuter())
			{
				return Outer->GetWorld();
			}
		}

		virtual FPObject* GetOuter()
		{
			return Outer;
		}
};