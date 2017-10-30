#include "tensor.h"
#include <iostream>
#include <cstring>
using namespace mdls;

tensor::tensor() :
element()
{

}

tensor& tensor::operator=(const tensor& t)
{
	free(_pv);
	init(t._eshape);
	pv_alloc(t._eshape.Alloc_size);

	memcpy( _pv, t._pv, sizeof(elemt) * t._eshape.Alloc_size);


	return *this;
}

tensor::tensor(int n0, int n1, int n2, int n3) :
	element	(n0, n1, n2, n3)
{



}

tensor::tensor(const shape& s) :
	element()
{
	init(s);
	pv_alloc(s.Alloc_size);
}

tensor::tensor(const tensor& t) :
	element(t)
{

}
