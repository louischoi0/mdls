#include "session.h"

using namespace mathm;
#define LAYER_TYPE_NUM 10

session::session() :
	input(false),
	weight(false)
{
	init();
}


session::session(int size) :
first(true),
input(false),
weight(false),
layer_count(size),
_occupied_size(0),

type_session(CUSTOM),
session_route(new cube_map[6])

{
	init();
}

session::session(session_type t) :
first(true),
layer_count(size_for_session_type[t]),
_occupied_size(0),
input(false),
weight(false),

session_route(new cube_map[6]),
type_session(t)
{
	init();

}


void session::make_layer_type_arr(session_type t, layer_ex_type* tt)
{

}

void session::init()
{
	callback_after_layer_runs = new layerTask[MAX_LAYER_COUNT];

	for (int j = 0; j < MAX_LAYER_COUNT; j++)
		callback_after_layer_runs[j] = callback::nop;

	occupied = new int[MAX_LAYER_COUNT] { 0, };

	exist_count = new int[20]{ 0 , };
	layer_arr = new ltype[30];
	layer_ref = new layer*[MAX_LAYER_COUNT];
	
	index_layer_by_type = new int*[LAYER_TYPE_NUM];

	for (int i = 0; i < LAYER_TYPE_NUM; i++)
		index_layer_by_type[i] = new int[MAX_LAYER_COUNT]{ 0 , };


}

void mdls::session::add_layer(layer_ex_type t)
{
	layer_arr[count_added] = t;

	index_layer_by_type[t][exist_count[t]++] = count_added;

	count_added++;
}

int mdls::session::get_index(layer_extended_enum type, int index)
{
	return index_layer_by_type[type][index];
}

void mdls::session::run()
{
	if(input)
		layer_ref[0]->set_tensor(Input, _input_tensor);

	if (weight)
		layer_ref[0]->set_tensor(Weight, _weight_tensor);

	for (int i = 0; i < _occupied_size; i++)
	{
		if(!layer_ref[i]->proc_possible())
		{
			cout << "Layer have not weight or input tensor at layer num " << i << endl;
			return;
		}
	
		layer_ref[i]->proceed();
		callback_after_layer_runs[i](layer_ref[i]);
		
		layer_ref[i]->set_tag_inform();

		if (i < _occupied_size - 1)
		{
			layer_ref[i + 1]->set_tensor(Input, layer_ref[i]->get_output_tensor());
			layer_ref[i]->set_backward_lay(layer_ref[i + 1]);
		}
	}

	for (int i = _occupied_size - 1; i >= 0; i--)
	{
		layer_ref[i]->proceed_inverse();
	}

}

void mdls::session::print_object()
{
	tfunc::print_object(layer_ref[0]->get_input_tensor());
	for (int i = 0; i < _occupied_size; i++)
		tfunc::print_object(layer_ref[i]->get_tensor(Output));
	for (int i = 0; i < _occupied_size; i++)
		tfunc::print_object(layer_ref[i]->get_tensor(Grad_In));


}


void mdls::session::add_layer(layer* l)
{
	ltype t = l->get_type();

	layer_arr[count_added] = t;
	occupied[count_added] = 1;

	index_layer_by_type[t][exist_count[t]++] = count_added;

	layer_ref[count_added] = l;
	count_added++;

}

void mdls::make_layer(layer** c, layer_ex_type t)
{
	if (t == CONVOLUTION)
		*c = new convolution();
	else if (t == AFFINE)
		*c = new affine();
	else if (t == RELU)
		*c = new relu();
	else if (t == SOFTMAX)
		*c = new softmax();
	else if (t == POOLING)
		*c = new pooling();
	else if (t == BATCH_NORMALIZATION)
		*c = new batch_normalization();

	else
		return;
}

void session::preprocessing()
{
	for (int i = 0; i < count_added; i++)
	{
		if (occupied[i] == 0)
			make_layer(&layer_ref[i], layer_arr[i]);
		
		if (i != 0)
			layer_ref[i]->set_forward_lay(layer_ref[i - 1]);	
	
		layer_ref[i]->set_this_layer_count(get_layer_count(i));
	}

	layer_ref[0]->set_start_layer(true);
	layer_ref[count_added - 1]->set_end_layer(true);

	_occupied_size = count_added;

}



cube_iter session::get_cube_iter(layer_ex_type t, int index, int pos)
{
	return NULL;
}


void session::init_map()
{

}


void session::set_layer_inform
(layer_ex_type t, int cindex, int depth , int ninput, int rinput, int cinput,
	int neqiv, int reqiv, int ceqiv, int rstride, int cstride )
{

}

void session::set_layer_weight_tensor(layer_extended_enum ly , int index , tensor* tns)
{
	int idx = get_index(ly , index);
	layer_ref[idx]->set_tensor(Weight , tns);
}

void session::set_layer_input_tensor(layer_extended_enum ly, int index, tensor* tns)
{
	int idx = get_index(ly, index);
	layer_ref[idx]->set_tensor(Input, tns);
}

void session::inverse_process()
{

}

void session::inverse_preprocess()
{



}

void session::process()
{




}

void session::process_unt(int unt)
{

}

layer *session::get_layer(layer_extended_enum ly, int index)
{
	return layer_ref[index_layer_by_type[ly][index]];
}

layer *session::get_layer(int index)
{
	return layer_ref[index];
}

void session::set_layer_tensor(ltype l, int idx, Tensor_Index_In_Layer pos, tensor* t)
{
	int idxx = get_index(l, idx);
	layer_ref[idxx]->set_tensor(pos, t);

}
int session::get_layer_count(int idx)
{
	for(int i = 0; i < LAYER_TYPE_NUM; i++)
		for (int j = 0; j < MAX_LAYER_COUNT; j++)
		{
			if (index_layer_by_type[i][j] == idx)
			{
				return j;
			}

		}

}