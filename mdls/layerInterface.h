#pragma once
#include "element.h"

namespace mdls
{
	typedef class NodeMap;

	class ILayer
	{
		public:
		virtual void proceed() = 0;
		virtual void proceed_inverse() = 0;

		virtual void set_up_nodes() = 0;	




	//	virtual void write_node_map(NodeMap**) = 0;
	};

}