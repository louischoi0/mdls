#include "generator.h"
generator::generator() :
proceeding(0),
_lays_last(0)
{
	
}

void generator::init_layers(layer** p, type_layer* t, int size)
{

	layer* lay_last = 0;

	for (int i = 0; i < size; i++)
	{
		p[i] = (layer*)malloc(sizeof(softmax));
		produce_layerex(p[i], t[i]);
		lay_last = p[i];
	}
}


void generator::produce_layerex(layer* p, layer_ex_type t)
{
	if (t == AFFINE)
	{
		affine* a = new affine();
		memcpy(p, a, sizeof(affine));
	}

	else if (t == SOFTMAX)
	{
		softmax* a = new mdls::softmax();
		memcpy(p, a, sizeof(mdls::softmax));
	}

	else if (t == RELU)
	{
		relu* a = new mdls::relu();
		memcpy(p, a, sizeof(mdls::relu));
	}

	else if (t == CROSS_ENTROPY)
	{
		cross_entropy* a = new cross_entropy();
		memcpy(p, a, sizeof(cross_entropy));
	}


	else
	{
		return;
	}
}

/*
void generator::proceed_reverse()
{
	for (int i = 5; i >= 0; i--)
	{
		if (i == 5)
		{
			if (_lays[i]->get_type() == CROSS_ENTROPY)
			{
				_lays[i]->initialize_inverse(_lays[i]);
				_lays[i]->proceed_inverse();
			}
			continue;
		}

		_lays[i]->initialize_inverse(_lays[i + 1]);
		_lays[i]->proceed_inverse();

		cout << "dif of weight of " << i;
		cout << ":" << endl;

		if (! _lays[i]->get_dif_of_weight()->get_emptied())
		{
			print_matrix(_lays[i]->get_dif_of_weight());
			cout << endl;
		}
		
		if (!_lays[i]->get_dif_of_output()->get_emptied())
		{
			print_matrix(_lays[i]->get_dif_of_output());
			cout << endl;
		}
	}
		
}
*/

