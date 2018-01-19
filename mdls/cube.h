#pragma once
#include "matrix.h"
#include "tensor.h"

using namespace mathm;


namespace mdls
{
	typedef matrix** matrix_iter;

	class cube : public element
	{
		typedef cube** cube_iter;

	public :
		cube(int depth, int row , int col );
		cube();
		cube(const cube* c);

		cube& operator=(cube& c )
		{
			memcpy(this, &c, sizeof(cube));

			free(_pv);
			delete[] local_param;

			_pv = D_MALC(_eshape.Alloc_size);
			local_param = new int[5]{ 0, };

			memcpy(_pv, c._pv, sizeof(elemt) * _eshape.Alloc_size);
			memcpy(local_param, c.local_param, sizeof(int) * 5);

			return *this;
		};

		std::string _tag;
		int* local_param;

		
	public:
		elemt* get_surface(int depth) { return _pv + _offset.Channel * depth; };
		int get_alloc_size() { return _eshape.Alloc_size; };

		void init_val(int d,  int r , int c);
		void init_val(elemt* p, int row, int col, int depth);

		void reshape( int depth , int row, int col);
		void reshape(int row, int col);
		

		int get_occupied() { return _eshape.Alloc_size; };
		int get_channel() { return _eshape.Channel; };
		

		void matrix_to_cube(const matrix*);

		void initialize_cube(elemt);
		void initialize_cube();
		void initialize_cube_ladder();
		void set_p(elemt* p) { _pv = p; };

		void add(elemt v);
		void multiply(elemt v);

		int get_row_count() { return _eshape.Row; };
		int get_col_count() { return _eshape.Col; };
		int get_depth_channel() { return _eshape.Channel; };

		elemt* get_p() { return _pv; };
		elemt* get_p_by_surface(int surface) { return _pv + ( surface * D_OFF(this)); };

		int* get_local_param() { return local_param; };
		void pass_param(int i, int ii, int iii, int iiii, int iiiii);

		void set_surface_with_matrix(int surface , matrix* m);
		void set_surface_with_p(int surface, elemt* p);

		int get_channel_offset() { return _offset.Channel; };
	
		void split_to_matrix_by_col(int depth , int row_index , elemt* out);
		void split_to_matrix_by_row(int depth , int col_index , elemt* out);

		void col_to_surface_row_to_depth( cube_iter& out , int col_cut_to_row );

		elemt* get_new_p_by_surface(int depth);

		void set_tag(std::string s) { _tag = s; };
		std::string get_tag() { return _tag; };

		elemt* get(int d, int r, int c);
		elemt* get(int inx);
		
		void set(int d, int r, int c, elemt val);
		void set(int inx , elemt val);

		matrix* cube_to_matrix();

		void transpose(target_dm n1, target_dm n2, target_dm n3);
		int get_count() { return (D(this) * R(this) * C(this)); };

		int get_transpose_index(int d, int r, int c) { return (D_OFF(this) * d) + (C_OFF(this) * r) + c; };
		void rotate_180dgr();
	};
	
	typedef cube** cube_iter;

#define IF_TWO_CUBE_EQAL_FORM(c1,c2) ((c1->get_row_count()==c2->get_row_count()) && (c1->get_col_count()==c2->get_col_count()))
	

}
