#pragma once
#include "element.h"

namespace mdls
{
	typedef class NodeMap;

	class layerITF
	{
		public:
		virtual void proceed() = 0;
		virtual void proceed_inverse() = 0;
	//	virtual void write_node_map(NodeMap**) = 0;
	};

}