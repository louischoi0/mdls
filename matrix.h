#pragma once
#include <iostream>
#include <cmath>
#include "loop_callback.h"
#include "macro.h"
#define D_MALC( size ) (elemt*)malloc( sizeof(elemt) * size )

using namespace std;
namespace mathm {


	class matrix_db : public element
	{
		typedef matrix_db matrix;

	public:
		matrix_db(int len, int hei);
		matrix_db();
		matrix_db(const matrix&);
		~matrix_db();

		matrix& operator=(const matrix&);

	public:

		int converter(int x, int y) const { return y * _eshape.Row + x; };
		int get_transpose_index(int x, int y) { return x* _eshape.Col + y; };

		void set_v(int x, int y, elemt v) { *(_pv + converter(x, y)) = v; };
		void init_val();

		void initialize();
		void initialize_to(elemt v);
		void init_val(elemt* p, int row, int col);
		void resize(int row , int );

		void append_to_col(elemt* p, int r_size, int c_size);
		void append_to_row(elemt* p, int r_size, int c_size);

		void init_val(int , int);
		elemt* get_p() const { return _pv; };

		elemt get_v(int x, int y) const { return *(_pv + converter(x, y)); };

		void read_and_write(int x, int y, int* out);
		void set_p(elemt* d) { _pv = d; };

		void copy_p(elemt* p , int row , int col );

		void init();
		
		void add_state_variable(target_dm pos, int val);

		void transpose() { element::transpose( n0, n1 , n3, n2); };
	
	};
	
	typedef matrix_db matrix;
	typedef void (*CALLBACK_FINISH_MATRIX)(matrix*, elemt* pv , int row , int col);

	typedef void(*CALLBACK_SINGLE_LOOP_P_2_WITH_OFFSET)(elemt* popt, elemt* copi, int* popt_off, int* copi_off);
	
	typedef void(*CALLBACK_SET_POPTS_MATRIX)
		(   elemt* popt , elemt* copi,
			elemt* popt_local, elemt* copi_local,
			int* popt_offset, int* copi_offset,
			int* popt_offset_row_end, int* copi_offset_row_end);

	typedef void(*CALLBACK_WITH_TWO_MTX)(const matrix* , matrix& );
	typedef elemt (*CALLBACK_AFTER_ROW_END)(elemt* );
	
	typedef void (*CALLBACK_SINGLE_LOOP_P_1)(elemt* d );

	typedef void (*CALLBACK_SINGLE_LOOP_P_2)(elemt* d, elemt* dd);
	typedef void(*CALLBACK_SINGLE_LOOP_P_2_N)(elemt** d, elemt* dd, int n);
	typedef void (*CALLBACK_SINGLE_LOOP_V)(elemt d);

	typedef CALLBACK_SINGLE_LOOP_P_2 CALLBACK_ROW_LOOP_P;

	typedef void(*CALLBACK_WITH_TWO_ELEM_P)(elemt* d1 , elemt* d2 , elemt* od);
	typedef elemt(*CALLBACK_WITH_TWO_ELEM_V)(elemt, elemt);

	typedef elemt(*CALLBACK_AFTER_ROW_END_RETURN)(elemt* );
	
	typedef void (*CALLBACK_AFTER_ROW_END_2)(elemt *, elemt* );

	typedef CALLBACK_SINGLE_LOOP_P_1 CALLBACK_SINGLE_LOOP_P;
	typedef CALLBACK_SINGLE_LOOP_P_2 CALLBACK_ROW_END_2;
	
	void pass_func(elemt* m1);
	void pass(matrix* m1);

	void add_one(matrix*);
	void print_matrix_with_p(const elemt* p_, int row_size_, int col_size_);
	
	void add_p1_to_p2(elemt* popt, elemt* copi);
	void get_sum_of_row(matrix* mtx, int col, elemt* copi);
		
	void set_value_to(matrix* mtx, elemt i );
	
	void devide_ign_zero(matrix* m1, matrix* m2);
	void devide( matrix* m1 , matrix* m2, matrix& out);
	void devide(matrix* m1, matrix* m2);
	
	void dot_test(matrix* m1, matrix* m2, matrix& out);
	void transpose(matrix* m, matrix& out );
	void swap_elemt(elemt* d, elemt* copi);
	
	matrix* make_matrix_by_malloc(int row, int col);

	void inc_popts(elemt** popts, int size, int* off);

	typedef elemt*(*LOOP_TYPE)(matrix* , matrix*, CALLBACK_WITH_TWO_ELEM_P );
	typedef void(*LAYER_FUNC)(matrix* m1, matrix* m2, matrix& out);
	typedef void(*ACTIVATE_FUNC)(matrix* m1);

	void softmax(matrix* m , matrix& out );
	void minus(matrix*, matrix*);
	void minus(matrix*, matrix*, matrix&);

	void multiply(matrix*, elemt const_elemt);	

	void relu(matrix* m, matrix&);

	void callback_for_get_inverse_of_relu(matrix* m, matrix*, matrix& out);


#define SIZE_EQ(m ,row,col) ((m->get_row_count() == row) && (m->get_col_count() == col))
	
#define IF_DOT_OPTS_POSSIBLE( m1 , m2 ) (m1->get_row_count() == m2->get_col_count())
#define IF_TWO_MATRIX_EQAL_FORM( m1 , m2 ) (m1->get_col_count() == m2->get_col_count() && m1->get_row_count() == m2->get_row_count() )

#define SIZE(M) (M->get_row_count() * M->get_col_count())
#define RSIZE(M) (M.get_row_count() * M.get_col_count())

#define ROW(M) M->get_row_count()
#define COL(M) M->get_col_count()

#define RROW(M) M.get_row_count()
#define RCOL(M) M.get_col_count()

#define IF_TRANSPOSED_FORM(M1 , M2 ) (M1->get_col_count() == M2->get_row_count()) && (M1->get_row_count() == M2->get_col_count())

	typedef void(*CONV_M_CALL) (matrix*, matrix& , int , int );
	typedef void(*CONV_CALL) (elemt*, elemt*, int, int, int, int);

}