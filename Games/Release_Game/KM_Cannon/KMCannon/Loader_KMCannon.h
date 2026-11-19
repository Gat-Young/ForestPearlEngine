#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// KMCannon 로드 진입점   (대포 · 발사 FX 모듈을 프로젝트에 "꽂는" 곳)
//
//  호스트 프로젝트의 GameProjectLoader.cpp 에서 딱 두 줄만 부름
//    LoadClassRegist() 안 : Loader_KMCannon::RegisterClasses();
//    LoadAssets()      안 : Loader_KMCannon::LoadAssets();
//
//  클래스 · 에셋을 추가 / 수정할 때는 Loader_KMCannon.cpp 만 고침 → 호스트 코드는 그대로
//
namespace Loader_KMCannon
{
	void RegisterClasses();	//레벨 JSON 의 "class" 이름 → C++ 클래스 연결
	void LoadAssets();		//셰이더 · 블렌드 · 텍스처 · 머티리얼 · 메쉬
}
