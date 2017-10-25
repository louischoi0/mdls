#pragma once
#include "matrix.h"
#include "cube.h"
#include "loop_func.h"
#include "loop_callback.h"
using namespace mdls;
using namespace callback;

namespace mathm
{
	typedef void(*CONV_CUBE_LOOP_2)
		(elemt* p, elemt* pp, elemt* copi,
			int win_row, int win_col, int col_off_p, int col_off_pp,
			int inc_copi_p
			);

	void reciprocal(matrix* m, matrix& out);
	void print_matrix(const matrix_db* m);

	void log_matrix(matrix* m);
	void log_matrix(matrix* m, matrix& out);

	void convolution_matrix_loop(matrix* main, matrix* filter, matrix& copi, CONV_CALLBACK CONVCALL, int stride);

	void convolution_cube_loop(cube*, cube*, cube*, int strd, CONV_CALLBACK);
	void convolution_cube_loop(cube*, cube*, cube&, int stride_row, int stride_col, CONV_CUBE_LOOP_2, int inc_copi_position);

	void print_matrix_to_int(const matrix_db* m);

	void add(matrix* m1, matrix* m2, matrix& out);

	void dot(matrix* m1, matrix* m2, matrix& out);
	elemt* dot(matrix*, matrix*);

	void multiply(matrix* m1, matrix* m2, matrix& out);
	void multiply_ign_zero(matrix*, matrix*, matrix&);
	void compact_matrix_by_col(matrix*);
	void compact_matrix_by_col(matrix*, matrix&);

	void init_with_randomized(matrix* m);
	void init_with_randomized(matrix* m, int row, int col);

	void compact_col_by_number(matrix& m, int num);

	void max(matrix* m);

	typedef void(*LOCAL_FUNC_FOR_ELEM_LOOP)(elemt*, elemt*, int c_off_of_p, int stride_row, int stride_col);
	typedef void(*LOCAL_FUNC_FOR_CHN_LOOP)(elemt*, elemt*, int c_off_of_p, int d_off_of_p, int stride_row, int stride_col, int depth);

	typedef void(*LOCAL_FUNC_FOR_ELEM_LOOP_DOT)	(elemt* ooop, elemt* cccp, elemt* copi, int c_off_pp, int row_p, int row_pp);


}