#pragma once
#include "cube.h"
#include "matrix_extended.h"
#include "loop_func.h"

namespace mathm
{
	typedef void(*CALLBACK_CUBE_LOOP) (cube*, cube&, int range);
	typedef void(*CALLBACK_LOCAL_LOOP) (elemt* p, elemt* pp, int size);

	typedef cube** cube_iter;
	typedef LOCAL_FUNC_FOR_ELEM_LOOP_DOT LOCAL_FUNC_FOR_ELEM_OUT;
	typedef void(*CUBE_FUNC_2) (cube* c, cube& out);
	typedef void(*CUBE_FUNC_3) (cube* c, cube* cc, cube& out);
	typedef void(*CUBE_FUNC_1) (cube* c);

	void cube_iter_loop(cube_iter c, cube_iter out, CALLBACK_CUBE_LOOP CC, int range);
	void cube_iter_loop(cube_iter in, cube_iter out, CUBE_FUNC_2, int size);
	void cube_iter_loop(cube_iter c, cube_iter cc, cube_iter out, CUBE_FUNC_3 , int size);
	void cube_iter_loop(cube_iter t, CUBE_FUNC_1 , int size);

	void cube_loop_channel_first_with_stride
	(cube* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_CHN_LOOP CC);
	void cube_loop_with_stride
	(cube* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_ELEM_LOOP C);
	void cube_loop_with_stride
	(cube* p, cube* pp, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_ELEM_OUT CC);

	void cube_loop_with_stride_with_arr
	(cube* p, int** idx , elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_ELEM_LOOP C);
	
	void cube_loop(cube* p, cube* pp, elemt* copi, CALLBACK_WITH_TWO_ELEM_P CC);
	void cube_loop(cube*p, elemt* copi, CALLBACK_SINGLE_LOOP_P_2 CC);

	void cube_loop_for_dot(cube* p, cube* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT C);
	void cube_loop_for_dot(cube* p, matrix* m, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC);

	void tensor_loop_channel_first_with_stride
	(tensor* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_CHN_LOOP CC);

}
