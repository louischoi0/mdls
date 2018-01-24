#pragma once
#include "enumdef.h"
#include "element.h"

namespace mdls
{


	struct nodeDescription
	{
		int activation;
		int routine;


	};

	class Node
	{
		int NodeCount;

		elemt** relNode;

		int index_in_layer;
		int type;

		elemt out;
	public:
		Node(int count) :
			NodeCount(count)
		{
			relNode = new elemt*[count + 1];
			relNode[count] = &out;
		}

		Node()
		{
			relNode = new elemt*[3];
			relNode[2] = &out;
		}
		void act()
		{
			auto add = [](elemt** p)->void
			{
				**(p + 2) = **p + **(p + 1);
			};

			add(relNode);
		}

		elemt get_out()
		{
			return out;
		}
		elemt* get_out_addres()
		{
			return &out;
		}

		void set_elemtp_by_col(elemt* p, int idx)
		{
			relNode[idx] = p;
		};

	};
	class vHolder
	{
		Node* nd;
		std::pair<int, elemt>* buffer;

		int now;
		int epochs;
		int ops;

	public:
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
			buffer = new std::pair<int, elemt>[epc];
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

			for (int i = 0; i < count; i++, pp0++)
				pMap->set_elemtp_by_col(pp0, 0);
			for (int i = 0; i < count; i++, pp1++)
				pMap->set_elemtp_by_col(pp1, 1);
		}

	};

}