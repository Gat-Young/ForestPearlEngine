#include <filesystem>
#include <iostream>

#include "FbxToStaticMeshJsonTranslator.h"

// FBXToStaticMeshJsonTranslator
// 정해진 폴더에 있는 FBX 파일들을 읽어서 엔진이 사용하는
// StaticMesh / Material / Texture json 파일로 변환해주는 콘솔 툴.
//
// 사용법 :
//   FBXToStaticMeshJsonTranslator.exe [FBX입력폴더] [Json출력폴더]
//   인자를 생략하면 실행 파일 기준 기본 폴더(FBX, Json)를 사용한다.
int main(int argc, char* argv[])
{
	// 기본 입/출력 폴더. 필요에 맞게 수정해서 사용할 것.
	std::filesystem::path FbxInputDirectory = "FBX";
	std::filesystem::path JsonOutputDirectory = "Json";

	if (argc >= 2)
	{
		FbxInputDirectory = argv[1];
	}

	if (argc >= 3)
	{
		JsonOutputDirectory = argv[2];
	}

	std::cout << "FBX 입력 폴더 : " << FbxInputDirectory.string() << "\n";
	std::cout << "Json 출력 폴더 : " << JsonOutputDirectory.string() << "\n";

	FbxToStaticMeshJsonTranslator::TranslateDirectory(FbxInputDirectory, JsonOutputDirectory);

	return 0;
}
