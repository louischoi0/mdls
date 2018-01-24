#pragma once
#include "layert.h"
#include "of.h"
class Dense : public layer, public layerITF
{
	int ncount;

public:
	Dense(tensor* input,  int out) :
		ncount(out)
	{
		_tensor_map[Input] = input;
		initialize();
	}
	void initialize()
	{
		init = true;
	
		shape outShape(1, 1, 1, ncount);

		_tensor_map[Weight] = new tensor(outShape);
		_tensor_map[Weight]->initialize_elem_to(1);
	}
	void procced_inverse()
	{


	}
	void proceed() 
	{
		elemt* wp = _tensor_map[Weight]->get_p();

		tfunc::reduced_sum(  target_dm::Batch, _tensor_map[Input], *_tensor_map[Output]);
		tfunc::print_object(_tensor_map[Output]);
		tfunc::channel_times(*_tensor_map[Output] , ncount);
		_tensor_map[Output]->apply_to_elem_by_channel([](elemt* p, elemt* pp)->void {
			*p *= *pp;
		},wp);


	};

	void proceed_inverse() {};






};

	




