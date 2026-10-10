#include "FPGizmoNormalLineComponent.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <iostream>

FPGizmoNormalLineComponent::FPGizmoNormalLineComponent(FPActor* Owner, std::string& StaticMeshPath) : FPGizmoComponent(Owner,"FPGizmoNormalLineComponent", StaticMeshPath + "_NormalLine")
{
	FPGIZMO_NORMALLINEINFO NormalLine;
	NormalLine.r = 0.0f;
	NormalLine.g = 1.0f;
	NormalLine.b = 0.0f;
	NormalLine.a = 1.0f;
	MakeNormalLine(StaticMeshPath , NormalLine);
	RegistGizmoRenderList();
}

FPGizmoNormalLineComponent::~FPGizmoNormalLineComponent()
{
}

void FPGizmoNormalLineComponent::MakeNormalLine(const std::string& StaticMeshPath, FPGIZMO_NORMALLINEINFO& Line)
{
	//AssetManager 가져오기
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	//이미 노말 VertexBuffer가 존재하는지 검사
	if (AssetManager->HasVertexBuffer(StaticMeshPath + "_NormalLine")) { return; }

	//Manager로 부터 Static Mesh의 Vertics 정보 가져오기
	std::vector<FPMeshData> MeshData = AssetManager->GetMeshData(StaticMeshPath);


	//Static Mesh로 부터 Normal Vector 정보를 만들어냄
	std::vector<FPGIZMO_VERTEX> GizmoDatas;

	//점점을 순회하면서 노말 정보를 바탕으로 노말 Vertex를 만듬
	for (FPMeshData& InMeshData : MeshData)
	{
		for (VERTEX& Vertex : InMeshData.Vertices)
		{
			FPGIZMO_VERTEX StartPoint;
			StartPoint.x = Vertex.x;
			StartPoint.y = Vertex.y;
			StartPoint.z = Vertex.z;
			StartPoint.r = Line.r;
			StartPoint.g = Line.g;
			StartPoint.b = Line.b;
			StartPoint.a = Line.a;

			FPGIZMO_VERTEX EndPoint;
			EndPoint.x = (Vertex.x + Vertex.nx) * Line.length;
			EndPoint.y = (Vertex.y + Vertex.ny) * Line.length;
			EndPoint.z = (Vertex.z + Vertex.nz) * Line.length;
			EndPoint.r = Line.r;
			EndPoint.g = Line.g;
			EndPoint.b = Line.b;
			EndPoint.a = Line.a;

			GizmoDatas.push_back(StartPoint);
			GizmoDatas.push_back(EndPoint);
		}
	}

	isActive = true;

	MakeVertexBuffer(GizmoDatas, GizmoMeshPath);
}

void FPGizmoNormalLineComponent::SetGizmoMeshPath(const std::string& StaticMeshPath)
{
	this->GizmoMeshPath = StaticMeshPath + "_NormalLine";
	FPGIZMO_NORMALLINEINFO NormalLine;
	NormalLine.r = 0.0f;
	NormalLine.g = 1.0f;
	NormalLine.b = 0.0f;
	NormalLine.a = 1.0f;
	MakeNormalLine(StaticMeshPath, NormalLine);
	RegistGizmoRenderList();
}
