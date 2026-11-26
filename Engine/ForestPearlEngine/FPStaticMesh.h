#pragma once
#include "FPStreamableRenderAssetr.h"
#include "Renderers/FPRenderingCommon.h"
#include "FTransform.h"
#include <map>
#include <vector>
#include <string>

class FPMaterialInterface;

class FPStaticMesh : public FPStreamableRenderAsset
{
	private:
		//Mesh Data 정보
		std::string MeshPath;

		//Mesh가 가지는 소켓 정보
		std::map<std::string, FTransform> Sockets;
	
		//StaticMesh가 기본적으로 가지는 Material 정보
		FPMaterialInterface* Material = nullptr;

		int StencilRefValue = 0;
		bool bCustomDepthStencil = false;

		//Mesh BoundingBox(AABB)
		FPAABB BoundingBox;

		//Mesh BoxSphereBounds (BoundingBox로부터 계산)
		FPBoxSphereBounds Bounds;

	public:
		FPStaticMesh(std::string StaticMeshPath);

		void AddSocketData(std::string SocketName, FTransform SocketTransform);
		void SetMaterial(FPMaterialInterface* Material);
		FPMaterialInterface** GetStaticMeshMaterial();

		void SetStencilRefValue(int StencilRefValue);
		int GetStencilRefValue();
		void SetCustomDepthStencil(bool bCustomDepthStencil);
		bool GetCustomDepthStencil();

		std::string* GetMeshPath();
		bool GetSocketTransform(std::string SocketName, FTransform& OutSocketTransform);

		void SetBoundingBox(FPAABB BoundingBox);
		FPAABB GetBoundingBox();

		FPBoxSphereBounds GetBounds();
};