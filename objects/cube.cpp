#include "cube.h"

mdls::cube::cube() :
element(),
local_param(new int[5]{0,0,0,0,0})
{

}

mdls::cube::cube(const cube* c)
{
	memcpy(this, c, sizeof(cube));
	
	_pv = D_MALC(_eshape.Alloc_size);

	memcpy(_pv, c->_pv, sizeof(elemt) * _eshape.Alloc_size);
	local_param = new int[5];
	
	memcpy(local_param, c->local_param , sizeof(int) * 5);	
}

mdls::cube::cube(int depth, int row, int col) :
element(1 , row , col , depth),

local_param(new int[5]{ 0,0,0,0,0 })
{
	initialize_cube();
}
/*
cube& mdls::cube::operator=(cube& c)
{
	memcpy(this, &c, sizeof(cube));
	
	free(_pv);
	delete[] local_param;
	
	_pv = D_MALC(_eshape.Alloc_size);
	local_param = new int[5]{ 0, };

	memcpy(_pv, c._pv, sizeof(elemt) * _occupied_size);
	memcpy(local_param, c.local_param, sizeof(int) * 5);

	return *this;
}
*/
void mdls::cube::matrix_to_cube(const matrix* rm)
{
	//reshape(1, rm->get_row_count(), rm->get_col_count());
	
	elemt* p = _pv;
	elemt* pp = rm->get_p();

	for (int i = 0; i < _eshape.Row * _eshape.Col; i++)
		*p++ = *pp++;

}

void mdls::cube::initialize_cube_ladder()
{
	elemt* p = _pv;
	
	for( int i = 0 ; i < _eshape.Channel; i++)
		for(int j = 0 ; j < _eshape.Col ; j++)
			for (int r = 0; r < _eshape.Row; r++)
			{
				*p++ = i * 100 + j * 10 + r;
			}
}

void mdls::cube::initialize_cube(elemt a)
{
	elemt* p = _pv;
	for (int i = 0; i < _eshape.Alloc_size ; i++)
		*p++ = a;
}


void mdls::cube::initialize_cube()
{
	elemt* p = _pv;
	for (int i = 0; i < _eshape.Alloc_size; i++)
		*p++ = 0;

}
using namespace mdls;

void cube::init_val(int depth , int row, int col)
{
	_eshape.Row = row;
	_eshape.Col = col;
	_eshape.Channel = depth;
	_eshape.Alloc_size = row * col * depth;
}
void cube::init_val(elemt* p, int depth , int row, int col)
{
	free(_pv);

	_pv = p;
	_eshape.Col = col;
	_eshape.Row = row;
	_eshape.Channel = depth;
	_eshape.Alloc_size = col* row * depth;

}
void cube::reshape(int row, int col)
{
	if (row * col  > _eshape.Alloc_size)
	{
		elemt* p;
		p = (elemt*)(malloc(sizeof(elemt) * col * row ));
		memcpy(p, _pv, sizeof(elemt) * _eshape.Alloc_size);
		free(_pv);
		_pv = p;
		_eshape.Alloc_size = col * row;
	}

	init_val(1, row, col);
}

void cube::reshape(int depth, int row, int col)
{
	if (row * col * depth > _eshape.Alloc_size)
	{
		elemt* p;
		p = (elemt*)(malloc(sizeof(elemt) * col * row * depth));
		memcpy(p, _pv, sizeof(elemt) * _eshape.Alloc_size);
		free(_pv);
		_pv = p;
		_eshape.Alloc_size = col * row * depth;
	}

	init_val(depth, row, col);
}

void cube::set_surface_with_matrix(int surface , matrix* m)
{
	if (surface >= _eshape.Channel)
		return;

	elemt*p = m->get_p();
	elemt* _ppv = _pv + D_OFF(this) * surface;

	if (SIZE(m) == D_OFF(this))
	{
		memcpy(_ppv, p, sizeof(elemt) * D_OFF(this));
	}

	else 
	{
		if (SIZE(m) > D_OFF(this))
		{
			memcpy(_ppv, p, sizeof(elemt) * D_OFF(this));
		}

		else
		{
			memcpy(_ppv, p, sizeof(elemt) * SIZE(m));
		}
	}

}

void cube::split_to_matrix_by_col(int depth, int row_index, elemt* out)
{
	elemt* p = _pv + (depth * D_OFF(this)) + (row_index * C_OFF(this));

	for (int i = 0; i < _eshape.Col; i++)
	{
		*out++ = *p;
		p += C_OFF(this);
	}
}

void cube::split_to_matrix_by_row(int depth, int col_index, elemt* out)
{
	elemt* p = _pv + (depth * D_OFF(this)) + (col_index * C_OFF(this));

	for (int i = 0; i < _eshape.Row; i++)
	{
		*out++ = *p++;
	}
}

void cube::col_to_surface_row_to_depth(cube_iter& out, int col_to_row)
{
	elemt* copi;

	elemt* op = _pv;

	elemt* oop = op;
	elemt* ooop = op;

	out = new cube*[ _eshape.Channel ];

	for (int i = 0; i < _eshape.Channel ; i++)
	{
		out[i] = new cube(_eshape.Row, _eshape.Col / col_to_row, col_to_row);

		copi = out[i]->get_p();
		oop = op;

		for (int j = 0; j < _eshape.Row; j++)
		{
			ooop = oop;
			
			for (int k = 0; k < _eshape.Col; k++)
			{
				*copi++ = *ooop;
				ooop += C_OFF(this);
			}

			oop++;


		}

		op += D_OFF(this);
	}


}
void cube::pass_param(int i, int ii, int iii, int iiii, int iiiii)
{
	local_param[0] = i;

	local_param[1] = ii;
	local_param[2] = iii;
	local_param[3] = iiii;
	local_param[4] = iiiii;
}

elemt* cube::get_new_p_by_surface(int depth)
{
	elemt* p = D_MALC(_eshape.Row * _eshape.Col);

	memcpy(p, _pv + depth * (D_OFF(this)) , sizeof(elemt) * _eshape.Row * _eshape.Col );
	
	return p;
}

void cube::set_surface_with_p(int surface, elemt* p)
{

}

elemt* cube::get(int d, int r, int c)
{

	return ( _pv + D_OFF(this) * C_OFF(this) + R_OFF(this));
}


elemt* cube::get(int inx)
{
	return _pv + inx;
}

void cube::set(int d, int r, int c, elemt val)
{
	*(_pv + D_OFF(this) * C_OFF(this) + R_OFF(this)) = val;
}
void cube::set(int inx, elemt val)
{
	*(_pv + inx) = val;
}



void cube::transpose(target_dm n1 , target_dm n2 , target_dm n3)
{
	element::transpose( Dm::Batch , n1 , n2 , n3);
}

void cube::add(elemt v)
{
	for (int i = 0; i < get_count(); i++)
		_pv[i] += v;

}


void cube::multiply(elemt v)
{
	for (int i = 0; i < get_count(); i++)
		_pv[i] *= v;
}

