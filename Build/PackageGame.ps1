param
(
    [Parameter(Mandatory = $true)]
    [string]$SolutionDir,

    [Parameter(Mandatory = $true)]
    [string]$RunnerExe
)

$ErrorActionPreference = "Stop"

try
{
    # ------------------------------------------------------------
    # 경로 정리
    # ------------------------------------------------------------

    $SolutionDir = [System.IO.Path]::GetFullPath($SolutionDir)

    $EngineProjectPath = Join-Path `
        $SolutionDir `
        "Engine\ForestPearlEngine\ForestPearlEngine.vcxproj"

    $EngineAssetsPath = Join-Path `
        $SolutionDir `
        "Engine\ForestPearlEngine\Assets"

    $ReleaseGameRoot = Join-Path `
        $SolutionDir `
        "Games\Release_Game"

    $ReleaseDir = [System.IO.Path]::GetDirectoryName($RunnerExe)

    $ReleaseAssetsPath = Join-Path `
        $ReleaseDir `
        "Assets"


    Write-Host ""
    Write-Host "==========================================="
    Write-Host " ForestPearlEngine Release Packaging"
    Write-Host "==========================================="
    Write-Host ""

    Write-Host "[Package] Solution      : $SolutionDir"
    Write-Host "[Package] Engine Project: $EngineProjectPath"
    Write-Host "[Package] Runner        : $RunnerExe"
    Write-Host ""


    # ------------------------------------------------------------
    # ForestPearlEngine.vcxproj 확인
    # ------------------------------------------------------------

    if (-not (Test-Path $EngineProjectPath))
    {
        throw "ForestPearlEngine.vcxproj를 찾을 수 없습니다: $EngineProjectPath"
    }


    # ------------------------------------------------------------
    # ForestPearlEngine.vcxproj XML 읽기
    # ------------------------------------------------------------

    [xml]$ProjectXml = Get-Content `
        -LiteralPath $EngineProjectPath `
        -Raw


    # ------------------------------------------------------------
    # ProjectReference 검색
    #
    # XML Namespace에 영향받지 않도록 LocalName 사용
    # ------------------------------------------------------------

    $ProjectReferences = $ProjectXml.SelectNodes(
        "//*[local-name()='ProjectReference']"
    )

    if ($null -eq $ProjectReferences -or $ProjectReferences.Count -eq 0)
    {
        throw "ForestPearlEngine에 ProjectReference가 없습니다."
    }


    # ------------------------------------------------------------
    # Games\Release_Game 아래의 ProjectReference 검색
    # ------------------------------------------------------------

    $GameProjects = @()

    $EngineProjectDir = [System.IO.Path]::GetDirectoryName(
        $EngineProjectPath
    )

    $ReleaseGameRootFull = [System.IO.Path]::GetFullPath(
        $ReleaseGameRoot
    ).TrimEnd('\') + '\'


    foreach ($Reference in $ProjectReferences)
    {
        $Include = $Reference.Include

        if ([string]::IsNullOrWhiteSpace($Include))
        {
            continue
        }

        # 일반적인 Visual Studio ProjectReference는
        # vcxproj 기준 상대경로이므로 절대경로로 변환
        $ReferencePath = Join-Path `
            $EngineProjectDir `
            $Include

        $ReferencePath = [System.IO.Path]::GetFullPath(
            $ReferencePath
        )


        # Games\Release_Game 안에 있는 프로젝트인지 검사
        if ($ReferencePath.StartsWith(
            $ReleaseGameRootFull,
            [System.StringComparison]::OrdinalIgnoreCase))
        {
            $GameProjects += $ReferencePath
        }
    }


    # ------------------------------------------------------------
    # 게임 프로젝트 검사
    # ------------------------------------------------------------

    if ($GameProjects.Count -eq 0)
    {
        throw @"
ForestPearlEngine이 Reference하고 있는 게임 프로젝트를 찾지 못했습니다.

게임 프로젝트는 다음 경로 아래에 있어야 합니다:

$ReleaseGameRoot
"@
    }


    if ($GameProjects.Count -gt 1)
    {
        Write-Host ""
        Write-Host "[ERROR] 게임 프로젝트가 여러 개 발견되었습니다."

        foreach ($GameProject in $GameProjects)
        {
            Write-Host "  - $GameProject"
        }

        throw "Release_Game 아래의 게임 ProjectReference는 하나만 존재해야 합니다."
    }


    # ------------------------------------------------------------
    # 게임 프로젝트 정보 추출
    # ------------------------------------------------------------

    $GameProjectPath = $GameProjects[0]

    $GameProjectName = [System.IO.Path]::GetFileNameWithoutExtension(
        $GameProjectPath
    )

    $GameProjectDir = [System.IO.Path]::GetDirectoryName(
        $GameProjectPath
    )

    $GameAssetsPath = Join-Path `
        $GameProjectDir `
        "Assets"


    Write-Host "[Package] Game Project : $GameProjectPath"
    Write-Host "[Package] Game Name    : $GameProjectName"
    Write-Host "[Package] Game Assets  : $GameAssetsPath"
    Write-Host ""


    # ------------------------------------------------------------
    # Runner exe 확인
    # ------------------------------------------------------------

    if (-not (Test-Path $RunnerExe))
    {
        throw "Runners 실행 파일을 찾을 수 없습니다: $RunnerExe"
    }


    # ------------------------------------------------------------
    # 최종 exe 이름
    #
    # Runners.exe
    #     ↓
    # MyGame.exe
    # ------------------------------------------------------------

    $GameExePath = Join-Path `
        $ReleaseDir `
        "$GameProjectName.exe"


    # 기존 게임 exe가 있다면 삭제
    if (Test-Path $GameExePath)
    {
        Remove-Item `
            -LiteralPath $GameExePath `
            -Force
    }


    Copy-Item `
        -LiteralPath $RunnerExe `
        -Destination $GameExePath `
        -Force


    # Runners.exe 제거
    #
    # 복사 후 삭제하는 이유:
    # 빌드 자체는 Runners.exe로 정상 완료시키고
    # 패키징 단계에서 게임 exe로 변경하기 위함
    if (-not $RunnerExe.Equals(
        $GameExePath,
        [System.StringComparison]::OrdinalIgnoreCase))
    {
        Remove-Item `
            -LiteralPath $RunnerExe `
            -Force
    }


    Write-Host "[Package] Executable   : $GameExePath"


    # ------------------------------------------------------------
    # 기존 Release\Assets 제거
    #
    # 이전 게임 Asset이 남는 것을 방지
    # ------------------------------------------------------------

    if (Test-Path $ReleaseAssetsPath)
    {
        Write-Host "[Package] Cleaning Assets..."

        Remove-Item `
            -LiteralPath $ReleaseAssetsPath `
            -Recurse `
            -Force
    }


    New-Item `
        -ItemType Directory `
        -Path $ReleaseAssetsPath `
        -Force `
        | Out-Null


    # ------------------------------------------------------------
    # 폴더 내용 복사 함수
    # ------------------------------------------------------------

    function Copy-AssetDirectory
    {
        param
        (
            [string]$Source,
            [string]$Destination
        )


        if (-not (Test-Path $Source))
        {
            Write-Host "[Package] Assets not found, skipping:"
            Write-Host "          $Source"

            return
        }


        Write-Host "[Package] Copy Assets:"
        Write-Host "          $Source"
        Write-Host "       -> $Destination"


        # Assets 폴더 자체가 아니라
        # Assets 안의 내용만 복사
        Copy-Item `
            -Path (Join-Path $Source "*") `
            -Destination $Destination `
            -Recurse `
            -Force
    }


    # ------------------------------------------------------------
    # 1. Engine Assets
    # ------------------------------------------------------------

    Copy-AssetDirectory `
        -Source $EngineAssetsPath `
        -Destination $ReleaseAssetsPath


    # ------------------------------------------------------------
    # 2. Game Assets
    #
    # Game을 나중에 복사하므로
    # 동일한 파일은 Game Asset이 Engine Asset을 Override
    # ------------------------------------------------------------

    Copy-AssetDirectory `
        -Source $GameAssetsPath `
        -Destination $ReleaseAssetsPath


    # ------------------------------------------------------------
    # 완료
    # ------------------------------------------------------------

    Write-Host ""
    Write-Host "==========================================="
    Write-Host " Packaging Complete"
    Write-Host "==========================================="
    Write-Host ""
    Write-Host " Game : $GameProjectName"
    Write-Host " EXE  : $GameExePath"
    Write-Host " Asset: $ReleaseAssetsPath"
    Write-Host ""

    exit 0
}
catch
{
    Write-Host ""
    Write-Host "==========================================="
    Write-Host " Packaging Failed"
    Write-Host "==========================================="
    Write-Host ""

    Write-Error $_

    exit 1
}