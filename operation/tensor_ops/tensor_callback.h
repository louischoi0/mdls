#pragma once

#include "..\..\objects\layert.h"
#include "..\..\modules\counter_extens.h"
#include "..\macro\macro.h"

namespace mdls
{
	namespace callback
	{
		typedef void(*CALLBACK_SINGLE_LOOP_P_1) (elemt*);

		void channel_copy(elemt* mp, elemt* copi, int col_offset, int depth_offset, int count_row_to_copy, int count_col_to_copy, int depth);
		void block_copy(elemt* p, elemt* copi, int col_offset_p, int count_row_to_copy, int count_col_to_copy);

		void edge_to_zero(tensor* t0, tensor* t1, counter* f);
		void softmax_c_t(elemt* p, elemt* pp, counter* c);

		void softmax_row(elemt* p, elemt* pp, int size);
		void cb_conv(elemt* p, elemt* pp, elemt* o, const shape& s0, const shape& s1);

		void max(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1);
		void max_c(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1, const shape& s2);

		void acuumulate(elemt* p, elemt* pp, elemt* ac, const shape& s0, const shape& s1);
		void copy_value_ifn_edge(tensor* t0, tensor* t1, counter* f);

		void max_log(elemt* p, elemt* out, counter* f);
		void negative_to_zero(elemt* p);
		void negative_to_zero(elemt* p, elemt* out, counter* f);

		void multiply(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1);
		void tdot(elemt* p, elemt* pp, elemt* out, int pp_row, int p_row, int g);
	
		void right_edge_to_zero(tensor* t0, tensor* t1, counter* f);
		void left_edge_to_zero(tensor* t0, tensor* t1, counter* f);
	
		void nop(layer*);
		void c_tmultiply_s3(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1, const shape& s2);
		
		void c_tdevide(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1);
		void c_tmultiply(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1);
		
		void c_tsub(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1);
		void c_tadd(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1);

		void c_sqrt(elemt* p);

		void set_filter_from_map(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1);

		void set_filter_from_map(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1, const shape& s2);

		void accumulate(elemt* map, elemt* mask, elemt* out, const shape& s0, const shape& s1, const shape& s2);
		void cb_cdot(elemt* p, elemt* pp, elemt* out, int c_off_pp, int row_p, int n_use);
		void cb_multiply(elemt* p, elemt* pp, elemt* out, int not_usee, int not_use, int not_use_1);

		void callback_for_exp(elemt* p);
		void callback_for_exp(elemt* popt, elemt* copi);
	}

}

