#pragma once
#include "..\objects\layert.h"

namespace mdls
{

	class pooling : public layer
	{
	private:
		static int count;
		int* pooling_out_arr;
	
	public:
		pooling();
		pooling(tensor*);

		virtual void initialize();
		virtual void proceed();
		virtual void proceed_inverse();
		virtual bool proc_possible();
		virtual void set_output_inform_from_val();

		int* get_ref() { return pooling_out_arr; };

		virtual int get_layer_count() { return count; };

	};


}
