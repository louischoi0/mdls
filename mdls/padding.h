#pragma once
#include "of.h"
#include "tensor.h"
namespace mdls
{

	namespace padding
	{

		void add_padding_one_side(tensor& p, Dm n, bool right, int size);
		void add_padding(tensor& p, Dm n, int size);
		void get_rid_of_pad(tensor& p, Dm n, int size);	

	}

}

