#include "statistics.h"
#include "batch_norm.h"

using namespace mdls;
int batch_normalization::count = 0;

batch_normalization::batch_normalization() :
	layer(BATCH_NORMALIZATION)

{
	set_type_name("Batch Normalization");
}

batch_normalization::batch_normalization(tensor* input) :
	layer(BATCH_NORMALIZATION)
{
	set_type_name("Batch Normalization");
}
batch_normalization::batch_normalization(tensor* input, tensor* eqiv, tensor* bias) :
	layer(BATCH_NORMALIZATION)
{
	set_type_name("Batch Normalization");
}


void batch_normalization::proceed()
{
	initialize();
	shape s0 = _tensor_map[Input]->get_shape();

	offset o0 = offset(s0);
	
	_tensor_map[Output]->reshape(s0);

	tensor d = tensor(s0);

	shape s1(s0.Batch, s0.Batch, 1, s0.Row * s0.Col);

	statistics::teverage(_tensor_map[Input], everage);
	statistics::tdispersion(_tensor_map[Input], dispersion);

	loop::tensor_loop(_tensor_map[Input], &everage, d , 1, 1, callback::c_tsub);
	
	tensor disptem = dispersion;

	elemt* p = disptem.get_p();
	elemt* pp = d.get_p();

	elemt* gp = gamma.get_p();
	elemt* bp = beta.get_p();

	macro::add_in_range(p, mdef::h, disptem.get_alloc_size());
	
	loop::tensor_loop(&disptem, callback::c_sqrt, 1, 1);	

	for (int i = 0; i < d.get_batch_count() * d.get_channel_count(); i++)
	{
		macro::devide_in_range(pp, *p, d.get_row_count() * d.get_col_count() );
		pp += d.get_channel_offset();
	}

	x_hat = d;

	for (int i = 0; i < d.get_batch_count() * d.get_channel_count(); i++)
	{
		macro::multiply_in_range(pp, *gp, d.get_row_count() * d.get_col_count());
		macro::add_in_range(pp, *bp, d.get_row_count() * d.get_col_count());

		bp++;
		gp++;

		pp += d.get_channel_offset();
	}

	*_tensor_map[Output] = d;
}

void batch_normalization::initialize()
{
	if (!init)
	{
		shape s0 = _tensor_map[Input]->get_shape();
		_tensor_map[Weight] = new tensor(s0);

		beta = tensor(1, 1, B(_tensor_map[Input]), D(_tensor_map[Input]));
		gamma = tensor(1, 1, B(_tensor_map[Input]), D(_tensor_map[Input]));

		beta.initialize_elem_to(0);
		tfunc::init_tensor_with_random(gamma, 0, 1);

		dispersion = tensor(1, 1, B(_tensor_map[Input]), D(_tensor_map[Input]));
		everage = tensor(1, 1, B(_tensor_map[Input]), D(_tensor_map[Input]));

		surface_size = s0.Col * s0.Row;

		init = true;
	}
}

void batch_normalization::proceed_inverse()
{
	tensor e2 = tensor();
	tensor e1 = tensor();

	tensor dx_hat_sum_by_chn = tensor();
	tensor mt_dx_hat_x_hat;

	tensor dx_hat;
	tensor otns;

	shape s0 = _tensor_map[Input]->get_shape();
	elemt* p = gamma.get_p();
	
	if (end_layer)
	{
		dx_hat = tensor(s0);
		dx_hat.initialize_elem_to(1);
	}

	else
		dx_hat = *_backward_lay->get_tensor(Grad_In);

	dx_hat.apply_to_elem_by_channel(eeTask_lambda{
		*p_0 = *p_1;
	}, p);

	otns = dx_hat;
	int surface_size_loc = surface_size;

	otns.apply_to_all_elem([surface_size_loc](elemt* p )->void{
		*p_0 *= surface_size_loc;
	});

	tfunc::sum_tensor(Surface, &dx_hat, dx_hat_sum_by_chn);
	tfunc::tmultiply( &dx_hat, &x_hat , mt_dx_hat_x_hat );
	
	tfunc::sum_tensor(Surface , &mt_dx_hat_x_hat, e2);
	tfunc::tmultiply(&e2 , &x_hat, e2);


	tfunc::sum_tensor(Surface, &dx_hat, e1);

	p = e1.get_p();

	otns.apply_to_elem_by_channel([](elemt* p, elemt* pp)->void
	{
		*p -= *pp;
	}, p);

	p = e2.get_p();
	
	otns.apply_with_equal_form([](elemt* p , elemt* pp)->void
	{
		*p -= *pp;
	}, e2);

	p = dispersion.get_p();

	otns.apply_to_elem_by_channel([surface_size_loc](elemt* p, elemt* pp)->void
	{
		*p /= *pp * surface_size_loc;

	}, p);

	*_tensor_map[Grad_In] = otns;

}
