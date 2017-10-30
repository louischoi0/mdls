#include ".\hader\mdls.h"

using namespace mdls;

session s = session();
convolution conv_constructed = convolution();

tensor first_input(3, 3, 30, 30);

tensor conv0_kernel(4, 3, 2, 2);
tensor conv1_kernel(5 , 4 , 2 , 2);
tensor pool_kerenl(1 , 1, 4, 4);

tfunc::init_tensor_with_random(first_input, 0, 1);
//tensor have value by beeing randomized.

tfunc::init_tensor_with_random(conv0_kernel , 0 , 1);
tfunc::init_tensor_with_random(conv1_kernel, 0, 1);

// You can set session layout by passing enum of layer types, or pointer to layer constructed.

s.add_layer(BATCH_NORMALIZATION); // 30 30 3
s.add_layer(CONVOLUTION); // 2 2 3 -> 16 16 W
s.add_layer(RELU); // 16 16 W
s.add_layer(POOLING); // 4 4 W -> 4 4 W
s.add_layer(&conv_constructed); //  2 2 -> 2 2 ->SERIALZIE
s.add_layer(SOFTMAX); // SOFTMAX

s.preprocessing();
// Preprocessing func construct layers and index to each layer.
// So this must be called before set weight tensor or layer inform( stride , padding , etc).

// Convolution , Affine , Pooling layer must have weight tensor.
// So set weight tensor to each layer.

s.get_layer(SOFTMAX, 10);

s.set_input_tensor(&first_input);
// Set_input_tensor func of session set input tensor to first layer (in this case , convolution 0 layer).
s.set_layer_weight_tensor(CONVOLUTION , 0 , &conv0_kernel);
// So does it weight tensor.

s.set_layer_inform( CONVOLUTION , 0 , 2 , 2);
// First convoution layer have stride(2, 2) and padding of default value (0 , 0) .

s.set_layer_weight_tensor(CONVOLUTION, 1, &conv1_kernel);
// Set weight tensor to second convolution layer.

s.set_layer_inform(POOLING, 0, 4, 4 );
// Pooling 0 layer have stride (4 ,4 ) and default value of padding( 0 , 0 )

s.set_layer_callback(BATCH_NORMALIZATION, 0, [](layer* l)->void
{
  tensor* batch0_output = l->get_tensor(Output);
  padding::add_padding( *batch0_output, Surface , 1 );
});

// You cans set callback to act after each layer has proceeded.
// Arg l represent each layer.
// In this session, convolution 0 release output tensor of shape { 3 (Input batch) ,  4 (Weight Batch) , 15, 15)
// For next layer ( Convolution 1 ) to act ,
// it is needed that output tensor of convolution 0 be reshaped to ( 3, 4, 16 , 16 ).
// So i add padding to it`s output tensor.
// Arg 1 (Surface) means row and col.


// Before pooling layer proceed, previous layer need to add 1 padding to Colomn of output tensor.
// Because pooling layer has weight tensor of shape ( 1, 1, 2, 2) ,
// And output tensor of previous layer has shape of ( Batch number , 1 , 5 , 8 ).
// After callback func called, Convolution 1 pass to this layer the output tensor of shape ( Batch number , 1 , 6 , 8 ).

s.set_layer_callback(RELU, 1, [](layer* l)->void
{
  l->get_tensor(Output)->transpose(n0 , n1 , n3 , n2);
  // Nx represent dimmension x of tensor.
  // n0 Batch , n1 Channel , n2 Col , n3 Row.
  // And parameter 0, 1, 2, 3 represent Batch , Channel , Col , Row.


});

// Next layer ( Softmax 0 ) take Row as calculate unit.
// I would like to transpose output tensor by surface( Row and Col ).

s.run();

//layer* l0 = s.get_layer(0);
// layer* l1 = s.get_layer(1);
layer* l2 = s.get_layer(2);
layer* l3 = s.get_layer(3);

tfunc::print_object(l3->get_tensor(Grad_In));
// print layer 3 Input gradient tensor.
tfunc::print_object(l2->get_tensor(Grad_Wgt));
// print layer 2 Weight gradient tensor.

conv0_kernel.rotate(Rotate::D_90);
conv0_kernel.rotate(Rotate::D_180);
// and you can also rotate tensor by degrees.
