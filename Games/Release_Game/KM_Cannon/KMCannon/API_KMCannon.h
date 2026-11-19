#pragma once

class FPWorld;

//////////////////////////////////////////////////////////////////////////////
//
// KMCannon 외부 호출 창구
//
//  다른 클래스 · 다른 프로젝트는 이 헤더 하나만 include 해서 부름
//  (Cannon.h · CannonFireFX.h 같은 내부 헤더는 몰라도 됨)
//
//  새 기능 = 여기에 함수 하나 + API_KMCannon.cpp 에 몸통 하나
//  → 이미 이 헤더를 include 한 곳에서는 바로 쓸 수 있음
//
namespace API_KMCannon
{
	int FireAll(FPWorld* World);	//레벨의 모든 대포 발사, 쏜 개수 반환
}
