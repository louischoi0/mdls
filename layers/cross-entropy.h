#pragma once
#include "..\objects\layert.h"

namespace mdls
{
	class cross_entropy : public layer
	{
		static int count;
	public:
		cross_entropy();

		cross_entropy(tensor* input , tensor* true_lable);
		void initialize(int i);
		void initialize_inverse(tensor* );

		void proceed();
		void proceed_inverse();

		virtual bool proc_possible();
		virtual void set_output_inform_from_val();
		virtual int get_layer_count() { return count; };


	};

}

