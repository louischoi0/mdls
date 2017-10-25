#pragma once
#include "element.h"
using namespace mdls;


namespace macro
{
	typedef void(*ONE_ELEM)(elemt* p);

	int get_dv_size(int g_size, int w_size, int stride);
	int get_g_size(int dv_size, int w_size, int stride);

	void arr_opt_multiply(int* p, int** out, int m, int size);
	void arr_opt_add(int* p, int* pp , int** out, int size);

	void max_in_range_other_to_zero(elemt* p, elemt* pp, int range);
	elemt get_max_in_range(elemt* p, elemt* pp, int range);

	void minus_in_range(elemt* p, elemt* pp, elemt* out, int range);
	void minus_in_range(elemt* p, elemt v, elemt* out, int range);

	void add_in_range(elemt* p, elemt pp, int range);

	elemt get_sum_in_range(elemt* p, int range);

	void loop_elem(elemt* p, ONE_ELEM CC, int range);

	void multiply_in_range(elemt* p, elemt v, int range);

	void devide_in_range(elemt* p, elemt v, int range);
	
	elemt get_max_in_range(elemt* p, int size);

	void fill_args_pointer(elemt**& arg ,int i ,  elemt* p);

	void convert_index_by_rotate(int, int&, int&, const shape& );

}

