#pragma once
class FPObject
{
	protected:
		FPObject* Outer = nullptr;

	public:

		virtual ~FPObject() = default;

		//GetWorld¿ë
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