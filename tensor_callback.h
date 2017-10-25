#pragma once
#include "tensor.h"
#include "layert.h"
#include "loop_callback.h"
#include "counter_extens.h"
#include "macro.h"

namespace mdls
{
	namespace callback
	{
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

	}

}

