#include "cross-entropy.h"

using namespace mdls;

int cross_entropy::count = 0;

mdls::cross_entropy::cross_entropy() :
layer(CROSS_ENTROPY)
{


}

mdls::cross_entropy::cross_entropy(tensor* input, tensor* true_lable) :
	layer(CROSS_ENTROPY, input, true_lable)
{


}

void mdls::cross_entropy::initialize(int i)
{


}



void cross_entropy::proceed()
{
}

void cross_entropy::proceed_inverse()
{

}


void callback_log_for_cross_entropy(double* d)
{
}

/*
void log_matrix(matrix* m )
{
	loop_matrix(m, callback_log_for_cross_entropy);
}
*/

bool cross_entropy::proc_possible()
{
	return false;
}

void cross_entropy::set_output_inform_from_val()
{

}