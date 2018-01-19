#include "layert.h"

namespace mdls
{
	class batch_normalization : public layer
	{
	private:
		static int count;

		tensor gamma;
		tensor beta;

		tensor dispersion;
		tensor everage;

		tensor x_hat;

		int surface_size;

	public:
		batch_normalization();
		batch_normalization(tensor* input);
		batch_normalization(tensor* input, tensor* eqiv, tensor* bias);

	public:

		virtual void initialize();
		virtual void proceed_inverse();
		virtual void proceed();

		virtual int get_layer_count() { return count; };

		tensor get_gamma() { return gamma; };
		tensor get_beta() { return beta; };

	};

}

