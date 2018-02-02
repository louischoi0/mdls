MDLS
===================
Cuda , OpenCl등에 대한 의존성이 없는 경량화된 AI 라이브러리입니다.
C++로 제작되었습니다.
Convolution, Batch Normalization 등의 기본적인 레이어 다수를 제공합니다. 
텐서에 대한 Back propagation, Padding, TransPose 과 같은  기본적인 연산 또한 제공합니다.



Feature
===================
 지원하는 레이어는 다음과 같습니다.
> - Convolution 
> - Softmax
> - Affine 
> - Pooling
> - Relu
> - Cross Entropy
> - Batch Normalization
> - Dense

 
대표적으로 지원하는 텐서연산은 다음과 같습니다.
> - Back Propagation
> - Numerical gradients
> - Padding 
> - Transpose
> - Dispersion
> - Rotate

Documents
===================
MDLS는 여러 레이어를 가진 세션 단위로 연산을 수행합니다.
사용자는 자신이 원하는 레이어를 자유롭게 추가할 수 있습니다.


```C++
#include "mdls.h"
using namespace mdls;

session s = session();
convolution conv_constructed = convolution();

tensor first_input(3, 3, 30, 30);

tensor conv0_kernel(4, 3, 4, 4);
tensor conv1_kernel(5 , 4 , 2 , 2);
tensor pool_kerenl(1 , 1, 4, 4);
```
Session과 Covolution 레이어를 생성합니다.
Tensor의 Parameter 로 Batch, Channel, Column, Row 값을 받습니다. 
입력 텐서로 3개의 Batch , 3개의 Channel, 30개의 Column, 30개의 Row를 갖는 텐서를 생성합니다. 
```
tfunc::init_tensor_with_random(first_input, 0, 1);

tfunc::init_tensor_with_random(conv0_kernel , 0 , 1);
tfunc::init_tensor_with_random(conv1_kernel, 0, 1);
```
Convoltuion 레이어에 사용될 2개의 필터를 0과 1 사이의 값으로 랜덤하게 초기화 합니다.

```
s.add_layer(BATCH_NORMALIZATION); // 30 30 3
s.add_layer(CONVOLUTION); // 2 2 3 -> 16 16 W
s.add_layer(RELU); // 16 16 W
s.add_layer(POOLING); // 4 4 W -> 4 4 W
s.add_layer(&conv_constructed); //  2 2 -> 2 2 ->SERIALZIE
s.add_layer(SOFTMAX); // SOFTMAX
```
session::add_layer 메소드를 통해 세션에 레이어를 등록합니다.
우리가 생성한 텐서의 주소값을 Parameter로 넘겨줄수도 있고,
Layer Enum 값을 Parameter로 넘겨줄수도 있습니다.


```
s.preprocessing();

s.get_layer(SOFTMAX, 0);

s.set_input_tensor(&first_input);

s.set_layer_weight_tensor(CONVOLUTION , 0 , &conv0_kernel);
s.set_layer_inform( CONVOLUTION , 0 , 2 , 2);

s.set_layer_weight_tensor(CONVOLUTION, 1, &conv1_kernel);
s.set_layer_inform(POOLING, 0, 4, 4 );
```
session::preprocessiong 메소드로 등록된 각 레이어를 인덱싱합니다.
모든 레이어의 추가가 끝난후 레이어 정보를 등록하기전 **반드시** 호출 해주어야 합니다.

```
layer* session::get_layer(LayerEnum enum , int index);
layer* session::get_layer(int index);
```
session::get_layer 메소드는 세션의 등록된 레이어를 파라미터에 따라 레이어 포인터를 반환 합니다.

 
```
void session::set_layer_weight_tensor(LayerEnum enumv, int index,layer* weight_tensor);
```
set_layer_weight_tensor 메소드를 통해 Kernel(Filter) 혹은 Weight Tensor 를 등록합니다.
```
void session::set_layer_inform(LayerEnum enumv, int index, int stride_col , int stride_row , int padding_col = 0, int padding_row = 0);
```
set_layer_inform 메소드로 입력텐서와 Filter 계산시 적용될 스트라이드, 패딩을 결정합니다.

```
s.set_layer_callback(BATCH_NORMALIZATION, 0, [](layer* l)->void
{
  tensor* batch0_output = l->get_tensor(Output);
  padding::add_padding( *batch0_output, Surface , 1 );
});
```
session::set_layer_callback 메소드는 레이어 연산이 끝난후 수행될 Callback을 등록할 수 있습니다.
 위 경우 첫번째 BATCH NORMALIZATION 레어의 아웃풋 텐서는 30개의 Column과 30개의 Row 를 갖습니다.
 이후의 컨볼루션 연산을 수행하려면 (0,0,2,2)의 패딩이 필요합니다.
 
```
s.set_layer_callback(RELU, 1, [](layer* l)->void
{
  l->get_tensor(Output)->transpose(n0 , n1 , n3 , n2);

});
```

Relu 레이어 다음으로 Softmax 레이어가 연산을 수행합니다.
MDLS에서 Softmax는 Row를 계산 단위로 갖습니다.
예제에서는 Column단위로 계산을 하고 싶다고 가정한 경우입니다.

이를 위해 Relu의 아웃풋 텐서를 Row와 Column끼리 Transpose 합니다.

n0 ~ n3 변수는 상수값이며, 각각의 차원을 대표합니다.
tensor::transpose 메소드의 파라미터는 Transpose 할 텐서의 형상에 따라 n0~n3 값을 받습니다. 첫번째 부터 Batch, Channel, Column, Row 를 의미합니다. 
```
s.run();

layer* l0 = s.get_layer(0);
layer* l1 = s.get_layer(1);
layer* l2 = s.get_layer(CONVOLUTION , 1);

tfunc::print_object(l3->get_tensor(Grad_In));
tfunc::print_object(l2->get_tensor(Grad_Wgt));
```
session::run 메소드로 세션을 실행합니다.
run 메소드는 각 레이어 연산 전 레이어 사이에 연결되는 텐서의 차원값이 유효한지 검사합니다.

예를들어 3번째 레이어의 아웃풋 텐서가 (3,4,5,5) 의 차원값을 가지고, 4번째 레이어가 컨볼루션이며 (2,3,3,3) 차원값을 가지는 Kernel(Filter)를 가질경우 실패합니다.

layer::get_tensor 메소드로 각레이어의 입력,가중치,편향,입력미분,가중치미분,아웃풋 텐서의 포인터를 얻을수 있습니다.


```
conv0_kernel.rotate(Rotate::D_90);
conv0_kernel.rotate(Rotate::D_180);
```
또한 Tensor를 각도에따라 회전시킬수도 있습니다





