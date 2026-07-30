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
