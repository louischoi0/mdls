#include "poolingh.h"
#include "..\modules\counter_extens.h"

#include "..\operation\tensor_ops\of.h"

using namespace mdls;

int pooling::count = 0;

pooling::pooling( tensor* t) :
	layer(POOLING , t)
{
	set_type_name("Pooling");
	pstatic_count = &pooling::count;
	use_eqiv = false;
	inc_layer_count();

}


pooling::pooling() :
layer(POOLING)
{
	set_type_name("Pooling");
	pstatic_count = &pooling::count;
	use_eqiv = false;
	inc_layer_count();

}


void pooling::proceed()
{
	if (!init)
		initialize();
	
	shape s0 = _tensor_map[Input]->get_shape();
	shape s1 = _tensor_map[Weight]->get_shape();
	shape s2 = s0;

	counter c(s0, s1, s2);
	loop::loop_with_count(_tensor_map[Input], *_tensor_map[Output], _inf.stride_row , _inf.stride_col, callback::max_log, &c);

	pooling_out_arr = (int*)malloc(sizeof(int) * _tensor_map[Output]->get_shape().Alloc_size);

	memcpy(pooling_out_arr, c.get_log(0), sizeof(int) * _tensor_map[Output]->get_shape().Alloc_size);

}

void pooling::proceed_inverse()
{
	tensor bgrad;
	if (end_layer)
	{
		shape is = _tensor_map[Output]->get_shape();
		bgrad.reshape(is);

		bgrad.initialize_elem_to(1);
	}

	else
		bgrad = *_backward_lay->get_tensor(Grad_In);

	_tensor_map[Grad_In]->reshape(_tensor_map[Input]->get_shape());
	_tensor_map[Grad_In]->initialize_elem_to(0);

	int c = _tensor_map[Output]->get_shape().Alloc_size;
	int* p = pooling_out_arr;

	elemt* pig = _tensor_map[Grad_In]->get_p();
	elemt* bp = bgrad.get_p();

	for (int i = 0; i < c; p++, i++)
		pig[*p] = *bp++;

}

void pooling::initialize()
{
	int dvr = macro::get_dv_size(R(_tensor_map[Input]), _inf.stride_row, _inf.stride_row);
	int dvc = macro::get_dv_size(C(_tensor_map[Input]), _inf.stride_col, _inf.stride_row);

	shape s(B(_tensor_map[Input]), D(_tensor_map[Input]), dvc, dvr);
	_tensor_map[Output]->reshape(s);

	init_bias();
	init = true;
}


bool pooling::proc_possible()
{
	if (!layer::proc_possible())
		return false;

	shape s0 = _tensor_map[Input]->get_shape();
	shape s1 = _tensor_map[Weight]->get_shape();

	int dvr = macro::get_dv_size(s0.Row , s1.Row, _inf.stride_row);
	int dvc = macro::get_dv_size(s0.Col, s1.Col, _inf.stride_col);

	if (dvr * dvc < 0)
		return false;
	else 
		return true;
	
}


void pooling::set_output_inform_from_val()
{


}