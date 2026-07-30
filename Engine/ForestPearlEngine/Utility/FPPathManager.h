#pragma once
#include <filesystem>
#include <string>
#include <string_view>

class FPPathManager
{
public:
    static FPPathManager& Get()
    {
        static FPPathManager Instance;
        return Instance;
    }

    void Initialize(std::string_view ProjectName);

    std::string GetAssetPath(const std::string& RelativePath) const;

    std::string GetAssetRoot() const;

    std::string GetProjectName();

    std::wstring StringToWString(const std::string& String);

private:
    FPPathManager() = default;

private:
    std::filesystem::path AssetRoot;
    std::string ProjectName;
};