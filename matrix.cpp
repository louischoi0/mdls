#include "matrix.h"


using namespace mathm;
using namespace mdls;

mathm::matrix_db::~matrix_db()
{
	free(_pv);
}

mathm::matrix_db::matrix_db() :
element()
{

}

mathm::matrix_db::matrix_db(int len, int hei) :
element(1, 1, len, hei)
{

}

matrix_db::matrix_db(const matrix& a) 
{
	pv_alloc(a._eshape.Alloc_size);
	memcpy(_pv, a._pv, sizeof(elemt) * a._eshape.Alloc_size);

	element::init(a._eshape);
}

void mathm::matrix::init_val(int row, int col)
{
	element::init(1, 1, col, row);
}


void matrix::copy_p(elemt* p, int row, int col)
{
	size_t size = row * col;

	elemt* pval = (elemt*)malloc(sizeof(elemt) * size);
	memcpy(pval, p, sizeof(elemt) * size);

	element::init(pval, 1 , 1, row, col);
}

void mathm::matrix_db::init_val(elemt* p, int row, int col)
{
	free(_pv);

	element::init(1,1,col , row);

	set_p(p);
	_alloc_size = row * col;
}

void mathm::matrix::resize(int row , int col)
{
	if ( _alloc_size < row * col)
	{
		elemt* p;
		
		int initsize = sizeof(elemt) * col * row;

		p = (elemt*)(malloc(initsize));

		memcpy(p, _pv, _alloc_size );
		
		free(_pv);

		_pv = p;
		_alloc_size = col * row;
	}

	init_val(row, col);
}

void mathm::matrix_db::init_val()
{
	elemt* _pp = _pv;

	for (int i = 0; i < get_col_count() ; i++)
		for (int j = 0; j < get_row_count() ; j++)
			*_pp++ = 0;

}

void mathm::matrix_db::read_and_write(int x, int y, int* out)
{
	int v = get_v(x, y);
	*out = v;
}

matrix& matrix_db::operator=( const matrix& rhs )
{
	element::init__(rhs._eshape);
	memcpy(_pv , rhs._pv , sizeof(elemt) * rhs._eshape.Alloc_size);
	
	_tag = rhs._tag;

	return *this;
}


void mathm::matrix::append_to_col(elemt* p , int r_size, int c_size)
{
	if (r_size != _eshape.Row)
	{
		cout << "row size incorrect" << endl;
		return;
	}
	
	int osz = _eshape.Row * _eshape.Col;
	int nsz = osz + (r_size * c_size);

	elemt* new_p = D_MALC(nsz);
	
	memcpy( new_p, _pv, osz * sizeof(elemt));
	memcpy( new_p + osz , p ,  (nsz - osz) * sizeof(elemt) );

	free(_pv);

	_pv = new_p;
	_alloc_size += r_size * c_size;

	_eshape.Col += c_size;
}

void mathm::matrix::append_to_row(elemt* p, int r_size, int c_size)
{
	if (c_size != _eshape.Col)
	{
		cout << "col size incorrect" << endl;
		return;
	}
	
	matrix tm(_eshape.Col , _eshape.Row);
	mathm::transpose(this, tm);
	
	elemt* tp = tm.get_p();
	elemt* new_p = new elemt[( _eshape.Col * _eshape.Row + r_size * c_size)];

	memcpy(new_p, tp, sizeof(elemt) * _eshape.Col * _eshape.Row);
	memcpy(new_p + (_eshape.Col * _eshape.Row) , p, sizeof(elemt) * r_size * c_size);
	
	tm.add_state_variable( target_dm::Col , r_size);
	tm.set_p(new_p);

	mathm::transpose(&tm, *this);
}


void mathm::matrix::add_state_variable(target_dm pos, int val)
{
	if (pos == Row)
		_eshape.Row += val;
	else if (pos == Col)
		_eshape.Col += val;

	else if (pos == Volume)
		_alloc_size += val;

	else
		return;
}

