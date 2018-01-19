#include "macro.h"


int macro::get_dv_size(int g_size, int w_size, int stride)
{
	return ( ( g_size - w_size ) / stride ) + 1;
}

int macro::get_g_size(int dv_size, int w_size, int stride)
{
	return ((dv_size - 1) * stride) + w_size;
}

void macro::arr_opt_multiply(int* p, int** out, int m, int size)
{
	int* copi = *out;

	for (int i = 0; i < size; i++)
		copi[i] = p[i] * m;

}

void macro::arr_opt_add(int* p, int* pp , int** out, int size)
{
	int* copi = *out;

	for (int i = 0; i < size; i++)
		copi[i] = p[i] +  pp[i];
}

elemt macro::get_max_in_range(elemt* p, int size)
{
	elemt* lp = p;
	elemt maxc = 0;

	for (int i = 0; i < size; i++)
	{
		if (i == 0)
		{
			maxc = *lp;
			continue;
		}

		if (maxc < *(lp + i))
		{
			maxc = *(lp + i);
		}
	}

	return maxc;

}

void macro::max_in_range_other_to_zero(elemt* p, elemt* pp, int size)
{
	elemt* lp = p;
	elemt* lpp = pp;

	elemt maxc = 0;
	int idx = 0;

	for (int i = 0; i < size; i++)
	{
		if (i == 0)
		{
			maxc = *lp;
			continue;
		}

		if (maxc < *(lp + i))
		{
			maxc = *(lp + i);
			idx = i;
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (i == idx)
		{
			*(lpp + i) = maxc;
		}

		else
			*(lpp + i) = 0;
	}
}


void macro::minus_in_range(elemt* p, elemt* pp, elemt* out, int range)
{
	for (int i = 0; i < range; i++)
		*(out + i) = *(p + i) - *( pp + i );

}

void macro::minus_in_range(elemt* p, elemt v, elemt* out, int range)
{
	for (int i = 0; i < range; i++)
		*(out + i) = *(p + i) - v;

}

void macro::loop_elem(elemt * p, ONE_ELEM CC, int range)
{
	for (int i = 0; i < range; i++)
	{
		CC(p++);
	}

}


elemt macro::get_sum_in_range(elemt* p, int range)
{
	elemt v = 0;

	for (int i = 0; i < range; i++)
		v += *(p + i);

	return v;

}


void macro::devide_in_range(elemt* p, elemt v, int range)
{
	if (v == 0)
		return;

	for (int i = 0; i < range; i++)
		*(p + i) = (elemt) (* ( p +i) / v);

}

void macro::fill_args_pointer(elemt**& arg, int i , elemt* p)
{
	arg = new elemt*[i];

	for (int j = 0; j < i; j++)
	{
		arg[j] = p;
	}
}



void macro::multiply_in_range(elemt* p, elemt v, int range)
{
	for (int i = 0; i < range; i++)
		*p++ *= v;
}

void macro::add_in_range(elemt* p, elemt v, int range)
{
		*p++ += v;

}

