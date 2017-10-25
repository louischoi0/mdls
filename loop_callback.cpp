#include "loop_callback.h"
#include <cmath>

using namespace std;
void callback::callback_for_devide_ignore_zero(elemt* popt, elemt* copi)
{
	if (*popt == 0)
		return;

	*copi /= *popt;
}


void callback::callback_for_devide(elemt* d, elemt* dd, elemt* copi)
{
	*copi = *d / *dd;
}

void callback::callback_for_minus_3(elemt* p, elemt* pp, elemt* out)
{
	*out = *p - *pp;
}

void callback::callback_for_minus_2(elemt* popt, elemt* popted)
{
	*popted -= *popt;
}
void callback::callback_for_devide(elemt* popt, elemt* copi)
{
	*copi /= *popt;
}
void callback::callback_for_multiply_ignore_zero(elemt* p1, elemt* p2, elemt* copi)
{
	if (*p1 == 0)
	{
		*copi = *p2;

		return;
	}

	else if (*p2 == 0)
	{
		*copi = *p1;
		return;
	}

	*copi = *p1 * *p2;

}

void callback::callback_for_multiply_with_value(elemt* copi, elemt* popt)

{

	*copi *= *popt;
}

void callback::callback_for_multiply(elemt* popt, elemt* copi)
{
	*copi *= *popt;
}


void callback::negative_to_zero_value_to_one(elemt* p, elemt* copi)
{
	if (*p != 0)
		*copi = 1;
	else
		*copi = 0;
}

void callback::callback_for_relu(elemt* popt, elemt* copi)
{
	if (*popt > 0)
		*copi = *popt;
	else
		*copi = 0;
}

void callback::callback_for_exp(elemt* p)
{
	*p = exp(*p);
}

void callback::callback_for_exp(elemt* popt, elemt* copi)
{
	*copi = exp(*popt);
}


void callback::callback_for_reciprocal(elemt* p, elemt * copi)
{
	*copi = (elemt)(1 / *p);
}

void callback::callback_for_convolution_multiply(elemt* p, elemt* pp, elemt* copi)
{
	*copi += *p * *pp;
}

void callback::callback_for_dot(elemt* p1, elemt* p2, elemt* out)
{
	*out += (*p1) * (*p2);
}

void callback::callback_for_log(elemt* p, elemt * copi)
{
	*copi = exp(*p);
}

void callback::max_conv(elemt* p, elemt* copi, int wind_row, int wind_col)
{
	elemt* pcop = copi;

	for (int i = 0; i < wind_col; i++)
	{
		for (int j = 0; j < wind_row; j++)
		{
			if (j == 0 && i == 0)
			{
				*pcop = *p;
				p++;
				continue;
			}

			if (*pcop < *p)
				*pcop = *p;

			p++;
		}

	}


}

void callback::max_conv_params_with_inf(elemt* p, elemt* copi, int m_col_off , int wind_row, int wind_col)
{
	elemt* pp = p;
	elemt* ppp = p;
	elemt* pcop = copi;

	detail::func_informer* inst = detail::func_informer::get_instance();

	int count = inst->get_pcount();
	int** val_r = inst->get_value_arr(0);
	int** val_c = inst->get_value_arr(1);

	int wc = 0;
	int wr = 0;

	for (int i = 0; i < wind_col; i++)
	{
		ppp = pp;

		for (int j = 0; j < wind_row; j++)
		{
			if (j == 0 && i == 0)
			{
				*pcop = *ppp;
				ppp++;
				continue;
			}

			if (*pcop < *ppp)
			{
				*pcop = *ppp;
				wr = j;
				wc = i;
			}

			ppp++;
		}

		pp += m_col_off;
	}

	(*val_r)[count] = wr;
	(*val_c)[count] = wc;

}

void callback::max_conv_params(elemt* p, elemt* copi, int m_col_off, int wind_row, int wind_col)
{
	elemt* pp = p;
	elemt* ppp = p;
	elemt* pcop = copi;

	for (int i = 0; i < wind_col; i++)
	{
		ppp = pp;

		for (int j = 0; j < wind_row; j++)
		{
			if (j == 0 && i == 0)
			{
				*pcop = *ppp;
				ppp++;
				continue;
			}

			if (*pcop < *ppp)
			{
				*pcop = *ppp;
			}

			ppp++;
		}

		pp += m_col_off;
	}


}

void  callback::cb_cdot(elemt* p, elemt* pp, elemt* out, int c_off_pp, int row_p, int n_use)
{
	*out = 0;

	for (int i = 0; i < row_p; i++)
	{
		*out += *p * *pp;
		p++;
		pp += c_off_pp;
	}


}

void callback::cb_multiply(elemt* p, elemt* pp, elemt* out, int not_usee, int not_use, int not_use_1)
{
	*out = *p * *pp;
}

void callback::block_copy(elemt* p, elemt* copi, int col_offset_p, int count_row_to_copy, int count_col_to_copy)
{
	int copy_size = count_col_to_copy * sizeof(elemt);

	elemt* cop = copi;

	for (int i = 0; i < count_row_to_copy; i++)
	{
		memcpy(cop, p, copy_size);

		p += col_offset_p;
		cop += count_col_to_copy;
	}
}

void callback::channel_copy(elemt* mp, elemt* copi, int col_offset, int depth_offset, int count_row_to_copy, int count_col_to_copy, int depth)
{
	for (int i = 0; i < depth; i++)
	{
		block_copy(mp, copi, col_offset, count_row_to_copy, count_col_to_copy);
		copi += count_row_to_copy * count_col_to_copy;
		mp += depth_offset;
	}
}


void callback::cb_init_random(elemt* p , elemt* n )
{
}


void callback::cb_max(elemt* p, elemt* copi)
{
	if ( *p > *copi )
	{
		*copi = *p;
	}
}

void callback::cb_add(elemt* p, elemt* pp , elemt* out)
{
	*out = *p + *pp;
}


void callback::callback_for_multiply(elemt *p, elemt *pp, elemt* copi)
{
	*copi = *pp * *p;
}


void callback::callback_for_set_value_to(elemt * popt, elemt* copi)
{
	*copi = *popt;
}



elemt callback::insert_endl(elemt*)
{
	cout << endl;
	return 0;
}

void callback::print_element(elemt* p)
{
	cout << *p;
	cout << " ";
}

