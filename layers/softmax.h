#pragma once
#include "..\objects\layert.h"

namespace mdls
{
	
	class softmax : public layer
	{
	private:
		static int count;
	public:
		softmax();
		softmax(tensor* input );
		
		virtual void proceed();
		virtual void proceed_inverse();
		
		virtual void initialize();

		virtual int get_layer_count() { return count; };

	};

}