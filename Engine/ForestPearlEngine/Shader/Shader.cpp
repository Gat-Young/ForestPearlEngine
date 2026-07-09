#include "d3dcompiler.h"				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.
#include "Shader.h"

Shader::Shader(ID3D11Device& Device) : Device(Device)
{

}

// 기본 셰이더 시스템 구성
void Shader::ShaderCreate()
{
	//기본 셰이더 로드 & 설정
	ShaderLoad();
}

//기본 셰이더 시스템 갱신
void Shader::ShaderUpdate()
{
}

// 셰이더 시스템 제거
void Shader::ShaderRelease()
{
	VertexShader->Release();
	PixelShader->Release();
	VSCode->Release();
	PSCode->Release();

	VertexShader = nullptr;
	PixelShader = nullptr;
	VSCode = nullptr;
	PSCode = nullptr;
}

//셰이더 로드
HRESULT Shader::ShaderLoad()
{
	HRESULT hr = S_OK;

	//정점 셰이더 생성
	VertexShaderLoad(Filename, "VS_Main", "vs_5_0", &VertexShader, &VSCode);

	//픽셀 셰이더 생성
	PixelShaderLoad(Filename, "PS_Main", "ps_5_0", &PixelShader, &PSCode);

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
HRESULT Shader::VertexShaderLoad(const TCHAR* fxname, const CHAR* entry, const CHAR* target, ID3D11VertexShader** ppVS, ID3DBlob** ppCode)
{
	HRESULT hr = S_OK;

	//셰이더 컴파일
	ID3DBlob* pCode = nullptr;
	hr = ShaderCompile(fxname, entry, sm, &pCode);
	assert(SUCCEEDED(hr) && "정점 셰이더 컴파일 실패");

	//정점 셰이더 객체 생성
	ID3D11VertexShader* pVS = nullptr;
	hr = Device.CreateVertexShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pVS);
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

HRESULT Shader::PixelShaderLoad(const TCHAR* fxname, const CHAR* entry, const CHAR* target, ID3D11PixelShader** ppPS, ID3DBlob** ppCode)
{
	//셰이더 컴파일
	ID3DBlob* pCode = nullptr;
	hr = ShaderCompile(fxname, entry, sm, &pCode);
	assert(SUCCEEDED(hr) && "픽셀 셰이더 컴파일 실패");

	//정점 셰이더 객체 생성
	ID3D11PixelShader* pPS = nullptr;
	hr = Device.CreatePixelShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pPS);
	assert(SUCCEEDED(hr) && "픽셀 셰이더 객체 생성 실패");

	//완료후 외부 리턴
	*ppVS = pPS;
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
HRESULT Shader::ShaderCompile(const TCHAR* FileName, const CHAR* EntryPoint, const CHAR* ShaderModel, ID3DBlob** ppCode)
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
		&pError						//[출력] 컴팡일 에러 코드
		);

	assert(SUCCEEDED(hr) && "셰이더 컴파일 실패");

	//임시 객체 제거
	pError->Release();
	pError = nullptr;

	return hr;
}
