#include "convolution.h"
#include "of.h"
#include "padding.h"

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
using namespace mdls;

int convolution::count = 0;

convolution::convolution(tensor* input, tensor* eqiv) :
layer(CONVOLUTION, input, eqiv)
{
	eqiv_converted_by_im2col = new tensor();
	input_converted_by_im2col = new tensor();

	set_type_name("Convolution");
	pstatic_count = &convolution::count;
	inc_layer_count();
	index_eqaul_layers = *pstatic_count;

}

convolution::convolution() :
	layer(CONVOLUTION)
{
	set_type_name("Convolution");
	pstatic_count = &convolution::count;
	inc_layer_count();

	index_eqaul_layers = *pstatic_count;
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

void convolution::get_grad_weight(tensor* bgrad , tensor* in , tensor* filter , tensor& out)
{

}

void convolution::proceed_inverse()
{


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

