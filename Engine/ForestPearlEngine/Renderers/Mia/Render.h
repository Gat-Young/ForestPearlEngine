#pragma once

////////////////////////////////////////////////////////////
//
// 함수 선언 영역
//
// 렌더링 데이터 불러오기 및 해제
int	DataLoading();
void DataRelease();
int  ObjLoad();
void ObjRelease();
//void ObjUpdate(float dTime);

// 오브젝트 렌더링
void ObjDraw();

// 엔진 및 시스템 상태 갱신
void SystemUpdate();

// 게임 장면 렌더링
void SceneRender();
void ShowInfo();  // 도움말 출력

// T00_Line에서 추가
extern HWND g_hWnd;  // 윈도우 핸들


