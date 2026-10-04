<div align="center">

# 🌲 ForestPearlEngine

**DirectX 11 기반 C++ 게임 엔진 — Actor / Component 프레임워크와 데이터 기반 레벨 구성**

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![DirectX 11](https://img.shields.io/badge/DirectX-11-107C10?style=for-the-badge&logo=xbox&logoColor=white)
![Windows](https://img.shields.io/badge/Windows-10%20%7C%2011-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Visual Studio](https://img.shields.io/badge/Visual%20Studio-2022%20(v143)-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white)

[개요](#-개요) ·
[폴더 구조](#-폴더-구조) ·
[아키텍처](#-아키텍처) ·
[메인 루프](#-엔진-메인-루프) ·
[렌더링](#-렌더링-구조) ·
[입력](#-input-시스템) ·
[에셋](#-assets-구조) ·
[빌드](#-build-시스템) ·
[사용 가이드](#-콘텐츠-프로그래머-가이드) ·
[Demo](#-demo-프로젝트--tri_world)

</div>

---

## 📖 개요

**ForestPearlEngine**은 Win32 + DirectX 11 위에 직접 구현한 C++17 게임 엔진입니다.
엔진·게임·실행기를 서로 다른 프로젝트로 분리하고, 게임 코드는 엔진이 선언한 **5개의 Hook 함수**만 구현하면 실행 파일로 패키징됩니다.

| | 핵심 기능 | 설명 |
|:-:|---|---|
| 🧩 | **Actor / Component 프레임워크** | `FPObject → FPActor → FPPawn / FPAController / FPAGameMode`, 계층형 `FPSceneComponent` 트랜스폼 |
| 🎨 | **DX11 렌더러** | `Renderer`(프레임 흐름) + `RenderingDevice`(D3D11 래퍼) 2계층, MSAA x4, Priority 기반 드로우, 멀티 뷰포트 |
| 🎮 | **Input 시스템** | Raw Input(키보드·마우스) + XInput(게임패드) → Input Mapping Context → Input Action → 멤버 함수 바인딩 |
| 🕹️ | **Possess** | 컨트롤러가 Pawn을 빙의(Possess)하며, 빙의된 Pawn의 바인딩만 입력을 받음 |
| 🔩 | **Socket Attach** | StaticMesh JSON에 정의한 Socket에 컴포넌트/액터를 부착 |
| 📷 | **SpringArm + Camera** | 컨트롤러 회전을 따라가는 3인칭 카메라 |
| 📦 | **에셋 파이프라인** | FBX(ufbx) → VertexBuffer, JSON(Level / StaticMesh), 사전 컴파일 셰이더(`.vso` / `.pso`) |
| 🏭 | **클래스 레지스트리** | 문자열 이름 → 팩토리. Level JSON의 `"class"` 문자열로 액터를 생성 |
| 🛠️ | **자동 패키징** | Release\|x64 빌드 후 PowerShell 스크립트가 `<게임이름>.exe` + `Assets/`를 구성 |

---

## 📁 폴더 구조

### 최상위

```text
ForestPearlEngine/
├── ForestPearlEngine.sln          # 솔루션 (Engine / Runners / Games)
├── Build/
│   └── PackageGame.ps1            # Release 패키징 스크립트
├── Engine/
│   └── ForestPearlEngine/         # 엔진 본체 (Static Library)
├── Runners/
│   └── Runner/                    # 실행 파일 프로젝트 (main)
├── Games/
│   ├── Release_Game/              # 엔진 현재 버전과 연결되는 게임 프로젝트
│   │   ├── Tri_World/             #   ★ Demo 프로젝트 (솔루션에 포함)
│   │   ├── ShaderCode_Triangle/   #   커스텀 Material / 상수 버퍼 샘플
│   │   ├── Geometry_ModelSpace/
│   │   ├── SolarSystem_Ver02/
│   │   └── DX11_SetUp_Font/
│   └── Dev_Game/                  # 레거시 렌더러 시절의 학습용 프로젝트
│       ├── BaseGame/
│       ├── 05.Vertex(2D)+Face_Filling/
│       ├── DOHWA(Interface+GUID+Query)/
│       ├── DrawSolarSystem/
│       ├── HW_03_World+Rotation/
│       └── T06_Transform(I)_07_MatrixComp_(Move)_02/
├── GameTemplate/
│   ├── Readme.txt
│   └── TriWorldTemplate_Ver1.0.zip   # 새 게임 프로젝트용 템플릿
└── x64/                           # 빌드 산출물 (git 미추적)
    ├── Debug/
    └── Release/                   #   Tri_World.exe + Assets/
```

### Engine/ForestPearlEngine

```text
Engine/ForestPearlEngine/
├── ForestPearlEngine.h / .cpp        # 엔진 싱글톤: 윈도우, 메시지 펌프, 메인 루프
├── EngineLoader.h                    # 엔진 기본 에셋(Default Shader) 로드
├── GameProjectLoader.h               # 게임 프로젝트가 구현해야 하는 Hook 함수 선언
├── framework.h                       # Win32 공통 헤더
├── MCLOG.h                           # MCLOG(LogMC / WarningMC / ErrorMC) 로그 매크로
├── Base.cpp                          # 초기 콘솔 실험 코드 (빌드 제외)
│
├── FPGameInstance.h / .cpp           # GameInstance 싱글톤 + SubSystem 보관
├── FPGameInstanceSubSystem.h         # SubSystem 공통 베이스
├── FPGameTimer.h / .cpp              # [SubSystem] 고해상도 타이머 (DeltaTime)
├── FPGameProjectClassRegistry.h/.cpp # [SubSystem] 클래스 이름 → 팩토리
├── FPGameProjectSetting.h / .cpp     # [SubSystem] 윈도우/디스플레이 설정
├── FPAssetLoader.h / AssetLoader.cpp # [SubSystem] FBX · JSON · Shader 로딩
├── FPAssetManager.h / .cpp           # [SubSystem] 로딩된 에셋 보관소
├── FPMeshRenderList.h / .cpp         # [SubSystem] 메시 RenderItem 목록
├── FPTextRenderList.h / .cpp         # [SubSystem] 텍스트 UIContextItem 목록
├── FPCameraList.h / .cpp             # [SubSystem] CameraItem 목록
├── FPViewPortClient.h / .cpp         # [SubSystem] 뷰포트 관리
│
├── FPWorld.h / .cpp                  # World: Level 로드, Actor 목록, GameMode
├── FPAGameMode.h / .cpp              # GameMode: Controller 생성/Tick
├── FPAController.h / .cpp            # Controller: InputComponent, Possess, ControlRotation
│
├── Object/
│   ├── Object.h                      # FPObject (Outer 체인, GetWorld)
│   ├── Actor.h / .cpp                # FPActor (RootComponent, Attach, Transform)
│   ├── FPPawn.h / .cpp               # FPPawn (Possess 대상)
│   └── Components/
│       ├── InputComponent.h / .cpp        # 입력 큐 처리 + 바인딩 호출
│       ├── InputMappingContext.h / .cpp   # Key → InputAction 매핑
│       └── InputAction.h / .cpp           # 바인딩 목록
│
├── FPActorComponent.h                # 컴포넌트 베이스 (Owner)
├── FPSceneComponent.h / .cpp         # 트랜스폼 계층 / Socket
├── FTransform.h                      # Location / Rotation / Quaternion / Scale + 행렬
├── FPPrimitiveComponent.h            # 지오메트리 컴포넌트 베이스
├── FPMeshComponent.h / .cpp          # 메시 공통 (Fill / Cull / Priority / Material)
├── FPStaticMeshComponent.h / .cpp    # StaticMesh 렌더 + Socket Transform
├── FPCameraComponent.h / .cpp        # View / Projection 계산
├── FPSpringArmComponent.h / .cpp     # 스프링 암 (EndPoint Socket)
├── GizmoComponent.h / .cpp           # Grid / Axis 라인 메시
├── FPTextComponent.h / .cpp          # 2D 텍스트
├── FPMovementComponent.h / .cpp      # 이동 컴포넌트 (골격만 존재)
├── FPMovementCompoent.cpp            #   └ 빈 파일
├── InputValue.h                      # FInputValue
│
├── FPStreamableRenderAssetr.h        # 렌더 에셋 베이스
├── FPStaticMesh.h / .cpp             # StaticMesh 에셋 (VB, Socket, Material)
├── FPMaterialInterface.h             # Material 인터페이스
├── FPMaterial.h / .cpp               # 기본 Material (Default Shader)
│
├── Define/
│   ├── FPMath.h / .cpp               # Vector / Quaternion / Matrix
│   └── FPDataDefine.h / .cpp         # VERTEX, Topology, 에셋 데이터 구조체
├── Systems/
│   ├── InputSystem.h / .cpp          # [SubSystem] Raw Input + XInput
│   └── KeyStateEnum.h                # EKeyState
├── Shader/
│   └── ShaderFactory.h / .cpp        # 셰이더 로드/컴파일, InputLayout
├── Renderers/
│   ├── Renderer.h / .cpp             # 프레임 렌더링 흐름
│   ├── RenderingDevice.h / .cpp      # D3D11 Device 래퍼
│   ├── FPRenderingCommon.h           # RenderItem / CameraItem / UIContextItem
│   └── Legacy/                       # 이전 세대 렌더러 (솔루션 미포함)
│       ├── Legacy_Renderer.h / .cpp  #   FPRHI 기반 렌더러
│       ├── FPRHI/                    #   RHI 추상 인터페이스
│       ├── GDI/                      #   GDI 백엔드
│       ├── DirectX/                  #   DXRHI 백엔드
│       ├── Mia/ , MIARHI/            #   소프트웨어 렌더러 "MIA"
│       └── DOHWA/ , DOHWARHI/        #   소프트웨어 렌더러 "DOHWA(圖畵)"
├── Utility/
│   ├── FPPathManager.h / .cpp        # 에셋 경로 해석 (Debug / Release)
│   └── FPGameplayStatics.h / .cpp    # 액터 검색 유틸
├── Assets/
│   └── Shader/
│       ├── DefaultVertexShader.vsh   # 기본 VS (VS_Main)
│       ├── DefaultPixelShader.psh    # 기본 PS (PS_Main)
│       ├── DefaultShader.fx          # 참고용 (빌드 제외)
│       └── bin/                      # 빌드 시 생성: *.vso / *.pso (git 미추적)
└── Libraries/
    ├── DirectXTK/                    # SpriteBatch / SpriteFont
    ├── Ufbx/                         # FBX 로더 (ufbx.c / ufbx.h)
    └── nlohmann/                     # JSON (json.hpp)
```

### Runners/Runner

```text
Runners/Runner/
├── Main.cpp                    # main() → Runner::Run()
├── Runner.h / .cpp             # 엔진 구동 순서 호출
├── Runner.vcxproj              # Application + PackageReleaseGame 타깃
└── ReleaseGameProject.props    # GameProjectName / GameAssetDir 매크로
```

### Games/Release_Game/Tri_World

```text
Games/Release_Game/Tri_World/
├── Tri_World.vcxproj              # Static Library
├── GameProjectLoader.cpp          # ★ 엔진 Hook 함수 구현
├── GameMode.h / .cpp              # GameMode
├── GameController.h / .cpp        # 키 매핑 · Possess 전환 · 전역 토글
├── Player.h / .cpp                # Pawn (ToonLink + SpringArm + Camera)
├── Windmill.h / .cpp              # Pawn (풍차 몸체)
├── WindmillWing.h / .cpp          # Pawn (날개 1개, Socket 부착)
├── TripleWindmillWing.h / .cpp    # Pawn (날개 3단 체인)
├── TripleWingWindmill.h / .cpp    # Pawn (몸체 + 날개 3개 일체형)
├── Terrain.h / .cpp               # Actor (지형)
├── Tree.h / .cpp                  # Actor (나무)
├── Grid.h / .cpp                  # Actor (Gizmo Grid)
├── Axis.h / .cpp                  # Actor (Gizmo Axis)
├── UI.h / .cpp                    # Actor (텍스트 UI)
└── Assets/
    ├── Font/        Consolas10.sfont, 굴림9k.sfont
    ├── Level/       TriWorld.json
    ├── Model/       Terrain/ · ToonLink/ · Tree/ · Windmill/   (*.fbx)
    └── StaticMesh/  *_StaticMesh.json (6개)
```

---

## 🏛 아키텍처

### 프로젝트 의존 관계

엔진과 게임은 **Static Library**, Runner만 **Application**입니다.
엔진은 게임 코드를 모르고, 게임이 구현한 Hook 함수를 **링크 타임**에 연결합니다.

```mermaid
flowchart LR
    subgraph EXE["Runner (Application)"]
        M["main()"] --> R["Runner::Run()"]
    end
    subgraph ENG["ForestPearlEngine (Static Library)"]
        E["ForestPearlEngine"]
        H["GameProjectLoader.h<br/>Hook 선언"]
    end
    subgraph GAME["Tri_World (Static Library)"]
        G["GameProjectLoader.cpp<br/>Hook 구현"]
        A["Actor / Pawn / Controller"]
    end
    R -->|"호출"| E
    E -->|"호출"| H
    H -. "링크 타임 결합" .-> G
    G -->|"Register / Load"| A
    A -->|"엔진 헤더 include"| ENG
```

### 객체 계층

```mermaid
classDiagram
    FPObject <|-- FPGameInstance
    FPObject <|-- FPWorld
    FPObject <|-- FPActor
    FPObject <|-- FPActorComponent
    FPObject <|-- FPMaterialInterface
    FPObject <|-- FPStreamableRenderAsset
    FPActor <|-- FPPawn
    FPActor <|-- FPAController
    FPActor <|-- FPAGameMode
    FPActorComponent <|-- FPSceneComponent
    FPActorComponent <|-- FPMovementComponent
    FPSceneComponent <|-- FPPrimitiveComponent
    FPSceneComponent <|-- FPSpringArmComponent
    FPPrimitiveComponent <|-- FPMeshComponent
    FPPrimitiveComponent <|-- FPCameraComponent
    FPPrimitiveComponent <|-- GizmoComponent
    FPMeshComponent <|-- FPStaticMeshComponent
    FPMaterialInterface <|-- FPMaterial
    FPStreamableRenderAsset <|-- FPStaticMesh
```

`FPObject`는 `Outer` 포인터로 소유 체인을 형성합니다 (`Actor → World → GameInstance`).
어느 객체에서든 `GetWorld()`를 호출하면 Outer를 따라 올라가 `FPWorld`를 찾습니다.

### GameInstance SubSystem

`FPGameInstance`는 싱글톤이며, 게임 실행 중 하나만 존재해야 하는 시스템 10개를 배열로 보관합니다.
접근 방법은 모두 같습니다.

```cpp
FPAssetLoader* AssetLoader =
    static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
```

| SubSystem | Getter | 역할 |
|---|---|---|
| `FPGameTimer` | `GetGameTimer()` | `QueryPerformanceCounter` 기반. `DeltaTime()`(초), `DeltaTimeMS()`, `TotalTime()`, `Start/Stop/Reset` |
| `FPGameProjectClassRegistry` | `GetClassRegister()` | `Register<T>("이름")`, `Create("이름")`, `HasFactory("이름")` |
| `FPInputSystem` | `GetInputSystem()` | Raw Input / XInput 수집, 입력 큐 |
| `FPAssetManager` | `GetAssetManager()` | Mesh · VertexBuffer · StaticMesh · Level · GameMode · Shader 저장소 |
| `FPAssetLoader` | `GetAssetLoader()` | FBX / Level JSON / StaticMesh JSON / Shader 로딩 |
| `FPMeshRenderList` | `GetMeshRenderList()` | 메시 `RenderItem` 등록/해제 |
| `FPTextRenderList` | `GetTextRenderList()` | 텍스트 `UIContextItem` 등록/해제 |
| `FPCameraList` | `GetCameraList()` | `CameraItem` 등록/해제 |
| `FPGameProjectSetting` | `GetGameProjectSetting()` | 윈도우 클래스/타이틀, 창 크기(기본 960×600), 클라이언트 영역 크기, Aspect |
| `FPViewPortClient` | `GetViewPortClient()` | `MainGameViewPort` / `UIViewPort` / `TripleWaySplitViewPort` |

### 엔진 구성 요소 전체 목록

<details open>
<summary><b>코어 · 프레임워크</b></summary>

| 클래스 | 파일 | 설명 |
|---|---|---|
| `ForestPearlEngine` | `ForestPearlEngine.h/.cpp` | 엔진 싱글톤. 윈도우 생성, `WndProc`, 메시지 펌프, `PreInitialize / Initialize / GameLoop / Finalize`, GPU·모니터·VRAM 정보 조회 API, `SetZEnable` |
| `FPGameInstance` | `FPGameInstance.h/.cpp` | SubSystem 보관, `OpenLevel`, World 생명주기 전달 |
| `FPWorld` | `FPWorld.h/.cpp` | Level JSON으로 Actor 생성, `SpawnActor`, `GetController`, `GetAuthGameMode`, `GetGameTimer` |
| `FPAGameMode` | `FPAGameMode.h/.cpp` | `ControllerList`(클래스 이름)로 Controller 생성 후 Tick. 0번이 Player Controller |
| `FPAController` | `FPAController.h/.cpp` | `FPInputComponent` 소유, `Possess / UnPossess / GetPawn`, `AddYaw/Pitch/RollInput`(−180~180 정규화) |
| `FPObject` | `Object/Object.h` | `Outer`, `GetWorld()` |
| `FPActor` | `Object/Actor.h/.cpp` | `RootComponent`, `Initialize / BeginPlay / Tick`, `AttachToActor / AttachToComponent / DetachFromActor`, `SetRootComponent`, World Transform Get/Set |
| `FPPawn` | `Object/FPPawn.h/.cpp` | `PossessedBy / UnPossesed`, `GetController`, `AddControllerYaw/Pitch/RollInput` |
| `FPGameplayStatics` | `Utility/FPGameplayStatics.h/.cpp` | `GetActorOfClass`, `GetAllActorsOfClass`, `GetAllActors` |
| `FPPathManager` | `Utility/FPPathManager.h/.cpp` | 구성(Debug/Release)에 따른 에셋 경로, `StringToWString` |
| `MCLOG` | `MCLOG.h` | `MCLOG(LogMC, "fmt %d", v)` → `Log: [Class::Func] 메시지` |

</details>

<details open>
<summary><b>수학 · 데이터 정의</b></summary>

| 타입 | 파일 | 설명 |
|---|---|---|
| `FPVector2/3/4` | `Define/FPMath.h` | 벡터. `FPVector3`는 `+ - *`, `Length`, `Normalize` |
| `FPQuaternion` | `Define/FPMath.h` | 사원수 곱, `ToEuler`, `Normalize` |
| `FPMatrix` | `Define/FPMath.h` | `DirectX::XMMATRIX` 래퍼. `*`, `MatrixInverse`, `Transpose` |
| 함수 | `Define/FPMath.h` | `DegToRad / RadToDeg / Clamp`, `Rotate / Conjugate / Inverse / FromEuler / AngleAxis`, `NormalizeAxis`, `MatrtixTranslation / MatrixRotationQuaternion / MatrixScaling`, `MatrixLookAtLH / MatrixLookToLH / MatrixPerspectiveFovLH` |
| `FTransform` | `FTransform.h` | Location, Rotation(Euler, Degree), QuaternionRotation, Scale + 각 행렬, `Right / Up / Foraward` |
| `VERTEX` | `Define/FPDataDefine.h` | `x, y, z` + `r, g, b, a` (28 byte) |
| `Topology` | `Define/FPDataDefine.h` | `TRIANGLELIST`, `TRIANGLESTRIP`, `LINELIST` |
| 데이터 구조체 | `Define/FPDataDefine.h` | `FPMeshData`, `FPStaticMeshData`, `SOCKET_TRANSFORM`, `FPVertexBufferData`, `FPActorData` |
| `FInputValue` | `InputValue.h` | `X, Y, Z, bBool, Float` |

> 좌표계는 **왼손 좌표계, Y-Up, +Z 전방**입니다. 회전 벡터는 `{ Pitch(X), Yaw(Y), Roll(Z) }`이며 단위는 Degree입니다.

</details>

---

## 🔄 엔진 메인 루프

### 진입점

```cpp
// Runners/Runner/Runner.cpp
void Runner::Run()
{
    ForestPearlEngine& FPEngine = ForestPearlEngine::GetGameEngine();

    if (!FPEngine.PreInitialize()) return;   // 부팅
    if (!FPEngine.Initialize())    return;   // 레벨 시작
    FPEngine.GameLoop();                     // 메인 루프
    FPEngine.Finalize();                     // 종료
}
```

### 부팅 순서

```mermaid
sequenceDiagram
    participant Run as Runner
    participant Eng as ForestPearlEngine
    participant Game as Game Hook
    participant GI as FPGameInstance
    participant Ren as Renderer

    Run->>Eng: PreInitialize()
    Eng->>GI: Get() - SubSystem 10개 생성
    Eng->>Game: RegistProjectName()
    Note over Eng: CreateFPEWindow()<br/>ViewPort 생성<br/>Raw Input 등록
    Eng->>Ren: InitializeRenderer(hwnd)
    Note over Eng: LoadEngineAssets() - Default Shader
    Eng->>Game: LoadLevel()
    Eng->>Game: LoadClassRegist()
    Eng->>Game: LoadAssets()

    Run->>Eng: Initialize()
    Eng->>Game: ReturnStartLevel()
    Eng->>GI: OpenLevel(이름) - GameMode / Actor 생성
    Eng->>GI: Initialize() - GameMode, Actor 순
    Eng->>GI: BeginPlay() - GameMode, Actor 순

    Run->>Eng: GameLoop()
    Run->>Eng: Finalize()
```

| 단계 | 함수 | 내용 |
|:-:|---|---|
| 1 | `PreInitialize()` | GameInstance 생성 → `RegistProjectName()` → 윈도우 생성 → ViewPort 생성 → Raw Input 등록 → Renderer 초기화 → 엔진 에셋 로드 → `LoadLevel()` → `LoadClassRegist()` → `LoadAssets()` |
| 2 | `Initialize()` | `OpenLevel(ReturnStartLevel())` → `World::Initialize()` → `World::BeginPlay()` |
| 3 | `GameLoop()` | 아래 프레임 루프 |
| 4 | `Finalize()` | `World::Finalize()`(Actor 삭제) → `Renderer::Finalize()` |

`FPWorld::OpenLevel`은 Level JSON의 `gamemode` 문자열로 GameMode를 만들고, `actors` 배열의 각 항목을 `ClassRegistry`로 생성해 이름과 Transform을 적용합니다.

### 프레임 루프

```cpp
// Engine/ForestPearlEngine/ForestPearlEngine.cpp
void ForestPearlEngine::GameLoop()
{
    while (bEngineLoop)
    {
        if (!MessagePump()) break;      // ① Windows 메시지

        FPGameInstance::Get().Tick();   // ② 게임 로직

        Render->ClearBackBuffer();      // ③ 렌더링
        Render->ObjectRendering();
        Render->UIRendering();
        Render->RenderTargetPresent();
    }
}
```

```mermaid
flowchart TD
    A["① MessagePump<br/>PeekMessage 루프"] -->|"WM_QUIT"| Z["루프 종료"]
    A --> B["② FPGameInstance::Tick"]
    B --> B1["InputSystem::TickInputSystem<br/>Pressed 이벤트 생성 + 게임패드 폴링"]
    B1 --> B2["GameTimer::Tick<br/>DeltaTime 갱신"]
    B2 --> B3["World::Tick"]
    B3 --> B4["GameMode::Tick → Controller::Tick<br/>입력 큐 처리, ControlRotation 적용"]
    B4 --> B5["모든 Actor::Tick<br/>RootComponent::Tick 재귀 - 행렬 갱신"]
    B5 --> C["③ ClearBackBuffer"]
    C --> D["ObjectRendering"]
    D --> E["UIRendering"]
    E --> F["RenderTargetPresent - VSync"]
    F --> A
```

**① 메시지 펌프** — 큐가 빌 때까지 `PeekMessage`로 처리합니다. `WndProc`이 처리하는 메시지는 다음과 같습니다.

| 메시지 | 처리 |
|---|---|
| `WM_NCCREATE` | `lpCreateParams`의 엔진 포인터를 `GWLP_USERDATA`에 저장 |
| `WM_INPUT` | `FPInputSystem::HandleRawInput` |
| `WM_ACTIVATE` | `FPInputSystem::ResetKeyStates` (포커스 전환 시 키 상태 초기화) |
| `WM_SIZE` | 창 크기 갱신 → 모든 ViewPort 재계산 → `Renderer::ResizeRenderTarget` |
| `WM_DESTROY` | `PostQuitMessage(0)` |

**② 게임 로직** — 입력 → 시간 → 월드 순서로 갱신합니다. Controller가 Actor보다 먼저 Tick 되므로, 입력에 의한 이동이 같은 프레임의 행렬 계산에 반영됩니다.

**③ 렌더링** — 컴포넌트가 등록해 둔 목록을 읽어 그립니다 (다음 절 참고).

> [!IMPORTANT]
> `FPActor::Tick()`이 `RootComponent->Tick()`을 호출해 트랜스폼 행렬을 계산합니다.
> 액터의 `Tick()`을 오버라이드할 때는 반드시 `__super::Tick()`을 호출해야 합니다.

`ForestPearlEngine::StopEngine()`을 호출하면 `bEngineLoop`가 `false`가 되어 루프가 끝납니다.

---

## 🎨 렌더링 구조

### Engine과 Renderer의 관계

```mermaid
flowchart LR
    subgraph GAMESIDE["게임 로직 계층"]
        SM["FPStaticMeshComponent<br/>GizmoComponent"]
        CAM["FPCameraComponent"]
        TXT["FPTextComponent"]
    end
    subgraph LISTS["GameInstance SubSystem"]
        ML["FPMeshRenderList<br/>RenderItem"]
        CL["FPCameraList<br/>CameraItem"]
        TL["FPTextRenderList<br/>UIContextItem"]
        VP["FPViewPortClient"]
    end
    subgraph RENDER["렌더링 계층"]
        RD["Renderer"]
        DEV["RenderingDevice"]
        SF["ShaderFactory"]
    end
    SM -->|"등록"| ML
    CAM -->|"등록"| CL
    TXT -->|"등록"| TL
    ML -->|"조회"| RD
    CL -->|"조회"| RD
    TL -->|"조회"| RD
    VP -->|"조회"| RD
    RD --> DEV
    SF --> DEV
    DEV --> D3D["Direct3D 11 / DXGI"]
```

- `ForestPearlEngine`이 `Renderer`와 `RenderingDevice`(싱글톤)를 소유하고, 매 프레임 `Clear → Object → UI → Present`를 호출합니다.
- **게임 코드는 D3D를 직접 호출하지 않습니다.** 컴포넌트가 생성될 때 자신의 데이터를 가리키는 **포인터 묶음**(`RenderItem` 등)을 목록에 등록하고, 소멸될 때 해제합니다.
- 등록된 항목은 컴포넌트 멤버의 **주소**를 담고 있으므로, 컴포넌트가 값을 바꾸면 별도 제출 과정 없이 다음 프레임에 반영됩니다.
- VertexBuffer, Shader, InputLayout 같은 GPU 리소스는 엔진 계층에서 `void*`로 다루고, `RenderingDevice` 내부에서만 D3D 타입으로 캐스팅합니다.

| 계층 | 클래스 | 책임 |
|---|---|---|
| 프레임 흐름 | `Renderer` | 초기화 순서, 정렬, 카메라/뷰포트 순회, 상수 버퍼 채우기, 폰트 출력, 리사이즈 |
| 디바이스 | `RenderingDevice` | Device / Context / SwapChain / RTV / DSV / State / Buffer 생성과 바인딩, 하드웨어 정보 |
| 셰이더 | `ShaderFactory` | `.vso` / `.pso` 로드, 런타임 컴파일, InputLayout 생성 |

### 렌더러에 전달되는 데이터

```cpp
// Renderers/FPRenderingCommon.h
struct RenderItem        // 메시 1개
{
    int*  Priority;   bool* Active;
    std::vector<void*>* VB;          // 서브 메시별 VertexBuffer
    std::vector<int>*   VertexSize;  // 서브 메시별 정점 수
    int*  Stride;     int*  Offset;
    bool* isFill;     bool* isCull;
    FPMatrix* Location; FPMatrix* Rotation; FPMatrix* Scale;
    Topology* Topo;
    void** VertexShader; void** PixelShader; void** VBLayout;
    void** VertexConst;  void** PixelConst;  // Material 상수 버퍼 데이터
};

struct CameraItem        // 카메라 1개
{
    FPMatrix* Location; FPMatrix* Rotation; FPMatrix* Scale;
    FPMatrix* View;     FPMatrix* Projection;
    bool* Active;       bool* TripleCam;
};

struct UIContextItem     // 텍스트 1줄
{
    bool** active; int* x; int* y; FPVector4* color;
    std::basic_string<TCHAR>* msg;
};
```

### 초기화 (`Renderer::InitializeRenderer`)

| 순서 | 호출 | 내용 |
|:-:|---|---|
| 1 | `SetDisplayMode` | 클라이언트 영역 크기, `R8G8B8A8_UNORM` |
| 2 | `CreateFactory` | `CreateDXGIFactory2` (Debug 구성은 Debug 플래그) |
| 3 | `CreateHighSpecDevice` | 어댑터를 열거해 소프트웨어 어댑터를 제외하고 **전용 VRAM이 가장 큰 GPU** 선택. Feature Level 11.1 → 11.0 |
| 4 | `CreateSwapChain` | `CreateSwapChainForHwnd`, MSAA x4, 창 모드, VSync 60Hz |
| 5 | `SetBackBufferToRenderTargetView` | BackBuffer → RenderTargetView |
| 6 | `CreateDepthStencil` | `D32_FLOAT`, MSAA x4 |
| 7 | `OMSetRenderTargets` | RTV + DSV 바인딩 |
| 8 | `GetDeviceInfo` | Feature Level 문자열, 어댑터 / 모니터 / VRAM 정보 수집 |
| 9 | `CreateSpriteBatch / CreateSpriteFont` | DirectXTK. 게임 에셋의 `Font/굴림9k.sfont` 사용 |
| 10 | `CreateDepthStencilStateCreate` | `DEPTH_ON` / `DEPTH_OFF` / `DEPTH_WRITE_OFF` |
| 11 | `RasterStateCreate` | `SOLID` / `WIREFRAME` / `CULLBACK` / `WIRECULLBACK` (CCW가 앞면) |
| 12 | `Create*ConstBuffer(256)` | 256 byte 상수 버퍼 5개 |

### 상수 버퍼 슬롯

| 셰이더 | 슬롯 | 버퍼 | 내용 | 갱신 단위 |
|:-:|:-:|---|---|---|
| VS | `b0` | Object | `World`, `View`, `Proj`, `WVP` | 오브젝트 × 뷰포트 |
| VS | `b1` | VertexShader | Material의 Vertex 상수 | 오브젝트 |
| VS | `b2` | VertexViewPort | ViewPort의 Vertex 상수 | 뷰포트 |
| PS | `b0` | PixelShader | Material의 Pixel 상수 | 오브젝트 |
| PS | `b1` | PixelViewPort | ViewPort의 Pixel 상수 | 뷰포트 |

`World = Scale × Rotation × Location`, `WVP = World × View × Proj` 로 CPU에서 계산해 전달합니다.

### 오브젝트 렌더링 (`Renderer::ObjectRendering`)

```mermaid
flowchart TD
    S["DepthStencil State 설정<br/>상수 버퍼 5개 바인딩"] --> Q["RenderList를 Priority Queue에 삽입<br/>Priority 값이 큰 항목부터"]
    Q --> C{"활성 카메라"}
    C --> V["ViewPort 선택<br/>Main 또는 TripleWaySplit"]
    V --> I{"RenderItem"}
    I -->|"Active = false"| I
    I --> ST["Rasterizer State - Fill / Cull<br/>InputLayout · Topology<br/>VS / PS 설정<br/>Material 상수 버퍼 갱신"]
    ST --> VPL{"ViewPort마다"}
    VPL --> MVP["SetViewPort<br/>Object 상수 버퍼 갱신<br/>ViewPort 상수 버퍼 갱신"]
    MVP --> DR["서브 메시마다<br/>IASetVertexBuffers → Draw"]
    DR --> VPL
    VPL --> I
```

- **정렬** — `FPMeshComponent::SetPriority(int)` 값이 큰 메시가 먼저 그려집니다.
- **Fill / Cull** — `SetMeshFill(bool)`, `SetMeshCull(bool)` 조합으로 4개의 Rasterizer State 중 하나가 선택됩니다.
- **Draw** — 인덱스 버퍼 없이 `Draw(VertexCount, 0)`로 그립니다. FBX 로딩 시 삼각형 코너마다 정점을 복제합니다.
- **깊이 테스트** — `ForestPearlEngine::SetZEnable(bool)`로 전역 On/Off.
- **카메라** — 렌더 큐는 한 번 소비되므로 현재는 활성 카메라 1대를 기준으로 그려집니다.

### 뷰포트 (`FPViewPortClient`)

| 이름 | 개수 | 용도 |
|---|:-:|---|
| `MainGameViewPort` | 1 | 기본 게임 화면 |
| `UIViewPort` | 1 | 텍스트 UI. 항상 클라이언트 영역 전체 |
| `TripleWaySplitViewPort` | 3 | 3분할 화면. `FPCameraComponent::OnTripleCam()`으로 전환. 뷰포트마다 다른 상수(`AniOn`, `BlendOn`) 전달 |

게임 뷰포트는 기준 화면비(960:600)를 유지하도록 계산되며, 창의 긴 축 방향으로 분할·중앙 정렬됩니다.

### UI 렌더링 (`Renderer::UIRendering`)

`UIViewPort`를 설정한 뒤 `SpriteBatch::Begin()` → 활성화된 `UIContextItem`마다 `SpriteFont::DrawString()` → `End()` 순으로 출력합니다.

### 리사이즈 (`Renderer::ResizeRenderTarget`)

`WM_SIZE` → RenderTarget 바인딩 해제 → RTV / DepthStencil 해제 → `ResizeBuffers` → RTV / DepthStencil 재생성 → 재바인딩

### 셰이더와 Material

| 항목 | 내용 |
|---|---|
| 입력 레이아웃 | `POSITION`(float3, offset 0), `COLOR`(float4, offset 12) |
| 기본 셰이더 | `DefaultVertexShader.vso`(WVP 변환), `DefaultPixelShader.pso`(정점 색 출력) |
| 로드 방식 1 | 사전 컴파일된 오브젝트 로드 — `LoadVertexShader("X.vso", owner)` |
| 로드 방식 2 | 런타임 컴파일 — `LoadVertexShader("X.fx", "VS_Main", "vs_5_0", owner)` (행 우선 행렬 패킹) |
| `FPMaterialInterface` | `SetVertexShader / SetPixelShader`, 각 포인터 Getter, `UpdateMaterial(DeltaTime)` |
| `FPMaterial` | 생성 시 기본 셰이더 + InputLayout 설정. `VertextConst / PixelConst`에 상수 구조체를 연결하면 `b1` / `b0`으로 전달 |

Material 선택 우선순위: **컴포넌트에 `SetMaterial`로 지정한 Material** → StaticMesh JSON의 `"Material"` 클래스 → 기본 `FPMaterial`

### Legacy 렌더러

`Renderers/Legacy/`에는 DX11 렌더러 이전 단계의 구현이 보존되어 있습니다 (현재 솔루션에는 포함되지 않음).

| 폴더 | 내용 |
|---|---|
| `FPRHI/` | `FPRHI`, `FPRHIDevice`, `FPRHIVertexBuffer` 추상 인터페이스 (DX9 스타일 API) |
| `GDI/` | GDI 백엔드 |
| `DirectX/` | `DXRHI` 백엔드 |
| `Mia/`, `MIARHI/` | 소프트웨어 렌더러 MIA와 RHI 어댑터 |
| `DOHWA/`, `DOHWARHI/` | 소프트웨어 렌더러 DOHWA(圖畵署)와 RHI 어댑터 |
| `Legacy_Renderer.h/.cpp` | FPRHI를 사용하는 렌더러 |

---

## 🎮 Input 시스템

Unreal Engine의 Enhanced Input과 비슷한 **Mapping Context → Input Action → Binding** 구조입니다.

```mermaid
flowchart LR
    subgraph DEVICE["장치"]
        K["키보드 / 마우스<br/>WM_INPUT"]
        P["게임패드<br/>XInputGetState"]
    end
    subgraph SYS["FPInputSystem"]
        H["HandleRawInput"]
        T["TickInputSystem"]
        QU["InputQueue<br/>FKeyInputInfo"]
    end
    subgraph COMP["FPInputComponent - Controller 소유"]
        IMC["FPInputMappingContext<br/>Key → IA 이름 + Modifier"]
        IA["FPInputAction<br/>바인딩 목록"]
    end
    K --> H --> QU
    P --> T --> QU
    QU -->|"ProcessInputTick"| IMC --> IA
    IA -->|"Possess 필터"| F["Pawn / Controller<br/>멤버 함수 호출"]
```

### 1. 수집 — `FPInputSystem`

| 장치 | 수집 방식 | 발생 이벤트 |
|---|---|---|
| 키보드 | `WM_INPUT` → `HandleKeyboardInput` | 상태 변화 시 `Down` / `Up`. 운영체제 키 반복 입력은 무시 |
| 키보드(누르고 있음) | `TickInputSystem` | `Pressed` 바인딩이 있는 키에 한해 매 프레임 `Pressed` 생성 |
| 마우스 | `WM_INPUT` → `HandleMouseInput` | 좌/우 버튼 `Down` · `Up`, 휠 버튼 `Down`. 값은 커서 위치를 800×600 기준 −1~1로 정규화한 `X`, `Y` |
| 게임패드 | `TickInputSystem` → `HandleGamepadInput` | 0번 컨트롤러. 버튼 `Down` / `Pressed` / `Up`, 스틱·트리거는 `Pressed` |

모든 이벤트는 하나의 구조체로 큐에 쌓입니다.

```cpp
struct FKeyInputInfo
{
    USHORT      VKey;        // 가상 키 코드 (게임패드는 0x100 이상)
    EKeyState   KeyState;    // None / Down / Pressed / Up
    FInputValue InputValue;  // X, Y, Z, bBool, Float
};
```

**게임패드 키 코드 (`XBOX_GAMEPAD`)**

| 코드 | 이름 | 값 |
|:-:|---|---|
| `0x100` | `GAMEPAD_LSTICK` | `X`, `Y` = −1 ~ 1 (데드존 0.1) |
| `0x101` | `GAMEPAD_RSTICK` | `X`, `Y` = −1 ~ 1 (데드존 0.1) |
| `0x102` ~ `0x105` | `GAMEPAD_A` / `B` / `X` / `Y` | `X` = 1 |
| `0x106` / `0x107` | `GAMEPAD_LEFT_SHOULDER` / `RIGHT_SHOULDER` | `X` = 1 |
| `0x108` / `0x109` | `GAMEPAD_START` / `BACK` | `X` = 1 |
| `0x10A` ~ `0x10D` | `GAMEPAD_DPAD_UP` / `DOWN` / `LEFT` / `RIGHT` | `X` = 1 |
| `0x10E` / `0x10F` | `GAMEPAD_LEFT_THUMB` / `RIGHT_THUMB` (스틱 클릭) | `X` = 1 |
| `0x110` / `0x111` | `GAMEPAD_LEFT_TRIGER` / `RIGHT_TRIGER` | `X` = 0 ~ 1 (임계값 초과 시) |

### 2. 매핑 — `FPInputMappingContext`

키 하나를 Input Action 이름과 Modifier에 연결합니다.

```cpp
GetInputComponent().AddMappingKey("IA_Move", 'W', { ESwizzle::YZX, ENegative::Positive });
```

| Modifier | 값 | 효과 |
|---|---|---|
| `ESwizzle` | `XYZ` | 그대로 |
| | `YZX` | `X → Y`, `Y → Z`, `Z → X` (1축 키 입력을 Y축 값으로 사용) |
| | `ZXY` | `X → Z`, `Y → X`, `Z → Y` |
| `ENegative` | `Positive` / `Negative` | `Negative`이면 값에 −1을 곱함 |

- 여러 키를 하나의 Input Action에 매핑할 수 있습니다 (예: W·A·S·D → `IA_Move`).
- **키 하나는 하나의 Input Action에만** 매핑됩니다. 같은 키를 다시 매핑하면 먼저 등록한 매핑이 유지됩니다.

### 3. 바인딩 — `FPInputAction`

```cpp
Controller->GetInputComponent().BindMethod(
    "IA_Move",            // Input Action 이름
    this,                 // 대상 객체
    EKeyState::Pressed,   // 호출 조건: Down / Pressed / Up
    &Player::Move);       // void (T::*)(FInputValue)
```

- 하나의 Input Action에 여러 객체의 함수를 바인딩할 수 있습니다.
- `Pressed`로 처음 바인딩하는 순간, 해당 Action에 매핑된 키들이 "누름 감시 목록"에 등록됩니다. **매핑(`AddMappingKey`)을 바인딩보다 먼저** 해야 합니다.

### 4. 처리 — `FPInputComponent::ProcessInputTick`

매 프레임 `FPAController::Tick()`에서 큐를 비우며 이벤트마다 다음을 수행합니다.

1. `VKey`로 Mapping Context 검색 → Input Action 이름 + Modifier
2. Input Action의 바인딩 순회
3. **Possess 필터** — 바인딩 대상이 *현재 Possess 중인 Pawn* 또는 *Controller 자신*일 때만 통과
4. `KeyState`가 바인딩의 호출 조건과 같을 때만 통과
5. Modifier(Negative → Swizzle) 적용 후 함수 호출

### 5. Possess

```cpp
Controller->Possess(Pawn);     // 이전 Pawn은 자동으로 UnPossess
Controller->UnPossess();
FPPawn* Pawn = Controller->GetPawn();
```

여러 Pawn이 같은 Input Action에 바인딩되어 있어도 **Possess 중인 Pawn만** 입력을 받습니다.
`AddControllerYawInput / PitchInput / RollInput`으로 누적한 Controller 회전은 `bUsePawnControlRotation = true`인 SpringArm이 사용합니다.

---

## 📦 Assets 구조

### 에셋 루트

에셋은 **엔진 에셋**과 **게임 에셋** 두 루트로 나뉘며, 로딩 함수의 `AssetOwner` 인자로 구분합니다.

| 구성 | 게임 에셋 루트 (`AssetOwner::User`) | 엔진 에셋 루트 (`AssetOwner::Engine`) | 컴파일된 셰이더 |
|---|---|---|---|
| Debug | `../../Games/Release_Game/<프로젝트명>/Assets` | `../../Engine/ForestPearlEngine/Assets` | `<루트>/Shader/bin/` |
| Release | `Assets` (exe 옆) | `Assets` (exe 옆) | `<루트>/Shader/` |

Debug 경로는 작업 디렉터리(`Runners/Runner`) 기준 상대 경로입니다. `<프로젝트명>`은 `FPPathManager::Get().Initialize("Tri_World")`로 지정합니다.

### 폴더 규칙

```text
Assets/
├── Font/          *.sfont               DirectXTK SpriteFont (굴림9k.sfont 필수)
├── Level/         *.json                레벨 = GameMode + Actor 배치
├── Model/         <모델>/*.fbx          원본 메시
├── StaticMesh/    *_StaticMesh.json     메시 + Topology + Socket + Material
└── Shader/        *.vsh *.psh *.fx      HLSL 소스
    └── bin/       *.vso *.pso           컴파일 결과 (빌드 시 생성)
```

| 로더 함수 | 기준 폴더 | 예 |
|---|---|---|
| `LoadFbxData(path, owner)` | `Model/` | `"Windmill/Windmill_Body.fbx"` |
| `LoadStaticMesh(name, file, owner)` | `StaticMesh/` | `"Tree_StaticMesh", "Tree_StaticMesh.json"` |
| `LoadLevelData(name, file, owner)` | `Level/` | `"TriWorld", "TriWorld.json"` |
| `LoadVertexShader(obj, owner)` / `LoadPixelShader(obj, owner)` | `Shader/bin/` (Debug) · `Shader/` (Release) | `"Demo.vso"` |
| `LoadVertexShader(src, entry, model, owner)` / `LoadPixelShader(...)` | `Shader/` | `"Demo.fx", "VS_Main", "vs_5_0"` |

### 로딩 파이프라인

```mermaid
flowchart LR
    FBX["Model/*.fbx"] -->|"LoadFbxData<br/>ufbx"| MD["FPMeshData<br/>VertexBuffer"]
    SMJ["StaticMesh/*.json"] -->|"LoadStaticMesh"| SMD["FPStaticMeshData"]
    LVJ["Level/*.json"] -->|"LoadLevelData"| LVD["FPActorData 목록<br/>GameMode 이름"]
    SH["Shader/bin/*.vso *.pso"] -->|"LoadVertex/PixelShader"| SHD["Shader 객체 + 바이트코드"]
    MD --> AM["FPAssetManager"]
    SMD --> AM
    LVD --> AM
    SHD --> AM
    AM -->|"GetStaticMeshData"| SMO["FPStaticMesh"] --> SMC["FPStaticMeshComponent"]
    AM -->|"GetLevelData"| W["FPWorld::OpenLevel"]
    AM -->|"GetVertex/PixelShader"| MAT["FPMaterial"]
```

**FBX 변환 규칙** — 왼손 좌표계 · Y-Up · 미터 단위로 로드하고, 모든 면을 삼각형화한 뒤 코너마다 정점을 생성합니다. 정점에는 노드 변환이 적용된 위치와 Vertex Color가 들어가며(색이 없으면 흰색), 메시 노드 하나가 VertexBuffer 하나가 됩니다.

### Level JSON

```json
{
  "gamemode": "GameMode",
  "actors": [
    {
      "class": "Terrain",
      "name": "Ground",
      "transform": {
        "location": { "x": 0.0,   "y": -0.5, "z": 0.0 },
        "rotation": { "x": 0.0,   "y": 0.0,  "z": 0.0 },
        "scale":    { "x": 128.0, "y": 1.0,  "z": 128.0 }
      }
    }
  ]
}
```

| 키 | 설명 |
|---|---|
| `gamemode` | `ClassRegistry`에 등록한 GameMode 클래스 이름 |
| `actors[].class` | `ClassRegistry`에 등록한 Actor 클래스 이름 |
| `actors[].name` | 액터 이름 (`GetName()`으로 조회) |
| `actors[].transform` | World 기준 `location` / `rotation`(Degree) / `scale` |

### StaticMesh JSON

```json
{
  "MeshData": [
    { "Path": "Windmill/Windmill_Body.fbx", "Topology": "TRIANGLELIST" }
  ],
  "Sockets": [
    {
      "Name": "WingPoint1",
      "Transform": {
        "Location": { "x": 0.0,   "y": 3.0, "z": -1.0 },
        "Rotation": { "x": -90.0, "y": 0.0, "z": 0.0 },
        "Scale":    { "x": 1.0,   "y": 1.0, "z": 1.0 }
      }
    }
  ],
  "Material": ""
}
```

| 키 | 설명 |
|---|---|
| `MeshData[].Path` | `Model/` 기준 FBX 경로. `LoadFbxData`에 넘긴 문자열과 같아야 함 |
| `MeshData[].Topology` | `TRIANGLELIST` / `TRIANGLESTRIP` / `LINELIST` |
| `Sockets[]` | 메시 로컬 기준 부착 지점. 비워 둘 수 있음 |
| `Material` | `ClassRegistry`에 등록한 Material 클래스 이름. `""`이면 기본 Material |

---

## 🛠 Build 시스템

### 요구 사항

| 항목 | 내용 |
|---|---|
| IDE | Visual Studio 2022 (Platform Toolset **v143**) |
| 언어 표준 | C++17, Unicode |
| SDK | Windows SDK 10.0 (Direct3D 11, DXGI 1.6, XInput, D3DCompiler) |
| 구성 | `Debug` / `Release` × `x64` / `x86` |

### 솔루션 구성

| 프로젝트 | 유형 | 참조 | 비고 |
|---|---|---|---|
| `Engine/ForestPearlEngine` | Static Library | → **게임 프로젝트** | HLSL 컴파일(FxCompile) 포함 |
| `Runners/Runner` | Application (Console) | → `ForestPearlEngine` | `DirectXTK.lib` 링크, 패키징 타깃 |
| `Games/Release/Tri_World` | Static Library | — | Include 경로 `$(SolutionDir)Engine` |

빌드 순서는 **Tri_World.lib → ForestPearlEngine.lib → Runner.exe** 입니다.
엔진 프로젝트가 게임 프로젝트를 참조하므로, 게임이 구현한 Hook 함수가 최종 실행 파일에 함께 링크됩니다.

### 외부 라이브러리

| 라이브러리 | 위치 | 용도 | 연결 방식 |
|---|---|---|---|
| DirectXTK | `Libraries/DirectXTK` | `SpriteBatch`, `SpriteFont` | 헤더 `Inc/`, 라이브러리 `Bin/Desktop_2022/$(Platform)/$(Configuration)/DirectXTK.lib` |
| ufbx | `Libraries/Ufbx` | FBX 로딩 | `ufbx.c`를 엔진에 포함해 컴파일 |
| nlohmann/json | `Libraries/nlohmann` | JSON 파싱 | 헤더 전용 |

### 빌드 절차

1. **DirectXTK 빌드** (최초 1회) — `Bin/` 폴더는 git에 포함되지 않습니다.
   `Engine/ForestPearlEngine/Libraries/DirectXTK/DirectXTK_Desktop_2022.sln`을 열어 사용할 구성(`Debug|x64`, `Release|x64`)으로 빌드합니다.
2. `ForestPearlEngine.sln`을 엽니다.
3. **Runner**를 시작 프로젝트로 설정합니다.
4. 구성을 선택하고 빌드합니다.

| 구성 | 결과 | 실행 |
|---|---|---|
| `Debug\|x64` | `x64/Debug/Runner.exe` | Visual Studio에서 F5. 에셋을 소스 폴더에서 직접 읽음 |
| `Release\|x64` | `x64/Release/<게임이름>.exe` + `x64/Release/Assets/` | 폴더째 배포 가능 |

### 셰이더 빌드

엔진 프로젝트의 **FxCompile** 항목이 HLSL을 컴파일합니다.

| 소스 | 종류 | 진입점 | 모델 | 출력 |
|---|:-:|---|:-:|---|
| `Assets/Shader/DefaultVertexShader.vsh` | Vertex | `VS_Main` | 5.0 | `Assets/Shader/bin/DefaultVertexShader.vso` |
| `Assets/Shader/DefaultPixelShader.psh` | Pixel | `PS_Main` | 5.0 | `Assets/Shader/bin/DefaultPixelShader.pso` |
| `Assets/Shader/DefaultShader.fx` | — | — | — | 빌드 제외 |

게임 프로젝트의 셰이더도 같은 방식으로 `Assets/Shader/bin/`에 출력하도록 설정합니다.

### Release 패키징

`Runner.vcxproj`의 `PackageReleaseGame` 타깃이 **Release|x64 빌드 직후** `Build/PackageGame.ps1`을 실행합니다.

```mermaid
flowchart TD
    A["Runner 빌드 완료<br/>Release x64"] --> B["PackageGame.ps1<br/>-SolutionDir -RunnerExe"]
    B --> C["ForestPearlEngine.vcxproj에서<br/>ProjectReference 검색"]
    C --> D{"Games/Release_Game 아래<br/>참조 개수"}
    D -->|"0개 또는 2개 이상"| X["Packaging Failed"]
    D -->|"1개"| E["게임 이름 = vcxproj 파일명"]
    E --> F["Runner.exe → 게임이름.exe"]
    F --> G["Release/Assets 삭제 후 재생성"]
    G --> H["엔진 Assets 복사"]
    H --> I["게임 Assets 복사<br/>같은 파일은 게임이 덮어씀"]
    I --> J["Packaging Complete"]
```

에셋 복사 규칙:

- `Shader` 폴더를 제외한 모든 항목을 그대로 복사합니다.
- 셰이더는 `Shader/bin/*`만 `Assets/Shader/`로 복사합니다 (`bin` 폴더와 HLSL 소스는 포함되지 않음).

```text
x64/Release/
├── Tri_World.exe
└── Assets/
    ├── Font/
    ├── Level/
    ├── Model/
    ├── Shader/        DefaultVertexShader.vso, DefaultPixelShader.pso
    └── StaticMesh/
```

### 실행할 게임 바꾸기

1. 게임 프로젝트를 `Games/Release_Game/<이름>/`에 두고 솔루션에 추가합니다.
2. `ForestPearlEngine` 프로젝트의 **참조**를 해당 게임 프로젝트 하나로 교체합니다.
3. 게임의 `RegistProjectName()`에서 `FPPathManager::Get().Initialize("<이름>")`을 폴더 이름과 같게 지정합니다.

> `Runners/Runner/ReleaseGameProject.props`에는 `GameProjectName`, `GameAssetDir` 매크로가 정의되어 있으나, 현재 패키징은 이 파일이 아니라 엔진 프로젝트의 참조를 기준으로 동작합니다.

---

## 📝 콘텐츠 프로그래머 가이드

`Games/Release_Game/Tri_World`를 기준으로 게임을 만드는 순서입니다.

### 0. 프로젝트 만들기

- `GameTemplate/TriWorldTemplate_Ver1.0.zip`을 템플릿으로 프로젝트를 만듭니다.
- 구성 유형 **Static Library**, C++17, 추가 포함 디렉터리 `$(SolutionDir)Engine`
- 엔진 헤더는 `#include "ForestPearlEngine/..."` 형태로 포함합니다.
- [실행할 게임 바꾸기](#실행할-게임-바꾸기) 절차로 엔진에 연결합니다.

### 1. GameProjectLoader.cpp — 엔진 Hook 구현

엔진이 부팅 중 호출하는 함수 5개를 구현합니다.

| 함수 | 호출 시점 | 할 일 |
|---|---|---|
| `RegistProjectName()` | 윈도우 생성 전 | 프로젝트 이름, 창 크기 |
| `LoadLevel()` | 렌더러 초기화 후 | Level JSON 등록 |
| `LoadClassRegist()` | `LoadLevel` 다음 | 클래스 등록 |
| `LoadAssets()` | `LoadClassRegist` 다음 | FBX, StaticMesh, Shader 로드 |
| `ReturnStartLevel()` | `Initialize()` | 시작 레벨 이름 반환 |

```cpp
#include "ForestPearlEngine/GameProjectLoader.h"
#include "GameMode.h"
#include "GameController.h"
#include "Player.h"
// ...

void RegistProjectName()
{
    FPPathManager::Get().Initialize("Tri_World");          // 폴더 이름과 동일

    FPGameProjectSetting* Setting =
        static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
    Setting->SetWinWidth(960);
    Setting->SetWinHeight(600);
    Setting->ProjectSetting();                             // 창 제목 = 프로젝트 이름
}

void LoadLevel()
{
    FPAssetLoader* Loader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());
    Loader->LoadLevelData("TriWorld", "TriWorld.json", AssetOwner::User);
}

void LoadClassRegist()
{
    FPGameProjectClassRegistry* Registry =
        static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());

    Registry->Register<GameMode>("GameMode");
    Registry->Register<GameController>("GameController");
    Registry->Register<Player>("Player");
    // Level JSON · GameMode · StaticMesh JSON에서 이름으로 쓰는 모든 클래스
}

void LoadAssets()
{
    FPAssetLoader* Loader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

    Loader->LoadFbxData("Windmill/Windmill_Body.fbx", AssetOwner::User);               // ① FBX
    Loader->LoadStaticMesh("Windmill_Body_StaticMesh",
                           "Windmill_Body_StaticMesh.json", AssetOwner::User);         // ② StaticMesh
}

std::string ReturnStartLevel() { return "TriWorld"; }
```

> [!NOTE]
> 등록하는 클래스는 `FPObject`를 상속하고 **기본 생성자**가 있어야 합니다.

### 2. GameMode — 사용할 Controller 지정

```cpp
class GameMode : public FPAGameMode { /* Initialize / BeginPlay / Tick override */ };

void GameMode::Initialize()
{
    ControllerList.push_back("GameController");   // 0번 = Player Controller
    __super::Initialize();                        // 여기서 Controller가 생성됨
}
```

### 3. Controller — 키 매핑과 전역 입력

```cpp
void GameController::Initialize()
{
    // ① 매핑: Input Action 이름, 키, Modifier
    GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'W', { ESwizzle::YZX, ENegative::Positive });
    GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'S', { ESwizzle::YZX, ENegative::Negative });
    GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'A', { ESwizzle::XYZ, ENegative::Negative });
    GetInputComponent().AddMappingKey("IA_SetMoveTriangel", 'D', { ESwizzle::XYZ, ENegative::Positive });
    GetInputComponent().AddMappingKey("IA_SetMoveTriangel",
        static_cast<USHORT>(XBOX_GAMEPAD::GAMEPAD_LSTICK), { ESwizzle::XYZ, ENegative::Positive });

    // ② 바인딩: Controller 자신에게 바인딩한 함수는 Possess 대상과 무관하게 호출됨
    GetInputComponent().BindMethod("IA_NextActor", this, EKeyState::Down, &GameController::NextPawn);

    __super::Initialize();
}

void GameController::NextPawn(FInputValue Value)
{
    ControllPawnIndex = (ControllPawnIndex + 1) % ControllPawnSize;
    Possess(ControllPawn[ControllPawnIndex]);
}
```

### 4. Actor / Pawn — 컴포넌트 조립

| 베이스 | 용도 | Tri_World 예 |
|---|---|---|
| `FPActor` | 배치만 하는 오브젝트 | `Terrain`, `Tree`, `Grid`, `Axis`, `UI` |
| `FPPawn` | Possess 대상 | `Player`, `Windmill`, `WindmillWing`, `TripleWindmillWing`, `TripleWingWindmill` |

```cpp
void Player::Initialize()
{
    // 메시를 Root로
    Mesh = new FPStaticMeshComponent(this, "ToonLinkTriangle_StaticMesh");
    SetRootComponent(Mesh);
    Mesh->SetMeshCull(false);

    // 자식 컴포넌트
    ShieldPivot = new FPSceneComponent(this);
    ShieldPivot->SetupAttachment(Mesh);
    ShieldPivot->SetRelativeLocation({ 0.0f, 2.0f, 0.0f });

    // 3인칭 카메라
    SpringArm = new FPSpringArmComponent(this);
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 50.0f;
    SpringArm->SetRelativeRotation({ 30.0f, 0.0f, 0.0f });
    SpringArm->bUsePawnControlRotation = true;

    PlayerCamera = new FPCameraComponent(this);
    PlayerCamera->SetupAttachment(SpringArm);

    // 입력 바인딩 + Possess
    FPAController* Controller = GetWorld()->GetController(0);
    Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Player::Move);
    Controller->Possess(this);
}

void Player::Move(FInputValue Value)
{
    float Delta = GetWorld()->GetGameTimer()->DeltaTime();
    RootComponent->AddWorldOffset({ Value.X * 10.0f * Delta, 0.0f, Value.Y * 10.0f * Delta });
}

void Player::Tick()
{
    // ... 로직
    __super::Tick();   // 필수: 컴포넌트 트랜스폼 갱신
}
```

**생명주기**

| 함수 | 시점 | 권장 작업 |
|---|---|---|
| `Initialize()` | 레벨의 모든 액터가 생성된 뒤 | 컴포넌트 생성, 입력 바인딩 |
| `BeginPlay()` | 모든 액터의 `Initialize()` 이후 | 다른 액터 검색, Attach |
| `Tick()` | 매 프레임 | 로직 후 `__super::Tick()` |

### 5. 컴포넌트 레퍼런스

#### `FPSceneComponent` — 트랜스폼 계층

| 분류 | 함수 |
|---|---|
| 부착 | `SetupAttachment(Parent, SocketName = "")`, `DetachFromComponent()`, `GetAttachmentRoot()` |
| 설정 (부모 기준) | `SetRelativeLocation`, `SetRelativeRotation`, `SetRelativeScale3D` |
| 설정 (월드 기준) | `SetWorldLocation`, `SetWorldRotation`, `SetWorldScale3D` |
| 증분 | `AddLocalOffset`, `AddLocalRotation`(자신의 축 기준 회전), `AddRelativeLocation`, `AddRelativeRotation`(부모 축 기준 회전), `AddWorldOffset`, `AddWorldRotation` |
| 조회 | `GetComponentTransform / Location / Rotation / Quat / Scale`, `GetRelativeTransform / Location / Rotation / Scale3D`, `GetSocketTransform(name)` |

월드 변환은 `Scale = 부모 × 로컬`, `Rotation = 부모 사원수 × 로컬 사원수`, `Location = 부모 위치 + Rotate(부모 회전, 로컬 위치 × 부모 스케일)`로 계산합니다. Socket에 붙은 경우 부모 대신 Socket의 월드 Transform을 사용합니다.

#### `FPStaticMeshComponent` — 메시 렌더링

```cpp
auto* Body = new FPStaticMeshComponent(this, "Windmill_Body_StaticMesh");  // LoadStaticMesh 이름
```

| 함수 | 설명 |
|---|---|
| `SetMeshFill(bool)` | `true` Solid / `false` Wireframe |
| `SetMeshCull(bool)` | 뒷면 제거 |
| `SetActive(bool)` | 렌더링 On/Off |
| `SetPriority(int)` | 큰 값이 먼저 그려짐 |
| `SetMaterial(FPMaterialInterface*)` | Material 교체 |
| `SetTopology("TRIANGLELIST")` | Topology 변경 |
| `SetStaticMesh(name)` | 메시 교체 |
| `GetSocketTransform(name)` | Socket의 월드 Transform |

#### `FPCameraComponent` — 카메라

| 멤버 | 기본값 | 설명 |
|---|---|---|
| `Fov` | 45° | 시야각 |
| `Aspect` | 960 / 600 | 화면비 |
| `Zn` / `Zf` | 1 / 100 | 근/원 평면 |
| `Active` | `true` | 사용 여부 |
| `OnTripleCam()` | — | 3분할 뷰포트 토글 |

컴포넌트의 월드 위치·회전으로 View 행렬을 만들기 때문에, SpringArm이나 다른 컴포넌트에 붙이는 것만으로 카메라가 움직입니다.

#### `FPSpringArmComponent` — 스프링 암

| 멤버 | 설명 |
|---|---|
| `TargetArmLength` | 자식(카메라)이 놓이는 거리. 암의 전방(+Z) 반대 방향 |
| `bUsePawnControlRotation` | `true`면 부모 회전 대신 **Controller의 회전**을 사용 |

Pawn에서 `AddControllerYawInput(v)`, `AddControllerPitchInput(v)`를 호출하면 카메라가 회전합니다.

#### `GizmoComponent` — Grid / Axis

```cpp
GridComponent = new GizmoComponent(this);
GRIDINFO Info;  Info.width = 128;  Info.height = 128;    // scale(간격), r, g, b, a 지정 가능
GridComponent->MakeGrid(&Info);
SetRootComponent((FPSceneComponent*)GridComponent);

AxisComponent = new GizmoComponent(this);
GIZMO_AXISINFO Axis;                                     // length = 5, scale = 1
AxisComponent->MakeAxis(&Axis);                          // X 빨강 / Y 초록 / Z 파랑
```

`LINELIST`로 그려지며 `SetActive`, `SetPriority`, `SetMeshFill`, `SetMeshCull`, `SetTopology`를 제공합니다.

#### `FPTextComponent` — 화면 텍스트

```cpp
FPTextComponent* Text = new FPTextComponent();
Text->SetTextData(&bShow, 10, 10, { 1.0f, 1.0f, 0.0f, 1.0f }, _T("Hello"));
//                 ↑ 표시 여부 bool의 주소 (여러 텍스트가 하나의 플래그를 공유 가능)
```

좌표는 클라이언트 영역 픽셀, 색은 RGBA(0~1)입니다. 내용이 바뀌면 `SetTextData`를 다시 호출합니다.

#### `FPInputComponent` — 입력

Controller가 소유합니다. `GetWorld()->GetController(0)->GetInputComponent()`로 접근하며 `AddMappingKey`, `BindMethod`를 제공합니다 ([Input 시스템](#-input-시스템) 참고).

#### `FPMovementComponent`

`UpdatedComponent`를 가지는 골격만 정의되어 있습니다.

### 6. Socket과 Attach

```cpp
// 액터를 다른 액터의 Root 메시 Socket에 부착
AttachToActor(Body, "WingPoint1");

// 액터를 특정 컴포넌트에 부착
AttachToComponent(ShieldPivot);

// 컴포넌트를 Socket에 부착
Wing1->SetupAttachment(Body, "UpScaleWingPoint1");

// 분리
DetachFromActor();
```

- Socket은 StaticMesh JSON의 `Sockets`에 정의합니다.
- Socket 이름이 없거나 찾을 수 없으면 부모 컴포넌트의 Transform에 붙습니다.
- Attach 후 `SetRelative*`로 Socket 기준 오프셋을 조정합니다.

### 7. 유틸리티

```cpp
// 액터 검색 (ClassRegistry에 등록한 이름)
FPActor* P = FPGameplayStatics::GetActorOfClass(GetWorld(), "Player");

std::vector<FPActor*> Windmills;
FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Windmill", Windmills);

// 시간
float Delta = GetWorld()->GetGameTimer()->DeltaTime();

// 런타임 스폰 (Initialize → BeginPlay 즉시 호출)
FPActor* NewTree = GetWorld()->SpawnActor("Tree", "RuntimeTree");

// 엔진 기능
ForestPearlEngine::GetGameEngine().SetZEnable(false);              // 깊이 테스트
ForestPearlEngine::GetGameEngine().GetAdapterDescription(0);       // GPU 이름
ForestPearlEngine::GetGameEngine().StopEngine();                   // 종료
```

### 8. 커스텀 Material (선택)

`Games/Release_Game/ShaderCode_Triangle`의 `CB2Material`이 예시입니다.

```cpp
class CB2Material : public FPMaterial
{
    struct alignas(16) VSConstBuffer { float r, g, b, a; float per; float x; };
    VSConstBuffer* Vscb;
public:
    CB2Material()
    {
        Vscb = new VSConstBuffer;
        VertextConst = Vscb;                 // VS b1 슬롯으로 전달됨
    }
    void UpdateMaterial(float DeltaTime) override { /* Vscb 값 갱신 */ }
};

// 사용
MyMaterial = new CB2Material();
MyMaterial->SetVertexShader("Demo.vso");     // LoadAssets()에서 미리 로드
MyMaterial->SetPixelShader("Demo.pso");
Mesh->SetMaterial(MyMaterial);
```

셰이더에서는 [상수 버퍼 슬롯](#상수-버퍼-슬롯) 표에 맞춰 `cbuffer`를 선언합니다.

---

## 🎬 Demo 프로젝트 — Tri_World

> 위치: [`Games/Release_Game/Tri_World`](Games/Release_Game/Tri_World)

SpringArm 카메라, Possess 전환, Socket 부착, 계층 트랜스폼을 한 장면에서 확인하는 데모입니다.

### 실행

| 방법 | 절차 |
|---|---|
| 패키지 실행 | `Release\|x64` 빌드 → `x64/Release/Tri_World.exe` |
| 디버그 실행 | `Debug\|x64`, 시작 프로젝트 **Runner** → F5 |

### 씬 구성 (`Assets/Level/TriWorld.json`, 액터 310개)

| 클래스 | 수 | 베이스 | 구성 | 설명 |
|---|:-:|---|---|---|
| `Terrain` | 1 | `FPActor` | StaticMesh | 128×128 지형 |
| `Tree` | 300 | `FPActor` | StaticMesh | 나무 |
| `Grid` | 1 | `FPActor` | Gizmo | 128×128 그리드 |
| `Axis` | 1 | `FPActor` | Gizmo | 월드 축 |
| `UI` | 1 | `FPActor` | Text × 19 | FPS, 도움말, GPU / 해상도 / 렌더 상태 |
| `Player` | 1 | `FPPawn` | StaticMesh + ShieldPivot + SpringArm + Camera | 시작 시 Possess. 제자리에서 계속 회전 |
| `Windmill` | 2 | `FPPawn` | StaticMesh (몸체) | 이동 / 회전 / 크기 조절 |
| `WindmillWing` | 1 | `FPPawn` | StaticMesh (날개) | `Windmill`의 `WingPoint1` Socket에 부착. Player에게 옮겨 붙일 수 있음 |
| `TripleWindmillWing` | 1 | `FPPawn` | StaticMesh × 3 | 날개 → 날개 → 날개로 이어지는 3단 Socket 체인. `Windmill2`의 `TripleWingPoint`에 부착 |
| `TripleWingWindmill` | 1 | `FPPawn` | StaticMesh × 4 | 몸체 + `UpScaleWingPoint1~3` Socket에 크기·속도가 다른 날개 3개 |

**Socket 정의**

| StaticMesh | Socket |
|---|---|
| `Windmill_Body_StaticMesh` | `WingPoint1`, `UpScaleWingPoint1`, `UpScaleWingPoint2`, `UpScaleWingPoint3`, `TripleWingPoint` |
| `Windmill_Wing_StaticMesh` | `WingPoint1` |
| `ToonLink_StaticMesh`, `ToonLinkTriangle_StaticMesh` | `ShieldPivot`, `HeadPivot` |

### 조작법

조작 대상은 **현재 Possess 중인 Pawn**입니다. Possess 순서는 `Player → Windmill → Windmill2 → TripleWingWindmill`이며 시작 대상은 `Player`입니다.

#### ⌨️ 키보드 · 마우스

| 키 | 동작 | 대상 |
|:-:|---|---|
| <kbd>W</kbd> <kbd>A</kbd> <kbd>S</kbd> <kbd>D</kbd> | 이동 (전 / 좌 / 후 / 우) | Possess 중인 Pawn |
| <kbd>I</kbd> <kbd>K</kbd> | 카메라 Pitch 회전 | Player |
| <kbd>J</kbd> <kbd>L</kbd> | 카메라 Yaw 회전 | Player |
| <kbd>Q</kbd> <kbd>E</kbd> | Y축 회전 (+ / −) | Windmill, TripleWingWindmill |
| <kbd>R</kbd> <kbd>F</kbd> | 크기 확대 / 축소 | Windmill, TripleWingWindmill |
| <kbd>.</kbd> <kbd>,</kbd> | 날개 크기 확대 / 축소 | Player, Windmill |
| <kbd>Z</kbd> | 날개를 Player 머리(`HeadPivot`)에 부착 / 풍차로 복귀 | Player |
| <kbd>X</kbd> | 날개를 Player 주위를 도는 쉴드(`ShieldPivot`)로 부착 / 풍차로 복귀 | Player |
| <kbd>Space</kbd> | 채우기 전환 (Solid ↔ Wireframe) | 전체 |
| <kbd>F1</kbd> | 도움말 UI 표시 전환 | 전체 |
| <kbd>F2</kbd> | Grid 표시 전환 | 전체 |
| <kbd>F3</kbd> | Axis 표시 전환 | 전체 |
| <kbd>F4</kbd> | 뒷면 제거 전환 | 전체 |
| <kbd>F5</kbd> | 깊이 테스트 전환 | 전체 |
| <kbd>↑</kbd> <kbd>↓</kbd> <kbd>←</kbd> <kbd>→</kbd> | `IA_SetMoveWindmill`에 매핑만 되어 있음 (바인딩된 동작 없음) | — |

- **마우스** — 엔진은 좌 / 우 / 휠 버튼 입력을 수집하지만, Tri_World에는 마우스에 매핑된 조작이 없습니다.
- **Possess 전환** — 키보드에는 매핑되어 있지 않습니다. 게임패드 D-Pad를 사용합니다.

#### 🎮 게임패드 (Xbox / XInput)

| 입력 | 동작 | 대상 |
|:-:|---|---|
| **L-Stick** | 이동 | Possess 중인 Pawn |
| **R-Stick** | 카메라 회전 (좌우 Yaw / 상하 Pitch) | Player |
| **D-Pad →** | 다음 Pawn으로 Possess 전환 | 전체 |
| **D-Pad ←** | 이전 Pawn으로 Possess 전환 | 전체 |
| **L-Stick 클릭** / **R-Stick 클릭** | Y축 회전 (− / +) | Windmill, TripleWingWindmill |
| **LT** / **RT** | 크기 축소 / 확대 (누른 정도에 비례) | Windmill, TripleWingWindmill |
| **LB** / **RB** | 날개 크기 축소 / 확대 | Player, Windmill |
| **A** | 날개를 Player 머리에 부착 / 복귀 | Player |
| **B** | 날개를 Player 쉴드로 부착 / 복귀 | Player |

<details>
<summary><b>Input Action 매핑 전체 (GameController.cpp)</b></summary>

| Input Action | 키보드 | 게임패드 | 호출 조건 | 바인딩 |
|---|---|---|:-:|---|
| `IA_SetMoveTriangel` | W / A / S / D | L-Stick | Pressed | `Player::Move`, `Windmill::Move`, `TripleWingWindmill::Move` |
| `IA_SetMoveCamera` | I / J / K / L | R-Stick | Pressed | `Player::CameraMove` |
| `IA_SetMoveWindmill` | ↑ / ← / ↓ / → | — | — | 없음 |
| `IA_SetRotateWindmill` | Q(+) / E(−) | R-Stick 클릭(+) / L-Stick 클릭(−) | Pressed | `Windmill::Rotate`, `TripleWingWindmill::Rotate` |
| `IA_SetScaleWindmill` | R(+) / F(−) | RT(+) / LT(−) | Pressed | `Windmill::Scaling`, `TripleWingWindmill::Scaling` |
| `IA_SetScaleWing` | .(+) / ,(−) | RB(+) / LB(−) | Pressed | `Player::SetScaleWing`, `Windmill::SetScaleWing`, `WindmillWing::SetScaleWing`, `TripleWindmillWing::SetScaleWing` |
| `IA_AttachHead` | Z | A | Down | `Player::AttachHead` |
| `IA_AttachShield` | X | B | Down | `Player::AttachShield` |
| `IA_NextActor` | — | D-Pad → | Down | `GameController::NextPawn` |
| `IA_PrevActor` | — | D-Pad ← | Down | `GameController::PrevPawn` |
| `IA_SetFillTriangel` | Space | — | Down | `GameController::SetFillTriangel` |
| `IA_SetCullTriangel` | F4 | — | Down | `GameController::SetCullTriangle` |
| `IA_SetUITriangel` | F1 | — | Down | `GameController::SetActiveViewHelp` |
| `IA_SetGrid` | F2 | — | Down | `GameController::SetGridOn` |
| `IA_SetAxis` | F3 | — | Down | `GameController::SetAxisOn` |
| `IA_SetDepthStencilBuffer` | F5 | — | Down | `GameController::SetActiveDepthStencilBuffer` |

</details>

---

## 🙏 Credits

| 항목 | 출처 |
|---|---|
| [DirectXTK](https://github.com/microsoft/DirectXTK) | Microsoft — SpriteBatch / SpriteFont |
| [ufbx](https://github.com/ufbx/ufbx) | FBX 로더 |
| [nlohmann/json](https://github.com/nlohmann/json) | JSON 파서 |
| `FPGameTimer` | Frank Luna, *GameTimer* (2011) 기반 |
| Toon Link 모델 | © Nintendo / Sora — The VG Resource (ripped by Mystie). 학습·데모 용도 |

<div align="center">

**ForestPearlEngine** · [teamHUI](https://github.com/teamHUI/ForestPearlEngine)

</div>
