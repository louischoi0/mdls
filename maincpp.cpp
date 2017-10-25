#define _CRTDBG_MAP_ALLOC
#include <iostream>

#include "padding.h"
#include "session.h"
#include "statistics.h"

using namespace mathm;
using namespace std;
using namespace mdls;

int main()
{

	tensor k(1, 2, 8, 6);
	tensor kk(2, 2, 2, 2);

	tensor a1( 2, 2, 6 , 5);
	tensor a2( 3, 2, 5 , 7);

	tensor dot( 2, 3, 6, 7);
	
	tensor in_out = tensor();
	tensor weight_out = tensor();

	dot.initialize_elem_to(1);

//	tfunc::init_tensor_ladder(a1);
//	tfunc::init_tensor_ladder(a2);

	a1.initialize_elem_to(1);
	a2.initialize_elem_to(1);

	tfunc::af_inv_acm_chn(&dot , &a1 , &a2 , in_out , weight_out);
	
	tfunc::print_object(&weight_out);

	tensor aw(1, 1,  3 , 4);

	batch_normalization bt = batch_normalization();

	tensor tt(1,1, 2,2);
	tensor k1 (2, 1, 2, 2);

	k1.initialize_elem_to(0.1);
	kk.initialize_elem_to(0.1);

	tfunc::init_tensor_ladder(k);
	aw.initialize_elem_to(0.5);

	session s = session();
	convolution conv_constructed = convolution();

	// You can set session layout by passing enum of layer types, or pointer to layer constructed.

	s.add_layer(&bt);
	s.add_layer(CONVOLUTION); // 4 3
	s.add_layer(BATCH_NORMALIZATION);
	s.add_layer(AFFINE); // 8 , 4
//	s.add_layer(&conv_constructed); // 3 , 5 -> 4 , 6
//	s.add_layer(RELU); 
//	s.add_layer(POOLING); // 4 , 4
//	s.add_layer(RELU);
	s.add_layer(SOFTMAX);

	s.preprocessing();
	// Preprocessing func construct layers and index to each layer.
	// So this must be called before set weight tensor or layer inform( stride , padding , etc).

	// Convolution , Affine , Pooling layer must have weight tensor.
	// So set weight tensor to each layer.

	s.set_input_tensor(&k); 
	// Set_input_tensor func of session set input tensor to first layer (in this case , convolution 0 layer).
	s.set_layer_weight_tensor(CONVOLUTION , 0 , &kk); 
	// So does it weight tensor.
	
	s.set_layer_inform( CONVOLUTION , 0 , 2 , 2); 
	// First convoution layer have stride(2, 2) and padding of default value (0 , 0) .
	s.set_layer_inform(CONVOLUTION, 1, 1, 1); 
	// Second convolution layer have stride(1,1) and padding of default value (0 , 0) .

	s.set_layer_weight_tensor(CONVOLUTION, 1, &k1); 
	// Set weight tensor to second convolution layer.
	
	s.set_layer_weight_tensor(AFFINE, 0, &aw); 
	// Set weight tensor to Affine 0 layer.

	s.set_layer_inform(POOLING,0,  2, 2, 1 ,1); 
	// set layer inform ( stride(2,2) , padding(1,1) ).

	s.set_layer_tensor(POOLING, 0, Weight , &tt);
	// You can set weight tensor by using func set_layer_tensor passing layer index in layer to argument.

	// Set weight tensor of shape (1,1,2,2) 
	// Pooling layer don`t care about Batch , Channel , and value of each element.
	// It only care about Colomn and Row of weight tensor.

	/*
	s.set_layer_callback(CONVOLUTION, 0, [](layer* l)->void
	{
		shape s = 
			shape( B(l->get_tensor(Input)) * B(l->get_tensor(Weight)), 1,
				l->get_tensor(Output)->get_shape().Col, l->get_tensor(Output)->get_shape().Row);
		
		tensor* t = l->get_tensor(Output);
		
		t->reshape(s);
	});
	*/

	// You cans set callback to act after each layer has proceeded.
	// Arg l represent each layer.
	// In this session, convolution 0 release output tensor of shape ( Batch number of input , Batch number of weight , Col converted, Row converted ).
	// For next layer ( Affine 0 ) to act , 
	// it is needed that output tensor of convolution 0 be reshaped to ( Batch number of input * Batch number of weight, 1 , Col converted , Row converted ).
	// We set channel of this tensor to 1. because convolution 0 has compacted channel from 3 to 1. and Affine 0 layer has weight tensor with 1 channel. 

	s.set_layer_callback(CONVOLUTION, 1, [](layer* l)->void
	{
		//padding::add_padding_one_side( *l->get_tensor(Output) , Col , true, 1);
		//padding::add_padding_one_side(*l->get_tensor(Output), Row, true, 1);

	});

	// Before pooling layer proceed, previous layer need to add 1 padding to Colomn of output tensor.  
	// Because pooling layer has weight tensor of shape ( 1, 1, 2, 2) ,
	// And output tensor of previous layer has shape of ( Batch number , 1 , 5 , 8 ).
	// After callback func called, Convolution 1 pass to this layer the output tensor of shape ( Batch number , 1 , 6 , 8 ).

	s.set_layer_callback(RELU, 1, [](layer* l)->void
	{
		//l->get_tensor(Output)->transpose(n0 , n1 , n3 , n2);
		// Nx represent dimmension x of tensor.
		// n0 Batch , n1 Channel , n2 Col , n3 Row.
		// And parameter 0, 1, 2, 3 represent Batch , Channel , Col , Row.
	});

	// Next layer ( Softmax 0 ) take Row as calculate unit.
	// I would like to transpose output tensor by surface( Row and Col ). Not volume(Channel).

	s.run();
	
	// session runs.

	layer* cov = s.get_layer(CONVOLUTION, 0);
	tfunc::print_object(cov->get_tensor(Grad_Wgt));

	//s.print_object();
	//print output tensor of each layers.


	system("pause");

}