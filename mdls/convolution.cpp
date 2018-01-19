#include "convolution.h"
#include "of.h"
#include "padding.h"
using namespace mdls;

int convolution::count = 0;
void bbb(elemt* p, elemt* pp, elemt* ppp, const shape& s0, const shape& s1, const shape& s2)
{
	for (int i = 0; i < s1.Col; i++)
	{
		for (int j = 0; j < s1.Row; j++)
		{
			*(pp + j) += *(p+j);

		}

		pp += s1.Row;
		p += s0.Row;
	}
}

void ccc(elemt* p, elemt* pp, elemt* ppp, const shape& s0, const shape& s1, const shape& s2)
{
	for (int i = 0; i < s1.Col; i++)
	{
		for (int j = 0; j < s1.Row; j++)
		{
			*(ppp + j) += *(p);
		}

		ppp += s2.Row;

	}
}


convolution::convolution(tensor* input, tensor* eqiv) :
layer(CONVOLUTION, input, eqiv)
{
	eqiv_converted_by_im2col = new tensor();
	input_converted_by_im2col = new tensor();
	
	pstatic_count = &convolution::count;
	index_eqaul_layers = *pstatic_count;

	set_type_name("Convolution");

	use_eqiv = true;
	inc_layer_count();

}

convolution::convolution() :
	layer(CONVOLUTION)
{
		
	eqiv_converted_by_im2col = new tensor();
	input_converted_by_im2col = new tensor();

	pstatic_count = &convolution::count;
	index_eqaul_layers = *pstatic_count;

	set_type_name("Convolution");

	use_eqiv = true;
	inc_layer_count();
}
void convolution::ready_for_process()
{

}

void convolution::proceed()
{
	if(!init)
		initialize();

	shape os = _tensor_map[Weight]->get_shape();

	int tdvr = macro::get_dv_size(_tensor_map[Input]->get_row_count(), os.Row, _inf.stride_row);
	int tdvc = macro::get_dv_size(_tensor_map[Input]->get_col_count(), os.Col, _inf.stride_col);

	tfunc::im2_col(_tensor_map[Input], *input_converted_by_im2col , _inf.stride_row , _inf.stride_col, os.Row , os.Col );
	tfunc::im2_col(_tensor_map[Weight], *eqiv_converted_by_im2col , 1 , 1 , os.Row , os.Col );
	
	eqiv_converted_by_im2col->transpose(n0, n1, n3, n2);
	
	tfunc::edot(input_converted_by_im2col, eqiv_converted_by_im2col, *_tensor_map[Output]);

	shape as( B(_tensor_map[Input]), B(_tensor_map[Weight]) ,tdvc, tdvr);

	_tensor_map[Output]->reshape(as);

}

void convolution::initialize()
{
	int erow = _tensor_map[Weight]->get_row_count();
	int ecol = _tensor_map[Weight]->get_col_count();

	int tdvr = macro::get_dv_size(_tensor_map[Input]->get_row_count(), erow, _inf.stride_row);
	int tdvc = macro::get_dv_size(_tensor_map[Input]->get_col_count(), ecol, _inf.stride_col);

	shape as(_tensor_map[Input]->get_batch_count(), _tensor_map[Weight]->get_batch_count(),
		tdvc, tdvr);

	input_converted_by_im2col = new tensor();
	eqiv_converted_by_im2col = new tensor();

	_tensor_map[Output] = new tensor(as);


	init = true;
}
void convolution::get_grad_input(const tensor* bgrad, const tensor* in, const tensor* filter, tensor& out)
{
	tensor fr = *filter;
	tensor pbg = *bgrad;

	shape s0 = bgrad->get_shape();
	shape s1 = fr.get_shape();
	shape s2 = in->get_shape();
	
	fr.rotate(Rotate::D_180);
	tfunc::batch_repeat(fr, B(in));

	padding::add_padding(pbg , Row , s1.Row - 1);
	padding::add_padding(pbg , Col , s1.Col -1);

	elemt* fp = fr.get_p();
	elemt* fpp = fp;
	elemt* pbp = pbg.get_p();
	
	out.reshape(shape( s2.Batch * s1.Batch , s2.Channel , s2.Col , s2.Row));
	out.initialize_elem_to(0);
	
	elemt* op = out.get_p();

	for (int i = 0; i < B(filter) * B(in); i++)
	{
		loop::loop_tensor_with_one_chn_repeat_inc_outc(pbp, fp, op, pbg.get_shape(), s1 , s2 ,_inf.stride_col , _inf.stride_row,
			[](elemt* p, elemt* pp, elemt* op, const shape& s0, const shape& s1, const shape& s2 , int cols , int rows)->void
		{
			elemt** bp;
			elemt** fp;
			elemt** of;

			int dvr = macro::get_dv_size(s0.Row , s1.Row , rows);
			int dvc = macro::get_dv_size(s0.Col, s1.Col, cols);

			macro::fill_args_pointer(bp, 4, p);
			macro::fill_args_pointer(fp, 3, pp);
			macro::fill_args_pointer(of, 3, op);

			for (int i = 0; i < dvc; i++)
			{
				bp[2] = bp[3];
				for (int k = 0; k <  dvr;  k++)
				{
					fp[0] = fp[1];
					bp[1] = bp[2];

					for (int y = 0; y < s1.Col; y++)
					{
						bp[0] = bp[1];
						for (int z = 0; z < s1.Row; z++)
						{

							*of[0] += *bp[0] * *fp[0];

							fp[0]++;
							bp[0]++;
						}

						bp[1] += s0.Row;
					}

					of[0] ++;
					bp[2] += rows;
				}

				bp[3] += s0.Row * cols;
			}


		});

		fp += B_OFF((&fr));
		pbp += D_OFF((&pbg));
		op += B_OFF((&out));

	}

	tfunc::compact_batch_accum(out, B(filter));

}


void convolution::get_grad_weight(const tensor* bgrad , const tensor* in , const tensor* filter , tensor& out)
{
	tensor intp = *in;
	tensor intpfw = *in;

	intpfw.initialize_elem_to(0);

	tensor tbg = *bgrad;
	tensor wtt = *filter;

	elemt** ip;
	elemt** ipfw;
	elemt** bgp;

	shape s0 = bgrad->get_shape();
	shape s1 = in->get_shape();
	shape s2 = filter->get_shape();

	out.reshape(s2);
	out.initialize_elem_to(0);

	tfunc::batch_repeat(intpfw , B(filter));

	macro::fill_args_pointer(ip , 5 , intp.get_p());
	macro::fill_args_pointer(ipfw, 5 , intpfw.get_p());
	macro::fill_args_pointer(bgp , 5 , tbg.get_p() );
	int s = 0;

	for (int k = 0; k < B(filter); k++)
	{
		for (int i = 0; i < B(in); i++, s++)
		{
			bgp[0] = bgp[4] + (i * B_OFF(bgrad)) +  (k * D_OFF(bgrad) );
			ipfw[0] = ipfw[4] + (s * B_OFF(in));
			ip[0] =  ip[4] + (i * B_OFF(in));

			loop::loop_tensor_with_one_chn_repeat_inc_outc_fs( bgp[0] ,ip[0] , ipfw[0] , s0, s1 ,s1, s2, _inf.stride_col, _inf.stride_row,
				[](elemt* p, elemt* pp, elemt* op, const shape& s0, const shape& s1, const shape& s11, const shape& s2 , int cols , int rows)->void
			{
				elemt** bp = new elemt*[5]{ p , p , p , p , p };
				elemt** ip = new elemt*[5]{ pp , pp , pp ,pp , pp };
				elemt** ipfw = new elemt*[5]{ op , op , op , op , op };

				for (int i = 0; i < s0.Col; i++)
				{
					ipfw[2] = ipfw[3];
					ip[2] = ip[3];

					for (int j = 0; j < s0.Row; j++)
					{
						ipfw[1] = ipfw[2];
						ip[1] = ip[2];

						for (int k = 0; k < s2.Col; k++)
						{
							ipfw[0] = ipfw[1];
							ip[0] = ip[1];

							for (int y = 0; y < s2.Row; y++)
							{
								*ipfw[0] += *ip[0] * *bp[0];

								ipfw[0]++;
								ip[0]++;
							}

							ipfw[1] += s1.Row;
							ip[1] += s1.Row;

						}

						bp[0]++;
						ipfw[2] += rows;
						ip[2] += rows;
					
					}

					
					ipfw[3] += cols * s1.Row;
					ip[3] += cols * s1.Row;
				
				}
			});

		}

	}


	tfunc::compact_batch_accum(intpfw, B(in));

	loop::loop_tensor_with_same_chn_count(&intpfw ,&wtt , out , _inf.stride_col , _inf.stride_row , 
		[](elemt* p , elemt* pp , elemt* op , const shape& s0 , const shape& s1 , const shape& s2 , int cols , int rows)->void
	{
		elemt** ip;
		elemt** wo;

		macro::fill_args_pointer(ip, 4, p);
		macro::fill_args_pointer(wo, 3, op);

		for (int i = 0; i < s0.Col; i++)
		{
			ip[2] = ip[3];
			for (int j = 0; j < s0.Row; j++)
			{
				wo[0] = wo[1];
				ip[1] = ip[2];
				for(int k = 0; k < s1.Col; k++)
				{ 
					ip[0] = ip[1];
					for (int y = 0; y < s1.Col; y++)
					{
						*wo[0] += *ip[0];
						wo[0]++;

						ip[0] ++;

					}

					ip[1] += s0.Row;
				}

				ip[2] += rows;
			}

			ip[3] += cols * s0.Row;

		}

	});


}

void convolution::proceed_inverse()
{
	tensor* bgrad = _backward_lay->get_tensor(Output);
	get_grad_input(bgrad, _tensor_map[Input], _tensor_map[Weight],*_tensor_map[Grad_In]);
	get_grad_weight(bgrad, _tensor_map[Input], _tensor_map[Weight], *_tensor_map[Grad_Wgt]);
}

void convolution::set_output_inform_from_val()
{


}


bool convolution::proc_possible()
{
	if (!layer::proc_possible())
		return false;

	shape s0 = _tensor_map[Input]->get_shape();
	shape s1 = _tensor_map[Weight]->get_shape();

	if (s0.Channel != s1.Channel)
		return false;

	if (s0.Row < s1.Row || s0.Col < s1.Col)
		return false;

	return true;
}	

