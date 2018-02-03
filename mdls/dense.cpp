#include "Dense.h"
#include "session.h"
	
using namespace mdls;

Dense::Dense(session& s, tensor* in, tensor* weight) :
	layer(s,in,weight)
{
	ncount = weight->get_row_count();
	_tensor_map[Input] = in;
	_tensor_map[Weight] = weight;

	self = (ILayer*)this;
}

Dense::Dense(session& s, tensor* input, int out) :	
	ncount(out)
{
	_tensor_map[Input] = input;
	self = (ILayer*)this;
}

Dense::Dense(session& s, tensor* weight) :
	ncount(weight->get_shape().Alloc_size)
{
	_tensor_map[Weight] = weight;
	self = (ILayer*)this;
}
