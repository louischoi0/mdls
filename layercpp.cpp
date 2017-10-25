#include "layer.h"
layer::layer(ly_type, matrix* input, matrix* set)
{

}


layer::layer(ly_type, matrix*)
{

}

layer::layer(ly_type t) :

occupied_arr(new size_t[6]{ 0 ,}),
allocated_arr(new size_t[6]{ 0, }),

_input_row(0),
_input_col(0),
_input_depth(0),

_eqiv_row(0),
_eqiv_col(0),
_eqiv_depth(0),

_output_row(0),
_output_col(0),
_output_depth(0),

_stride_col(1),
_stride_row(1),

_padding_row(0),
_padding_col(0),

_forward_lay(0),
_backward_lay(0),

start_layer(false),
end_layer(false),
got_eqiv(false),
got_input(false),
eqiv_informed(false),
havecube(new bool[3]{false , false , false}),
input_informed(true),
use_eqiv(true),

drop_out(false),

cube_iter_map(new cube_iter[6]),

type(t)

{
}


using namespace mdls;
layer::layer(ly_type t , cube* input) :
occupied_arr(new size_t[6]{ 0 , }),
allocated_arr(new size_t[6]{ 0, }),

_input_row(ROW(input)),
_input_col(COL(input)),
_input_depth(D(input)),

_eqiv_row(0),
_eqiv_col(0),
_eqiv_depth(0),

_output_row(0),
_output_col(0),
_output_depth(0),

_forward_lay(0),
_backward_lay(0),

_stride_row(1),
_stride_col(1),

_padding_row(0),
_padding_col(0),

start_layer(false),
end_layer(false),
got_eqiv(false),
got_input(false),
havecube(new bool[3]{ false , false , false }),
eqiv_informed(false),
input_informed(true),
use_eqiv(true),
drop_out(false),

cube_iter_map(new cube_iter[6]),

type(t)
{
	init_map();
}

layer::layer(ly_type t, cube* input , cube* eqiv) :
occupied_arr(new size_t[6]{ 1 , 1 ,0 , 0 , 0 ,0}),
allocated_arr(new size_t[6]{10 , 10 , 0, 0 , 0 , 0}),

_input_row(ROW(input)),
_input_col(COL(input)),
_input_depth(D(input)),

_eqiv_row(ROW(eqiv)),
_eqiv_col(COL(eqiv)),
_eqiv_depth(DEP(eqiv)),

_output_row(0),
_output_col(0),
_output_depth(0),
_stride_row(1),
_stride_col(1),

_padding_row(0),
_padding_col(0),

_forward_lay(0),
_backward_lay(0),

start_layer(false),
end_layer(false),
got_eqiv(false),
got_input(false),
havecube(new bool[3]{ false , false , false }),
eqiv_informed(false),
input_informed(true),
use_eqiv(true),

cube_iter_map(new cube_iter[6]),

type(t)
{
	init_map();
}

int layer::get_occupied_count(int pos)
{
	return occupied_arr[pos];
}

int layer::get_allocated_count(int pos)
{
	return allocated_arr[pos];
}


void layer::set_cube(int pos, cube* c)
{
	cube_iter ci = (cube_iter_map[pos]);
	int idx = occupied_arr[pos]++;

	ci[idx] = new cube(c);


	if (pos == 0)
		got_input = true;
	if (pos == 1)
		got_eqiv = true;
}

void layer::set_iter_size__initailize(cube* c, int arrp , int size)
{
	delete(cube_iter_map[arrp]);
	cube_iter_map[arrp] = new cube*[size + 10] {c};

	occupied_arr[arrp] = size;
	allocated_arr[arrp] = size + 10;

}

void layer::set_cube_iter(int pos, cube_iter c)
{
	cube_iter_map[pos] = c;
	havecube[pos] = true;
}

void layer::dettach_cube_iter(int pos, cube_iter c)
{
	cube_iter_map[pos] = 0;
	havecube[pos] = false;
	occupied_arr[pos] = 0;

}



void layer::initialize_memory_cube(int pos, int count, int depth, int row, int col)
{
	cube_iter ci = cube_iter_map[pos];

	ci = new cube*[count];

	for (int i = 0; i < count; i++)
	{
		ci[i] = new cube(depth , row , col);
	}

	cube_iter_map[pos] = ci;
	occupied_arr[pos] = count;

}

void layer::set_inform
(int depth, int input_count, int input_row, int input_col, 
	int filter_count, int filter_row, int filter_col,
	int stride_row, int stride_col, int rpadding , int cpadding)
{
	_input_depth = depth;

	_eqiv_row = filter_row;
	_eqiv_col = filter_col;

	_input_row = input_row;
	_input_col = input_col;

	_stride_row = stride_row;
	_stride_col = stride_col;

	allocated_arr[0] = input_count;
	allocated_arr[1] = filter_count;

	eqiv_informed = true;
	input_informed = true;

};


int layer::get_oc_count(int pos)
{
	return occupied_arr[pos];
	
}

void layer::set_inform(layer* l)
{
	_input_depth =l->get_cube_inform(0,0);

	_eqiv_row = l->get_cube_inform(1 , 1);
	_eqiv_col = l->get_cube_inform(1 , 2);

	_input_row = l->get_cube_inform(0 , 1);
	_input_col = l->get_cube_inform(0 , 2);

	_stride_row = l->get_cube_inform(3 , 1);
	_stride_col = l->get_cube_inform(3 , 2);

	allocated_arr[0] = l->get_oc_count(0);
	allocated_arr[1] = l->get_oc_count(1);

}

int layer::get_cube_inform(int pos, int row_col_dep)
{
	if (pos == 0)
	{
		if (row_col_dep == 0)
		{
			return _input_depth;
		}

		else if (row_col_dep == 1)
		{
			return _input_row;
		}

		else if (row_col_dep = 2)
		{
			return _input_col;
		}
	}

	else if (pos == 1)
	{
		if (row_col_dep == 0)
			return _eqiv_depth;

		else if (row_col_dep == 1)
			return _eqiv_row;
		else if (row_col_dep == 2)
			return _eqiv_col;
	}

	else if (pos == 2)
	{
		if (row_col_dep == 0)
			return _output_depth;
		else if (row_col_dep == 1)
			return _output_row;
		else if (row_col_dep == 2)
			return _output_col;
	}

	else if (pos == 3)
	{
		if (row_col_dep == 1)
			return _stride_row;
		else if (row_col_dep == 2)
			return _stride_col;
	}
}

void layer::copy_cube_iter(cube_iter* i, int pos ,int size)
{
	delete cube_iter_map[pos];
	cube_iter_map[pos] = new cube*[size];

	for (int i = 0 ; i < occupied_arr[pos]; i++)
	{
		delete cube_iter_map[pos][i];
	}

	for (int j = 0; j < size; j++)
	{
		cube_iter_map[pos][j] = new cube(*(*i)[j]);
	}

	occupied_arr[pos] = size;
	allocated_arr[pos] = size;
}

void layer::eqiv_map_init_with_random()
{
	mathm::set_dv(1000);

	for (int i = 0; i < allocated_arr[1]; i++)
		init_random(cube_iter_map[1][i]);

	occupied_arr[1] = allocated_arr[1];
}


void layer::eqiv_map_init_with_random(int count, int dep, int row, int col , int dv)
{
	mathm::set_dv(dv);
	
	for (int i = 0; i < count; i++)
			init_random(cube_iter_map[1][i]);

	occupied_arr[1] = count;
	_eqiv_depth = dep;
	_eqiv_row = row;
	_eqiv_col = col;
}


void layer::set_forward_lay(layer* l)
{
	_forward_lay = l;
}

void layer::inform_passing(layer_inform& inf)
{
	inf.t = type;
	inf.eqiv_depth = _eqiv_depth;
	inf.eqiv_col = _eqiv_col;
	inf.eqiv_row = _eqiv_row;
	inf.eqiv_count = occupied_arr[1];

	inf.input_depth = _input_depth;
	inf.input_col = _input_col;
	inf.input_row = _input_row;
	inf.input_count = occupied_arr[0];

	inf.output_col = _output_col;
	inf.output_row = _output_row;
	inf.output_depth = _output_depth;
	inf.output_count = occupied_arr[2];

	inf.stride_col = _stride_col;
	inf.stride_row = _stride_row;
}


void layer::set_tag_inform(std::string s)
{
	std::string nstr = s;
	std::string c = " Cube";

	std::string out;

	if(start_layer)
	for (int i = 0; i < occupied_arr[0]; i++)
	{
		out = nstr + " input " + std::to_string(i) + c;
		cube_iter_map[0][i]->set_tag(out);
	}

	if(use_eqiv)
	for (int i = 0; i < occupied_arr[1]; i++)
	{
		out = nstr + " weight " + std::to_string(i) + c;
		cube_iter_map[1][i]->set_tag(out);
	}

	for (int i = 0; i < occupied_arr[2]; i++)
	{
 		out = nstr + " output " + std::to_string(i) + c;
		cube_iter_map[2][i]->set_tag(out);
	}
	/*
	for (int i = 0; i < occupied_arr[3]; i++)
	{
		out = nstr + " weight inverse " + std::to_string(i) + c;
		cube_iter_map[2][i]->set_tag(out);
	}
	for (int i = 0; i < occupied_arr[4]; i++)
	{
		out = nstr + " input inverse " + std::to_string(i) + c;
		cube_iter_map[2][i]->set_tag(out);
	}
	for (int i = 0; i < occupied_arr[5]; i++)
	{
		out = nstr + " bias " + std::to_string(i) + c;
		cube_iter_map[2][i]->set_tag(out);
	}

	*/
}


void mdls::inc_layer_type_count(int* i, std::string s)
{
	s += std::to_string(*i);
	i++;
}
void layer::set_input_inform(int count ,int depth ,int row ,int col)
{
	if (count != NOT_MODIFIED)
		allocated_arr[0] = count;

	if (depth != NOT_MODIFIED)
		_input_depth = depth;

	if (row!= NOT_MODIFIED)
		_input_row = row;
	
	if (col != NOT_MODIFIED)
		_input_col = col;

	input_informed = true;
}

void layer::set_weight_inform(int count, int depth, int row, int col, int rstride , int cstride, int rpadding , int cpadding)
{
	if(count != NOT_MODIFIED)
		allocated_arr[1] = count;
	
	if (depth != NOT_MODIFIED)
		_eqiv_depth = depth;

	if (row != NOT_MODIFIED)
		_eqiv_row = row;

	if (col != NOT_MODIFIED)
		_eqiv_col = col;

	if (rstride != NOT_MODIFIED)
		_stride_row = rstride;
		
	if (cstride != NOT_MODIFIED)
		_stride_col = cstride;

	if (rpadding != NOT_MODIFIED)
		_padding_row = rpadding;

	if (cpadding != NOT_MODIFIED)
		_padding_col = cpadding;

	eqiv_informed = true;

}

void layer::set_output_inform_from_val() 
{
	if (!start_layer)
	{
		allocated_arr[0] = _forward_lay->get_oc_count(2);
		occupied_arr[0] = _forward_lay->get_oc_count(2);

		_input_depth = _forward_lay->get_cube_inform(2, 0);
		_input_row = _forward_lay->get_cube_inform(2, 1);
		_input_col = _forward_lay->get_cube_inform(2, 2);
		
		cube_iter_map[0] = _forward_lay->get_output_cube();

	}
	

	allocated_arr[2] = allocated_arr[0];
	allocated_arr[3] = allocated_arr[0];
	allocated_arr[4] = allocated_arr[1];

	allocated_arr[5] = allocated_arr[2];

	_output_depth = _input_depth;
	_output_row = _input_row;
	_output_col = _input_col;

};


void layer::get_input_from_forward()
{
	layer* l = _forward_lay;

	occupied_arr[0] = l->get_oc_count(2);

	_input_depth = l->get_cube_inform(2, 0);
	_input_row = l->get_cube_inform(2, 1);
	_input_col = l->get_cube_inform(2, 2);

	cube_iter_map[0] = l->get_output_cube();

	_output_depth = _input_depth;
	_output_row = _input_row;
	_output_col = _input_col;



}

void layer::preprocessing()
{
	set_output_inform_from_val();

	if ( !havecube[1] && use_eqiv )
	initialize_memory_cube(1, allocated_arr[1], _eqiv_depth, _eqiv_row, _eqiv_col);

	initialize_memory_cube(2, allocated_arr[2], _output_depth, _output_row, _output_col);

	initialize_memory_cube(3, allocated_arr[3], _input_depth, _input_row, _input_col);

	if (use_eqiv)
	{
		initialize_memory_cube(4, allocated_arr[4], _eqiv_depth, _eqiv_row, _eqiv_col);
		initialize_memory_cube(5, allocated_arr[5], _output_depth, _output_row, _output_col);
	
		for (int i = 0; i < allocated_arr[5]; i++)
			init_random(cube_iter_map[5][i]);

		cube_iter_to_initailize(cube_iter_map[4], occupied_arr[4], 1);
	}
	
	drop_out_index = new int[_eqiv_row * _eqiv_col * _eqiv_depth]{ 0, };

	set_tag_inform(type_name + std::to_string(index_eqaul_layers));
}

void layer::preprocessing(layer* flayer)
{
	set_forward_lay(flayer);
	set_output_inform_from_val();

	if ( !havecube[1] && use_eqiv )
	initialize_memory_cube(1, allocated_arr[1], _eqiv_depth, _eqiv_row, _eqiv_col);

	initialize_memory_cube(2, allocated_arr[2], _output_depth, _output_row, _output_col);
	initialize_memory_cube(3, allocated_arr[3], _input_depth, _input_row, _input_col);

	if (use_eqiv)
	{
		initialize_memory_cube(4, allocated_arr[4], _eqiv_depth, _eqiv_row, _eqiv_col);
		initialize_memory_cube(5, allocated_arr[5], _output_depth, _output_row, _output_col);

		for (int i = 0; i < allocated_arr[5]; i++)
			init_random(cube_iter_map[5][i]);
		
		cube_iter_to_initailize(cube_iter_map[4], occupied_arr[4], 1);


	}

	drop_out_index = new int[_eqiv_row * _eqiv_col * _eqiv_depth ]{ 0, };
	set_tag_inform(type_name + std::to_string(index_eqaul_layers));
}

void layer::init_map()
{
	for (int i = 0; i < 6; i++)
	{
		int j = allocated_arr[i];
		cube_iter_map[i] = new cube*[j];
	}

}

void layer::set_cube_iter(int pos, cube_iter i, int size)
{
	set_cube_iter(pos, i);
	occupied_arr[pos] = size;
	allocated_arr[pos] = size;

	havecube[pos] = true;
}

void layer::add_bias()
{
	cube_iter_loop(cube_iter_map[2], cube_iter_map[5], cadd ,occupied_arr[2]);
}

void layer::process_drop_out(int weight_number)
{
	if (!drop_out)
		return;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0 , (_eqiv_row * _eqiv_col * _eqiv_depth));

	generator.seed(distribution(generator));

	drop_out_count = (_eqiv_row * _eqiv_col * _eqiv_depth) * drop_out_rate; 
	int count = (_eqiv_row * _eqiv_col * _eqiv_depth);
	
	for (int i = 0; i < drop_out_count; i++)
	{
		drop_out_index[i] = distribution(generator);
		val_drop_outed[i] = *cube_iter_map[1][weight_number]->get(drop_out_index[i]);
		
		*cube_iter_map[1][weight_number]->get(drop_out_index[i]) = 0;
	
	}

}


void layer::drop_out_close(int weight_number)
{
	for (int i = 0; i < drop_out_count ; i++)
		cube_iter_map[1][weight_number]->set(drop_out_index[i], val_drop_outed[i]);
			
}

void layer::proceed_inverse()
{
	cube_iter outdif = _backward_lay->get_cube_iter(3);
	cube_iter_copy(cube_iter_map[3], outdif, allocated_arr[2] );	
}


