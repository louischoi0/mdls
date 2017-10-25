#pragma once
#include "layert.h"


namespace mdls
{
	
	class convolution : public layer
	{
	private :
		static int count;

	public:
		convolution();
		convolution(tensor* input, tensor* eqiv);

	private :
		tensor* input_converted_by_im2col;
		tensor* eqiv_converted_by_im2col;
		tensor* output_tensor_temporary;

		tensor* temporal_input_w_p;
		tensor* temporal_weight_w_p;
		void get_grad_weight(tensor* bgrad, tensor* in, tensor* filter, tensor& out);
	
	public:
		virtual void initialize();

		virtual void proceed();

		virtual void proceed_inverse();
		virtual void set_output_inform_from_val();
		virtual int get_layer_count() { return count; };
		virtual void ready_for_process();
		
		virtual bool proc_possible();
		void test(tensor* , tensor* , tensor* , tensor& );
	};


}
