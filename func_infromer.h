#pragma once

namespace detail
{
	class func_informer
	{
		private:
			func_informer() {};
	
			static bool instflag;
			static func_informer* instance;

			static int* varr_size;

			static bool val_arr_init;

			int* pcount;

			int base;
			int pooling;

			int use;

			int** value_arr;
			int** ref_arr;

	public:
		void inc_func_active_count();
		void set_activate_pointer(int i);

		void initialize_count();

		inline int get_pcount() { return *pcount; };

		static func_informer* get_instance();
		
		void set_value_arr_size(int indx , int len);

		int** get_value_arr(int i ) { return &(value_arr[i]); };
		int** get_ref_arr(int i) { return &(ref_arr[i]); };

		void set_ref_arr(int i , int* a) { ref_arr[i] = a; };
		void set_use_num(int v) { use = v; };

		void initialize_arr();

	};

}


