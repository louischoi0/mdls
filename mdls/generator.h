#pragma once

#include "affine.h"
#include "softmax.h"
#include "relu.h"
#include "cross-entropy.h"

using namespace mdls;


typedef layer_extended_enum layer_ex_type;


namespace mdls
{
	static class generator
	{
	public:
		generator( );

	private:
		int proceeding;

	public:
		layer** _lays;
		layer* _lays_last;

		static void produce_layerex(layer* p, matrix* input, layer_ex_type t);
		static void produce_layerex(layer* p, type_layer t);
		static void init_layers(layer** p, type_layer* t, int size);
	};

}
