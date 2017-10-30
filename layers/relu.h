#pragma once
#include "..\objects\layert.h"

namespace mdls 
{

	class relu : public layer
	{

		static int count;
	public:
			relu();
			relu(tensor* input);

			virtual void proceed();
			virtual void proceed_inverse();
			virtual void initialize();
			virtual int get_layer_count() { return count; };

			int* arr_negative;
	};
}
