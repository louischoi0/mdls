#pragma once
#include "..\objects\layert.h"

#include <random>

namespace mdls
{
	class affine : public layer
	{
	private:
		static int count;
		void get_weight(const tensor& bgrad, const tensor& in, const tensor& filter, tensor& iout, tensor& wout);


	public:
		affine();
		affine(tensor* input);
		affine(tensor* input  , tensor* weight);
		affine(tensor* input, tensor* eqiv, tensor* bias);
	
	public:

		virtual void initialize_inverse();

		virtual void proceed();
		virtual void proceed_inverse();

		virtual int get_layer_count() { return count; };
		bool proc_possible();
	
	};


}

