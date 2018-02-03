#pragma once

#include "matrix_extended.h"
#include "layerInterface.h"
#include "node.h"

#include <string>
using namespace mathm;	



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
	
	typedef CALLBACK_WITH_TWO_ELEM_P ACTIVE_CALLBACK;
	typedef layer_extended_enum type_layer;
	
	class session;

	class layer_w_t
	{
	public:
		typedef layer_w_t layer;

		typedef tensor** tensor_map;
		typedef tensor* tensor_iter;

		layer();
		layer(const session& s, tensor* input, tensor* eqiv);
		layer(ly_type);
		layer(ly_type, tensor* input);
		layer(ly_type, tensor* input, tensor* eqiv);

	private:
		void initialize_tensor();
		void eqiv_set();

	protected:

		NodeMap* map;

		int* pstatic_count;
		int index_eqaul_layers;

		int drop_out_count;

		int* drop_out_index;
		bool* val_drop_outed;

		bool nodeSet;

		layer* _forward_lay;
		layer* _backward_lay;

		tensor_map _tensor_map;

		tensor* grad_weight_by_input;
		tensor* grad_input_by_weight;
		
		session const* sess;


		layer_extended_enum _type;

		layer_inform _inf;
		bool init;
		bool init_iv;

		bool drop_out;
		bool use_eqiv;

		std::string type_name;

		bool start_layer;
		bool end_layer;

		double drop_out_rate;

	public:
		void add_bias();
		void set_iter_size__initailize(tensor* c, int pos, int size);

		void remove_tensor(Tensor_Index_In_Layer pos) { _tensor_map[pos] = 0; _inf.ht[pos] = false; };

		tensor* get_tensor_iter(int i) { return _tensor_map[i]; };

		NodeMap* get_node_map() { return map; };

		tensor* get_input_tensor() { return _tensor_map[0]; };
		tensor* get_eqiv_tensor() { return _tensor_map[1]; };
		tensor* get_output_tensor() { return _tensor_map[2]; };

		tensor* get_weight_inverse_tensor() { return _tensor_map[3]; };
		tensor* get_input_inverse_tensor() { return _tensor_map[4]; };
		tensor* get_bias_tensor() { return _tensor_map[5]; };


	protected:
		ILayer* self;
	public:
		void write_in_graph_node_relt(Node** n);

		
		inline void proceed()
		{
			self->proceed();
		}
		inline void proceed_inverse()
		{
			self->proceed_inverse();
		}

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
		virtual void init_with_cast() {};
		
		inline void set_forward_lay(layer* m) { _forward_lay = m; };
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


		void proceed_by_nodes()
		{
			if (!nodeSet)
				self->set_up_nodes();

			map->operate_nodes();
		}

	};

	void inc_layer_type_count(int* i, std::string s);

} 

typedef layer_w_t layer;