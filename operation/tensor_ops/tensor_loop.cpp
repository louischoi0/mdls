#include "tensor_loop.h"

#include "..\macro\macro.h"

using namespace mdls;
using namespace loop;
using namespace macro;

void loop::loop_tns_inc_output_by_one(tensor* t, tensor* mask, tensor& out, int stride_row, int stride_col, TASK_TTT_SS task)
{
	// output tensor pointer increased by one at every mement task acts

	int dvr = macro::get_dv_size(t->get_row_count(), mask->get_row_count(), stride_row);
	int dvc = macro::get_dv_size(t->get_col_count(), mask->get_col_count(), stride_col);

	const shape is = t->get_shape();
	const shape ms = mask->get_shape();

	elemt** in;
	elemt** mk;
	elemt** o;

	fill_args_pointer(in, 4, t->get_p());
	fill_args_pointer(mk, 3, mask->get_p());
	fill_args_pointer(o, 4, out.get_p());

	for (int z = 0; z < B(t); z++)
		for (int l = 0; l < B(mask); l++)
		{
			in[0] = t->get_p() + B_OFF(t) * z;
			mk[0] = mask->get_p() + B_OFF(mask) * l;

			in[1] = in[0];
			mk[1] = mk[0];

			for (int j = 0; j < D(t); j++)
			{
				in[2] = in[1];
				mk[2] = mk[1];

				for (int y = 0; y < dvc; y++)
				{
					in[3] = in[2];
					mk[2] = mk[1];

					for (int i = 0; i < dvr; i++)
					{
						task(in[3], mk[2], o[3]++, is, ms);
						in[3] += stride_row;
					}

					in[2] += stride_col * C_OFF(t);
				}

				mk[1] += D_OFF(mask);
				in[1] += D_OFF(t);
			}

		}
}



void loop::loop_tns_surface_mask(tensor* t , tensor* mask , tensor& out, int stride_row , int stride_col , TASK_TTT_SS task)
{
	//main tensor and mask tensor must have same channel(depth) 
	//main tensor and output tensor must have same format( B , D , C ,R )


	int dvr = macro::get_dv_size(t->get_row_count(), mask->get_row_count(), stride_row);
	int dvc = macro::get_dv_size(t->get_col_count(), mask->get_col_count(), stride_col);

	const shape is = t->get_shape();
	const shape ms = mask->get_shape();

	elemt** in;
	elemt** mk;
	elemt** o;

	fill_args_pointer(in, 4, t->get_p());
	fill_args_pointer(mk, 3, mask->get_p());
	fill_args_pointer(o, 4, out.get_p());

	for (int z = 0; z < B(t); z++)
		for (int l = 0; l < B(mask); l++)
		{
			in[0] = t->get_p() + B_OFF(t) * z;
			mk[0] = mask->get_p() + B_OFF(mask) * l;
			
			in[1] = in[0];
			mk[1] = mk[0];

			for (int j = 0; j < D(t); j++)
			{
				o[2] = o[1];
				in[2] = in[1];
				mk[2] = mk[1];

				for (int y = 0; y < dvc; y++)
				{
					o[3] = o[2];
					in[3] = in[2];
					mk[2] = mk[1];

					for (int i = 0; i < dvr; i++)
					{
						task(in[3], mk[2], (o[3]), is, ms);
						in[3] += stride_row;
						o[3] += stride_row;
					}

					in[2] += stride_col * C_OFF(t);
					o[2] += stride_col * C_OFF((&out));
				}

				mk[1] += D_OFF(mask);
				in[1] += D_OFF(t);
				o[1] += D_OFF((&out));

			}

		}
}


void loop::tensor_loop(tensor* t, TASK_T task ,int stride_row , int stride_col)
{
	elemt* p = t->get_p();

	for (int i = 0; i < t->get_alloc_size(); i++)
		task(p++);
}

void loop::tensor_loop_with_filter_and_two_tensor(tensor* t, tensor* filter, tensor& out, int stride_col, int stride_row, TASK_TTT_SSS task0)
{
	// t and out have same shape
	shape s0 = t->get_shape();
	shape s1 = filter->get_shape();
	shape s2 = out.get_shape();

	int dvc = macro::get_dv_size(s0.Col, s1.Col, stride_col);
	int dvr = macro::get_dv_size(s0.Row, s1.Row, stride_row);

	elemt* p0 = t->get_p();
	elemt* p1 = filter->get_p();
	elemt* po = out.get_p();

	elemt* pp0 = p0;
	elemt* ppo = po;


	for (int i = 0; i < B(t) * D(t); i++)
	{
		pp0 = t->get_p() + ( D_OFF(t) * i);
		ppo = out.get_p() + (D_OFF((&out)) * i);

		for (int y = 0; y < dvc; y++)
		{	
			p0 = pp0 + ( C_OFF(t) * y );
			po = ppo + ( C_OFF((&out)) * y);

			for (int k = 0; k < dvr; k++)
			{
				task0(p0, p1, po, s0, s1, s2 );

				p0 += stride_row;
				po += stride_row;
			}
		}
		

		p1 += D_OFF(filter);
	}


}
	
void loop::tensor_loop(tensor* t, tensor* tt, tensor& out, int stride_col, int stride_row , TASK_TTT_SS T)
{
	// t and tt have same shape
	shape s0 = t->get_shape();
	shape s1 = tt->get_shape();

	int dvc = macro::get_dv_size(s0.Col, 1, stride_col);
	int dvr = macro::get_dv_size(s0.Row, 1, stride_row);

	elemt* p0 = t->get_p();
	elemt* p1 = tt->get_p();

	elemt* po = out.get_p();

	for (int i = 0; i < B(t) * D(t); i++)
	{
		for (int y = 0; y < dvc; y++)
		{
			for (int k = 0; k < dvr; k++)
			{
				T(p0, p1, po, s0, s1);
				
				p0 += stride_row;
				p1 += stride_row;
				po ++;
			
			}
		}
	}
}


void loop::tensor_loop(target_dm d, tensor* p, tensor* out, int stride_row, int stride_col, CALLBACK_WITH_FCT_TT CC)
{
	int lop_size_row = get_dv_size(R(p), 1, stride_row);
	int lop_size_col = get_dv_size(C(p), 1, stride_col);

	shape rs = p->get_shape();
	shape ls = out->get_shape();

	counter c(rs, ls);
	c.set_target(d);

	for (int i = 0; i < p->get_alloc_size(); i++)
	{
		CC(p, out, &c);
	}

}


void loop::tensor_loop(tensor* p , tensor* out , int stride_row , int stride_col , CALLBACK_WITH_FCT_TT CC )
{
	int lop_size_row = get_dv_size(R(p) , 1, stride_row);
	int lop_size_col = get_dv_size(C(p) , 1, stride_col);

	shape rs = p->get_shape();
	shape ls = out->get_shape();

	counter c( rs , ls );

	for (int i = 0; i < out->get_alloc_size(); i++)
	{
		CC(p, out , &c);
	}
}

void loop::tensor_loop_channel_first_with_stride
(tensor* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_CHN_LOOP CC)
{
	int lop_size_row = get_dv_size(R(p), fil_size_row, stride_row);
	int lop_size_col = get_dv_size(C(p), fil_size_col, stride_col);

	elemt** e;
	elemt** c;

	fill_args_pointer( e, 3 ,  p->get_p());
	fill_args_pointer( c, 1 ,  copi);

	for (int y = 0; y < B(p); y++)
	{
		e[1] = e[0];

			for (int i = 0; i < lop_size_col; i++)
			{
				e[2] = e[1];

				for (int j = 0; j < lop_size_row; j++)
				{
					CC(e[2], c[0] , C_OFF(p), D_OFF(p), fil_size_row, fil_size_col, D(p));

					e[2] += stride_row;
					c[0] += fil_size_row * fil_size_col * D(p);
				}

				e[1] += C_OFF(p) * stride_col;
			}
		
			e[0] += B_OFF(p);
	}

}
void outer_loop_accum_chn()
{

}

void loop::loop_with_count(element* p , element& out , int str_r , int str_c ,TASK_WITH_FCT_TT task , counter* f)
{
	shape s = p->get_shape();

	int dvr = macro::get_dv_size(s.Row, str_r, str_r);
	int dvc = macro::get_dv_size(s.Col, str_c, str_c);

	elemt* ip = p->get_p();
	
	shape rs(B(p), D(p), dvc, dvr);
	shape e(1, 1, str_r, str_c);

	elemt* pp = out.get_p();
	
	f->set_local_shape(e);

	f->initialize_log(1, rs.Alloc_size);

	for (int i = 0; i < B(p) * D(p) ; i++)
		for (int j = 0; j < dvc; j++)
		{
			for (int y = 0; y < dvr; y++)
			{
				task(ip, pp, f);
			
				ip += str_r;
				pp++;

				f->inc_count(1);
				f->inc_count(0 , str_r , Row);
			}

			ip += C_OFF(p) * ( str_c - 1 );
			f->inc_count(0, str_c - 1, Col);
		}
}

void loop::loop_tns_row(tensor* t , tensor& out, TASK_LOCAL_EE task)
{
	elemt* pp = out.get_p();
	elemt* p = t->get_p();
	int rowl = t->get_row_count();

	for (int i = 0; i < t->get_batch_count(); i++)
		for (int j = 0; j < t->get_channel_count(); j++)
			for(int y = 0; y < t->get_col_count() ; y++)

		{
			task(p, pp, rowl);
			p += t->get_col_offset();
			pp += out.get_col_offset();
		}


}
void loop::loop_tns_f_dot_linear_chn(element* p, element* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC)
{
	// this function suppose that p and pp have same Batch and Channel

	elemt** in;
	elemt** eqiv;

	elemt** out;

	macro::fill_args_pointer(in, 2, p->get_p());
	macro::fill_args_pointer(eqiv, 3, pp->get_p());
	macro::fill_args_pointer(out, 1, copi);

	int lop_size_row = pp->get_row_count();
	int lop_size_col = p->get_col_count();

		for (int i = 0; i < B(pp); i++)
		{
			in[1] = p->get_p() + (B_OFF(p) * i);
			eqiv[2] = pp->get_p() + (B_OFF(pp) * i);

			for (int z = 0; z < D(p); z++)
			{
				in[0] = in[1];
				eqiv[1] = eqiv[2];

				for (int j = 0; j < lop_size_col; j++)
				{
					eqiv[0] = eqiv[1];

					for (int k = 0; k < lop_size_row; k++)
					{
						CC(in[0], eqiv[0]++, out[0]++, R(pp), R(p), R(pp));
					}

					in[0] += C_OFF(p);
				}

				eqiv[2] += D_OFF(pp);
				in[1] += D_OFF(p);
			}
		}
	

}

void loop::loop_tns_f_dot_inc_batch(element* p, element* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC)
{
	elemt** in;
	elemt** eqiv;

	elemt** out;

	macro::fill_args_pointer(in, 2, p->get_p());
	macro::fill_args_pointer(eqiv, 3, pp->get_p());
	macro::fill_args_pointer(out, 1, copi);

	int lop_size_row = pp->get_row_count();
	int lop_size_col = p->get_col_count();

	for (int y = 0; y < B(p); y++)
	{
		for (int i = 0; i < B(pp); i++)
		{
			in[1] = p->get_p() + (B_OFF(p) * y);
			eqiv[2] = pp->get_p() + (B_OFF(pp) * i);

			for (int z = 0; z < D(p); z++)
			{
				in[0] = in[1];
				eqiv[1] = eqiv[2];
				
				for (int j = 0; j < lop_size_col; j++)
				{
					eqiv[0] = eqiv[1];

					for (int k = 0; k < lop_size_row; k++)
					{
						CC(in[0], eqiv[0]++, out[0]++, R(pp), R(p), R(pp));
					}

					in[0] += C_OFF(p);
				}

				eqiv[2] += D_OFF(pp);
				in[1] += D_OFF(p);
			}
		}
	}
}

void loop::loop_tns_f_dot_inc_chn(element* p, element* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC)
{
	elemt** in;
	elemt** eqiv;
	
	elemt** out;

	macro::fill_args_pointer(in ,2 ,p->get_p());
	macro::fill_args_pointer(eqiv, 2, pp->get_p());
	macro::fill_args_pointer(out , 1 , copi);

	int lop_size_row = pp->get_row_count();
	int lop_size_col = p->get_col_count();

	for(int y = 0; y < D(p); y++)
	{
		eqiv[1] = eqiv[0];

		for (int i = 0; i < D(pp); i++)
		{
			in[1] = in[0];
			eqiv[2] = eqiv[1];

			for (int j = 0; j < lop_size_col; j++)
			{
				eqiv[2] = eqiv[1];

				for (int k = 0; k < lop_size_row; k++)
				{
					CC(in[1], eqiv[2]++, out[0]++, R(pp), R(p), R(pp));
				}

				in[1] += C_OFF(p);
			}
			
			eqiv[1] += D_OFF(pp);
		}

		in[0] += D_OFF(p);
	}
}


void loop::loop_tensor_with_one_chn_repeat_linear
(elemt *p, elemt* pp, elemt* out,
	const shape& s0, const shape& s1, const shape& s2,
	int stride_col, int stride_row, lambda_inner_s3_with_stride f)
{
	// this operation apply only to tensor with one batch

	elemt* p0 = p;
	elemt* p1 = pp;
	elemt* po = out;

	elemt* pp1 = p1;

	offset o1(s1);

	for (int j = 0; j < s1.Channel; j++)
	{
		p1 = pp1 + (o1.Channel * j);
		f(p0, p1, po, s0, s1, s2, stride_col, stride_row);
	}
}

void loop::loop_tensor_with_one_chn_repeat_inc_outc_fs(elemt *p, elemt* pp, elemt* out,
	const shape& s0, const shape& s1, const shape& s2, const shape& fs,
	int stride_col, int stride_row, lambda_inner_s3_with_stride_fs f)
{
	elemt* p0 = p;
	elemt* p1 = pp;
	elemt* po = out;

	elemt* pp1 = p1;

	offset o1(s1);
	offset o2(s2);

	for (int j = 0; j < s1.Channel; j++)
	{
		f(p0, p1, po, s0, s1, s2, fs ,stride_col, stride_row);

		p1 += o1.Channel;
		po += o2.Channel;
	}


}

void loop::loop_tensor_with_one_chn_repeat_inc_outc(elemt *p, elemt* pp, elemt* out,
	const shape& s0, const shape& s1, const shape& s2,
	int stride_col, int stride_row, lambda_inner_s3_with_stride f)
{
	elemt* p0 = p;
	elemt* p1 = pp;
	elemt* po = out;

	elemt* pp1 = p1;

	offset o1(s1);
	offset o2(s2);

	for (int j = 0; j < s1.Channel; j++)
	{
		f(p0, p1, po, s0, s1, s2, stride_col, stride_row);
		
		p1 += o1.Channel;
		po += o2.Channel;
	}


}


void loop::loop_tensor_with_one_chn_repeat
(elemt *p, elemt* pp, elemt* out,
	const shape& s0, const shape& s1, const shape& s2,
	int stride_col, int stride_row, lambda_inner_s3_with_stride f)
{
	// this operation apply only to tensor with one batch
	// 

	elemt* p0 = p;
	elemt* p1 = pp;
	elemt* po = out;

	elemt* pp0 = p0;
	elemt* pp1 = p1;
	elemt* ppo = po;

	offset o0(s0);
	offset o1(s1);
	offset o2(s2);

	for (int j = 0; j < s1.Channel; j++)
	{
		p0 = pp0;
		po = ppo;

		p1 = pp1 + (o1.Channel * j);

		for (int i = 0; i < s0.Channel; i++)
		{
			f(p0, p1, po, s0, s1, s2, stride_col, stride_row);

			po += o2.Channel;
			p0 += o0.Channel;
		}
	}
}

void loop::chn_task_linear(elemt* p , elemt* pp , elemt* out , const shape& s0 , const shape& s1 , const shape& s2, int cols , int rows , lambda_inner_s3_with_stride f)
{
	offset o0(s0);
	offset o1(s1);
	offset o2(s2);

	for (int i = 0; i < s0.Channel; i++)
	{
		f(p, pp, out, s0, s1, s2, cols, rows);

		p += o0.Channel;
		pp += o1.Channel;
		out += o2.Channel;
	}


}

void loop::loop_tensor_with_one_chn_repeat_in_capture
(   elemt *p, elemt* pp, elemt* out,
	const shape& s0, const shape& s1, const shape& s2,
	int stride_col, int stride_row, lambda_void f)
{
	// this operation apply only to tensor with one batch
	// 

	elemt* p0 = p;
	elemt* p1 = pp;
	elemt* po = out;

	elemt* pp0 = p0;
	elemt* pp1 = p1;
	elemt* ppo = po;

	offset o0(s0);
	offset o1(s1);
	offset o2(s2);

	for (int j = 0; j < s1.Channel; j++)
	{
		for (int i = 0; i < s0.Channel; i++)
		{
			f();
		}
	}
	

}


void loop::loop_tensor_with_eq_chn_by_acm(tensor* p , tensor* pp , tensor& out , lambda_inner_s3 f)
{
	// p and pp have same channel

	int outer_size = p->get_batch_count() * p->get_channel_count();

	int inner_chn_size = pp->get_channel_count();

	int inner_size = p->get_col_count() * p->get_row_count();

	elemt* tp = p->get_p();
	elemt* tpp = pp->get_p();
	elemt* op = out.get_p();

	shape s0 = p->get_shape();
	shape s1 = pp->get_shape();
	shape s2 = out.get_shape();

	for(int y = 0; y < p->get_batch_count() ; y++)
	for (int i = 0 ; i < pp->get_batch_count() ; i++)
	{
		tp = p->get_p() + ( p->get_batch_offset() * y );
		tpp = pp->get_p() + (pp->get_batch_offset() * i);

		for (int k = 0; k < inner_chn_size; k++)
		{

			f(tp, tpp, op, s0, s1, s2);

			tp += p->get_channel_offset();
			tpp += pp->get_channel_offset();

		}

		op += out.get_channel_offset();

	}



}

void a(lambda_base_p3 f, const shape&, const shape&, const shape&)
{



}

void loop::loop_tensor_with_same_chn_count(tensor* t, tensor* tt, tensor& out,int cols , int rows, lambda_inner_s3_with_stride f)
{
	elemt* p = t->get_p();
	elemt* pp = tt->get_p();
	elemt* op = out.get_p();

	for (int i = 0; i < B(t) * D(t); i++)
	{
		f(p, pp, op, t->get_shape(), tt->get_shape(), out.get_shape(), 1, 1);

		p += D_OFF(t);
		pp += D_OFF(tt);

		op += D_OFF((&out));
	}

}


