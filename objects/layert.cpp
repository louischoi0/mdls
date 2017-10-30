#include "layert.h"
#include "..\operation\tensor_ops\of.h"

using namespace mdls;
typedef layer_w_t layer;


std::string str_route(int i)
{
	if (i == 0)
		return " Input";
	else if (i == 1)
		return " Weight";
	else if (i == 2)
		return " Output";
	else if (i == 3)
		return " Grad_in";
	else if (i == 4)
		return " Grad_Wgt";
	else if (i == 5)
		return " Bias";

}

layer::layer_w_t() :
	end_layer(false),
	start_layer(false),
	init(false),
	bias_rand(true)
{
	initialize_tensor();
	eqiv_set();
}


layer::layer_w_t(ly_type t) :
	_tensor_map(new tensor*[5]),
	_type(t),
	end_layer(false),
	start_layer(false),
	init(false),
	bias_rand(true)
{
	initialize_tensor();
	eqiv_set();
}


layer::layer_w_t(ly_type t, tensor* input) :
	_tensor_map(new tensor*[5]),
	_type(t),
	end_layer(false),
	start_layer(false),
	init(false),
	bias_rand(true)
{
	set_tensor(Tensor_Index_In_Layer::Input, input);

	initialize_tensor();
	eqiv_set();

}

layer::layer_w_t(ly_type t , tensor* input, tensor* eqiv) :
	_tensor_map(new tensor*[5]),
	_type(t),
	init(false),
	bias_rand(true)
{

	set_tensor(Tensor_Index_In_Layer::Input, input);
	set_tensor(Tensor_Index_In_Layer::Weight, eqiv);

	initialize_tensor();
	eqiv_set();

}




void layer::set_layer_inform(int stride_row, int stride_col, int padding_row , int padding_col )
{
	_inf.stride_row = stride_row;
	_inf.stride_col = stride_col;
	_inf.padding_row = padding_row;
	_inf.padding_col = padding_col;
}


void layer::initialize_tensor()
{
	_tensor_map = new tensor*[6];

	for (int i = 0; i < 6; i++)
		_tensor_map[i] = new tensor();

	_inf.ht[Weight] = !use_eqiv;

}


void layer::proceed_inverse()
{

}

void layer::set_output_inform_from_val()
{

}

void layer::end_layer_task()
{
	shape s = _tensor_map[Input]->get_shape();

	_tensor_map[Grad_In]->reshape(s);
	_tensor_map[Grad_In]->initialize_elem_to(1);


}


void layer::eqiv_set()
{
	if (_type == CONVOLUTION, AFFINE, CROSS_ENTROPY)
		use_eqiv = true;
	else
		use_eqiv = false;

}

bool layer::proc_possible()
{
	if (start_layer)
		if (!_inf.ht[Input])
			return false;
	
	if (use_eqiv)
		return _inf.ht[Weight];
	
	else
		return true;
}

void layer::set_tag_inform()
{
	int count = get_this_layer_count();

	if ( start_layer && _inf.ht[0] )
		_tensor_map[Input]->set_tag(type_name + std::to_string(count) + str_route(0));

	for (int i = 1; i < 6; i++)
	{
		if(_inf.ht[i])
			_tensor_map[i]->set_tag(type_name + std::to_string(count) + str_route(i) );
	}
}

void layer::init_bias()
{

	_tensor_map[Bias]->reshape(_tensor_map[Output]->get_shape());


	if (bias_rand)
	{
		tfunc::init_tensor_with_random(*_tensor_map[Bias], 0, 1);

	}
	else
		_tensor_map[Bias]->initialize_elem_to(0);


	_inf.ht[Bias] = true;
}

void layer::add_bias()
{
	tfunc::tadd(*_tensor_map[Output], _tensor_map[Bias]);
}

