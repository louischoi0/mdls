#include "relu.h"
#include "of.h"

using namespace mdls;

int relu::count = 0;

relu::relu() :
layer(RELU)
{
	set_type_name("Relu");
	set_layer_inform(1, 1);

	pstatic_count = &relu::count;

	use_eqiv = false;
	inc_layer_count();

}


relu::relu(tensor* input) :
layer(RELU , input )
{
	set_type_name("Relu");

	pstatic_count = &relu::count;
		
	use_eqiv = false;
	inc_layer_count();

}



void relu::proceed()
{
	shape s = _tensor_map[Input]->get_shape();
	*_tensor_map[Output] = *_tensor_map[Input];
	
	_tensor_map[Weight]->reshape(s);
	_tensor_map[Weight]->initialize_elem_to(1);

	counter c = counter(s,s,s);
	
	c.initialize_log(1, s.Alloc_size);

	loop::loop_with_count
	(_tensor_map[Input], *_tensor_map[Output], _inf.stride_row, _inf.stride_col, callback::negative_to_zero, &c);

	arr_negative = (int*)malloc(sizeof(int) * _tensor_map[Output]->get_shape().Alloc_size);
	memcpy(arr_negative, c.get_log(0), sizeof(int) * _tensor_map[Output]->get_shape().Alloc_size);

	elemt* wp = _tensor_map[Weight]->get_p();
	int* ap = arr_negative;

	for (int i = 0; i < c.get_offset(2) ; i++, ap++)
		wp[*ap] = 0;

}

void relu::proceed_inverse()
{
	if (end_layer)
		end_layer_task();

	shape s0 = _tensor_map[Input]->get_shape();
	tensor bgrad(s0);

	if (end_layer)
		bgrad.initialize_elem_to(1);

	else
		bgrad = *_backward_lay->get_tensor(Grad_In);
	
	tfunc::tmultiply( &bgrad , _tensor_map[Weight] , *_tensor_map[Grad_In]);
	
	
}


void initialize_inverse()
{
}

void mdls::callback_for_relu(matrix* m1 , matrix* m2 , matrix& out)
{

}