#include "padding.h"

using namespace padding;
void padding::add_padding_one_side(tensor& p , Dm n ,bool right, int size)
{
	shape ns(p.get_shape());

	if (!(n == Row || n == Col))
	{
		return;
	}

	(*ns.p[n]) += size;

	shape nns(ns);
	tensor k(nns);

	if (right)
		loop::tensor_loop(&p, &k, 1, 1, callback::right_edge_to_zero);

	else
		loop::tensor_loop(&p, &k, 1, 1, callback::left_edge_to_zero);

	p = k;
}

void padding::add_padding(tensor& p, Dm n, int size)
{
	shape ns(p.get_shape());

	if (n <= n3)
		(*ns.p[n]) += size * 2;

	else if (n == target_dm::Surface)
	{
		*ns.p[Row] += size * 2;
		*ns.p[Col] += size * 2;
	}

	else if (n == target_dm::Volume)
	{
		*ns.p[Row] += size * 2;
		*ns.p[Col] += size * 2;
		*ns.p[Channel] += size * 2;
	}

	shape nns(ns);
	tensor k(nns);
	
	loop::tensor_loop(&p, &k, 1, 1, callback::edge_to_zero);
	
	p = k;
}

void padding::get_rid_of_pad(tensor& p, Dm n, int size)
{
	shape ns(p.get_shape());

	if (n <= n3)
		(*ns.p[n]) -= size * 2;

	else if (n == target_dm::Surface)
	{
		*ns.p[Row] -= size * 2;
		*ns.p[Col] -= size * 2;
	}

	else if (n == target_dm::Volume)
	{
		*ns.p[Row] -= size * 2;
		*ns.p[Col] -= size * 2;
		*ns.p[Channel] -= size * 2;
	}

	shape nns(ns);
	tensor k(nns);

	loop::tensor_loop(n , &p, &k, 1, 1, callback::copy_value_ifn_edge);

	p = k;
}


