#include "gradient.h"

#include "..\tensor_ops\tensor_callback.h"
#include "..\tensor_ops\tensor_loop.h"

#include "..\macro\macro.h"


using namespace mdls;
using namespace grad;

void grad::numerical_gradient_softmax(tensor* in, tensor& out)
{
	shape is = in->get_shape();
	out.reshape(is);

	gradient_operation_by_row(in, out, callback::softmax_row);
}

void grad::gradient_operation_by_row(tensor* in, tensor& out, TASK_EE_INT TASK)
{
	tensor rin(*in);
	tensor rcin(*in);
	elemt h = mdef::h;

	tensor rout(out);

	elemt* tp = new elemt[ in->get_alloc_size() ];

	elemt* rinp = rin.get_p();
	elemt* inp = in->get_p();

	elemt* routp = rout.get_p();
	elemt* outp = out.get_p();
	elemt* rcinp = rcin.get_p();

	for (int k = 0; k < B(in) * D(in) * C(in); k++)
	{
		for (int i = 0; i < R(in); i++)
		{
			*(rinp + i) -= mdef::h;
			TASK(rinp, routp, R(in));

			*(outp + i) = *(routp + i);
			*(rinp + i) = *(rcinp + i);

		}

		TASK(rinp, tp, R(in));

		macro::minus_in_range(tp, outp, outp, R(in));
		macro::devide_in_range(outp, mdef::h, R(in));

		rinp += C_OFF(in);
		routp += C_OFF(in);
		rcinp += +C_OFF(in);

		inp += C_OFF(in);
		outp += C_OFF(in);
	}

}


