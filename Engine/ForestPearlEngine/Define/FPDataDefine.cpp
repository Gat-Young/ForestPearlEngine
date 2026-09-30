#include "FPDataDefine.h"

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
