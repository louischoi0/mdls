#include "layer_extends.h"

using namespace mdls;

layer_extends::layer_extends(matrix* input) :
_main_lay( new layer( input) ),
_dif_weight( new matrix( 1, 1) )
{
}

layer_extends::layer_extends(matrix* input, matrix* eqiv) :
_main_lay( new layer(input , eqiv)),
_dif_weight( new matrix(1,1))
{

}

void layer_extends::init_weight_with_randomizing(int row, int col)
{
	EQIV(_main_lay)->resize(row, col);

	double* p = EQIV(_main_lay)->get_p();

	std::default_random_engine egn;
	std::uniform_int_distribution<long> dst(10, 100);

	for (int j = 0; j < col; j++)
		for (int i = 0; i < row; i++)
		{
			double d = dst(egn);
			*p++ = d / 100;
		}

}
