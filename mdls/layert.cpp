#include "layert.h"
#include "session.h"

using namespace mdls;
typedef layer_w_t layer;
const std::vector<string> strMap = { "Input", "Wehgit", "Output", "Grad_in", "Grad_Weight", "Bias" };

layer::layer_w_t(const session& s, tensor* in, tensor* eqiv) :
	_tensor_map(new tensor*[5]),
	init(false),
	sess(&s)
{
	set_tensor(Tensor_Index_In_Layer::Input, in);
	set_tensor(Tensor_Index_In_Layer::Weight, eqiv);

	initialize_tensor();
	eqiv_set();

}
layer::layer_w_t() :
	end_layer(false),
	start_layer(false),
	init(false)
{
	initialize_tensor();
	eqiv_set();
}

layer::layer_w_t(ly_type t) :
	_tensor_map(new tensor*[5]),
	_type(t),
	end_layer(false),
	start_layer(false),
	init(false)
{
	initialize_tensor();
	eqiv_set();
}


layer::layer_w_t(ly_type t, tensor* input) :
	_tensor_map(new tensor*[5]),
	_type(t),
	end_layer(false),
	start_layer(false),
	init(false)
{
	set_tensor(Tensor_Index_In_Layer::Input, input);

	initialize_tensor();
	eqiv_set();

}

layer::layer_w_t(ly_type t , tensor* input, tensor* eqiv) :
	_tensor_map(new tensor*[5]),
	_type(t),
	init(false)
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
	{
		if (_inf.ht[i])
			continue;
	
		_tensor_map[i] = new tensor();
		_inf.ht[i] = true;
	}
}

void layer::preprocessing()
{
	shape ss;

	if (_inf.ht[0])
	{
		ss = _tensor_map[0]->get_shape();
		_tensor_map[Tensor_Index_In_Layer::Output]->reshape(ss);
		_tensor_map[Tensor_Index_In_Layer::Grad_In]->reshape(ss);
		_tensor_map[Tensor_Index_In_Layer::Bias]->reshape(ss);

		if (!_inf.ht[1])
		{
			_tensor_map[Tensor_Index_In_Layer::Weight]->reshape(ss);
			_tensor_map[Tensor_Index_In_Layer::Grad_Wgt]->reshape(ss);
		}

	}

	if (_inf.ht[1])
	{
		ss = _tensor_map[1]->get_shape();
		_tensor_map[Tensor_Index_In_Layer::Grad_Wgt]->reshape(ss);
	}
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
		_tensor_map[Input]->set_tag(type_name + std::to_string(count) + strMap[0]);

	for (int i = 1; i < 6; i++)
	{
		if(_inf.ht[i])
			_tensor_map[i]->set_tag(type_name + std::to_string(count) + strMap[i] );
	}
}


