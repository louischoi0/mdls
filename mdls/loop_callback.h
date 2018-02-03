#pragma once
#include <iostream>
#include <random>
#include "macro.h"


namespace mdls
{

	namespace callback
	{
		static int dv = 1;

		static void set_dv(int d) { dv = d; };
		typedef void(*CONV_CALLBACK) (elemt* img, elemt* filter, elemt* copi);

		void cb_add(elemt* p, elemt* pp, elemt* out);

		void callback_for_sum_3(elemt* p, elemt* pp, elemt* copi);
		void callback_for_sum(elemt *, elemt*);
		void callback_for_devide(elemt* d, elemt* dd, elemt* copi);
		void callback_for_dot(elemt*, elemt*, elemt*);
		void callback_for_set_value_to(elemt * p, elemt* pp);
		void callback_for_multiply(elemt *p, elemt *pp, elemt* copi);
		elemt insert_endl(elemt*);
		void print_element(elemt* p);
		void print_element_to_int(elemt* p);
		void callback_for_multiply_with_value(elemt*, elemt*);
		void callback_for_multiply(elemt*, elemt*);

		void callback_for_devide(elemt* popt, elemt* copi);
		void callback_for_devide_ignore_zero(elemt* d, elemt* copi);

		void callback_for_multiply_ignore_zero(elemt*, elemt*, elemt*);
		void callback_for_minus_3(elemt*, elemt*, elemt*);
		void callback_for_minus_2(elemt*, elemt*);
		void negative_to_zero_value_to_one(elemt* p, elemt* copi);
		void callback_for_relu(elemt* p, elemt* out);
		void callback_for_log(elemt* p, elemt * copi);
		void callback_for_convolution_multiply(elemt* p, elemt* pp, elemt* copi);
		void callback_for_reciprocal(elemt* p, elemt * copi);
		void max_conv(elemt* p, elemt* copi, int wind_row, int wind_col);

		void cb_max(elemt* p, elemt* copi);
		void max_conv_params(elemt* m, elemt* copi, int col_off, int wind_row, int wind_col);
		void max_conv_params_with_inf(elemt* p, elemt* copi, int m_col_off, int wind_row, int wind_col);

		void cb_cdot(elemt* p, elemt* pp, elemt* out, int c_off_pp, int row_p, int row_pp);
		void cb_multiply(elemt* p, elemt* pp, elemt* out, int not_use_0, int not_use_1, int not_use_2);

		void block_copy(elemt* p, elemt* copi, int col_offset_p, int count_row_to_copy, int count_col_to_copy);
		void channel_copy(elemt* m, elemt* copi, int col_offset_p, int dep_offset_p, int count_row_to_copy, int count_col_to_copy, int depth);

		void cb_init_random(elemt* p, elemt* not_use);

		void callback_for_exp(elemt* popt, elemt* copi);
		void callback_for_exp(elemt* p);

	}
};

