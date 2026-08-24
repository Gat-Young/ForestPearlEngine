#include "FPViewPortClient.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"

FPViewPortClient::FPViewPortClient()
{

}

std::vector<FPViewPort*> FPViewPortClient::GetViewPort(FPViewPortName ViewPortName)
{
	return ViewPortArray[static_cast<size_t>(ViewPortName)];
}

void FPViewPortClient::CreateViewPort()
{
	FPGameProjectSetting* ProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());

	//ViewPort 持失
	float Aspect = ProjectSetting->GetDisplayAspect();

	int DisplayWidth = ProjectSetting->GetDisplayWidth();
	int DisplayHeight = ProjectSetting->GetDeisplayHeight();

	//MainGameViewPort
	std::vector<FPViewPort*> MainGame;
	MainGame.resize(1);
	MainGame[0] = new FPViewPort();

	MainGame[0]->Width = ProjectSetting->GetDisplayWidth();
	MainGame[0]->Height = ProjectSetting->GetDeisplayHeight();
	ViewPortArray[static_cast<size_t>(FPViewPortName::MainGameViewPort)] = std::move(MainGame);

	//UIViewPort
	std::vector<FPViewPort*> UIViewPort;
	UIViewPort.resize(1);
	UIViewPort[0] = new FPViewPort();

	UIViewPort[0]->Width = ProjectSetting->GetDisplayWidth();
	UIViewPort[0]->Height = ProjectSetting->GetDeisplayHeight();
	ViewPortArray[static_cast<size_t>(FPViewPortName::UIViewPort)] = std::move(UIViewPort);

	//TripleWaySplitViewPort

	std::vector<FPViewPort*> TripleWaySplitViewPort;
	TripleWaySplitViewPort.resize(3);
	TripleWaySplitViewPort[0] = new FPViewPort();
	TripleWaySplitViewPort[1] = new FPViewPort();
	TripleWaySplitViewPort[2] = new FPViewPort();

	CalculateViewPortSize(TripleWaySplitViewPort, DisplayWidth, DisplayHeight, Aspect);
	ViewPortArray[static_cast<size_t>(FPViewPortName::TripleWaySplitViewPort)] = std::move(TripleWaySplitViewPort);
}

void FPViewPortClient::CalculateViewPortSize(std::vector<FPViewPort*> ViewPort, int Width, int Heigth, float Aspect)
{
	int ViewPortLength = ViewPort.size();
	if (ViewPortLength == 0) return;
	if (ViewPortLength == 1)
	{
		ViewPort[0]->Width = Width;
		ViewPort[0]->Height = Heigth;
		return;
	}

	bool LongAxisVertical = Heigth > Width;

	int NewViewPortWidth = 0;
	int NewViewPortHeight = 0;

	if (LongAxisVertical)
	{
		NewViewPortHeight = Heigth / ViewPortLength;
		NewViewPortWidth = NewViewPortHeight * Aspect;

		for (int i = 0; i < ViewPortLength; ++i)
		{
			ViewPort[i]->TopLeftX = Width/2 - NewViewPortWidth/2;
			ViewPort[i]->TopLeftY = 0.0f + (NewViewPortHeight * i);
			ViewPort[i]->Width = NewViewPortWidth;
			ViewPort[i]->Height = NewViewPortHeight;
		}
	}
	else
	{
		NewViewPortWidth = Width / ViewPortLength;
		NewViewPortHeight = NewViewPortWidth * (1 / Aspect);

		for (int i = 0; i < ViewPortLength; ++i)
		{
			ViewPort[i]->TopLeftX = 0.0f + (NewViewPortWidth * i);
			ViewPort[i]->TopLeftY = Heigth/2 - NewViewPortHeight/2;
			ViewPort[i]->Width = NewViewPortWidth;
			ViewPort[i]->Height = NewViewPortHeight;
		}
	}
}

void FPViewPortClient::CalculateAllViewPortSize()
{
	FPGameProjectSetting* ProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());

	//ViewPort 持失
	float Aspect = ProjectSetting->GetDisplayAspect();

	int DisplayWidth = ProjectSetting->GetDisplayWidth();
	int DisplayHeight = ProjectSetting->GetDeisplayHeight();
	
	for (int i = 0; i < static_cast<size_t>(FPViewPortName::FPViewPort_MAX_SIZE); ++i)
	{
		CalculateViewPortSize(ViewPortArray[i], DisplayWidth, DisplayHeight, Aspect);
	}
}
