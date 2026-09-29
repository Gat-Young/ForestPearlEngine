#pragma once
#include <vector>

//정점 구조체
struct VERTEX
{
	float x, y, z;		//좌표 Position
	float r, g, b, a;	//색상 Diffuse Color
};

//메시 데이터 구조체
struct FPMeshData
{
	std::string Name;
	std::vector<VERTEX> Vertices;
};

//소켓용 트랜스폼
struct SOCKET_TRANSFORM
{
	std::string SocketName;

	float Location_x, Location_y, Location_z;
	float Rotation_x, Rotation_y, Rotation_z;
	float Scale_x, Scale_y, Scale_Z;
};

//Static 메시 데이터 구조체
struct FPStaticMeshData
{
	std::string MeshPath;
	std::string MeshTopology;
	std::vector<SOCKET_TRANSFORM> Sockets;
	std::string MaterialName;
};

//VertexBuffer Topology
enum class Topology
{
	TRIANGLELIST,
	TRIANGLESTRIP,
	LINELIST
};

Topology StringToTopology(std::string Topo)
{
	if (Topo == "TRIANGLELIST")
	{
		return Topology::TRIANGLELIST;
	}

	if (Topo == "TRIANGLESTRIP")
	{
		return Topology::TRIANGLESTRIP;
	}

	if (Topo == "LINELIST")
	{
		return Topology::LINELIST;
	}

	return Topology::TRIANGLELIST;
}

//VertexBuffer 데이터 구조체
struct FPVertexBufferData
{
	void* VertexBuffer;
	int Size;
	int Stride;
	int Offset;
};

struct FPActorData
{
	std::string ClassName;
	std::string ActorName;
	float Location_x, Location_y, Location_z;
	float Rotation_x, Rotation_y, Rotation_z;
	float Scale_x, Scale_y, Scale_Z;
};