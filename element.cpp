#include "element.h"

using namespace mdls;

element::element(int n0, int n1, int n2, int n3) :
	_eshape(n0, n1, n2, n3),
	_offset(_eshape)
{
	pv_alloc(n0 * n1 * n2 * n3);
}

element::element() :
	_pv(new elemt[1]),
	_eshape(0,0,0,0),
	_offset(_eshape)
{
	_alloc_size = 1;
}


void element::init(const shape& s)
{
	_eshape = shape(s);
	_offset = offset(s);

	_alloc_size = s.Batch * s.Channel * s.Col *s.Row;
}

void element::init(int n0, int n1, int n2, int n3)
{
	_eshape = shape(n0, n1, n2, n3);
	_offset = offset(_eshape);

	_alloc_size = _eshape.Batch * _eshape.Channel * _eshape.Col * _eshape.Row;
}


void element::initialize_elem_to(elemt v)
{
	elemt* p = _pv;

	for (int i = 0; i < _eshape.Alloc_size; i++)
		*p++ = v;

}

void element::reshape(const shape& s)
{
	if (_eshape.Alloc_size >= s.Alloc_size)
	{
		init(s);
		return;
	}
	
	else
	{
		elemt* t = new elemt[ s.Alloc_size ];
		memcpy(t, _pv, sizeof(elemt) * _eshape.Alloc_size);

		free(_pv);
		_pv = (elemt*)malloc(sizeof(elemt) * s.Alloc_size);

		memcpy(_pv, t, sizeof(elemt) * _eshape.Alloc_size);

		init(s);
	}

	
}

void element::transpose(Dm n0, Dm n1, Dm n2, Dm n3)
{
	shape new_shape = shape(_eshape, n0, n1, n2, n3);
	offset new_offset = offset(new_shape);

	elemt* ppv = _pv;
	elemt* temp = new elemt[new_shape.Alloc_size];
	elemt* res = temp;
	 
	for (int y = 0; y < _eshape.Batch; y++)
	{
		elemt* lttt = temp;
		elemt* oppp = ppv;

		for (int k = 0; k < _eshape.Channel; k++)
		{
			elemt* ltt = lttt;
			elemt* opp = oppp;

			for (int j = 0; j < _eshape.Col; j++)
			{
				elemt* lt = ltt;
				elemt* op = opp;

				for (int i = 0; i < _eshape.Row; i++)
				{
					*lt = *op;

					lt += *new_offset.p[n3];
					op++;
				}

				ltt += *new_offset.p[n2];
				opp += *_offset.p[2];
			}

			lttt += *new_offset.p[n1];
			oppp += *_offset.p[1];

		}

		temp += *new_offset.p[n0];
		ppv += *_offset.p[0];
	}

	memcpy(_pv, res, sizeof(elemt) * _eshape.Alloc_size);
	init(new_shape);

}

element::element(const element& t)
{
	init(t._eshape);
	pv_alloc(t._eshape.Alloc_size);

	memcpy(_pv, t._pv, sizeof(elemt) * _eshape.Alloc_size);
}

element& element::operator=(const element& t)
{
	reshape(t._eshape);
	memcpy(_pv, t._pv, sizeof(elemt) * _eshape.Alloc_size);

	return *this;
}

void element::rotate(int rt)
{
	elemt* ppv = new elemt[_eshape.Alloc_size];

	elemt* p = ppv;
	elemt* pp = _pv;

	elemt* bp = p;
	elemt* bpp = pp;

	shape csh(_eshape);
	offset coff(_offset);

	if (rt % 2 == 0)
	{
		csh = shape(csh.Batch , csh.Channel , csh.Row, csh.Col);
		coff = offset(csh);
	}

	int row = 0;
	int col = 0;

	int crow = 0;
	int ccol = 0;

	int lc = 0;
	int clc = 0;
	int index = 0;
	int nindex = 0;
	
	for (int y = 0; y < B(this); y++)
	{
		p = bp;
		pp = bpp;

		for (int j = 0; j < D(this) ; j++)
		{
			for (int j = 0; j < C(this); j++)
			{
				for (int i = 0; i < R(this); i++)
				{
					ccol = col;
					crow = row;

					convert_index_by_rotate(rt, ccol, crow, csh);
					
					index = col * _offset.Col + row;
					nindex = ccol * coff.Col + crow;

					*(p + nindex) = *(pp + index);

					row++;
				}
				row = 0;
				col++;			
			}

			row = 0;
			col = 0;

			p += coff.Channel;
			pp += coff.Channel;
		}

		bpp += B_OFF(this);
		bp += B_OFF(this);
	}

	memcpy(_pv, ppv, sizeof(elemt)* _eshape.Alloc_size);
	init(csh);

}

void mdls::convert_index_by_rotate(int i, int& c, int& r, const shape& s)
{
	int col = c;
	int row = r;

	int mcol = s.Col;
	int mrow = s.Row;

	if (i == Rotate::D_90)
	{
		c = row;
		r = mrow - col - 1;
	}

	else if (i == Rotate::D_180)
	{
		r = mrow - row - 1;
		c = mcol - col - 1;
	}

	else if (i == Rotate::D_270)
	{
		c = mcol - row - 1;
		r = col;
	}

	if (LOG_LEVEL == 1)
		std::cout << col << " , " << row << " to " << c << " , " << r << std::endl;

	else
		return;
}




void element::apply_to_elem_by_channel(std::function<void(elemt* p, elemt* pp)> f, elemt * opts)
{
	elemt* p = _pv;
	elemt* op = opts;
	for (int i = 0; i < _eshape.Col * _eshape.Row; i++)
	{
		opts = op;
		
		for (int j = 0; j < _eshape.Batch * _eshape.Channel; j++)
		{
			f( ( p + j * D_OFF(this) ) , opts);
			opts++;
		}

		p++;
	}

}

void element::apply_to_all_elem(std::function<void(elemt*)> f)
{
	elemt* p = _pv;

	for (int i = 0; i < _eshape.Alloc_size; i++)
		f(p++);

}

void element::apply_with_equal_form(std::function< void(elemt*, elemt* )> f, const element& e)
{
	elemt* p = _pv;
	elemt* pp = e.get_p();

	if ( _eshape.Alloc_size != e.get_alloc_size())
	{
		return;
	}

	for (int i = 0; i < _eshape.Alloc_size; i++)
		f(p++, pp++);

}
