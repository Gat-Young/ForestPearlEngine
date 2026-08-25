#pragma once
#include "FPGameInstance.h"
#include "FPGameProjectClassRegistry.h"
#include "FPAssetLoader.h"
#include "Utility/FPPathManager.h"
#include "FPGameProjectSetting.h"


void RegistProjectName();

void LoadLevel();

void LoadClassRegist();

void LoadAssets();

std::string ReturnStartLevel();