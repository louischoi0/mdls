
#include "..\objects\tensor.h"
#pragma once
#include <string>

namespace mdls
{
#define CF(i) tensor_iter_map[i][0]

#define NOT_MODIFIED -100


#define EQIV get_eqiv_matrix()
#define OUTPUT get_output_matrix()
#define INPUT get_input_matrix()

#define INP 0
#define WEIGHT 1
#define OUT 2

	struct layer_inform
	{
		ly_type t;

		shape _shape[3]{ shape() , shape() , shape() };

		bool ht[6]{ false , false, false , false , false , false};

		size_t stride_row = 1;
		size_t stride_col = 1;

		size_t padding_row = 0;
		size_t padding_col = 0;


	};

	typedef void(*TTT)(tensor* t, tensor* tt, tensor& o);

	typedef layer_extended_enum type_layer;

	class layer_w_t
	{
	public:
		typedef layer_w_t layer;

		typedef tensor** tensor_map;
		typedef tensor* tensor_iter;

		layer_w_t();
		layer_w_t(ly_type);
		layer_w_t(ly_type, tensor* input);
		layer_w_t(ly_type, tensor* input, tensor* eqiv);

	private:
		void initialize_tensor();
		void eqiv_set();


	protected:
		int* pstatic_count;
		int index_eqaul_layers;

		int drop_out_count;

		int* drop_out_index;
		bool* val_drop_outed;

		layer* _forward_lay;
		layer* _backward_lay;

		tensor_map _tensor_map;

		tensor* grad_weight_by_input;
		tensor* grad_input_by_weight;

		layer_extended_enum _type;

		layer_inform _inf;
		bool init;
		bool init_iv;

		bool drop_out;
		bool use_eqiv;
		bool bias_rand;

		std::string type_name;

		bool start_layer;
		bool end_layer;



		double drop_out_rate;

	public:

		void init_bias();

		void add_bias();
		void set_iter_size__initailize(tensor* c, int pos, int size);

		void remove_tensor(Tensor_Index_In_Layer pos) { _tensor_map[pos] = 0; _inf.ht[pos] = false; };

		tensor* get_tensor_iter(int i) { return _tensor_map[i]; };
		void init_before_proc();

		tensor* get_input_tensor() { return _tensor_map[0]; };
		tensor* get_eqiv_tensor() { return _tensor_map[1]; };
		tensor* get_output_tensor() { return _tensor_map[2]; };

		tensor* get_weight_inverse_tensor() { return _tensor_map[3]; };
		tensor* get_input_inverse_tensor() { return _tensor_map[4]; };
		tensor* get_bias_tensor() { return _tensor_map[5]; };



	public:

		inline void set_tensor(Tensor_Index_In_Layer pos, tensor* p)
		{
			_tensor_map[pos] = p;
			_inf.ht[pos] = true;
		};

		void set_layer_inform(int stride_row , int stride_col , int padding_row = 0 , int padding_col = 0);
		void set_tensor_shape( Tensor_Index_In_Layer idex , shape s){ _tensor_map[idex]->reshape(s); };

		tensor* get_tensor(Tensor_Index_In_Layer index) { return _tensor_map[index]; };

		void set_tensor__copy(tensor* in , Tensor_Index_In_Layer i);

		virtual void initialize(int i) {};
		virtual void proceed() {};

		inline void set_forward_lay(layer* m) { _forward_lay = m; _inf.ht[Input] = true; };
		inline void set_backward_lay(layer* m) { _backward_lay = m; };

		inline shape get_tensor_shape(Tensor_Index_In_Layer i) { return _tensor_map[i]->get_shape(); };
		inline layer_inform get_layer_inform() { return _inf; };

		void eqiv_map_init_with_random(int count, int dep, int row, int col, int dv);
		void eqiv_map_init_with_random();

		layer_extended_enum get_type() { return _type; };

		void set_tag_inform();
		void inform_passing(layer_inform& t);

		void set_type_name(std::string s) { type_name = s; };

		virtual bool proc_possible();

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

		void set_input_from_forward() { _tensor_map[0] = _forward_lay->get_output_tensor(); };

		int get_layer_count() { return *pstatic_count; };
		void inc_layer_count() { (*pstatic_count)++; };

		int get_this_layer_count() { return index_eqaul_layers; };

		void set_start_layer(bool v) { start_layer = v;};
		void set_end_layer(bool v) { end_layer = v; };
		void set_this_layer_count(int i) { index_eqaul_layers = i; };
		void end_layer_task();

		virtual void ready_for_process() {};

		void set_drop_out(double rate) {
			drop_out = true; drop_out_rate = rate;
		};

		bool process_possible();

		void get_grad_in_case(bool input_or_weight, int input_index, int weight_index, tensor& to_write);

	};

	void inc_layer_type_count(int* i, std::string s);
	typedef layer_w_t layer;

}
