#pragma once
#include "matrix_extended.h"
#include "of.h"
#include "tensor_callback.h"
using namespace mathm;
using namespace callback;

namespace statistics
{
	void everage(matrix* m, matrix& out);
	void teverage(tensor* p, tensor& out);
	void dispersion(matrix* m, matrix& out);
	void tdispersion(tensor* p, tensor& out);
}

