#pragma once

#include "tensor.h"
#include "macro.h"

using namespace mdls;

namespace grad
{

	typedef void (*TASK_EE_INT)(elemt*, elemt*, int size);

	void numerical_gradient_softmax(tensor* in, tensor& out);
	void gradient_operation_by_row(tensor* in, tensor& out, TASK_EE_INT TASK);


}