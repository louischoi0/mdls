#pragma once

#include "matrix_extended.h"
#include "cube_func.h"
#include <string>
using namespace mathm;

namespace mdls
{
#define CF(i) cube_iter_map[i][0]

#define NOT_MODIFIED -100

		
#define EQIV get_eqiv_matrix()
#define OUTPUT get_output_matrix()
#define INPUT get_input_matrix()

#define INP 0
#define WEIGHT 1
#define OUT 2

	typedef CALLBACK_WITH_TWO_ELEM_P ACTIVE_CALLBACK;
	typedef layer_extended_enum type_layer;
		
	typedef cube_iter* cube_map;

	class layer
	{

	public:
		layer(ly_type);
		layer(ly_type, cube* input);
		layer(ly_type, cube* input, cube* eqiv);
		layer(ly_type, matrix* input, matrix* set);
		layer(ly_type, matrix*);

	protected:
		int* pstatic_count;
		int index_eqaul_layers;

		size_t* occupied_arr;
		size_t* allocated_arr;

		size_t _input_row;
		size_t _input_col;
		size_t _input_depth;

		size_t _eqiv_row;
		size_t _eqiv_col;
		size_t _eqiv_depth;

		size_t _output_row;
		size_t _output_col;
		size_t _output_depth;

		size_t _stride_row;
		size_t _stride_col;

		size_t _padding_row;
		size_t _padding_col;

		int drop_out_count;

		int* drop_out_index;
		bool* val_drop_outed;

		layer* _forward_lay;
		layer* _backward_lay;

		cube_map cube_iter_map;

		layer_extended_enum type;

		bool input_informed;
		bool eqiv_informed;

		bool got_input;
		bool got_eqiv;

		bool drop_out;

		bool* havecube;

		bool use_eqiv;

		std::string type_name;

		bool start_layer;
		bool end_layer;

		double drop_out_rate;

	private:
		void init_map();

	public:
		void add_bias();

		void initialize_memory_cube(int pos, int count, int depth, int row, int col);
		void set_iter_size__initailize(cube* c, int pos, int size);

		void dettach_cube_iter(int pos, cube_iter c);

		cube_iter get_cube_iter(int i) { return cube_iter_map[i]; };

		cube_iter get_input_cube() { return cube_iter_map[0]; };
		cube_iter get_eqiv_cube() { return cube_iter_map[1]; };
		cube_iter get_output_cube() { return cube_iter_map[2]; };

		cube_iter get_weight_inverse_cube() { return cube_iter_map[3]; };
		cube_iter get_input_inverse_cube() { return cube_iter_map[4]; };
		cube_iter get_bias_cube() { return cube_iter_map[5]; };

	public:

		void add_cube(int pos, cube* p)
		{
			cube_iter_map[pos][occupied_arr[pos]] = p;
			occupied_arr[pos]++;
		};

		void set_inform
		(int depth, int n_input, int r_input, int c_input,
			int n_eqiv, int r_eqiv, int c_eqiv,
			int r_stride, int c_stride,
			int r_padding = 1, int c_padding = 1
		);

		void set_inform(layer*);

		void set_cube_iter(int pos, cube_iter, int size);
		void set_cube_iter(int pos, cube_iter);
		void set_cube(int, cube*);

		int get_occupied_count(int);
		int get_allocated_count(int);

		void copy_cube_iter(cube_iter* i, int pos, int size);

		virtual void initialize(int i) {};
		virtual void proceed() {};

		void set_forward_lay(layer* m);
		void set_backward_lay(layer* m) { _backward_lay = m; };

		int get_oc_count(int pos);
		int get_cube_inform(int pos, int row_col_dep);

		void eqiv_map_init_with_random(int count, int dep, int row, int col, int dv);
		void eqiv_map_init_with_random();

		layer_extended_enum get_type() { return type; };

		void set_tag_inform(std::string);
		void inform_passing(layer_inform& t);

		void set_type_name(std::string s) { type_name = s; };

		virtual bool proc_possible() { return true; };

		virtual bool proc_poss()
		{
			return (use_eqiv && input_informed && eqiv_informed) || (!use_eqiv && input_informed);
		};

		void set_weight_inform
		(int count, int depth, int row, int col,
			int rstride = 1, int cstride = 1, int rpadding = 0, int cpadding = 0);

		void set_input_inform(int count, int depth, int row, int col);
		virtual void set_output_inform_from_val();

		void get_input_from_forward();
		void preprocessing(layer*);
		void preprocessing();
		virtual void proceed_inverse();

		void process_drop_out(int weight_number);
		void drop_out_close(int weight_number);

		void set_input_from_forward() { cube_iter_map[0] = _forward_lay->get_output_cube(); };

		int get_layer_count() {	return *pstatic_count; };
		void inc_layer_count() { (*pstatic_count)++; };
		
		int get_this_layer_count() { return index_eqaul_layers; };

		void set_start_layer(bool v) { start_layer = v; input_informed = !v; };
		void set_end_layer(bool v) { end_layer = v; };
		void set_this_layer_count(int i) { index_eqaul_layers = i; };
		
		virtual void ready_for_process() {};

		void set_drop_out(double rate) {
			drop_out = true; drop_out_rate = rate;
		};

};

void inc_layer_type_count(int* i, std::string s);



}

