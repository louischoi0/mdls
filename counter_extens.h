#pragma once

#include "element.h"

namespace mdls
{

	class counter
	{
		shape* s_arr;
		shape* c_arr;

		shape lc_shape;

		offset* o_arr;
		
		target_dm** off_dm;
		target_dm** init_dm;

		size_t ls[4];

		int* ct_arr;

		shape _s;
		shape _c;

		offset _o;

		Dm target;

		bool counting;

		int use_num;
		int* count;
		int loop_count;

		int** _log_arr;

	public:
		counter();

		counter(const shape& s);
		counter(const shape& rs , const shape& ls);
		counter(const shape&, const shape&, const shape&);

		void initialize(int size, shape* s);

		void inc_count_all();

		void inc_count_f();
		void inc_count(int i);
		void inc_count(int i , int str ,target_dm d);

		void inc_loop_count() { loop_count++; counting = true; };

		void init();
		void init(size_t use_num);
		void set_shape(int index, const shape& s);
		void set_offdm(int index , target_dm n0 , target_dm n1 , target_dm n2 , target_dm n3);
		void set_init_dm(int index, target_dm n0, target_dm n1, target_dm n2, target_dm n3);

		void inc_loop(target_dm n);

		inline shape get_orginal_shape() { return _s; };
		inline shape get_counted_shape() { return _c; };

		shape get_orginal_shape(int );
		void get_counted_shape(int , shape& );
		void init_count(int i, target_dm d);
		
		
		inline int get_loop_count() { return loop_count; };
	
		int get_offset();
		int get_offset(int i);
				
		void callback_end() { counting = false; };
		void set_target(Dm n) {
			target = n;
		};
		
		Dm get_target() {
			return target;
		};

		shape get_local_shape() { return lc_shape; };
		void set_local_shape(const shape& l) { lc_shape = l; };
		void initialize_log(int size, int local_size);
		void write_log(int index, int local_index, int val);

		void add_count(int index, int v) { count[index] += v; };
	
		int* get_log(int i) { return _log_arr[i]; };
	};



	typedef	counter fct;
}