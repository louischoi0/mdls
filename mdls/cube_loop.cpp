#include "cube_loop.h"
#include "func_infromer.h"

void mathm::cube_loop(cube* p, elemt* copi, CALLBACK_SINGLE_LOOP_P_2 CC)
{
	elemt* op = p->get_p();
	elemt* oop = op;
	elemt* ooop = op;

	elemt* cop = copi;

	int lop_size_row = p->get_row_count();
	int lop_size_col = p->get_col_count();

	for (int i = 0; i < D(p); i++)
	{
		oop = op;
		for (int j = 0; j < lop_size_col; j++)
		{
			ooop = oop;

			for (int k = 0; k < lop_size_row; k++)
			{
				CC(ooop++,  copi++);
			}


			oop += C_OFF(p);
		}

		op += D_OFF(p);
	}
}

void mathm::cube_loop(cube* p , cube* pp , elemt* copi , CALLBACK_WITH_TWO_ELEM_P CC)
{
	elemt* op = p->get_p();
	elemt* oop = op;
	elemt* ooop = op;

	elemt* cp = pp->get_p();
	elemt* ccp = cp;
	elemt* cccp = ccp;

	elemt* cop = copi;

	int lop_size_row = p->get_row_count();
	int lop_size_col = p->get_col_count();

	for (int i = 0; i < D(p); i++)
	{
		oop = op;
		ccp = cp;

		for (int j = 0; j < lop_size_col; j++)
		{
			ooop = oop;
			cccp = ccp;

			for (int k = 0; k < lop_size_row; k++)
			{
				CC(ooop++, cccp++, copi++);
			}


			oop += C_OFF(p);
			ccp += C_OFF(pp);
		}

		op += D_OFF(p);
		cp += D_OFF(pp);
	}


}

void mathm::cube_loop_with_stride(cube* p, cube* pp, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_ELEM_OUT CC)
{
	elemt* op = p->get_p();
	elemt* oop = op;
	elemt* ooop = op;

	elemt* cp = pp->get_p();
	elemt* ccp = cp;
	elemt* cccp = ccp;

	elemt* cop = copi;

	int lop_size_row = ((p->get_row_count() - fil_size_row) / stride_row) + 1;
	int lop_size_col = ((p->get_col_count() - fil_size_col) / stride_col) + 1;

	for (int i = 0; i < D(p); i++)
	{
		oop = op;
		ccp = cp;

		for (int j = 0; j < lop_size_col; j++)
		{
			ooop = oop;
			cccp = ccp;

			for (int k = 0; k < lop_size_row; k++)
			{

				CC(ooop, cccp , copi++, C_OFF(p), fil_size_row, fil_size_col);
				ooop += stride_row;
				cccp += stride_row;
			}


			oop += C_OFF(p) * stride_col;
			ccp += C_OFF(pp) * stride_col;
		}

		op += D_OFF(p);
		cp += D_OFF(pp);
	}

}
void mathm::cube_loop_with_stride_with_arr
(cube* p, int** idx, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_ELEM_LOOP CC)
{
	elemt* op = p->get_p();
	elemt* pp = op;
	elemt* cp = op;
	
	int c_off = p->get_row_count();

	detail::func_informer* inst = detail::func_informer::get_instance();

	int lop_size_row = ((p->get_row_count() - fil_size_row) / stride_row) + 1;
	int lop_size_col = ((p->get_col_count() - fil_size_col) / stride_col) + 1;

	int index_main = 0;

	for (int i = 0; i < D(p); i++)
	{
		pp = op;

		for (int j = 0; j < lop_size_col; j++)
		{
			cp = pp;

			for (int k = 0; k < lop_size_row; k++)
			{
				CC(cp, copi++, C_OFF(p), fil_size_row, fil_size_col);
				cp += stride_row;

				(*idx)[index_main++] = (k * stride_row) + ( ( j * stride_col ) * c_off );

				inst->inc_func_active_count();
			}

			pp += C_OFF(p) * stride_col;
		}

		op += D_OFF(p);
	}

	inst->initialize_count();
}
void mathm::cube_loop_with_stride(cube* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_ELEM_LOOP CC)
{
	elemt* op = p->get_p();
	elemt* pp = op;
	elemt* cp = op;


	elemt* cop = copi;

	int lop_size_row = ((p->get_row_count() - fil_size_row) / stride_row) + 1;
	int lop_size_col = ((p->get_col_count() - fil_size_col) / stride_col) + 1;

	for (int i = 0; i < D(p); i++)
	{
		pp = op;

		for (int j = 0; j < lop_size_col; j++)
		{
			cp = pp;

			for (int k = 0; k < lop_size_row; k++)
			{
				CC(cp, copi, C_OFF(p), fil_size_row, fil_size_col);
				cp += stride_row;
				copi++;
			}

			pp += C_OFF(p) * stride_col;
		}

		op += D_OFF(p);
	}


}

void mathm::cube_loop_channel_first_with_stride
(cube* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col, LOCAL_FUNC_FOR_CHN_LOOP CC)
{
	elemt* pp = p->get_p();
	elemt* cp = pp;

	elemt* cop = copi;

	int lop_size_row = ((p->get_row_count() - fil_size_row) / stride_row) + 1;
	int lop_size_col = ((p->get_col_count() - fil_size_col) / stride_col) + 1;
	
	for (int i = 0; i < lop_size_col; i++)
	{
		pp = cp;

		for (int j = 0; j < lop_size_row; j++)
		{
			CC(pp, cop, C_OFF(p), D_OFF(p), fil_size_row, fil_size_col, D(p));

			pp += stride_row;
			cop += fil_size_row * fil_size_col * D(p);
		}

		cp += stride_col * C_OFF(p);
	}

}


void mathm::cube_loop_for_dot(cube* p, matrix* m, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC)
{
	elemt* cp = m->get_p();
	elemt* ccp = cp;
	elemt* cccp = cp;

	elemt* op = p->get_p();
	elemt* oop = op;
	elemt* ooop = op;

	elemt* cop = copi;

	int lop_size_row = m->get_row_count();
	int lop_size_col = p->get_col_count();;

	for (int i = 0; i < D(p); i++)
	{
		oop = op;
		ccp = cp;

		for (int j = 0; j < lop_size_col; j++)
		{
			cccp = ccp;
			ooop = oop;

			for (int k = 0; k < lop_size_row; k++)
			{
				CC(ooop, cccp, copi++, ROW(m), R(p), ROW(m));
			}

			oop += C_OFF(p);
			
		}

		op += D_OFF(p);
	}
}

void mathm::cube_loop_for_dot(cube* p, cube* pp, elemt* copi, LOCAL_FUNC_FOR_ELEM_LOOP_DOT CC)
{
	elemt* cp = pp->get_p();
	elemt* ccp = cp;
	elemt* cccp = cp;

	elemt* op = p->get_p();
	elemt* oop = op;
	elemt* ooop = op;

	elemt* cop = copi;

	int lop_size_row = pp->get_row_count();
	int lop_size_col = p->get_col_count();;

	for (int i = 0; i < D(p); i++)
	{
		oop = op;
		ccp = cp;

		for (int j = 0; j < lop_size_col; j++)
		{
			cccp = ccp;
			ooop = oop;

			for (int k = 0; k < lop_size_row; k++)
			{
				CC(ooop, cccp, cop, C_OFF(pp), R(p), R(pp));
				cop++;
			}

			oop += C_OFF(p);
		}

		cp += D_OFF(pp);
		op += D_OFF(p);
	}
}

void mathm::cube_iter_loop(cube_iter t, CUBE_FUNC_1  CC,int size)
{
	for (int i = 0; i < size; i++)
		CC(t[i]);
}


void mathm::cube_iter_loop(cube_iter in, cube_iter out, CUBE_FUNC_2 CC, int size)
{
	for (int i = 0; i < size; i++)
		CC(in[i], *out[i]);
}

void mathm::cube_iter_loop(cube_iter c, cube_iter cc, cube_iter out, CUBE_FUNC_3 CC, int size)
{
	for (int i = 0; i < size; i++)
		CC(c[i], cc[i], *out[i]);
}

void mathm::cube_iter_loop(cube_iter c, cube_iter out, CALLBACK_CUBE_LOOP CC, int range)
{
	for (int i = 0; i < range; i++)
		CC(c[i], *out[i], range);	

}

