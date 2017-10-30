#pragma once

#include "..\tensor_ops\tensor_loop.h"
#include "..\tensor_ops\tensor_callback.h"

namespace mdls
{
	namespace tfunc
	{
		void print_object(tensor* p);
		void nhwc_to_hwio(tensor* c, tensor& out, int stride_row, int stride_col, int count_row_to_copy, int count_col_to_copy);
		void add_padding(tensor& p, Dm n, int size);
	
		
		void softmax(tensor* p, tensor& out);
		void softmax_s(tensor* p, tensor& out);

		void init_tensor_ladder(tensor& c);
		void init_tensor_with_random(tensor& t,int s , int e );
	
		void im2_col(tensor* c, tensor& p, int strid_row, int strid_col, int count_row_to_copy, int count_col_to_copy);
		
		void edot(element* t, element* tt, tensor& out);
		void compact_col(tensor* t, int size);
		void compact_channel(tensor* t, int size);
		void tmultiply(tensor* t, tensor* tt, tensor& out);
	
		void tdot(tensor* t, tensor* tt, tensor& out);
		
		void channel_times(tensor& t, int times);
		void batch_times(tensor& t , int times);

		void colmn_times(tensor& t ,int times);
		void batch_repeat(tensor& t, int times);

		void compact_batch_evrg(tensor& t, int nsize);
		void compact_batch_accum(tensor& t, int nsize);
		void sum_tensor(target_dm dm, const tensor* p, tensor& out);
		void tdot_acm(tensor* p, tensor* pp, tensor& out);
		void af_inv_acm_chn(tensor* dout, tensor* in_tsp, tensor* weight_tsp, tensor& dout_in, tensor &dweight_in);
		void tdot_n(tensor* t, tensor* tt, tensor& o);
	
		void tadd(tensor& t, tensor*tt);
	
	}

}

