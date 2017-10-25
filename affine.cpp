#include "affine.h"
#include "of.h"
using namespace mdls;

int affine::count = 0;


affine::affine() :
	layer(layer_extended_enum::AFFINE)
{
	pstatic_count = &affine::count;
	set_type_name("Affine");

	use_eqiv = true;
	inc_layer_count();

}

affine::affine(tensor* i, tensor* w) :
	layer(layer_extended_enum::AFFINE, i, w)
{
	pstatic_count = &affine::count;
	set_type_name("Affine");

	use_eqiv = true;
	inc_layer_count();

}


affine::affine(tensor* input) :
layer(layer_extended_enum::AFFINE, input)
{
	pstatic_count = &affine::count;
	set_type_name("Affine");

	use_eqiv = true;
	inc_layer_count();

}

affine::affine(tensor* input, tensor* eqiv, tensor* bias) :
layer(layer_extended_enum::AFFINE, input , eqiv )
{
	pstatic_count = &affine::count;
	set_type_name("affine");
	inc_layer_count();

}



void affine::initialize_inverse()
{

}

void affine::proceed()
{
	shape s0 = _tensor_map[Input]->get_shape();
	shape s1 = _tensor_map[Weight]->get_shape();

	_tensor_map[Output]->reshape(shape(s0.Batch , s1.Batch , s0.Col , s1.Row));

	tfunc::tdot_acm(_tensor_map[Input], _tensor_map[Weight], *_tensor_map[Output]);
}

void mdls::callback_for_inversed_of_affine( matrix* backward , matrix* input , matrix& out )
{
	matrix m = *input;
	transpose(input, m);
	out.resize( backward->get_row_count() , input->get_row_count() );
	mathm::dot(&m, backward, out);

}

void affine::proceed_inverse()
{
	grad_input_by_weight = new tensor();
	grad_weight_by_input = new tensor();
	
	tensor bgrad;
	tensor ev_w = *_tensor_map[Weight];
	tensor ev_i = *_tensor_map[Input];
	tensor bgradtmp;

	tfunc::compact_batch_evrg(ev_w , ev_w.get_batch_count() );
	tfunc::compact_batch_evrg(ev_i, ev_i.get_batch_count()  );

	ev_w.transpose(n0, n1, n3, n2);
	ev_i.transpose(n0, n1, n3, n2);

	if (!end_layer)
		bgrad = *_backward_lay->get_tensor(Grad_In);

	else
	{
		shape s( B(_tensor_map[Input]) * B(_tensor_map[Weight]), D(_tensor_map[Input]), C(_tensor_map[Input]), R(_tensor_map[Weight]));
		bgrad = tensor(s);
		bgrad.initialize_elem_to(1);
	}

	bgradtmp = bgrad;

	get_weight
	( bgrad , *_tensor_map[Input], *_tensor_map[Weight], *grad_input_by_weight, *grad_weight_by_input);

	_tensor_map[Grad_In]->reshape(_tensor_map[Input]->get_shape());
	_tensor_map[Grad_Wgt]->reshape(_tensor_map[Weight]->get_shape());

	tfunc::compact_batch_evrg(bgrad , _tensor_map[Weight]->get_batch_count() );
	tfunc::compact_batch_evrg(bgradtmp, _tensor_map[Input]->get_batch_count());

	loop::loop_tns_f_dot_inc_batch(&bgrad, &ev_w, _tensor_map[Grad_In]->get_p(), callback::tdot);
	loop::loop_tns_f_dot_inc_batch(&ev_i, &bgradtmp,  _tensor_map[Grad_Wgt]->get_p(), callback::tdot);

}

bool affine::proc_possible()
{
	if (!layer::proc_possible())
		return false;

	shape s0 = _tensor_map[Input]->get_shape();
	shape s1 = _tensor_map[Weight]->get_shape();

	if (s0.Channel == s1.Channel)
	{
		if (s0.Row == s1.Col)
			return true;
	}
	
	else
	{
		_tensor_map[Weight]->reshape(shape(1, s0.Channel, s0.Row, 4));
		_tensor_map[Weight]->initialize_elem_to(1);
		return true;
	}


}

void affine::get_weight(const tensor& bgrad, const tensor& in, const tensor& filter, tensor& iout, tensor& wout)
{
	tensor intmp = in;
	tensor filtmp = filter;
	tensor bgradtmp = bgrad;

	shape is = in.get_shape();
	shape ws = filter.get_shape();

	tfunc::batch_times(intmp, filter.get_batch_count() );
	tfunc::batch_repeat(filtmp, in.get_batch_count());

	is.Batch *= ws.Batch;
	ws.Batch *= is.Batch;
	
	is = shape(is);
	ws = shape(ws);

	iout.reshape(is);
	wout.reshape(ws);

	filtmp.transpose(n0, n1, n3, n2);
	intmp.transpose(n0, n1, n3, n2);

	loop::loop_tns_f_dot_linear_chn(&intmp, &bgradtmp, wout.get_p(), callback::tdot);
	loop::loop_tns_f_dot_linear_chn(&bgradtmp, &filtmp, iout.get_p(), callback::tdot);


} 
