#include "of.h"


#include <iostream>
using namespace std;
using namespace mdls;
using namespace macro;



void tfunc::init_tensor_ladder(tensor& c)
{
	elemt* p = c.get_p();
	for (int i = 0; i < c.get_alloc_size(); i++)
	{
		*p++ = i;
	}
}

void tfunc::init_tensor_with_random(tensor& c,int s , int e )
{
	default_random_engine rnd;

	normal_distribution	<elemt> rn(0.5, 2.0);
	elemt* p = new elemt[ B((&c)) * R((&c)) * D((&c)) * C((&c))];

	rnd.seed(rn(rnd));

	for (int i = 0; i < B((&c)) * R((&c)) * D((&c)) * C((&c)); i++)
	{
		elemt v = 0;

		while (true)
		{
			v = rn(rnd);

			if (v >= s && v <= e)
				break;
		}

		p[i] = v;

	}

	memcpy(c.get_p(), p, sizeof(elemt) *  B((&c)) * R((&c)) * D((&c)) * C((&c)));
}

void tfunc::softmax_s(tensor* p , tensor& out)
{
	shape s = p->get_shape();

	out.reshape(s);

	loop::loop_tns_row(p, out, softmax_row);
}


void tfunc::softmax(tensor* p, tensor& out)
{
	counter* c = new counter(p->get_shape(), out.get_shape());	
	shape s = p->get_shape();

	out.init(s);

	loop::loop_with_count(p, out, 1 , 1 , mdls::callback::softmax_c_t , c);


}

void tfunc::print_object(tensor* t)
{
	elemt* p = t->get_p();

	cout << t->get_tag() << endl;

	for (int i = 0; i < t->get_batch_count(); i++)
	{
		cout << "batch num : " << i << endl;
	
		for (int j = 0; j < t->get_channel_count(); j++)
		{
			cout << "channel num : " << j << endl;
			for (int y = 0; y < t->get_col_count(); y++)
			{
				for (int k = 0; k < t->get_row_count(); k++)
				{
					cout << *p++ << " ";
				}

				cout << endl;
			}
		
			cout << endl;
		}
		cout << endl;
	}

}

void tfunc::tdot(tensor* t, tensor* tt, tensor& out)
{
	shape s0 = t->get_shape();
	shape s1 = tt->get_shape();

	if (!(s0.Row == s1.Col))
		return;

	if (s0.Channel != s1.Channel)
		return;

	shape os(s0.Batch * s1.Batch, s0.Channel, s0.Col, s1.Row);

	out.reshape(os);
	
	elemt* p = out.get_p();

	loop::loop_tns_f_dot_inc_chn(t, tt, p,callback::tdot);

}

void tfunc::tmultiply(tensor* t , tensor* tt, tensor& out )
{
	shape s0 = t->get_shape();
	shape s1 = tt->get_shape();

	if (!(s0 == s1))
		return;

	out.reshape(s0);

	loop::tensor_loop(t, tt, out, 1, 1, multiply);

}

void tfunc::nhwc_to_hwio(tensor* c, tensor& out, int stride_row, int stride_col, int count_row_to_copy, int count_col_to_copy)
{
	int fr = macro::get_dv_size(c->get_row_count(), count_row_to_copy, stride_row);
	int fc = get_dv_size(c->get_col_count(), count_col_to_copy, stride_col);

	if (out.get_alloc_size() < fr * fc * c->get_channel_count() * c->get_batch_count())
		out.pv_alloc(fr * fc * c->get_channel_count() * c->get_batch_count());

	loop::tensor_loop_channel_first_with_stride
	(c, out.get_p(), stride_row, stride_col, count_row_to_copy, count_col_to_copy, callback::channel_copy);

	out.init(c->get_batch_count(), fr * fc , c->get_channel_count()  , count_row_to_copy * count_col_to_copy );
}


void loop_element( element* t , CALLBACK_SINGLE_LOOP_P_1 CC)
{
	elemt* p = t->get_p();
	for (int i = 0; i < t->get_batch_count(); i++)
	{
		cout << "batch num : " << i << endl;

		for (int j = 0; j < t->get_channel_count(); j++)
		{
			cout << "channel num : " << j << endl;
			for (int y = 0; y < t->get_col_count(); y++)
			{
				for (int k = 0; k < t->get_row_count(); k++)
				{
					CC(p++);
				}

				cout << endl;
			}

			cout << endl;
		}
		cout << endl;
	}
}


void tfunc::im2_col(tensor* c, tensor& cc, int stride_row, int stride_col, int count_row_to_copy, int count_col_to_copy)
{

	int lop_size_row = get_dv_size(R(c), count_row_to_copy, stride_row);
	int lop_size_col = get_dv_size(C(c), count_col_to_copy, stride_col);

	shape s(1, c->get_batch_count() , lop_size_row * lop_size_col ,  D(c) * count_row_to_copy * count_col_to_copy);

	cc.reshape(s);
	elemt* p = cc.get_p();

	loop::tensor_loop_channel_first_with_stride
	(c, p, stride_row, stride_col, count_row_to_copy, count_col_to_copy, channel_copy);
}


void tfunc::edot(element* t, element* tt , tensor& out )
{
	if (R(t) != COL(tt))
	{
		cout << "invalid cube and matrix form" << endl;
		return;
	}

	shape is = t->get_shape();
	shape ws = tt->get_shape();

	shape s( is.Channel , ws.Channel, is.Col , ws.Row);

	out.reshape(s);

	loop::loop_tns_f_dot_inc_chn(t, tt, out.get_p() , cb_cdot);

}

void tfunc::compact_col(tensor* t, int size)
{	
	elemt* ppv = t->get_p();
	elemt* p0 = ppv;

	int dvc = C(t) / size;
	int alc = B(t) * D(t) * dvc * R(t);

	elemt* enp = new elemt[alc];

	elemt* np = enp;
	elemt* nnp = np;

	for (int i = 0; i < B(t) * D(t) * dvc; i++)
	{
		for (int o = 0; o < R(t); o++)
			*(nnp + o) = 0;

		for (int y = 0; y < size; y++)
		{
			np = nnp;
			p0 = ppv;

			for (int j = 0; j < R(t); j++, np++, p0++)
				*np += *(p0 + (y * C_OFF(t)));
		}

		nnp += C_OFF(t);
		ppv += size * C_OFF(t);
	
	}

	shape s = t->get_shape();
	s.Col = dvc;

	s = shape(s);
	t->reshape(s);

	memcpy(t->get_p(), enp, sizeof(elemt) * B(t) * D(t) * dvc * R(t));

}

void tfunc::compact_channel(tensor* t, int size)
{
	elemt* ppv = t->get_p();
	elemt* p0 = ppv;

	int dvd = D(t) / size;
	elemt* enp = new elemt[B(t) * C(t) * dvd * R(t)];

	elemt* np = enp;
	elemt* nnp = np;


	for (int i = 0; i < B(t) * dvd; i++)
	{
		memcpy(nnp , ppv , sizeof(elemt) * R(t) * C(t));

		for (int y = 1; y < size; y++)
		{
			np = nnp;
			p0 = ppv;

			for (int k = 0; k < R(t) * C(t); k++)
				*np++ += *p0++;
			
		}

		nnp += D_OFF(t);
		ppv += size * D_OFF(t);

	}

	ppv = (elemt*)malloc(sizeof(elemt) * B(t) * C(t) * dvd * R(t));
	memcpy(ppv, enp, sizeof(elemt) * B(t) * C(t) * dvd * R(t));

	shape s = t->get_shape();
	s.Channel = dvd;

	s = shape(s);

	t->init(ppv, s.Batch, s.Channel, s.Col, s.Row);


}


void tfunc::channel_times(tensor& t , int times)
{
	elemt* p = t.get_p();
	shape s0 = t.get_shape();

	s0.Channel *= times;

	s0 = shape (s0);

	tensor temp(s0);

	elemt* tempp = temp.get_p();
	elemt** ip;
	elemt** outp;

	fill_args_pointer( ip , 5, p);
	fill_args_pointer( outp , 5 , tempp);

	for (int i = 0; i < B((&t)); i++)
	{
		ip[1] = p + i * B_OFF((&t)); 
		outp[1] = temp.get_p() + i * B_OFF((&temp));

		for (int y = 0; y < R((&t)) * C((&t)) ; y++)
		{
			ip[0] = ip[1];
			outp[0] = outp[1];
			
			tempp = outp[0];

			for (int k = 0; k < D((&t)); k++)
			{
				for (int j = 0; j < times; j++)
				{
					*tempp = *ip[0];
					tempp += D_OFF((&temp));
				}

				ip[0] += D_OFF((&t));
			}
			
			ip[1]++;
			outp[1]++;
		}
	}

	t = temp;
}

void tfunc::colmn_times(tensor& t, int times)
{
	mdls::shape s0 = t.get_shape();

	s0.Row = s0.Row * times;

	s0 = shape(s0);

	tensor tmp = t;
	tmp.reshape(s0);

	elemt* p = t.get_p();
	elemt* op = tmp.get_p();

	for (int i = 0; i < B((&t)) * D((&t)); i++)
		for (int y = 0; y < C((&t)); y++)
		{
			for (int k = 0; k < R((&t)); k++)
			{
				for (int j = 0; j < times; j++)
				{
					*op++ = *p;
				}

				*p++;

			}
		}

	t = tmp;

}

void tfunc::batch_times(tensor& t , int times)
{
	t.transpose(n1, n0, n2, n3);
	channel_times(t, times);
	t.transpose(n1, n0, n2, n3);
}

void tfunc::batch_repeat(tensor& t, int times)
{
	shape s = t.get_shape();
	size_t bs = sizeof(elemt) * s.Alloc_size;

	elemt* p = (elemt*)malloc(bs * times);
	elemt* pbs = p;

	for (int i = 0; i < times; i++, pbs += s.Alloc_size )
		memcpy(pbs, t.get_p(), bs);

	free( t.get_p() );

	s.Batch *= times;
	s = shape(s);
	
	t.init(p, s.Batch, s.Channel, s.Col, s.Row);

}
void tfunc::compact_batch_evrg(tensor& t, int nsize)
{
	compact_batch_accum(t, nsize);

	static elemt nn = nsize;

	t.apply_to_all_elem([](elemt* p)->void
	{
		*p /= nn;
	} );

}

void tfunc::compact_batch_accum(tensor& t , int nsize)
{
	shape s = t.get_shape();
	int dvb = s.Batch / nsize ;
	elemt* p = t.get_p();
	elemt* pp = p;

	elemt* wp = p;

	for (int i = 0; i < s.Alloc_size / s.Batch; i++)
	{
		pp = p;
		wp = p;

		for (int k = 0; k < dvb; k++)
		{
			for (int j = 1; j < nsize; j++)
			{
				*wp += *(pp + (B_OFF((&t)) * j));
			}

			pp += nsize * B_OFF((&t));
			wp += B_OFF((&t));
		}

		p += 1;
	}

	t.reshape(shape(dvb, s.Channel, s.Col, s.Row));

}

void tfunc::sum_tensor(target_dm dm , const tensor* p, tensor& out )
{
	shape s0 = p->get_shape();
	shape s1(s0);

	int lop_size_k = 1;
	int lop_size_i = 1;

	for (int i = 0; i < 4; i++)
	{
		if (i > dm)
		{
			lop_size_k *= *s0.p[i];
			*s1.p[i] = 1;
		}

		else
			lop_size_i *= *s0.p[i];
	
	}

	s1 = shape(s1);
	out.reshape(s1);
	out.initialize_elem_to(0);

	elemt* op = out.get_p();
	elemt* ip = p->get_p();

	for (int i = 0; i < lop_size_i; i++)
	{
		for (int k = 0; k < lop_size_k; k++)
		{
			*op += *ip;
			ip++;
		}

		op++;
	}

}
void tfunc::tdot_acm(tensor* p, tensor* pp, tensor& out)
{

	loop::loop_tensor_with_eq_chn_by_acm(p, pp, out, [](elemt* p, elemt* pp, elemt* out, const shape& s0, const shape& s1, const shape& s2)->void
	{
		elemt* sp = p;
		elemt* spp = pp;

		elemt* sspp = spp;

		for (int k = 0; k < s0.Col; k++)
		{
			spp = sspp;

			for (int i = 0; i < s1.Row; i++)
			{
				p = sp;
				pp = spp;

				for (int j = 0; j < s1.Col; j++)
				{
					*out += *p++ * *pp;
					pp += s1.Row;
				}

				out++;
				spp++;

			}

			sp += s0.Row;


		}
	});


}


void tfunc::tdot_n(tensor* t , tensor* tt , tensor& o)
{
	o.reshape(shape( t->get_batch_count(), t->get_channel_count(), t->get_col_count(), tt->get_row_count()));
	o.initialize_elem_to(0);

	loop::loop_tensor_with_same_chn_count(t, tt, o, 
		[](elemt* p0 , elemt* p1 , elemt* op , const shape& s0 , const shape& s1 , const shape& s2 , int row , int col)->void
	{
		elemt* pp1 = p1;
		elemt* pp0 = p0;

		for (int k = 0; k < s0.Col; k++)
		{
			for (int i = 0; i < s1.Row; i++)
			{
				pp1 = p1 + i;
				pp0 = p0;

				for (int j = 0; j < s1.Col; j++)
				{
					*op += *pp0++ * *pp1;

					pp1 += s1.Row;
				}

				op++;

			}

			p0 += s0.Row;
		}
	});



}


void tfunc::af_inv_acm_chn(tensor* dout, tensor* in_tsp, tensor* weight_tsp , tensor& dout_in, tensor &dweight_in)
{
	// dout and in_tsp have same batch count
	shape s0 = dout->get_shape();
	shape s1 = in_tsp->get_shape();

	shape s2 = weight_tsp->get_shape();

	dweight_in.reshape(s2);
	dweight_in.initialize_elem_to(0);

	dout_in.reshape(s1);
	dout_in.initialize_elem_to(0);

	tensor kd = *dout;
	tensor kintp = *in_tsp;
	tensor kwt = *weight_tsp;

	kintp.transpose(n0, n1, n3, n2);
	kwt.transpose(n0, n1, n3, n2);

	elemt* pd = dout->get_p();
	elemt* wtp = kwt.get_p();

	elemt* intp = kintp.get_p();

	elemt* outp = dout_in.get_p();
	elemt* outwp = dweight_in.get_p();
	
	elemt* ppd = pd;
	elemt* intpp = intp;
	elemt* outpp = outp;

	shape sw = kwt.get_shape();
	tensor td = *dout;
	
	tensor ttwt = kwt;

	tfunc::compact_channel(&td , td.get_channel_count() );
	tfunc::channel_times(td, sw.Channel);

	tfunc::compact_batch_accum(ttwt, ttwt.get_batch_count());
	tfunc::batch_times(ttwt, s1.Batch);

	tdot_n(&td, &ttwt , dout_in);
	
	td = *dout;
	tfunc::compact_batch_accum(td, td.get_batch_count());
	tfunc::channel_times(td, sw.Channel);

	tfunc::compact_batch_accum(kintp , s1.Batch);
	tfunc::batch_times(kintp, sw.Batch);
	
	tfunc::tdot_n(&kintp , &td , dweight_in);
	
}
