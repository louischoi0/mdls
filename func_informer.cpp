#include "func_infromer.h"

using namespace detail;

bool func_informer::instflag = false;
func_informer* func_informer::instance = 0;
int* func_informer::varr_size = new int[10];
bool func_informer::val_arr_init = false;

void func_informer::set_value_arr_size(int idx, int len)
{
	if (!func_informer::val_arr_init)
	{
		value_arr = new int*[10];
		func_informer::val_arr_init = true;
	}
		
	value_arr[idx] = new int[len];
}

func_informer* func_informer::get_instance()
{
	if (!instflag)
	{
		func_informer::instance = new func_informer();
		instflag = true;
	}
		
	return func_informer::instance;

}

void func_informer::inc_func_active_count()
{
	(*func_informer::pcount)++;
}

void func_informer::set_activate_pointer(int i)
{
	if (i == 0)
	{
		pcount = &pooling;
	}
}

void func_informer::initialize_count()
{
	*pcount = 0;
}

void func_informer::initialize_arr()
{
	for (int i = 0; i < use; i++)
		delete[] value_arr[i];

	use = 0;
}