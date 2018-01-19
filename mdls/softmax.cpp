#include "softmax.h"
#include "gradient.h"

using namespace mdls;

int softmax::count = 0;

softmax::softmax() :
layer(SOFTMAX)
{
	pstatic_count = &softmax::count;

	set_type_name("Softmax");
	set_layer_inform(1, 1);

	use_eqiv = false;
	inc_layer_count();

}


softmax::softmax( tensor* input ) :
layer(SOFTMAX , input )
{
	pstatic_count = &softmax::count;

	set_type_name("Softmax");
	use_eqiv = false;
	inc_layer_count();

}


void softmax::proceed_inverse()
{
	grad::numerical_gradient_softmax(_tensor_map[Input], *_tensor_map[Grad_In]);

}

void softmax::proceed()
{
	tfunc::softmax_s(_tensor_map[Input], *_tensor_map[Output]);


}

void softmax::initialize(int i)
{


}