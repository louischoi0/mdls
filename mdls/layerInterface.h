#pragma once
#include "element.h"


class layerITF
{
public:
	virtual void proceed() = 0;
	virtual void proceed_inverse() = 0;
	virtual void write_node_map(NodeMap** )=0;
};

	