//
//! Demo.fx : 기본 셰이더 소스.
//!
//! \author	김기홍 Kihong Kim / onlysonim@gmail.com 
//! \date   2003.11.04. Updated. DX9.x 
//! \date   2010.12.01. Updated. DX11, Jun.2010
//! \date   2016.12.27. Updated. DX11/12, Window SDK 8.1 / Window 10 SDK 10.0.18362
//! \date   2018.12.30. Updated. DX11.x/12.x, Windows 10 SDK 10.0.18362
//! \date   2020.08.22. Updated. DX11.x/12.x, Windows 10 SDK 10.0.19041 
//! \date   2024.12.10. Updated. DX11.x/12.x, Windows 10 SDK 10.0.22621 (VS22)
//! \date   2025.09.01. Updated. DX11.x/12.x, Windows 10 SDK 10.0.26100 (VS22)
//

 

////////////////////////////////////////////////////////////////////////////// 
//
//! Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

float4 VS_Main( float4 pos : POSITION ) : SV_POSITION
{
    return pos;
}





////////////////////////////////////////////////////////////////////////////// 
//
//! Pixel Shader Main : 픽셀 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

float4 PS_Main( float4 pos : SV_POSITION ) : SV_Target
{
	//지정색 출력.
	float4 col = {1, 0, 1, 1};

    return col;
}



/**************** end of file "Demo.fx" ***********************/
