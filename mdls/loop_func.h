#pragma once
#include "matrix.h"

using namespace mathm;

namespace loop
{
	void loop_matrix(matrix* m, elemt v, CALLBACK_SINGLE_LOOP_P_2 CB);

	void loop_matrix_for_dot(const matrix*, const matrix*, matrix&, CALLBACK_WITH_TWO_ELEM_P);

	void loop_two_matrix(matrix * m1, matrix* m2, elemt *copi, CALLBACK_WITH_TWO_ELEM_P CB);
	void loop_two_matrix(matrix* m1, matrix* m2, matrix& out, CALLBACK_WITH_TWO_ELEM_P CB);

	void loop_row(matrix* m1, int col, CALLBACK_SINGLE_LOOP_P_2, elemt* copi);

	void loop_matrix(matrix* m, elemt* value, CALLBACK_SINGLE_LOOP_P_2);
	void loop_matrix(matrix* m, CALLBACK_SINGLE_LOOP_P);
	void loop_matrix_with_p(elemt* p, int x, int y, CALLBACK_SINGLE_LOOP_P);

	void loop_matrix_with_row_end_call_const(const matrix* m, CALLBACK_SINGLE_LOOP_P, CALLBACK_AFTER_ROW_END);
	void loop_base_for_dot(const matrix* m, const matrix*, elemt* copi, CALLBACK_WITH_TWO_ELEM_P);
	void loop_for_transpose(matrix* m1, matrix* m2, CALLBACK_SINGLE_LOOP_P_2 CB);

	void loop_for_transpose(elemt* m1, elemt* m2, int dep , int row , int col , CALLBACK_SINGLE_LOOP_P_2 CB);

	void loop_base_row_twice(matrix* m, matrix* m2, matrix& out, CALLBACK_WITH_TWO_ELEM_P, CALLBACK_WITH_TWO_ELEM_P);

	void loop_matrix_with_row_end_call_2(matrix* m, matrix& out, CALLBACK_SINGLE_LOOP_P_2, CALLBACK_AFTER_ROW_END_2);
	void loop_matrix_with_row_end_call(matrix* m, CALLBACK_ROW_LOOP_P, CALLBACK_AFTER_ROW_END_2);

	elemt* loop_matrix_with_add(const matrix*, const matrix*, CALLBACK_WITH_TWO_ELEM_P);
	elemt* loop_matrix_with_multiple(const matrix*, const matrix*, CALLBACK_WITH_TWO_ELEM_P);

	void loop_two_matrix_extends
	(matrix* m1, matrix* copi,
		int start_1, int row_off_1, int col_off_1, int win_row, int win_col, CONV_CALL CB, int copi_inc_position);

	void loop_two_matrix_extends
	(matrix* m1, matrix* m2, elemt* copi,
		int start_1, int start_2, int row_off_1, int row_off_2, int col_off_1, int col_off_2, CALLBACK_WITH_TWO_ELEM_P CB);

}