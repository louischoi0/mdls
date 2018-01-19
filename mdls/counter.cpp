#include "counter_extens.h"
#include "macro.h"

using namespace mdls;

#define	LOG_LEVEL 0

counter::counter( const shape& s0, const shape& s1, const shape& s2) :
	use_num(3),
	loop_count(0)
{
	shape s_arr_[3] = { s0 , s1  , s2 };
	initialize(3 , s_arr_);

	off_dm = new target_dm*[2];
	init_dm = new target_dm*[2];

	off_dm[0] = new target_dm[4]{ Batch , Channel , Col , Row };
	off_dm[1] = new target_dm[4]{ Batch , Channel , Col , Row };

	init_dm[0] = new target_dm[4]{ Batch , Channel , Col , Row };
	init_dm[1] = new target_dm[4]{ Batch , Channel , Col , Row };

}


counter::counter(const shape& in, const shape& out) :
	use_num(2),
	loop_count(0)
{
	shape s_arr_[2] = { in , out };
	initialize(2, s_arr_);

	ct_arr = new int[2];
	off_dm = new target_dm*[2];
	init_dm = new target_dm*[2];

	off_dm[0] = new target_dm[4]{Batch , Channel , Col , Row};
	off_dm[1] = new target_dm[4]{ Batch , Channel , Col , Row };

	init_dm[0] = new target_dm[4]{ Batch , Channel , Col , Row };
	init_dm[1] = new target_dm[4]{ Batch , Channel , Col , Row };

}

counter::counter(const shape& s) :
	count(0),
	loop_count(0)
{
	count = new int[2]{ 0,0 };

	_s = shape(s);
	_o = offset(s);
};

counter::counter() :
	count(0)
{

};

void counter::inc_count(int i)
{
	if (!counting)
	{
		loop_count++;
		counting = false;
	}

	count[i]++;
}

void counter::inc_count_f()
{
	if (!counting)
	{
		loop_count++;
		counting = false;
	}

	(count[0])++;
};

void counter::init()
{
	_c = shape();
	_o = offset(_c);
}

int counter::get_offset(int i)
{
	return count[i];
}




int counter::get_offset()
{
	int v = 0;

	v += (*_c.p[3]) * (*_o.p[3]);
	v += (*_c.p[2]) * (*_o.p[2]);
	v += (*_c.p[1]) * (*_o.p[1]);
	v += (*_c.p[0]) * (*_o.p[0]);

	return v;

}
void counter::set_shape(int index, const shape& s)
{
	s_arr[index] = s;
	o_arr[index] = offset(s);
}

void counter::init(size_t t)
{
	s_arr = new shape[t];
	c_arr = new shape[t];

	o_arr = new offset[t];
	ct_arr = new int[t];

}

void counter::set_offdm(int index, target_dm n0, target_dm n1, target_dm n2, target_dm n3 )
{
	off_dm[index] = new target_dm[4]{ n0 , n1 , n2 , n3 };

}

void counter::inc_loop(target_dm n)
{

	for (int i = 0; i < use_num; i++)
	{
		int init_t_dm = init_dm[i][n];

		c_arr[i].p[n] += 1;

		if (init_t_dm != DM_NULL)
			c_arr[i].p[init_t_dm] = 0;
	}


	loop_count++;
}



void counter::inc_count_all()
{
	if (!counting)
	{
		loop_count++;
		counting = true;
	}

	for (int i = 0; i < use_num; i++)
		inc_count(i);

}


void counter::get_counted_shape(int i, shape& s)
{
	if (i >= use_num)
		return;

	int c = count[i];

	for (int j = 0; j < 4; j++)
	{
		*s.p[j] = c / *o_arr[i].p[j];
		c -= *s.p[j] * *o_arr[i].p[j];
	}

	s = shape(s);
}

shape counter::get_orginal_shape(int i)
{
	return s_arr[i];
}

void counter::initialize(int size, shape* s_arr_)
{
	use_num = size;
	count = new int[size];

	c_arr = new shape[use_num];
	s_arr = new shape[use_num];
	o_arr = new offset[use_num];

	for (int i = 0; i < use_num; i++)
	{
		count[i] = 0;
		s_arr[i] = s_arr_[i];
		c_arr[i] = shape();
		o_arr[i] = s_arr_[i];
	}

}

void counter::initialize_log(int size, int local_size)
{
	_log_arr = new int*[size];
	
	int** pl = _log_arr;

	for (int i = 0; i < size; i++)
		*pl++ =(int*) malloc( sizeof(int) * local_size);

}

void counter::write_log(int indx, int lindx, int val)
{
	_log_arr[indx][lindx] = val;
}


void counter::init_count(int i, target_dm d)
{
	shape s = shape();

	get_counted_shape(i, s);

	*s.p[d] = 0;

	count[i] = 0;

	for (int j = 0; j < 4; j++)
	{
		if (j != d)
			count[i] += *o_arr[i].p[j] * *s.p[j];
	}

}

void counter::inc_count(int i, int str ,target_dm d)
{
	count[i] += *o_arr[i].p[d] * str;

}