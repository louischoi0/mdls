#include "indexer.h"
using namespace mdls;


indexer::indexer()
{

}

indexer::indexer(int size) :
	layer_count(size),
	occupied_count(0),
	layer_acting(new ly_type[layer_count]),
	count_each_layer(new size_t[LAY_ENUM_COUNT]),
	index_each_layer(new size_t*[LAY_ENUM_COUNT]),
	input_count_each(new size_t*[LAY_ENUM_COUNT]),
	eqiv_count_each(new size_t*[LAY_ENUM_COUNT]),
	output_count_each(new size_t*[LAY_ENUM_COUNT])
{
	for (int i = 0; i < LAY_ENUM_COUNT; i++)
	{
		count_each_layer[i] = {0,};
		input_count_each[i] = new size_t[MAX_COUNT_EQUL_LAY];
		eqiv_count_each[i] = new size_t[MAX_COUNT_EQUL_LAY];
		output_count_each[i] = new size_t[MAX_COUNT_EQUL_LAY];
		index_each_layer[i] = new  size_t[MAX_COUNT_EQUL_LAY];

	}

	for (int j = 0; j < LAY_ENUM_COUNT; j++)
		for (int i = 0; i < MAX_COUNT_EQUL_LAY; i++)
		{
			input_count_each[j][i] = 0;
			eqiv_count_each[j][i] = 0;
			output_count_each[j][i] = 0;
			index_each_layer[j][i] = -1;
		}

}

void indexer::set_indexer_item_inform( layer_inform inf )
{



}

void indexer::add_index(layer_inform inf)
{

}

int indexer::get_layer_index(ly_type l, int i)
{
	return 0;

};
