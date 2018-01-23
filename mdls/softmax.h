#pragma once
#include "layert.h"
#include "of.h"

namespace mdls
{

	class softmax : public layer
	{
	private:
		virtual void write_node_map() {};
		static int count;
	public:
		softmax();
		softmax(tensor* input );
		
		virtual void proceed();
		virtual void proceed_inverse();
		
		virtual void initialize(int);

		virtual int get_layer_count() { return count; };

	};

}