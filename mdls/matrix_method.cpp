#include "matrix_extended.h"
#include <random>
#include "loop_func.h"

using namespace mathm;
using namespace callback;
using namespace loop;

void mathm::dot(matrix* m1, matrix* m2, matrix& out)
{
	if (!SIZE_EQ((&out), m2->get_row_count(), m1->get_col_count()))
		out.resize(m2->get_row_count(), m1->get_col_count());

	loop_matrix_for_dot(m1, m2, out, callback_for_dot);
}

void mathm::print_matrix_with_p(const elemt* p_, int row_size_, int col_size_)
{
	const elemt* p = p_;

	for (int i = 0; i < col_size_; i++)
	{
		for (int j = 0; j < row_size_; j++, p++)
		{
			cout << *p;
			cout << " ";
		}

		cout << endl;
	}

}
void mathm::print_matrix(const matrix* m)
{

	loop_matrix_with_row_end_call_const(m, print_element, insert_endl);
}

void mathm::print_matrix_to_int(const matrix_db* m)
{
	loop_matrix_with_row_end_call_const(m, print_element_to_int, insert_endl);

}
void mathm::compact_matrix_by_col(matrix* base, matrix& out)
{
	if (base->get_row_count() == 1)
	{
		out = *base;
		return;
	}

	matrix copi = *base;
	compact_matrix_by_col(&copi);

	memcpy(&out, &copi, sizeof(matrix));
}

void mathm::compact_matrix_by_col(matrix* base)
{
	if (base->get_row_count() == 1)
		return;

	loop_matrix_with_row_end_call(base, add_p1_to_p2, callback_for_set_value_to);

	base->init_val(1, base->get_col_count());

	
}

void callback_for_add_one(elemt* p)
{
	(*p)++;
}

void mathm::add_one(matrix* m1)
{
	loop_matrix(m1, callback_for_add_one);
}

void mathm::pass_func(elemt* m)
{
}

void mathm::pass(matrix* m1)
{
	loop_matrix(m1, pass_func);
}

void mathm::add_p1_to_p2(elemt* popt, elemt* copi)
{
	*copi += *popt;
}

void callback::callback_for_sum(elemt* p, elemt* pp)
{
	*pp += *p;
}

void mathm::get_sum_of_row(matrix* mtx, int col, elemt* copi)
{
	loop_row(mtx, col, callback_for_sum, copi);
}

void get_sum_of_two_elements(elemt* p, elemt *pp, elemt* copi)
{
	*copi = *p + *pp;
}



void mathm::multiply(matrix* m1, matrix* m2, matrix& out)
{
	if (!IF_TWO_MATRIX_EQAL_FORM(m1, m2))
	{
		cout << " invalid two matrix format \n";
		return;
	}

	if (!IF_TWO_MATRIX_EQAL_FORM(m1, (&out)))
		out.resize(m2->get_row_count(), m1->get_col_count());

	loop_two_matrix(m1, m2, out, callback_for_multiply);
}

void mathm::set_value_to(matrix* m, elemt v)
{
	loop_matrix(m, &v, callback_for_set_value_to);
}

void callback::print_element_to_int(elemt* p)
{
	cout << (int)(*p);
}

void mathm::add(matrix* m1, matrix* m2, matrix& out)
{
	loop_two_matrix(m1, m2, out, callback_for_sum_3);
}

void callback::callback_for_sum_3(elemt* p, elemt* pp, elemt* copi)
{
	*copi = *p + *pp;
}

void mathm::transpose(matrix* m, matrix& out)
{
	out.resize(COL(m), ROW(m));

	loop_for_transpose(m, &out, callback_for_set_value_to);

};


void mathm::matrix::initialize()
{
	int k = 0;
	elemt* p = get_p();

	for (int i = 0; i < get_col_count(); i++)
		for (int j = 0; j < get_row_count(); j++)
			*p++ = k++;
}


void mathm::matrix::initialize_to(elemt v)
{
	elemt* p = get_p();

	for (int i = 0; i < get_col_count(); i++)
		for (int j = 0; j < get_row_count(); j++)
			*p++ = v;
}

matrix* mathm::make_matrix_by_malloc(int row, int col)
{
	matrix* m = (matrix*)malloc(sizeof(matrix));

	elemt* p = (elemt*)malloc(sizeof(elemt) * row * col);
	m->init_val(p, row, col);

	return m;
}


void mathm::devide(matrix* m1, matrix* m2, matrix& out)
{
	elemt* p1 = m1->get_p();
	elemt* p2 = m2->get_p();
	elemt* pout = out.get_p();

	loop_two_matrix(m1, m2, out, callback_for_devide);
}

void callback_for_sf(elemt* d)
{
	*d = exp(*d);
}

void exp_element(element& m)
{

}

void exp_matrix(matrix* m)
{
	loop_matrix(m, callback_for_sf);
}


void softmax( element* e, element& out )
{
}


void mathm::softmax(matrix* m, matrix& out)
{
	if (!IF_TWO_MATRIX_EQAL_FORM(m, (&out)))
	{
		cout << " Not invalid form - softmax \n";
		return;
	}
	elemt* p = out.get_p();

	matrix mm = *m;

	exp_matrix(&mm);

	matrix dividing = mm;

	compact_matrix_by_col(&dividing);

	cout << "dividing :" << endl;
	print_matrix(&dividing);
	cout << endl;

	loop_two_matrix_extends(&mm, &dividing, p, 0, 0, 1, 0, m->get_row_count(), 1, callback_for_devide);

}

void mathm::minus(matrix* m1, matrix* m2)
{
	loop_matrix(m2, m1->get_p(), callback_for_minus_2);
}

void mathm::devide(matrix* m1, matrix* m2)
{
	loop_matrix(m2, m1->get_p(), callback_for_devide);
}

void mathm::devide_ign_zero(matrix* m1, matrix* m2)
{
	loop_matrix(m2, m1->get_p(), callback_for_devide_ignore_zero);
}

void mathm::multiply_ign_zero(matrix* m1, matrix* m2, matrix& out)
{
	loop_two_matrix(m1, m2, out, callback_for_multiply_ignore_zero);
}

void mathm::minus(matrix* m1, matrix* m2, matrix& out)
{
	loop_two_matrix(m1, m2, out, callback_for_minus_3);
}

void mathm::multiply(matrix* m1, const elemt v)
{
	loop_matrix(m1, v, callback_for_multiply_with_value);
}

void mathm::relu(matrix* m , matrix& out )
{
	loop_matrix(m, out.get_p(), callback_for_relu);
}

void mathm::init_with_randomized(matrix* m, int row, int col)
{
	m->resize(row, col);
	init_with_randomized(m);
}

void mathm::init_with_randomized(matrix* m)
{
	elemt* p = m->get_p();

	std::default_random_engine egn;
	std::uniform_int_distribution<long> dst(1, 40);

	for (int j = 0; j < m->get_col_count(); j++)
	{
		for (int i = 0; i < m->get_row_count(); i++)
		{
			elemt d = dst(egn);

			*p++ = d / 100;

		}
	}
}

void mathm::reciprocal(matrix* m , matrix& out )
{
	if (!IF_TWO_MATRIX_EQAL_FORM(m, (&out)))
		out.resize(ROW(m), COL(m));

	loop_matrix( m , out.get_p()  , callback_for_reciprocal );
}

void mathm::log_matrix(matrix* m)
{
	loop_matrix(m, m->get_p(), callback_for_log);
}

void mathm::log_matrix(matrix* m , matrix& out )
{
	if (IF_TWO_MATRIX_EQAL_FORM(m, (&out)))
		out.resize(ROW(m), COL(m));

	loop_matrix(m, out.get_p(), callback_for_log);
}


void mathm::convolution_matrix_loop(matrix* main, matrix* filter, matrix& out, CONV_CALLBACK CONVCALL, int stride)
{
	int loop_1_size = main->get_col_count() - filter->get_col_count() + 1;
	int loop_2_size = main->get_row_count() - filter->get_row_count() + 1;

	int loop_3_size = filter->get_row_count();
	int loop_4_size = filter->get_col_count();

	int row_filter = filter->get_row_count();
	int row_main = main->get_row_count();

	elemt* p1_main = main->get_p();
	elemt* p1_filter = filter->get_p();

	elemt* p2_main = main->get_p();
	elemt* p2_filter = filter->get_p();

	elemt* p3_main = main->get_p();
	elemt* p3_filter = filter->get_p();

	elemt* p4_main = main->get_p();
	elemt* p4_filter = filter->get_p();

	elemt* pout = out.get_p();


	for (int l1 = 0; l1 < loop_1_size; l1++)
	{
		p2_main = p1_main;

		for (int l2 = 0; l2 < loop_2_size; l2++)
		{
			p3_main = p2_main;
			p2_filter = p1_filter;

			for (int l3 = 0; l3 < loop_3_size; l3++)
			{
				p4_main = p3_main;

				for (int l4 = 0; l4 < loop_4_size; l4++)
				{
					CONVCALL(p4_main++, p2_filter++, pout);
				}

				p3_main += row_main;
			}

			pout++;
			p2_main += 1;
		}

		p1_main += row_main;

	}
}



void mathm::max(matrix* m)
{
	elemt* p = D_MALC(COL(m));
	elemt* pp = p;

	for (int i = 0; i < COL(m); i++)
	{
		*pp = 0;
		
		loop_row( m , i , cb_max, pp);
	
		pp++;
	}
	
	m->init_val( p , 1 , m->get_col_count());

}


void mathm::compact_col_by_number(matrix& p, int num)
{
	if (num > RCOL(p))
		return;

	elemt* pp = p.get_p();
	elemt* ppp = pp;

	for (int k = 0; k < RCOL(p) / num; k++)
	{
		pp = ppp;

		for (int i = 0; i < RROW(p); i++)
		{
			for (int j = 0; j < num - 1; j++)
			{
				*pp += *(pp + ( RROW(p) * j));
			}

			pp++;
		}


		ppp += num * RROW(p);

	}

	pp = p.get_p() + RCOL(p);
	ppp = p.get_p() + ( num * RCOL(p) );

	for (int k = 0; k < ( RCOL(p) / num) - 1; k++)
	{
		for (int i = 0; i < RROW(p); i++)
		{
			*(pp + i) = *(ppp + i);
		}

		pp += RCOL(p);
		ppp += RCOL(p) * num;
	}

	p.init_val( RROW(p) , RCOL(p) / num );

}
