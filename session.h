#pragma once

#include "cube.h"
#include  "layer_header.h"
#include "generator.h"



namespace mdls
{
#define NOT_MODIFIED -100
#define MAX_LAYER_COUNT 30

	enum session_type
	{
		Lenet = 0,
		GoogLenet = 1,
		CUSTOM = 9
	};

	typedef void(*layerTask)(layer*);

	const size_t size_for_session_type[10] = { 10 , 10 , 10 , 10 , 10 , 10 , 10 , 10 , 10 , 10 };
	typedef layer_extended_enum ltype;
	typedef layer** map_layer;
	typedef cube_iter* cube_map;
	typedef cube_map* cube_route;

	class session
		{
		public:
			session();
			session(int size);
			session(session_type);

		private:
			bool first;

			bool input;
			bool weight;

			int _occupied_size;
			int count_added;
			session_type type_session;

			int* occupied;

			type_layer* type_arr;

			size_t layer_count;
			layer** layer_ref;
			
			cube_route session_route;

			ltype* layer_arr;
			
			int* exist_count;
			int** index_layer_by_type;

			tensor* _input_tensor;
			tensor* _weight_tensor;
			
			layerTask* callback_after_layer_runs;

		private:
			bool check_proc_pos();

		public:
			void process();


			void preprocessing();

			void set_layer_inform
			(ltype l ,int i , int stride_col, int stride_row, int padding_col= 0 , int padding_row = 0)
			{ 
				get_layer(l, i)->set_layer_inform(stride_row , stride_col , padding_row , padding_col);
			};

			void set_use_layer_num(int i) { if(type_session == layer_count) layer_count = i; };

			void init();
			void make_layer_type_arr(session_type t, layer_ex_type* tt);

			bool check_init_is_possible();
			void run();

			void init_map();

			void add_layer(layer* t);
			void add_layer(layer_ex_type t);

			cube_iter get_cube_iter(layer_ex_type t, int count_index, int pos);
		
			void process_unt(int unt);
			void inverse_preprocess();
			void inverse_process();

			void set_layer_inform
			(layer_ex_type t, int cindex, int depth ,int ninput,  int rinput, int cinput,
				int neqiv , int reqiv, int ceqiv , int rstride = 1 , int cstride = 1  );

			int get_layer_count(int index);

			void set_layer_weight_tensor(layer_ex_type t, int index , tensor* tns);
			void set_layer_input_tensor(layer_ex_type t, int index, tensor* tns);

			layer* get_start_layer() { return *layer_ref; };
			int get_index(layer_extended_enum type, int index);
			
			layer* get_layer(int index);
			layer* get_layer(layer_extended_enum type, int index);

			void set_input_tensor(tensor* t) { _input_tensor = t; input = true; };
			void set_weight_tensor(tensor* t) { _weight_tensor = t; weight = true; };

			void set_layer_callback(int i, layerTask t) { callback_after_layer_runs[i] = t; };
			void set_layer_callback(ltype l ,int idx, layerTask t) { callback_after_layer_runs[get_index(l, idx)] = t; };

			void set_layer_tensor(ltype l , int idx , Tensor_Index_In_Layer pos,  tensor* );
			
			void print_object();
};

	void make_layer(layer** c, layer_ex_type t);

}