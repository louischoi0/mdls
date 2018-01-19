#pragma once
#include "layer.h"
#include <random>


namespace mdls {

	class layer_extends
	{
	public :
		layer_extends(matrix* input);
		layer_extends(matrix* input , matrix* eqiv);

	protected:
		layer_extends* _forward_lay;
		layer* _main_lay;
		layer_extends* _backward_lay;
		
		matrix* _dif_weight;
		matrix* _bias_node;

		int layer_pos;
		void initialize();

		layer_extended_enum type;

	public:
		virtual void initialize(int ) = 0;
		void set_inverse_activity(LAYER_FUNC f) { _main_lay->set_layer_main_func(f); };

		void init_weight_with_randomizing(int row , int col);
		 
		virtual void proceed() { _main_lay->proceed(); };
		virtual void calculrate_dif_of_weight() { _main_lay->inverse_func( _backward_lay->get_dif_weight_matrix(), _main_lay->get_input_matrix(), *_dif_weight); };

		virtual void initialize_inverse() {  };

		layer_db* get_main_layer() { return _main_lay; };

		matrix* get_input_matrix() { return _main_lay->get_input_matrix(); };
		matrix* get_eqiv_matrix() { return _main_lay->get_eqiv_matrix(); };
		matrix* get_output_matrix() { return _main_lay->get_output_matrix(); };
		matrix* get_dif_weight_matrix() { return _dif_weight; };

		void set_type(layer_extended_enum t) { type = t; };

		void set_forward_lay(layer_extends* forward) { _forward_lay = forward; };
		void set_backward_lay(layer_extends* backward) { _backward_lay = backward; };
		
		layer_extended_enum get_type() { return type; };
		void init(matrix* );
		
	};

}  