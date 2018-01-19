#pragma once
#include "layert.h"

#define LAY_ENUM_COUNT 10
#define MAX_COUNT_EQUL_LAY 10

namespace mdls
{
	class indexer
	{
	private:
		size_t occupied_count;

		size_t layer_count;

		ly_type* layer_acting;

		size_t* count_each_layer;

		size_t** index_each_layer;

		size_t** input_count_each;
		size_t** eqiv_count_each;
		size_t** output_count_each;

		int accm_input_count;
		int accm_eqiv_count;
		int accm_output_count;

	public:
		indexer(int);
		indexer();

		void add_index(layer_inform inf);
		int get_layer_index(ly_type l, int i);

		void set_indexer_item_inform( layer_inform inf );

	};
}