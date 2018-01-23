#pragma once

#include <iostream>
#include <cstring>
#include "enumdef.h"
#include <functional>



#ifndef LOG_LEVEL 
#define LOG_LEVEL 0

#endif

namespace mdls
{
	class vHolder
	{
		Node* nd;
		std::pair<int,elemt>* buffer;
		
		int now;
		int epochs;
		int ops;

	public :
		vHolder(Node* n) :
			now(0),
			nd(n)
		{

		}
		vHolder() :
			now(0)
		{

		}

		vHolder(int epc) :
			now(0),
			epochs(epc)
		{
			buffer = new std::pair<int , elemt>[epc];
		};

		void set_epochs(int epc)
		{
			epochs = epc;
			buffer = new std::pair<int, elemt>[epc];
		}

		void avante()
		{
			buffer[now++] = std::pair<int, elemt>(now, nd->get_out());
		}

	};


	class Node
	{
		int NodeCount;
		
		elemt** relNode;

		int index_in_layer;
		int type;
	
		elemt out;
	public :
		Node()
		{

		}

		elemt get_out()
		{
			return out;
		}
		void set_elemtp_by_col(elemt* p , int idx)
		{
			relNode[idx] = p;

		};

};

	class NodeMap
	{
		int idxTensor;
		
		int countInNode;
		int countWeiNode;
		
		Node* iMap;
		vHolder* hMap;

	public:

		NodeMap(int count) 
		{
			iMap = new Node[count];
			hMap = new vHolder[count];

		}

		NodeMap(int count, elemt** p0, elemt** p1)
		{
			Node* pMap = iMap;

			elemt* pp0 = *p0;
			elemt* pp1 = *p1;

			for (int i = 0; i < count; i++,pp0++)
				pMap->set_elemtp_by_col(pp0,0);
			for (int i = 0; i < count; i++, pp1++)
				pMap->set_elemtp_by_col(pp1, 1);
		}

	};

	using namespace std;

#define eTask_lambda [](elemt* p)->void
#define eeTask_lambda [](elemt* p, elemt* opts)->void
#define p_0 p
#define p_1 opts

	typedef void(*eTask)(elemt* p);
	typedef void(*evTask)(elemt* p, elemt v);
	typedef void(*eeTask_chn)(elemt* p, elemt* v);

#define C_OFF(c) c->get_col_offset()
#define R_OFF(c) 1
#define D_OFF(c) c->get_channel_offset()
#define B_OFF(c) c->get_batch_offset()

#define B(c) c->get_batch_count()
#define C(c) c->get_col_count()
#define R(c) c->get_row_count()
#define D(c) c->get_channel_count()

#define A(c) c->get_alloc_size()

#define EQUL(c,cc) ( ( B(c) == B(cc)) &&( D(c) == D(cc) ) && ( R(c) == R(cc) ) && ( C(c) == C(cc) ) )
#define SIZE(c) ( B(c) * R(c) * D(c)  * C(c) )

	typedef Dm* dformat;

	struct shape
	{
		int Row;
		int Col;
		int Channel;
		int Batch;
		int Alloc_size;

		int* p[4];

		shape& operator=(const shape& s)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			*p[0] = *s.p[n0];
			*p[1] = *s.p[n1];
			*p[2] = *s.p[n2];
			*p[3] = *s.p[n3];

			Alloc_size = Batch * Row * Col * Channel;
		
			return *this;
		}

		shape() :
			Row(0),
			Col(0),
			Channel(0),
			Batch(0)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			Alloc_size = Row* Col * Channel * Batch;
		}


		shape(int bat, int chn, int col, int row) :
			Row(row),
			Col(col),
			Channel(chn),
			Batch(bat)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			Alloc_size = Row* Col * Channel * Batch;
		}

		shape(const	shape& s) 
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			*p[0] = *s.p[0];
			*p[1] = *s.p[1];
			*p[2] = *s.p[2];
			*p[3] = *s.p[3];

			Alloc_size = Batch * Row * Col * Channel;

		}

		shape(const shape& s, Dm n0, Dm n1, Dm n2, Dm n3) 
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			*p[0] = *s.p[n0];
			*p[1] = *s.p[n1];
			*p[2] = *s.p[n2];
			*p[3] = *s.p[n3];		

			Alloc_size = Row* Col * Channel * Batch;
		}

		bool operator==(const shape& rhs)
		{
			for (int i = 0; i < 4; i++)
			{
				if (*p[i] != *rhs.p[i])
					return false;
			}

			return true;
		}

		bool operator!=(const shape& rhs)
		{
			return !operator==(rhs);
		}
	};
	
	

	struct offset
	{
		int Row;
		int Col;
		int Channel;
		int Batch;

		int* p[4];

		offset() :
			Row(0),
			Col(0),
			Channel(0),
			Batch(0)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;
		}

		offset& operator=(const offset& o)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			Batch = o.Batch;
			Channel = o.Channel;
			Col = o.Col;
			Row = o.Row;

			return *this;
		}

		offset(const offset& o)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;
		
			Batch = o.Batch;
			Channel = o.Channel;
			Col = o.Col;
			Row = o.Row;
		}

		offset(const shape& s) :
			Row(1)
		{
			p[0] = &Batch;
			p[1] = &Channel;
			p[2] = &Col;
			p[3] = &Row;

			Col = s.Row;
			Channel = Col * s.Col;
			Batch = Channel * s.Channel;

		}

	};


	class element
	{

	protected:
		elemt* _pv;

		shape _eshape;
		offset _offset;

		size_t _alloc_size;
		std::string _tag;

	public:

		element();
		element(int, int, int, int);
		element(const element&);

		shape get_shape() const { return _eshape; };
		std::string get_tag() { return _tag; };

		element& operator= (const element&);

		int get_row_count() const { return _eshape.Row; };
		int get_col_count() const { return _eshape.Col; };
		int get_channel_count() const { return _eshape.Channel; };
		int get_batch_count() const { return _eshape.Batch; };

		int get_col_offset() const { return _offset.Col; };
		int get_row_offset() const { return _offset.Row; };
		int get_channel_offset() const { return _offset.Channel; };
		int get_batch_offset() const { return _offset.Batch; };

		void reshape(const shape& s) ;
		void init(const shape& s);

		void init__(const shape& s) { init(s); pv_alloc(s.Alloc_size); };

		void init(int n0, int n1, int n2, int n3);
		void init(elemt* p, int n0, int n1, int n2, int n3) {
			_pv = p; init(n0, n1, n2, n3);
		};

		inline void set_p(elemt* v) { _pv = v; };
		inline elemt* get_p() const  { return _pv; };

		void rotate(int);

		inline void pv_alloc(int count) { _pv = (elemt*)malloc(sizeof(elemt) * count); _eshape.Alloc_size = count; _alloc_size = count; };
		inline int get_alloc_size() const { return _eshape.Alloc_size; };

		void initialize_elem_to(elemt v);
		void set_tag(std::string s) { _tag = s; };
		
		void transpose(Dm n0, Dm n1, Dm n2, Dm n3);

		void apply_to_elem_by_channel(std::function<void (elemt* p , elemt* pp )> , elemt * opts);
		void apply_to_all_elem(std::function< void(elemt*) > a);
		void apply_with_equal_form(std::function< void(elemt* ,elemt*) >, const element& );
		void apply_to_elem_by_batch(std::function<void(elemt* p, elemt* pp)> f, elemt * opts);
	};

	void convert_index_by_rotate(int i, int& c, int& r, const shape& s);


}

