#pragma once

#include <filesystem>
#include <set>
#include <string>

#include "Libraries/Ufbx/ufbx.h"
#include "Libraries/nlohmann/json.hpp"

// FBX 파일을 읽어서 엔진이 사용하는 StaticMesh / Material / Texture json 양식으로
// 변환해주는 클래스.
// - 지정된 입력 폴더의 FBX 파일들을 전부 읽는다.
// - 파일마다 메쉬를 가진 노드 단위로 StaticMesh json 파일을 생성한다. (파일명 = Name + ".json")
// - 머티리얼은 "기본 Material"(재질당 1개, 색상만) + "Material Instance"(메쉬당 1개, 색상 복제 + Texture 배열) 두 가지로 나눠서 저장한다.
// - 텍스처는 실제 이미지 파일 기준으로 중복 없이 한 번만 Texture.json으로 저장한다.
class FbxToStaticMeshJsonTranslator
{
public:
	// FBX 입력 폴더 안의 모든 FBX 파일을 읽어서 Json 출력 폴더에 변환 결과를 저장한다.
	// FbxInputDirectory  : FBX 파일들이 위치한 폴더
	// JsonOutputDirectory : 변환된 StaticMesh/Material/Texture json 파일을 저장할 폴더 (하위에 StaticMesh, Material, Texture 폴더가 생성됨)
	static void TranslateDirectory(const std::filesystem::path& FbxInputDirectory, const std::filesystem::path& JsonOutputDirectory);

private:
	// FBX 파일 하나를 읽어서 그 안의 메쉬를 가진 노드들을 전부 StaticMesh json으로 변환한다.
	// ExportedMaterialNames / ExportedTextureNames : 입력 폴더 전체에서 이미 저장한 기본 Material / Texture 이름 집합 (중복 저장 방지용)
	// 반환값 : 하나 이상의 메쉬를 변환했으면 true
	static bool TranslateFbxFile(const std::filesystem::path& FbxFilePath, const std::filesystem::path& JsonOutputDirectory, std::set<std::string>& ExportedMaterialNames, std::set<std::string>& ExportedTextureNames);

	// 메쉬를 가진 노드 하나를 static_mesh.json 양식으로 변환한다. (Texture 필드는 포함하지 않음 - Material Instance 쪽에만 존재)
	static nlohmann::ordered_json BuildStaticMeshJson(const ufbx_node& Node, const std::string& StaticMeshName, const std::string& MeshDataPath, const nlohmann::ordered_json& SocketsJson, const nlohmann::ordered_json& MaterialJson);

	// 노드의 자식 노드들 중 메쉬가 없는 노드(로케이터/더미 노드)를 소켓 목록으로 변환한다.
	static nlohmann::ordered_json BuildSocketsJson(const ufbx_node& Node);

	// 메쉬에 연결된 첫번째 머티리얼을 처리한다.
	// - 기본 Material("<재질명>_Material.json")은 재질당 한 번만 저장한다.
	// - 이 메쉬 전용 Material Instance("<재질명>_<StaticMeshName>_MaterialInstance.json")를 저장한다. (기본 머티리얼 색상 복제 + Texture 배열)
	// StaticMesh json에 들어갈 "Material" 참조 항목(Material Instance 이름만 있는 형태)을 반환한다.
	// 연결된 머티리얼이 없으면 이름이 빈 Material 항목만 반환한다.
	static nlohmann::ordered_json ExportMeshMaterial(const ufbx_mesh& Mesh, const std::string& StaticMeshName, const std::filesystem::path& JsonOutputDirectory, std::set<std::string>& ExportedMaterialNames, std::set<std::string>& ExportedTextureNames);

	// 머티리얼에 연결된 텍스처들을 실제 이미지 파일 기준으로 Texture.json으로 변환해서 저장한다. (같은 파일이면 한 번만 저장)
	// 반환값은 Material Instance json에 들어갈 "Texture" 배열(Slot, Name)이다.
	static nlohmann::ordered_json ExportMaterialTextures(const ufbx_material& Material, const std::filesystem::path& JsonOutputDirectory, std::set<std::string>& ExportedTextureNames);

	// 머티리얼의 색상 맵(ufbx_material_map)을 {r,g,b,a} json으로 변환한다.
	// 값이 지정되어 있지 않으면 기본값(Default*)을 사용한다.
	static nlohmann::ordered_json MakeColorJson(const ufbx_material_map& ColorMap, double DefaultR, double DefaultG, double DefaultB, double DefaultA);

	// ufbx_vec3를 {x,y,z} json으로 변환한다.
	static nlohmann::ordered_json MakeVec3Json(const ufbx_vec3& Vector);

	// json 데이터를 들여쓰기 4칸으로 지정된 경로에 파일로 저장한다. (상위 폴더는 자동 생성)
	static void SaveJsonToFile(const nlohmann::ordered_json& JsonData, const std::filesystem::path& FilePath);

	// 파일 이름으로 사용할 수 없는 문자를 '_'로 치환한다. 이름이 비어있으면 "Unnamed"를 사용한다.
	static std::string SanitizeFileName(const std::string& Name);

	// ufbx_string -> std::string 변환
	static std::string ConvertUfbxString(const ufbx_string& String);
};
