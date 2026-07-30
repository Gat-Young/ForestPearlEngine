#pragma once
#include "FPGameInstance.h"
#include "FPGameProjectClassRegistry.h"
#include "FPAssetManager.h"
#include "Utility/FPPathManager.h"

void RegistProjectName();

void LoadLevel();

void LoadClassRegist();

void LoadAssets();

std::string ReturnStartLevel();