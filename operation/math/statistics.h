#pragma once
#include "..\tensor_ops\of.h"
#include "..\..\objects\tensor.h"

using namespace mdls;
using namespace callback;

namespace statistics
{
	void everage(element* m, element& out);
	void teverage(tensor* p, tensor& out);
	void dispersion(element* m, element& out);
	void tdispersion(tensor* p, tensor& out);
}

