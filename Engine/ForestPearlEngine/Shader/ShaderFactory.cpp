#include "d3dcompiler.h"				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.
#include "ShaderFactory.h"
#include "../Renderers/RenderingDevice.h"
#include <iostream>

//싱글톤 셰이더 Factory 가져오기
ShaderFactory& ShaderFactory::GetShaderFactory()
{
	static ShaderFactory ShaderFactorySingleton;

	return ShaderFactorySingleton;
}

HRESULT ShaderFactory::VertexShaderLoad(const TCHAR* Objectname, void** ppVS, void** ppCode)
{
	ID3DBlob* pCode = nullptr;
	
	//정점 셰이더 파일 로드
	HRESULT hr = D3DReadFileToBlob(Objectname, &pCode);
	assert(SUCCEEDED(hr) && "정점 셰이더 로딩 실패");

	//정점 셰이더 객체 생성
	ID3D11VertexShader* pVS = nullptr;
	hr = RenderingDevice::GetRenderingDevice().GetDXDevice()->CreateVertexShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pVS);
	assert(SUCCEEDED(hr) && "정점 셰이더 객체 생성 실패");

	//완료후 외부 리턴
	*ppVS = pVS;
	*ppCode = pCode;

	return hr;
}

HRESULT ShaderFactory::PixelShaderLoad(const TCHAR* Objectname, void** ppPS, void** ppCode)
{
	ID3DBlob* pCode = nullptr;

	//픽셀 셰이더 파일 로드
	HRESULT hr = D3DReadFileToBlob(Objectname, &pCode);
	assert(SUCCEEDED(hr) && "픽셀 셰이더 로딩 실패");

	//픽셀 셰이더 객체 생성
	ID3D11PixelShader* pPS = nullptr;
	hr = RenderingDevice::GetRenderingDevice().GetDXDevice()->CreatePixelShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pPS);
	assert(SUCCEEDED(hr) && "픽셀 셰이더 객체 생성 실패");

	//완료후 외부 리턴
	*ppPS = pPS;
	*ppCode = pCode;

	return hr;
}

// 정점 셰이더 로드
// 지정 셰이더 파일(FX)을 컴파일 하고 셰이더 객체에 담아리턴
//
// param		fxname		셰이더 파일명
// param		entry		셰이더 진입점
// param		sm			셰이더 모델명
// param[out]	ppShader	셰이더 오브젝트
// param[out]	ppCode		셰이더 코드 (컴파일된, 바이너리)
// return	성공시 S_OK, 실패시 DX 에러코드
//
HRESULT ShaderFactory::VertexShaderLoad(const TCHAR* filename, const CHAR* entry, const CHAR* target, void** ppVS, void** ppCode)
{
	HRESULT hr = S_OK;

	//셰이더 컴파일
	ID3DBlob* pCode = nullptr;
	hr = ShaderCompile(filename, entry, target, &pCode);
	assert(SUCCEEDED(hr) && "정점 셰이더 컴파일 실패");

	//정점 셰이더 객체 생성
	ID3D11VertexShader* pVS = nullptr;
	hr = RenderingDevice::GetRenderingDevice().GetDXDevice()->CreateVertexShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pVS);
	assert(SUCCEEDED(hr) && "정점 셰이더 객체 생성 실패");

	//완료후 외부 리턴
	*ppVS = pVS;
	*ppCode = pCode;

	return hr;
}

// 픽셀 셰이더 로드
// 픽셀 셰이더 파일(FX)을 컴파일 하고 셰이더 객체에 담아리턴
//
// param		fxname		셰이더 파일명
// param		entry		셰이더 진입점
// param		sm			셰이더 모델명
// param[out]	ppShader	셰이더 오브젝트
// param[out]	ppCode		셰이더 코드 (컴파일된, 바이너리)
// return	성공시 S_OK, 실패시 DX 에러코드
//

HRESULT ShaderFactory::PixelShaderLoad(const TCHAR* filename, const CHAR* entry, const CHAR* target, void** ppPS, void** ppCode)
{
	HRESULT hr = S_OK;

	//셰이더 컴파일
	ID3DBlob* pCode = nullptr;
	hr = ShaderCompile(filename, entry, target, &pCode);
	assert(SUCCEEDED(hr) && "픽셀 셰이더 컴파일 실패");

	//픽셀 셰이더 객체 생성
	ID3D11PixelShader* pPS = nullptr;
	hr = RenderingDevice::GetRenderingDevice().GetDXDevice()->CreatePixelShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pPS);
	assert(SUCCEEDED(hr) && "픽셀 셰이더 객체 생성 실패");

	//완료후 외부 리턴
	*ppPS = pPS;
	*ppCode = pCode;

	return hr;
}

// 셰이더 소스 컴파일
//
// param		FileName	셰이더 파일명
// param		EntryPoint	셰이더 진입점
// param		ShaderModel	셰이더 모델
// param[out]	ppCode		셰이더 코드 (컴파일된, 바이너리)
// return	성공시 S_OK, 실패시 DX 에러코드
//
HRESULT ShaderFactory::ShaderCompile(const TCHAR* FileName, const CHAR* EntryPoint, const CHAR* ShaderModel, ID3DBlob** ppCode)
{
	HRESULT hr = S_OK;
	ID3DBlob* pError = nullptr;

	//컴파일 옵션 1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;	//행우선 행렬 처리, 구형 DX 이전까지의 전통적인 방식. 속도가 요구된다면 "열우선"으로 처리할 것
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//열우선 행렬 처리. 속도의 향상이 있지만, 행렬을 전치 후 GPU에 공급해야 함
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	Flags |= D3DCOMPILE_DEBUG;								//디버깅 옵션 추가.
#endif
	
	//셰이더 소스 컴파일
	hr = D3DCompileFromFile(FileName,
		nullptr, nullptr,
		EntryPoint,
		ShaderModel,
		Flags,						//컴파일 옵션 1
		0,							//컴파일 옵션 2, Effect 컴파일시 적용됨. 이외에는 무시됨.
		ppCode,						//[출력] 컴파일된 셰이더 코드
		&pError						//[출력] 컴파일 에러 코드
		);

	assert(SUCCEEDED(hr) && "셰이더 컴파일 실패");

	//임시 객체 제거
	if (pError != nullptr)
	{
		pError->Release();
		pError = nullptr;
	}

	return hr;
}

HRESULT ShaderFactory::CreateInputLayout(void* InVSCode, void** ReturnLayout)
{
	HRESULT hr = S_OK;

	//정점 입력구조 객체 생성
	//함께 사용될 셰이더(컴파일된 바이너리 코드)가 필요
	ID3D11InputLayout* Layout = nullptr;
	ID3DBlob* VScode = static_cast<ID3DBlob*>(InVSCode);
	
	// 정점 입력 구조 
	// GPU에 공급될 기하데이터 - 개별 정점의 데이터 구조와 용도등의 정보를 구성
	//
	// 일반적인 환경하에서 올바른 렌더링 결과를 기대한다면 아래의 조건이 동일 또는 호환
	// 1.정점 버퍼 데이터.  Vertex Buffer Data
	// 2.정점 구조 Vertex Format (Input Layout)
	// 3.셰이더 (함수) 입력구조.  Vertex Shader (Input Layout)
	// 4.셰이더 (함수) 입출력 시멘틱 (Semantics)
	// 5.각종 변환 처리 Vertex Transform
	// 
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = RenderingDevice::GetRenderingDevice().CreateInputLayout(layout, ARRAYSIZE(layout), VScode, &Layout);

	*ReturnLayout = Layout;
	return hr;
}