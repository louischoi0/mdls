#pragma once
#include "cube_loop.h"
#include <functional>
#include "counter_extens.h"
using namespace mdls;

namespace loop
{
	typedef std::function<void(elemt* , elemt* , elemt* )> lambda_base_p3;
	typedef std::function<void(elemt* p, elemt*, elemt*, const shape&, const shape&, const shape&)> lambda_inner_s3;
	typedef std::function<void(elemt* p, elemt*, elemt*, const shape&, const shape&, const shape&, int stride_col, int stride_row)> lambda_inner_s3_with_stride;
	typedef std::function<void(elemt* p, elemt*, elemt*, const shape&, const shape&, const shape& , const shape& , int stride_col, int stride_row)> lambda_inner_s3_with_stride_fs;

	typedef std::function<void(tensor*, tensor*, tensor& , lambda_inner_s3) > lambda_tns;
	typedef std::function<void()> lambda_void;

	typedef counter fct;
	typedef void(*CALLBACK_WITH_FCT_TT) (tensor* t0, tensor* t1, fct* fc);
	typedef void(*TASK_LOCAL_EE)(elemt* e, elemt* ee,  int local_size);
	typedef void(*TASK_TTT_SS)(elemt*, elemt*, elemt* , const shape&, const shape&);
	typedef void(*TASK_TTT_SSS)(elemt*, elemt*, elemt*, const shape&, const shape&, const shape& );

	typedef void(*TASK_WITH_FCT_TT)(elemt*, elemt*, counter*);

	typedef void(*TASK_T)(elemt* e);
	
	void tensor_loop_channel_first_with_stride
	(tensor* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_CHN_LOOP CC);
	void tensor_loop(tensor* p, tensor* out, int stride_row, int stride_col, CALLBACK_WITH_FCT_TT CC);

	void tensor_loop(target_dm d, tensor* p, tensor* out, int stride_row, int stride_col, CALLBACK_WITH_FCT_TT CC);
	void tensor_loop(tensor* t, TASK_T task, int stride_row, int stride_col);
	void loop_with_count(element* p, element& out, TASK_WITH_FCT_TT task, counter* counter);
	void loop_with_count(element* p, element& out, int str_r, int str_c, TASK_WITH_FCT_TT task, counter* f);
	void loop_with_count(element* p, element& out, int str_r, int str_c, TASK_WITH_FCT_TT task, counter* f);

	void loop_tns_row(tensor* t, tensor& out , TASK_LOCAL_EE task);
	void loop_tns_f_dot_inc_batch(element* p, element* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC);
	void loop_tns_f_dot_inc_chn(element* p, element* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC);
	void loop_tns_surface_mask(tensor* t, tensor* mask, tensor& out, int stride_row, int stride_col, TASK_TTT_SS task);

	void loop_tns_inc_output_by_one(tensor* t, tensor* mask, tensor& out, int stride_row, int stride_col, TASK_TTT_SS task);
	void tensor_loop(tensor* t, tensor* tt, tensor& out, int stride_col, int stride_row, TASK_TTT_SS T);
	void tensor_loop_with_filter_and_two_tensor(tensor* t, tensor* filter, tensor& out, int stride_col, int stride_row, TASK_TTT_SSS task0);
	void loop_tns_f_dot_linear_chn(element* p, element* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC);
	void loop_tensor_with_eq_chn_by_acm(tensor* p, tensor* pp, tensor& out, lambda_inner_s3 f);
	void loop_tensor_with_one_chn_repeat
	(elemt *p, elemt* pp, elemt* out,
		const shape& s0, const shape& s1, const shape& s2,
		int stride_col, int stride_row, lambda_inner_s3_with_stride f);
	void loop_tensor_with_one_chn_repeat_in_capture
	(elemt *p, elemt* pp, elemt* out,
		const shape& s0, const shape& s1, const shape& s2,
		int stride_col, int stride_row, lambda_void f);

	void loop_tensor_with_one_chn_repeat_linear
	(elemt *p, elemt* pp, elemt* out,
		const shape& s0, const shape& s1, const shape& s2,
		int stride_col, int stride_row, lambda_inner_s3_with_stride f);

	void loop_tensor_with_same_chn_count(tensor* t, tensor* tt, tensor& out, int , int , lambda_inner_s3_with_stride f);
	void chn_task_linear(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1, const shape& s2, int cols, int rows, lambda_inner_s3_with_stride f);

	void loop_tensor_with_one_chn_repeat_inc_outc(elemt *p, elemt* pp, elemt* out,
		const shape& s0, const shape& s1, const shape& s2,
		int stride_col, int stride_row, lambda_inner_s3_with_stride f);
	void loop_tensor_with_one_chn_repeat_inc_outc_fs(elemt *p, elemt* pp, elemt* out,
		const shape& s0, const shape& s1, const shape& s2, const shape& fs,
		int stride_col, int stride_row, lambda_inner_s3_with_stride_fs f);
}



