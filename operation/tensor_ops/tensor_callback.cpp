#include "tensor_callback.h"
#include <cmath>
#include <cstring>

using namespace mdls;
using namespace callback;
using namespace std;

void callback::negative_to_zero(elemt* p )
{
	if (*p < 0)
		*p = 0;
}

void callback::negative_to_zero(elemt* p, elemt* out, counter* f)
{
	int idx = f->get_offset(2);

	if (*p < 0)
	{
		*out = 0;
		f->write_log(0, idx, f->get_offset(0));

		f->inc_count(2);
	}

	else
		*out = *p;

}

void callback::max_log(elemt* p  , elemt* out,  counter* f)
{
	elemt* ip = p;

	shape s0 = f->get_orginal_shape(0);
	shape s1 = f->get_local_shape();

	int indx = f->get_offset(1);

	int r = 0;
	int c = 0;

	int val = 0;

	int stride_row;

	for (int i = 0; i < s1.Col; i++)
	{
		ip = p;

		for (int j = 0; j < s1.Row; j++)
		{
			if (*out <= *ip)
			{
				*out = *ip;
				r = j;
				c = i;
			}

			ip++;
		}

		p += s0.Row;
	}

	val = f->get_offset(0) + r + (c * f->get_orginal_shape(0).Row);
	f->write_log(0, indx, val);

}


void callback::max(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1)
{
	elemt* ip = p;

	*out = 0;

	for (int i = 0; i < s1.Col; i++)
	{
		ip = p;

		for (int j = 0; j < s1.Row; j++)
		{
			if (*out <= *ip)
				*out = *ip;

			ip++;
		}

		p += s0.Row;
	}


}

void callback::acuumulate(elemt* p, elemt* pp, elemt* ac, const shape& s0, const shape& s1)
{
	elemt* ip = p;
	elemt* acc = ac;

	for (int i = 0; i < s1.Col; i++)
	{
		ip = p;
		acc = ac;

		for (int j = 0; j < s1.Row; j++)
		{
			*acc++ += *ip * *pp;

			ip++;
			pp++;
		}

		ac += s0.Row;
		p += s0.Row;
	}

}


void callback::cb_conv(elemt* p, elemt* pp, elemt* o, const shape& s0, const shape& s1)
{
	*o = 0;
	elemt* ip = p;

	for (int i = 0; i < s1.Col; i++)
	{
		ip = p;

		for (int j = 0; j < s1.Row; j++)
		{
			*o += *ip * *pp;

			ip++;
			pp++;
		}

		p += s0.Row;
	}

}

void callback::copy_value_ifn_edge(tensor* t0, tensor* t1 , counter* f)
{
	shape ks;
	f->get_counted_shape(0, ks);

	shape ns = f->get_orginal_shape(1);
	shape os = f->get_orginal_shape(0);

	elemt* p = t0->get_p();
	elemt* cop = t1->get_p();

	bool edge = false;

	int* pad = new int[4]{
		-*ns.p[0] + *os.p[0],
		-*ns.p[1] + *os.p[1],
		-*ns.p[2] + *os.p[2],
		-*ns.p[3] + *os.p[3]
	};

	Dm dim = f->get_target();
	int idx = 0;

	if (dim <= 3)
	{
		if(*ks.p[dim] < pad[dim] / 2 || *ks.p[dim] >= *ns.p[dim] + (pad[dim] / 2) )
		{
			edge = true;
		}

	}

	else if (dim == Surface || dim == Volume)
	{
		if (dim == Surface)
			idx = 2;

		else if (dim == Volume)
			idx = 3;

		for (int j = 0; j < idx; j++)
		{
			if ((*ks.p[3 - j] < (pad[3 - j] / 2) ) || (*ks.p[3 - j] >= *ns.p[3 - j] + ( pad[3 - j] / 2) ))
			{
				edge = true;
				break;
			}

		}
	}

	if (edge)
	{
		f->inc_count(0);
		f->callback_end();
		return;
	}

	else
	{
		*(cop + f->get_offset(1)) = *(p + f->get_offset(0));
		f->inc_count_all();
		f->callback_end();
	}

}


void callback::edge_to_zero(tensor* t0, tensor* t1, counter* f)
{
	shape ks;
	f->get_counted_shape(1 , ks);

	shape ns = f->get_orginal_shape(1);
	shape os = f->get_orginal_shape(0);

	elemt* p = t0->get_p();
	elemt* cop = t1->get_p();

	bool edge = false;

	int* pad = new int[4]{
		*ns.p[0] - *os.p[0],
		*ns.p[1] - *os.p[1],
		*ns.p[2] - *os.p[2],
		*ns.p[3] - *os.p[3]
	};


	for (int i = 0; i < 4; i++)
	{
		int a = *ks.p[i];

		if (pad[i] == 0)
			continue;

		if ( ( a < pad[i] / 2  ) || (a >= (*os.p[i] + (pad[i] / 2) ) ) )
		{
			edge = true;
			break;
		}

	}

	if (edge)
	{
		*(cop + f->get_offset(1)) = 0;
		f->inc_count(1);
	}

	else
	{
		*(cop + f->get_offset(1)) = *(p + f->get_offset(0));
		f->inc_count_all();
	}

	f->callback_end();
}
void callback::left_edge_to_zero(tensor* t0, tensor* t1, counter* f)
{
	shape ks;
	f->get_counted_shape(1, ks);

	shape ns = f->get_orginal_shape(1);
	shape os = f->get_orginal_shape(0);

	elemt* p = t0->get_p();
	elemt* cop = t1->get_p();

	bool edge = false;

	int* pad = new int[4]{
		*ns.p[0] - *os.p[0],
		*ns.p[1] - *os.p[1],
		*ns.p[2] - *os.p[2],
		*ns.p[3] - *os.p[3]
	};


	for (int i = 0; i < 4; i++)
	{
		int a = *ks.p[i];

		if (pad[i] == 0)
			continue;
		if ((a < pad[i] ))
		{
			edge = true;
			break;
		}

	}

	if (edge)
	{
		*(cop + f->get_offset(1)) = 0;
		f->inc_count(1);
	}

	else
	{
		*(cop + f->get_offset(1)) = *(p + f->get_offset(0));
		f->inc_count_all();
	}

	f->callback_end();
}
void callback::right_edge_to_zero(tensor* t0, tensor* t1, counter* f)
{
	shape ks;
	f->get_counted_shape(1, ks);

	shape ns = f->get_orginal_shape(1);
	shape os = f->get_orginal_shape(0);

	elemt* p = t0->get_p();
	elemt* cop = t1->get_p();

	bool edge = false;

	int* pad = new int[4]{
		*ns.p[0] - *os.p[0],
		*ns.p[1] - *os.p[1],
		*ns.p[2] - *os.p[2],
		*ns.p[3] - *os.p[3]
	};


	for (int i = 0; i < 4; i++)
	{
		int a = *ks.p[i];

		if (pad[i] == 0)
			continue;
		if (a >= (*os.p[i] + pad[i] - 1))
		{
			edge = true;
			break;
		}

	}

	if (edge)
	{
		*(cop + f->get_offset(1)) = 0;
		f->inc_count(1);
	}

	else
	{
		*(cop + f->get_offset(1)) = *(p + f->get_offset(0));
		f->inc_count_all();
	}

	f->callback_end();
}


void callback::softmax_row(elemt* p, elemt* pp, int range)
{
	elemt maxv = macro::get_max_in_range(p, range);
	elemt* temp = new elemt[range];

	elemt sum = 0;

	macro::minus_in_range(p, maxv, temp, range);

	macro::loop_elem(temp, callback_for_exp, range);
	sum = macro::get_sum_in_range(temp, range);

	macro::devide_in_range(temp, sum, range);

	memcpy(pp, temp, sizeof(elemt) * range);
}

void callback::softmax_c_t(elemt* p, elemt* pp, counter* c)
{
	int range = *c->get_orginal_shape(0).p[Row];

	elemt maxv = macro::get_max_in_range(p, range);
	elemt* temp = new elemt[range];

	elemt sum = 0;

	macro::minus_in_range(p, maxv, temp, range);

	macro::loop_elem(temp, callback_for_exp, range);

	macro::get_sum_in_range(temp, range);

	macro::devide_in_range(temp, sum, range);

	memcpy(pp, temp, sizeof(elemt) * range);

}
void callback::c_tdevide(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1)
{
	*p = *t / *tt;
}

void callback::c_tmultiply_s3(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1, const shape& s2)
{
	*p = *t * *tt;
}

void callback::c_tmultiply(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1)
{
	*p = *t * *tt;
}


void callback::c_tsub(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1)
{
	*p = *t - *tt;
}

void callback::c_tadd(elemt* t, elemt* tt, elemt* p, const shape& s0, const shape& s1)
{
	*p = *t + *tt;
}

void callback::c_sqrt(elemt* p)
{
	*p = std::sqrt(*p);
}

void callback::multiply(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1)
{
	*out = *p * *pp;
}

void callback::tdot(elemt* p, elemt* pp, elemt* out, int pp_row, int p_row, int g)
{
	*out = 0;

	for (int i = 0; i < p_row; i++)
	{
		*out += *p * *pp;
		p++;
		pp += pp_row;

	}


}

void callback::nop(layer*)
{

}

void callback::set_filter_from_map(elemt* p, elemt* pp, elemt* n_use, const shape& s0, const shape& s1, const shape& n_uses )
{
	offset o0(s0);
	offset o1(s1);

	for (int j = 0; j < s1.Col; j++)
	{
		for (int i = 0; i < s1.Row; i++)
		{
			*(pp + i) = *(p + i);
		}

		pp += o1.Col;
		p += o0.Col;
	}
}

void callback::set_filter_from_map(elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1)
{
	offset o0(s0);
	offset o1(s1);

	for (int j = 0; j < s1.Col; j++)
	{

		for (int i = 0; i < s1.Row; i++)
		{
			*(out + i) = *(p + i);
		}

		out += o1.Col;
		p += o0.Col;
	}
}


void callback::accumulate(elemt* map, elemt* mask, elemt* out, const shape& s0, const shape& s1, const shape& s2)
{

	offset o0(s0);
	offset o1(s1);
	offset o2(s2);

	elemt* mapp = map;
	elemt* maskp = mask;
	elemt* outp = out;

	for (int i = 0; i < s1.Col; i++)
	{
		out = outp;
		mask = maskp;

		for (int j = 0; j < s1.Row; j++)
		{
			*out *= *mask;

			out++;
			mask++;
		}

		out += o2.Col;
		mask += o1.Col;
	}



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

void callback::callback_for_exp(elemt* p)
{
	*p = exp(*p);
}

void callback::callback_for_exp(elemt* popt, elemt* copi)
{
	*copi = exp(*popt);
}
