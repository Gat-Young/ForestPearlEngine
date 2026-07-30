#include "FPPathManager.h"

void FPPathManager::Initialize(std::string_view ProjectName)
{
#ifdef _DEBUG
    AssetRoot =
        std::filesystem::path("../../Games/Release_Game")
        / ProjectName
        / "Assets";
#else
    AssetRoot = std::filesystem::path("Assets");
#endif

    AssetRoot = AssetRoot.lexically_normal();
}

std::string FPPathManager::GetAssetPath(const std::string& RelativePath) const
{
    return (AssetRoot / RelativePath)
        .lexically_normal()
        .string();
}

std::string FPPathManager::GetAssetRoot() const
{
    return AssetRoot.string();
}

