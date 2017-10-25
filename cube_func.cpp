#include "cube_func.h"
using namespace loop;
using namespace callback;
using namespace macro;

void mathm::cadd(cube* c, cube& cc)
{
	if (!EQUL(c, (&cc)) )
	{

		cout << "invalid cube form" << endl;
		return;

	}

	cube_loop(c, &cc, c->get_p(), cb_add );
}


void mathm::cdot(cube*c, matrix* m, matrix& out)
{
	if (R(c) != COL(m))
	{
		cout << "invalid cube and matrix form" << endl;
		return;
	}

	out.init_val( ROW(m), C(c));
	
	cube_loop_for_dot(c, m, out.get_p() , cb_cdot);
}

void mathm::cdot(cube* c, matrix* m, cube& out)
{
	if (R(c) != COL(m))
	{
		cout << "invalid cube and matrix form" << endl;
		return;
	}

	if (A((&out)) < D(c) * C(c) * ROW(m))
		out.reshape(D(c), ROW(m) , C(c));

	out.init_val(D(c), ROW(m), C(c));

	cube_loop_for_dot(c, m, out.get_p(), cb_cdot);

}

void mathm::cdot(cube* c, cube* cc, cube& out)
{
 	if ((D(c) != D(cc)) || (R(c) != C(cc)))
	{
		cout << "invalid cube form" << endl;
		return;
	}

	if (A((&out)) < D(c) * C(c) * R(cc))
		out.reshape(D(c), R(cc), C(c));

	out.init_val(D(c), R(cc), C(c));

	cube_loop_for_dot(c, cc, out.get_p(), cb_cdot);
}




void mathm::cmultiply(cube* c, cube* cc, cube& out )
{
	if (!EQUL(c, cc))
	{
		cout << "invalid cube form" << endl;
		return;
	}

	if (EQUL(c, (&out)))
		out.reshape(D(c), R(c), C(c));

	out.init_val(D(c), R(c), C(c));

	cube_loop_with_stride(c, cc, out.get_p(), 1 ,1, 1, 1, cb_multiply);
}

void pooling(cube* p, cube& out)
{
	int* psp = p->get_local_param();

	int stride_row = *psp++;
	int stride_col = *psp++;
	int fil_size_row = *psp++;
	int fil_size_col = *psp;

	cube_loop_with_stride(p,  out.get_p(), stride_row, stride_col, fil_size_row, fil_size_col, max_conv_params);
}


void mathm::pooling(cube* p, elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col)
{
	cube_loop_with_stride(p, copi, stride_row, stride_col, fil_size_row, fil_size_col, max_conv_params);
	//p->init_val(copi ,((R(p) - fil_size_row) / stride_row) + 1, ((C(p) - fil_size_row) / stride_row) + 1, p->get_depth());
}

void mathm::pooling(cube* p, int** ra ,elemt* copi, int stride_row, int stride_col, int fil_size_row, int fil_size_col)
{
	detail::func_informer* inst = detail::func_informer::get_instance();
	inst->set_activate_pointer(0);

	cube_loop_with_stride_with_arr(p, ra , copi, stride_row, stride_col, fil_size_row, fil_size_col, max_conv_params);
}

void mathm::pooling(cube*p, cube& out, int** ra, int stride_row, int stride_col, int fil_size_row, int fil_size_col)
{
	int ralloc_ = get_dv_size(p->get_row_count(), fil_size_row, stride_row);
	int calloc_ = get_dv_size(p->get_col_count(), fil_size_col, stride_col);

	detail::func_informer* inst = detail::func_informer::get_instance();
	inst->set_use_num(2);

	inst->set_activate_pointer(0);

	inst->set_value_arr_size(0, ralloc_ * calloc_ * D(p));
	inst->set_value_arr_size(1, ralloc_ * calloc_ * D(p));

	int** val_r = inst->get_value_arr(0);
	int** val_c = inst->get_value_arr(1);

	if (SIZE((&out)) < ralloc_ * calloc_ * D(p))
		out.reshape(D(p), ralloc_, calloc_);

	int* temp = new int[ralloc_ * calloc_* D(p)];

	cube_loop_with_stride_with_arr(p, ra, out.get_p() , stride_row, stride_col, fil_size_row, fil_size_col, max_conv_params_with_inf);

	arr_opt_multiply(*val_c, &temp , p->get_row_count(), ralloc_ * calloc_* D(p));
	arr_opt_add(*val_r, temp, &temp, ralloc_ * calloc_* D(p));

	arr_opt_add( *ra , temp , ra , ralloc_ * calloc_* D(p));

	out.init_val(D(p), ralloc_, calloc_);
	inst->initialize_arr();

}


void mathm::pooling_with_index_arr()
{

}


void mathm::cube_to_col(cube* c, elemt* p, int strid_row, int strid_col, int count_row_to_copy, int count_col_to_copy)
{
 	cube_loop_channel_first_with_stride
	(c, p, strid_row, strid_col, count_row_to_copy, count_col_to_copy, channel_copy);

}

void mathm::convert_imatrix_to_cube(matrix* m, elemt* res, int stride_row, int stride_col, int win_row, int win_col, int block_count, int block_depth)
{
	int block_size = win_row * win_col * block_depth;
	
	elemt* p = m->get_p();
	elemt* new_p = res;

	int b_off = win_row * win_col;

	int d_off = block_count * block_size;

	elemt* dest = 0;
	elemt* srcp = 0;

	for (int j = 0; j < block_count; j++)
		for (int q = 0; q < block_depth; q++)
			for (int i = 0; i < win_col; i++)
			{
				dest = &new_p[(i * win_row) + (q * (d_off)) + (j * (block_size))];
				srcp = p + (i * win_row) + (q * block_size) + (j * block_size * block_depth);

				memcpy(dest, srcp, sizeof(elemt) * win_row);
			}

}

void mathm::print_cube(cube* c)
{
	elemt* p = c->get_p();

	cout << c->get_tag() << endl;

	for (int i = 0; i < D(c) ; i++, p += D_OFF(c))
	{
		print_matrix_with_p(p, c->get_row_count(), c->get_col_count());
		cout << endl;
	}

	cout << endl;
}



void mathm::init_random(cube* c )
{
	default_random_engine rnd;
	
	normal_distribution	<elemt> rn(0.5, 2.0);
	elemt* p = new elemt[R(c) * D(c) * C(c)];

	rnd.seed(rn(rnd));

	for (int i = 0; i < R(c) * D(c) * C(c); i++)
	{
		elemt v = 0;
		while (true)
		{
			v = rn(rnd);

			if (v >= 0 && v <= 1)
				break;
		}

		p[i] = v;

	}

	memcpy(c->get_p(), p, sizeof(elemt) * R(c) * D(c) * C(c));
}

void mathm::crelu(cube* c, cube& out)
{
		cube_loop(c, out.get_p(), callback_for_relu);

}

void mathm::cubes_to_col(cube_iter iter, int size, elemt** p, int stride_row, int stride_col, int size_to_copy_row, int size_to_copy_col)
{
	free(*p);
	
	*p = D_MALC
	(get_dv_size(iter[0]->get_row_count(), size_to_copy_row, stride_row) *
		get_dv_size(iter[0]->get_col_count(), size_to_copy_col, stride_col) *
		size_to_copy_col * size_to_copy_row * size * D(iter[0]));

	elemt* cp = *p;

	for (int i = 0; i < size; i++)
	{
		cube_to_col(iter[i], cp, stride_row, stride_col, size_to_copy_row, size_to_copy_col);

		cp += get_dv_size(iter[i]->get_row_count(), size_to_copy_row, stride_row) *
			get_dv_size(iter[i]->get_col_count(), size_to_copy_col, stride_col) *
			size_to_copy_col * size_to_copy_row * D(iter[i]);
	}
}


void mathm::citer_passparam(cube_iter t, int size, int local1 , int local2 , int local3 , int local4 , int local5)
{
	for (int i = 0; i < size; i++)
		t[i]->pass_param(local1, local2, local3, local4, local5);
}


void mathm::set_tag(cube_iter it, std::string s, int layer_count, int size, int pos)
{
	for (int i = 0; i < size; i++)
		it[i]->set_tag(s + std::to_string(layer_count) + string_route(pos) + std::to_string(i));
}

std::string mathm::string_route(int i)
{
	if (i == 0)
		return " input";

	else if (i == 1)
		return " weight";

	else if (i == 2)
		return " output";
}

void mathm::cube_iter_to_initailize(cube_iter t, int size, elemt value)
{
	for (int i = 0; i < size; i++)
		t[i]->initialize_cube(value);

}

void mathm::cube_iter_copy(cube_iter dest, cube_iter src, int size)
{
	for (int i = 0; i < size; i++)
		memcpy(dest[i], src[i], sizeof(cube));
}

void mathm::numerical_gradient(cube* in, cube& out, CUBE_FUNC_2 CC)
{
	cube inh(*in);
	cube outh(out);
	cube outp(out);

	elemt h = 0.00001;

	inh.add(h);
	CC(&inh, outh);
	
	cout << " inh " << endl;

	print_cube(&outh);

	inh.add(-2 * h);
	CC(&inh, outp);

	cout << " inhh " << endl;
	print_cube(&outp);

	outp.multiply(-1);
	cadd(&outh, outp);

	outh.multiply(1 / (2 * h) );

	cout << " out " << endl;
	print_cube(&outh);


	out = outp;

}

void softmax_c_for_gradient(cube* in, cube& out)
{
	matrix inf(R(in), C(in));
	matrix minf(R(in), C(in));

	detail::func_informer* inst = detail::func_informer::get_instance();

	inst->set_activate_pointer(0);
	inst->set_use_num(1);

	if (SIZE((&out)) < SIZE(in))
		out.reshape(D(in), R(in), C(in));

	for (int i = 0; i< D(in); i++)
	{
		inf.set_p(in->get_new_p_by_surface(i));
		minf.set_p(in->get_new_p_by_surface(i));

		max(&minf);

		elemt* p = new elemt[inf.get_alloc_size()];

		loop_two_matrix_extends(&inf, &minf, p, 0, 0, 1, 0, ROW((&inf)), 1, callback_for_minus_3);

		inf.set_p(p);

		matrix sinf(inf);

		loop_matrix(&inf, callback_for_exp);
		loop_matrix(&sinf, callback_for_exp);

		compact_matrix_by_col(&sinf);

		loop_two_matrix_extends(&inf, &sinf, inf.get_p(), 0, 0, 1, 0, ROW((&inf)), 1, callback_for_devide);

		out.set_surface_with_matrix(i, &inf);
	
		inst->inc_func_active_count();
	
	}

	out.init_val(D(in), R(in), C(in));
}


void mathm::softmax_c(cube* in, cube& out)
{
	matrix inf(R(in), C(in));
	matrix minf(R(in), C(in));

	if (SIZE((&out)) < SIZE(in))
		out.reshape(D(in), R(in), C(in));

	for (int i = 0; i< D(in); i++)
	{
		inf.set_p(in->get_new_p_by_surface(i));
		minf.set_p(in->get_new_p_by_surface(i));

		max(&minf);

		elemt* p = new elemt[inf.get_alloc_size()];

		loop_two_matrix_extends(&inf, &minf, p, 0, 0, 1, 0, ROW((&inf)), 1, callback_for_minus_3);

		inf.set_p(p);

		matrix sinf(inf);

		loop_matrix(&inf, callback_for_exp);
		loop_matrix(&sinf, callback_for_exp);

		compact_matrix_by_col(&sinf);

		loop_two_matrix_extends(&inf, &sinf, inf.get_p(), 0, 0, 1, 0, ROW((&inf)), 1, callback_for_devide);

		out.set_surface_with_matrix(i, &inf);
	}

	out.init_val(D(in) , R(in) , C(in));

}

void mathm::numerical_gradient_softmax(cube* in , cube& out , int range)
{

}

void mathm::gradient_operation_by_row(cube* in , cube& out, CALLBACK_LOCAL_LOOP CC)
{
	cube rin(*in);
	cube rout(out);

	elemt* tp = new elemt[in->get_occupied()];

	elemt* rinp = rin.get_p();
	elemt* inp = in->get_p();

	elemt* routp = rout.get_p();
	elemt* outp = out.get_p();
	
	for (int k = 0; k < D(in) * C(in); k++)
	{
		for (int i = 0; i < R(in); i++)
		{
			*(rinp + i) -= mdef::h;
			CC(rinp, routp, R(in));

			*(outp + i) = *(routp + i);
			*(rinp + i) += mdef::h;
		}

		CC(rinp, tp, R(in));

		minus_in_range(tp, outp, outp, R(in));
		devide_in_range(outp, mdef::h, R(in));

		rinp += C_OFF(in);
		routp += C_OFF(in);

		inp += C_OFF(in);
		outp += C_OFF(in);
	}

}
