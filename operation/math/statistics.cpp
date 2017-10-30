#include "statistics.h"
using namespace statistics;

void statistics::teverage(tensor* p, tensor& out)
{
	shape s0 = p->get_shape();
	
	int tr = s0.Row * s0.Col;

	out.reshape(s0);

	tensor ev = *p;
	tensor dvd = tensor(s0);

	shape s1(s0.Batch, s0.Channel, s0.Col  * s0.Row ,1);

	ev.reshape(s1);

	dvd.initialize_elem_to(tr);

	tfunc::compact_col(&ev, s0.Col  * s0.Row );
	tfunc::colmn_times(ev, s0.Row * s0.Col);

	ev.reshape(s0);

	loop::tensor_loop(&ev, &dvd, out, 1, 1, callback::c_tdevide);
	

};

void statistics::tdispersion(tensor* t, tensor& out)
{
	shape s0 = t->get_shape();
	out.reshape(s0);
	
	tensor ev = tensor(s0);
	tensor bsev = tensor(s0);
	
	teverage(t, ev);

	loop::tensor_loop(t , &ev, bsev, 1, 1, callback::c_tsub);
	loop::tensor_loop(&bsev, &bsev, bsev, 1, 1, callback::c_tmultiply);
	
	statistics::teverage( &bsev , out);

};

void tdeverage(tensor* t, tensor& tt , target_dm Dm)
{




}

