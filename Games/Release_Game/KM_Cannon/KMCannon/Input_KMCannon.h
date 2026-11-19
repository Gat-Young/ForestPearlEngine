#pragma once

class FPWorld;
class FPInputComponent;

//////////////////////////////////////////////////////////////////////////////
//
// KMCannon 키 입력 창구 (Loader_KMCannon · API_KMCannon 과 같은 방식)
//
//  키 배치 · 무엇을 쏠지는 전부 여기서 정함. 컨트롤러는 아래 세 줄만 가짐
//    1) Initialize : Input_KMCannon::AddMappings(GetInputComponent());
//    2) Initialize : GetInputComponent().BindMethod(Input_KMCannon::IA_NAME, this, EKeyState::Down, &GameController::KMCannonInput);
//    3) 멤버 함수  : void KMCannonInput(FInputValue) { Input_KMCannon::OnKeyDown(GetWorld()); }
//
//  ※ 엔진 입력은 "컨트롤러(또는 빙의한 폰) 의 멤버 함수" 에만 이벤트를 보냄
//     (FPInputComponent : BindObj 가 PlayerController / PossessedPawn 이 아니면 건너뜀)
//     → 컨트롤러에 받는 함수 하나는 꼭 있어야 하고, 그 함수는 여기로 넘기기만 함
//
//  키 (전부 IA_NAME 하나로 묶고, 눌린 순간 어떤 키가 눌려 있는지 보고 나눔)
//    Enter          : 대포(Cannon) 발사
//    Enter 누른 채 1 : 배 전체 일제 사격
//    Enter 누른 채 2 : 배 왼쪽
//    Enter 누른 채 3 : 배 오른쪽
//
//  ※ 엔진은 키 하나에 액션 하나 (먼저 등록한 것이 이김) → 다른 곳에서 Enter · 1 · 2 · 3 을 쓰면 안 됨
//
namespace Input_KMCannon
{
	extern const char* IA_NAME;

	void AddMappings(FPInputComponent& Input);	//Enter · 1 · 2 · 3 → IA_NAME
	void OnKeyDown(FPWorld* World);				//IA_NAME 의 Down 에서 부름
}
