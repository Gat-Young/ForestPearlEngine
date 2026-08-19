#include "FPViewPortClient.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"

FPViewPortClient::FPViewPortClient()
{
	FPGameProjectSetting* ProjectSetting = static_cast<FPGameProjectSetting*>(FPGameInstance::Get().GetGameProjectSetting());
	//ViewPort »ý¼º

	float Aspect = ProjectSetting->GetDisplayAspect();
	
	int DisplayWidth = ProjectSetting->GetDisplayWidth();
	int DisplayHeight = ProjectSetting->GetDeisplayHeight();

	//MainGameViewPort
	FPViewPort* MainGame[1];
	MainGame[0] = new FPViewPort();


	MainGame[0]->Width = ProjectSetting->GetDisplayWidth();
	MainGame[0]->Height = ProjectSetting->GetDeisplayHeight();
	ViewPortArray[static_cast<size_t>(FPViewPortName::MainGameViewPort)] = MainGame;

	//UIViewPort
	FPViewPort* UIViewPort[1];
	UIViewPort[0] = new FPViewPort();
	UIViewPort[0]->Width = ProjectSetting->GetDisplayWidth();
	UIViewPort[0]->Height = ProjectSetting->GetDeisplayHeight();
	ViewPortArray[static_cast<size_t>(FPViewPortName::UIViewPort)] = UIViewPort;

	//TripleWaySplitViewPort

	FPViewPort* TripleWaySplitViewPort[3];
	TripleWaySplitViewPort[0] = new FPViewPort();
	TripleWaySplitViewPort[0]->TopLeftX = 0.0f;
	TripleWaySplitViewPort[0]->TopLeftY = ;
	TripleWaySplitViewPort[0]->Width = ProjectSetting->GetDisplayWidth()/3;
	TripleWaySplitViewPort[0]->Height = ProjectSetting->GetDeisplayHeight();

}

std::vector<FPViewPort*> FPViewPortClient::GetViewPort(FPViewPortName ViewPortName)
{
	return ViewPortArray[static_cast<size_t>(ViewPortName)];
}

void FPViewPortClient::CalculateViewPortSize(std::vector<FPViewPort*> ViewPort, int Width, int Heigth, float Aspect)
{
	//²ô¾Ó React React~
	int ViewPortLength = ViewPort.size();

	bool LongAxisVertical = Heigth > Width;

	int NewViewPortWidth = 0;
	int NewViewPortHeight = 0;

	if (LongAxisVertical)
	{
		NewViewPortHeight = Heigth / ViewPortLength;
		NewViewPortWidth = NewViewPortHeight * Aspect;

		for (int i = 0; i < ViewPortLength; ++i)
		{
			ViewPort[i]->TopLeftX = Width - NewViewPortWidth/2;
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
