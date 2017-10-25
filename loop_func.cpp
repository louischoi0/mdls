#include "loop_func.h"

void loop::loop_two_matrix_extends
(matrix* m1, matrix* m2, elemt* copi,
	int start_1, int start_2, int row_off_1, int row_off_2, int col_off_1, int col_off_2, CALLBACK_WITH_TWO_ELEM_P CB)
{
	elemt* cp = copi;

	elemt* p1 = m1->get_p() + start_1;
	elemt* p2 = m2->get_p() + start_2;

	elemt* pp1 = p1;
	elemt* pp2 = p2;

	for (int i = 0; i < m1->get_col_count(); i++)
	{
		pp1 = p1;
		pp2 = p2;

		for (int j = 0; j < m1->get_row_count(); j++)
		{

			CB(pp1, pp2, cp++);

			pp1 += row_off_1;
			pp2 += row_off_2;
		}

		p1 += col_off_1;
		p2 += col_off_2;

	}

}
void loop::loop_base_for_dot(const matrix* m1, const matrix_db* m2, elemt* copi, CALLBACK_WITH_TWO_ELEM_P CB)
{
	if (m1->get_row_count() != m2->get_col_count())
	{
		std::cout << " Matrix Format Invalid \n";
		return;
	}

	elemt* pm1 = m1->get_p();
	elemt* pm2 = m2->get_p();

	elemt* pout = copi;
	elemt* pcc = pout;

	for (int k = 0; k < m1->get_col_count(); k++)
	{
		elemt* ppm1 = pm1;
		elemt* ppm2 = pm2;

		for (int i = 0; i < m2->get_row_count(); i++)
		{
			elemt* pppm1 = ppm1;
			elemt* pppm2 = ppm2;

			*pcc = 0;

			for (int j = 0; j < m1->get_row_count(); j++)
			{
				CB(pppm1, pppm2, pcc);

				pppm1++;
				pppm2 += m2->get_row_count();
			}

			pcc++;
			ppm2++;
		}

		pm1 += m1->get_row_count();
	}
}

void loop::loop_matrix_for_dot(const matrix_db* m1, const matrix_db* m2, matrix_db& out, CALLBACK_WITH_TWO_ELEM_P CB)
{
	loop_base_for_dot(m1, m2, out.get_p(), CB);
}

void loop::loop_matrix_with_row_end_call(matrix* m, CALLBACK_ROW_LOOP_P CRL, CALLBACK_AFTER_ROW_END_2 CEC)
{
	int _col = m->get_col_count();
	int _row = m->get_row_count();

	elemt* p = m->get_p();
	elemt* po = p;

	elemt* s = new elemt(0);

	for (int i = 0; i < _col; i++, ++po)
	{
		*s = 0;

		for (int j = 0; j < _row; j++, ++p)
		{
			CRL(p, s);
		}

		CEC(s, po);
	}
}

void loop::loop_matrix_with_row_end_call_2(matrix* m, matrix& out, CALLBACK_SINGLE_LOOP_P_2 CB, CALLBACK_AFTER_ROW_END_2 CR)
{
	int _col = m->get_col_count();
	int _row = m->get_row_count();

	elemt* p = m->get_p();
	elemt* po = out.get_p();

	for (int i = 0; i < _col; i++)
	{
		elemt* s = new elemt(0);

		for (int j = 0; j < _row; j++)
		{
			CB(p++, s);
		}

		CR(s, po++);
	}

}

void loop::loop_matrix_with_row_end_call_const(const matrix_db* m, CALLBACK_SINGLE_LOOP_P CB, CALLBACK_AFTER_ROW_END CR)
{
	int _col = m->get_col_count();
	int _row = m->get_row_count();

	elemt* p = m->get_p();

	for (int i = 0; i < _col; i++)
	{
		for (int j = 0; j < _row; j++, p++)
		{
			CB(p);
		}

		CR(p);
	}
}

void loop::loop_matrix_with_p(elemt* p, int  row, int col, CALLBACK_SINGLE_LOOP_P CB)
{
	elemt* pp = p;

	for (int i = 0; i < col; i++)
		for (int j = 0; j < row; j++)
			CB(pp++);
}

void loop::loop_matrix(matrix* m, CALLBACK_SINGLE_LOOP_P CB)
{
	loop_matrix_with_p(m->get_p(), m->get_row_count(), m->get_col_count(), CB);
}

void loop::loop_row(matrix* mtx, int col, CALLBACK_SINGLE_LOOP_P_2 CR, elemt* copi)
{
	int _row_count = mtx->get_row_count();
	elemt* p = mtx->get_p() + col* _row_count;

	elemt* s = new elemt(0);

	for (int i = 0; i < _row_count; i++)
		CR(p++, s);

	*copi = *s;
}


void loop::loop_two_matrix(matrix * m1, matrix* m2, elemt *copi, CALLBACK_WITH_TWO_ELEM_P CB)
{
	if (!IF_TWO_MATRIX_EQAL_FORM(m1, m2))
	{
		cout << "not eqal form with two matrix\n";
		return;
	}

	int _col = m1->get_col_count();
	int _row = m1->get_row_count();

	elemt* p1 = m1->get_p();
	elemt* p2 = m2->get_p();

	elemt* pout = copi;

	for (int i = 0; i < _col; i++)
		for (int j = 0; j < _row; j++)
		{
			CB(p1++, p2++, pout++);
		}

}

void loop::loop_two_matrix(matrix* m1, matrix* m2, matrix& out, CALLBACK_WITH_TWO_ELEM_P CB)
{
	loop_two_matrix(m1, m2, out.get_p(), CB);
}

void loop::loop_matrix(matrix* m, elemt* v, CALLBACK_SINGLE_LOOP_P_2 CB)
{
	elemt* p = m->get_p();

	for (int i = 0; i < COL(m); i++)
		for (int j = 0; j < ROW(m); j++)
		{
			CB(p++, v++);
		}

}

void loop::loop_for_transpose(matrix* m1, matrix* m2, CALLBACK_SINGLE_LOOP_P_2 CB)
{
	elemt* p = m1->get_p();
	elemt* tp = m2->get_p();

	for (int i = 0; i < m1->get_col_count(); i++)
		for (int j = 0; j < m1->get_row_count(); j++)
		{
			elemt* tpp = tp + m1->get_transpose_index(j, i);
			CB(p++, tpp);
		}
}


void loop::loop_for_transpose(elemt* p, elemt* tp, int dep , int row , int col , CALLBACK_SINGLE_LOOP_P_2 CB)
{
	elemt* tpp = 0;
	elemt* cp = p;

	int d_off = row * col;

	for (int k = 0; k < dep; k++)
	{
		
		for (int i = 0; i < col; i++)
		{
			for (int j = 0; j < row; j++)
			{
				tpp = tp + (j * col) + i;
				CB(p++, tpp);
			}
	
		}
		
		tp += d_off;
	
	}
	
}

void loop::loop_matrix(matrix* m, elemt v, CALLBACK_SINGLE_LOOP_P_2 CB)
{
	elemt* p = m->get_p();
	for (int i = 0; i < m->get_col_count(); i++)

		for (int j = 0; j < m->get_row_count(); j++)
		{

			CB(p++, &v);
		}
}

