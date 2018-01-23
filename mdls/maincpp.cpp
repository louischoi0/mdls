#define _CRTDBG_MAP_ALLOC
#include <iostream>

#include "padding.h"
#include "session.h"
#include "statistics.h"

using namespace mathm;
using namespace std;
using namespace mdls;

int test_with_explain()
{

	using namespace mdls;

	session s = session();
	convolution conv_constructed = convolution();

	tensor first_input(3, 3, 32, 32);

	tensor conv0_kernel(4, 3, 2, 2);
	tensor conv1_kernel(5, 4, 2, 2);
	tensor pool_kerenl(1, 1, 4, 4);

	tfunc::init_tensor_with_random(first_input, 0, 1);
	//tensor have value by beeing randomized.

	tfunc::init_tensor_with_random(conv0_kernel, 0, 1);
	tfunc::init_tensor_with_random(conv1_kernel, 0, 1);

	// You can set session layout by passing enum of layer types, or pointer to layer constructed.

	//s.add_layer(BATCH_NORMALIZATION); // 30 30 3
	s.add_layer(CONVOLUTION); // 2 2 3 -> 16 16 W
	//s.add_layer(RELU); // 16 16 W
	//s.add_layer(POOLING); // 4 4 W -> 4 4 W
	//s.add_layer(&conv_constructed); //  2 2 -> 2 2 ->SERIALZIE
	s.add_layer(SOFTMAX); // SOFTMAX

	s.preprocessing();
	// Preprocessing func construct layers and index to each layer.
	// So this must be called before set weight tensor or layer inform( stride , padding , etc).

	// Convolution , Affine , Pooling layer must have weight tensor.
	// So set weight tensor to each layer.


	s.set_input_tensor(&first_input);
	// Set_input_tensor func of session set input tensor to first layer (in this case , convolution 0 layer).
	s.set_layer_weight_tensor(CONVOLUTION, 0, &conv0_kernel);
	// So does it weight tensor.


	// Next layer ( Softmax 0 ) take Row as calculate unit.
	// I would like to transpose output tensor by surface( Row and Col ).

	s.run();

	layer* l0 = s.get_layer(CONVOLUTION , 0);


	tfunc::print_object(l0->get_tensor(Grad_In));
	tfunc::print_object(l0->get_tensor(Grad_Wgt));


	conv0_kernel.rotate(Rotate::D_90);
	conv0_kernel.rotate(Rotate::D_180);	return 0;

}

int main()
{
	tensor t0(1, 2, 4, 4);
	tensor t1(1, 2, 4, 4);
	tensor out(1, 2, 4, 4);
	tfunc::init_tensor_ladder(t0);
	t1.initialize_elem_to(1);


	affine af(&t1,&t0);


	elemt* ep = t0.get_p();
	elemt* epp = t1.get_p();


	out.apply_to_all_elem([&ep, &epp](elemt* p)
	{
		*p = *ep++ + *epp++;
	});

	tfunc::print_object(&out);
	int i = 1;




//	test_with_explain();
}