#pragma once
#include "cube_loop.h"
#include <string>

#define NOT_USE -1

namespace mathm
{
	typedef void(*BASE_CALL)
		(elemt* p, elemt* pp, elemt* out,
			int l_dep_count, int l_row_count, int l_col_count,
			int d_off_f, int c_off_f, int d_off_s , int c_off_s	);
		
	void cadd(cube* c, cube& cc);

	void cdot(cube*c, matrix* m, matrix& out);
	void cdot(cube* c, cube* cc, cube& out);
	void cdot(cube* c, matrix* m, cube& out);

	void cmultiply(cube* c, cube* cc, cube& out);
	void crelu(cube* c, cube& out);

	void softmax_c(cube* in, cube& out);

	void pooling(cube*p, cube& out, int** ra, int stride_row, int stride_col, int fil_size_row, int fil_size_col);
	void pooling(cube* p, int** ra, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col);
	void pooling(cube* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col);
	void pooling_with_index_arr();

	void convert_imatrix_to_cube(matrix* m, elemt* res, int stride_row, int stride_col, int win_row, int win_col, int block_count, int block_depth);
	void cube_to_col(cube* c, elemt* p, int stide_row, int stride_col, int size_to_copy_row, int size_to_copy_col);
	void cubes_to_col(cube_iter i, int size , elemt** p, int stride_row, int stride_col, int size_to_copy_row, int size_to_copy_col);

	void print_cube(cube* c);

	void init_random(cube* c);

	void citer_passparam(cube_iter t, int size, int local1, int local2, int local3, int local4, int local5);

	void set_tag(cube_iter it, std::string s, int layer_count ,int size, int pos);
	std::string string_route(int i);
	
	void cube_iter_to_initailize(cube_iter t, int size, elemt value);
	void cube_iter_copy(cube_iter dest , cube_iter src , int size );

	void numerical_gradient(cube* in, cube& out, CUBE_FUNC_2 CC);
	void gradient_operation_by_row(cube* in, cube& out, CALLBACK_LOCAL_LOOP CC);
	void numerical_gradient_softmax(cube* in, cube& out, int range);
}