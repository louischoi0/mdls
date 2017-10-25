#pragma once

namespace mdls
{
	namespace mdef
	{

		const double h = 0.000001;
	}

	enum Rotate
	{
		D_90 = 0,
		D_180 = 1,
		D_270 = 2,
		D_360 = 3,
		D_0 = 4

	};

	typedef float elemt;

	typedef enum target_dm
	{
		DM_NULL = 10,

		Volume = 5,
		Surface = 4,

		Row = 3,
		Col = 2,
		Channel = 1,
		Batch = 0
	} Dm;

	const Dm n0 = Dm::Batch;
	const Dm n1 = Dm::Channel;
	const Dm n2 = Dm::Col;
	const Dm n3 = Dm::Row;


	enum layer_extended_enum
	{
		N = -1,

		AFFINE = 0,
		SOFTMAX = 1,
		RELU = 2,
		CROSS_ENTROPY = 3,
		CONVOLUTION = 4,
		POOLING = 5,
		BATCH_NORMALIZATION = 6
	};

	typedef layer_extended_enum ly_type;


	typedef enum Tensor_Index_In_Layer
	{
		Input = 0,
		Weight = 1,
		Output = 2,
		Grad_In = 3,
		Grad_Wgt = 4,
		Bias = 5
	} tensor_index;

	enum operating_type
	{
		DOT,
		MULTIPLY,
		MULTIPLY_IGN_ZERO,
		ADD
	};
}