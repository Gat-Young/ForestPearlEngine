#include "FPPathManager.h"
#include <Windows.h>

void FPPathManager::Initialize(std::string_view ProjectName)
{
    this->ProjectName = ProjectName;
#ifdef _DEBUG
    AssetRoot =
        std::filesystem::path("../../Games/Release_Game")
        / ProjectName
        / "Assets";

    EngineAssetRoot =
        std::filesystem::path("../../Engine/ForestPearlEngine") / "Assets";
#else
    AssetRoot = std::filesystem::path("Assets");
    EngineAssetRoot = std::filesystem::path("Assets");
#endif

    AssetRoot = AssetRoot.lexically_normal();
    EngineAssetRoot = EngineAssetRoot.lexically_normal();
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

std::string FPPathManager::GetEngineAssetPath(const std::string& RelativePath) const
{
    return (EngineAssetRoot / RelativePath)
        .lexically_normal()
        .string();
}

std::string FPPathManager::GetEngineAssetRoot() const
{
    return EngineAssetRoot.string();
}

std::string FPPathManager::GetEngineAssetShaderPath(const std::string& RelativePath) const
{
#ifdef _DEBUG
    return (EngineAssetRoot / "Shader" / "bin" / RelativePath)
        .lexically_normal()
        .string();
#else
    return (EngineAssetRoot / "Shader" / RelativePath)
        .lexically_normal()
        .string();
#endif
}

std::string FPPathManager::GetAssetShaderPath(const std::string& RelativePath) const
{
#ifdef _DEBUG
    return (AssetRoot / "Shader" / "bin" / RelativePath)
        .lexically_normal()
        .string();
#else
    return (AssetRoot / "Shader" / RelativePath)
        .lexically_normal()
        .string();
#endif
}

std::string FPPathManager::GetProjectName()
{
    return ProjectName;
}
std::wstring FPPathManager::StringToWString(const std::string& String)
{
    if (String.empty())
    {
        return {};
    }

    const int Size = MultiByteToWideChar(
        CP_UTF8,
        0,
        String.data(),
        static_cast<int>(String.size()),
        nullptr,
        0
    );

    if (Size <= 0)
    {
        return {};
    }

    std::wstring Result(Size, L'\0');

    MultiByteToWideChar(
        CP_UTF8,
        0,
        String.data(),
        static_cast<int>(String.size()),
        Result.data(),
        Size
    );

    return Result;
}
