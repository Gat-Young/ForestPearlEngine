# Water 엔진 적용 가이드

현재 Water의 흐르는 물 효과를 다른 프로젝트나 엔진 기능으로 옮길 때 필요한 파일, 렌더 패스, State 및 리소스 바인딩을 정리한다. 공통 에셋 로딩과 데모 장면 구성은 다루지 않는다.

이 문서는 **실시간 모델 반사와 큐브 환경맵 반사를 모두 유지하는 현재 구현**을 기준으로 한다. 물 효과를 위해 기존 엔진의 렌더링 패스를 추가하거나 순서를 변경하지 않았다.

## 1. 필요한 파일과 기능

아래 파일 경로는 이 README가 있는 Water 소스 폴더를 기준으로 한다.

| 파일 | 엔진에 적용할 기능 |
| --- | --- |
| `Assets/ShaderResource/WaterVertexShader.vsh` | 물 평면의 UV, 투영 좌표, 월드 공간의 카메라 방향 전달 |
| `Assets/ShaderResource/WaterPixelShader.psh` | 범프맵 흐름, 반사 UV 왜곡, 환경맵 반사 및 최종 색상 합성 |
| `WaterReflectionCaptureComponent.h/.cpp` | 원본 카메라를 수면 기준으로 반사한 캡처 View 계산. 실시간 모델 반사를 유지할 때 필요 |
| `WaterSettings.h` | 흐름 속도, 왜곡 강도, 물색, 모델 반사량, 환경맵 기여도 설정. 엔진의 물 재질 파라미터로 대체 가능 |
| `WaterMath.h` | UV 이동량 누적 및 반사 View 계산. 기존 엔진 유틸리티에 통합 가능 |

`AWater.h/.cpp`는 전체를 가져갈 필요 없이 다음 동작만 엔진의 물 컴포넌트나 재질 갱신 경로에 연결한다.

- 프레임 시간으로 `FlowOffset`을 갱신한다.
- 물 재질의 파라미터를 갱신한다.
- 기존 카메라와 반사 캡처를 연결한다.
- 수면 높이를 반사 캡처에 전달한다.
- 물 평면 자체를 반사 캡처에서 제외한다.

물 전용 셰이더 2개 외에 반사 대상 모델의 셰이더를 Water 전용으로 교체할 필요는 없다. 다만 아래의 컬링과 클립 평면 조건은 캡처 경로에서 충족해야 한다.

## 2. 렌더 패스 순서

물 효과와 직접 관련된 기존 패스는 다음 순서로 실행된다.

```text
PreDepthStencilRenderPass
    물 평면을 포함한 Opaque 메시의 메인 화면 깊이 기록
        ↓
SceneCaptureRenderPass
    반사 카메라로 장면을 WaterReflection에 렌더링
    물 평면 자체는 캡처에서 제외
    기존 Resolve 경로로 샘플링 가능한 반사 결과 준비
        ↓
MeshRenderPass → OpaqueRenderPass
    물 평면에 WaterVertexShader / WaterPixelShader 적용
    범프맵으로 반사 텍스처 UV와 환경맵 반사 방향 왜곡
    물 기본색, 환경맵, 모델 반사 합성
```

- `PreDepthStencilRenderPass`는 기존 엔진의 깊이 선행 처리이며 물 전용으로 추가한 패스가 아니다.
- 실시간 모델 반사는 기존 `SceneCaptureRenderPass`를 사용한다.
- 환경맵은 물 픽셀 셰이더에서 직접 샘플링하므로 추가 렌더 패스나 렌더 타깃이 필요 없다.
- 후처리, 기즈모, 노멀 표시, UI는 물 효과의 필수 단계가 아니다.
- Water의 표시 영역을 제한하기 위한 스텐실 마스크는 사용하지 않는다. 물 평면의 래스터라이징 결과와 깊이 테스트가 표시 영역을 결정한다.

## 3. 렌더링 State

다음은 현재 정상 깊이 테스트가 활성화된 실행 상태를 기준으로 한다. State는 Water가 직접 새로 생성하는 것이 아니라, 재질 및 캡처 설정에 따라 기존 렌더러가 바인딩한다.

### 3.1 깊이 선행 처리

| 항목 | 현재 설정 |
| --- | --- |
| Color Render Target | 없음 |
| Depth Target | 기존 메인 깊이 버퍼 |
| Pixel Shader | `nullptr` |
| Topology | `TRIANGLELIST` |
| Rasterizer | 물 재질 기준 Solid / Cull None |
| Depth Test | 활성화 |
| Depth Write | 활성화 |
| Depth Function | `LESS_EQUAL` |
| Stencil | 물에 대해 사용하지 않음 |

### 3.2 반사 캡처

현재 Water의 반사 대상은 Opaque 모델이다. 다른 프로젝트의 반사 대상은 각 재질의 렌더 분류와 상태를 따른다.

| 항목 | 현재 설정 |
| --- | --- |
| Render Target | `WaterReflection` |
| Depth Target | `WaterReflectionDepth` |
| 타깃 크기 | `1280 × 800` |
| Color Format | `RGBA8` |
| MSAA | `1` |
| Clear Color | `(0, 0, 0, 0)` |
| Depth Clear | `1.0` — 기존 프레임 깊이 초기화 경로에서 처리 |
| Viewport | 반사 타깃 전체 |
| View | 수면 기준 반사 View |
| Projection | 원본 카메라의 Projection |
| Topology | `TRIANGLELIST` |
| Rasterizer | 기본 Solid. 현재 반사 모델은 `TwoSided=true`이므로 Cull None |
| Depth State | `DS_DEPTH_ON` |
| Depth Test / Write | 모두 활성화 |
| Depth Function | `LESS_EQUAL` |
| Blend State | 현재 반사 모델은 `BS_DEFAULT` |
| Stencil | 현재 반사 모델에 대해 사용하지 않음 |

반사 View는 와인딩 방향을 뒤집는다. 현재 모델의 Cull None 설정은 이에 대한 대응이다. 다른 프로젝트에서 단면 재질의 모델을 반사할 경우 캡처별 컬링 방향 전환 등으로 별도 대응해야 한다.

반사 타깃의 배경 알파는 `0`, 현재 반사 모델의 출력 알파는 `1`이다. 물 셰이더는 이 알파를 모델 반사의 합성 비율에 사용한다. 다른 모델 재질로 교체할 때도 이 알파 의미를 유지해야 한다.

### 3.3 물 평면 렌더링

| 항목 | 현재 설정 |
| --- | --- |
| Render Target | 기존 `MeshRenderPass-RT-BuiltIn` |
| Depth Target | 기존 메인 깊이 버퍼 |
| 재질 분류 | `Opaque` |
| Input Layout | 엔진 기본 메시 레이아웃: POSITION / NORMAL / COLOR / TEXCOORD / TANGENT |
| Topology | `TRIANGLELIST` |
| Rasterizer | Solid / `TwoSided=true` → Cull None |
| Depth State | `DS_DEPTH_ON` |
| Depth Test / Write | 모두 활성화 |
| Depth Function | `LESS_EQUAL` |
| Blend State | `BS_DEFAULT` |
| Stencil | 사용하지 않음 |
| Vertex Shader | `WaterVertexShader` |
| Pixel Shader | `WaterPixelShader` |

`DS_DEPTH_ON`은 깊이 테스트와 쓰기를 켜고, `LESS_EQUAL` 비교를 사용하며, 스텐실은 비활성화한다.

`BS_DEFAULT`는 블렌딩을 비활성화하고 RGBA 전체를 기록한다. 설정상 소스 계수는 ONE, 대상 계수는 ZERO다.

물색과 반사색은 픽셀 셰이더 내부의 `lerp()`로 합성한다. 하드웨어 알파 블렌딩을 사용하는 투명 재질이 아니며, 최종 출력 알파는 `1`이다.

## 4. 셰이더 리소스 바인딩

### 4.1 텍스처

| 슬롯 | HLSL 이름 | 필요한 리소스 |
| --- | --- | --- |
| PS `t0` | `BumpMap` | R 채널을 높이로 사용하는 단일 범프맵. 선형 UNORM으로 읽어야 함 |
| PS `t1` | `ReflectionMap` | 반사 캡처 결과의 SRV |
| PS `t127` | `EnvTex` | 기존 SkyLight 큐브 환경맵 |

현재 물 재질은 범프 에셋 `WaterBump`와 반사 타깃 `WaterReflection`을 각각 `BumpMap`, `ReflectionMap`에 연결한다. `EnvTex`는 기존 렌더러의 SkyLight 경로에서 공급한다.

반사 타깃은 현재 그리는 메인 컬러 타깃과 별개다. 캡처 결과를 준비한 뒤, 메인 메시 패스에서 SRV로 읽는다.

### 4.2 Sampler State

| 슬롯 | HLSL 이름 | 현재 설정 |
| --- | --- | --- |
| PS `s0` | `BumpWrap` | Bilinear / Wrap |
| PS `s1` | `ReflectionClamp` | Bilinear / Clamp |
| PS `s15` | `EnvSampler` | Trilinear / Clamp |

범프맵은 이동한 UV가 경계를 넘어가도 반복되도록 Wrap을 사용한다. 반사 텍스처는 화면 가장자리에서 반복되지 않도록 Clamp를 사용한다.

### 4.3 Constant Buffer

| 슬롯 | 내용 | 공급 주체 |
| --- | --- | --- |
| VS `b0` | 기존 1280바이트 월드·뷰·투영 행렬 버퍼 | 기존 엔진 |
| PS `b1` | `SkyLightConstantBuffer`: `LightColor`, `LightIntensity` | 기존 SkyLight 경로 |
| PS `b3` | 물 전용 `WaterSettings` 버퍼 | 물 재질 파라미터 갱신 코드 |

VS `b0`는 기존 엔진의 행렬 배치와 일치해야 한다. 물 셰이더에서 사용하는 행렬만 따로 재배치하지 않는다.

PS `b3`의 현재 변수와 순서는 다음과 같다.

```hlsl
float FlowOffset;
float DistortionStrength;
float Tiling;
float ReflectionAmount;
float4 WaterColor;
float EnvironmentAmount;
float EnvironmentDistortionScale;
```

현재는 기존 재질 리플렉션과 `SetParameter()`를 사용하므로, 위 변수 이름과 버퍼 이름 `WaterSettings`를 유지해야 한다. 직접 버퍼를 업로드하는 방식으로 바꾼다면 HLSL 패킹과 16바이트 정렬도 일치시켜야 한다.

`FlowSpeed`와 `EnvironmentIntensity`는 PS `b3`에 직접 넣지 않는다.

- `FlowSpeed`: CPU에서 `FlowOffset`을 계산하는 데 사용한다.
- `EnvironmentIntensity`: SkyLight의 세기로 전달되어 PS `b1`에 반영된다.

## 5. 연결 시 유지할 조건

| 조건 | 적용 기준 |
| --- | --- |
| 갱신 순서 | 원본 카메라 행렬 갱신 후 반사 View 계산 |
| 렌더 순서 | 반사 캡처 완료 후 물 평면 렌더링 |
| 캡처 제외 | 물 평면 자체를 반사 캡처에서 제외 |
| 평면 방향 | 현재 환경 반사 노멀 계산은 수평 XZ 평면 기준 |
| UV 방향 | 현재 셰이더는 U 증가가 +X, V 증가가 -Z인 배치를 기준으로 함 |
| 범프 데이터 | R 채널 높이 값, sRGB 변환 없이 선형 샘플링 |
| 반사 컬링 | 반사 View의 와인딩 반전에 대응 |
| 클립 평면 | 반사 계산과 같은 수면 평면 사용 |
| 반사 알파 | 배경 0, 유효 모델 반사 영역 1이라는 현재 합성 규칙 유지 |
| 환경맵 | 기존 엔진은 첫 번째 활성 SkyLight의 환경맵과 조명 정보를 사용 |

현재 반사 View는 수면 높이를 전달받지만, 반사 대상 모델의 클리핑은 모델 셰이더의 월드 `y=0`으로 고정되어 있다. 엔진 적용 시 이 처리를 캡처별 클립 평면으로 연결해야 한다. 일반 화면의 모델까지 수면 아래에서 잘리지 않도록 적용 범위도 구분해야 한다.

현재 리소스 이름과 반사 타깃은 단일 수면 기준이다. 여러 수면을 독립적으로 적용하려면 반사 리소스와 캡처의 공유 또는 분리 정책이 필요하다.
