#pragma once
#include "enumdef.h"
#include "element.h"

namespace mdls
{

	typedef void(*layerRoutine)(elemt*, elemt*, elemt*);
	typedef void(*Activation)(elemt*, elemt*);

	class Node
	{
		layerRoutine routine;

		int NodeCount;
		elemt** relNode;

		int index_in_layer;
		int type;

		elemt out;
	public:
		inline void set_routine(layerRoutine routine)
		{
			this->routine = routine;
		}

		void set_flow(int flow) {
			NodeCount = flow;
		};

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
		int get_x()
		{	
			return *relNode[0];
		}
		int get_y()
		{
			return *relNode[1];
		}

		void set_up(int flow , elemt* p)
		{
			if (NodeCount < flow)
				return;

			for (int i = 0; i < flow; i++)
				relNode[i] = p++;
		}

		void act()
		{
			routine(relNode[0], relNode[1], relNode[2]);
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

	typedef void(*placerRoutine) (elemt** p, int nodeCount, elemt* out);

	class Placer
	{
		Node** relNode;
		Node* outNode;

		int flow;

		placerRoutine routine;
		Activation activation;
	public:
		void act()
		{
		};

		Placer()
		{
		}

		Placer(int flow) :
			flow(flow)
 		{
			relNode = new Node*[flow];
		}

		void init_rel_node(int flow, Node* p)
		{
			relNode = new Node*[flow];
			for (int i = 0; i < flow; i++)
				relNode[i] = p++;
		}

		void set_out_node(elemt* p)
		{

		}
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
		int pCount;

		Node* iMap;
		Node* oMap;

		Placer* placer;

		vHolder* hMap;

		shape nodeShape;
		int nodeCount;


	public:
		Node * get_node(int idx) { return &iMap[idx]; };
		int get_node_count() { return nodeCount; };

		NodeMap(shape sh) :
			nodeShape(sh),
			nodeCount(sh.Alloc_size)
		{

		}

		NodeMap(int count) :
			nodeCount(count),
			nodeShape(1, 1, 1, count)
		{
			iMap = new Node[count];
			hMap = new vHolder[count];
		}

		void set_node_address_x_by_interval(int interval, elemt* p)
		{
			elemt* pp = p;
			int j = 0;
			
			for (int i = 0; i < nodeCount / interval; i++)
			{
				pp = p;
				for (int k =0; k < interval; k++, j++)
					iMap[j].set_elemtp_by_col(pp++, 0);
			}
		}
		void set_node_address_y_by_interval(int interval, elemt* p)
		{
			elemt* pp = p;
			int j = 0;

			for (int i = 0; i < nodeCount / interval; i++,p++)
				for (int k =0; k < interval; k++ , j++)
					iMap[j].set_elemtp_by_col(p, 1);
		}

		void set_placer_routine(layerRoutine routine)
		{

		}
		
		void set_nodes_routine(layerRoutine routine)
		{
			for (int i = 0; i < nodeCount; i++)
				iMap[i].set_routine(routine);
		}

		void set_out_node_dens(int ncount, elemt* p)
		{
		}

		void set_placer_dens(int flowPerNode ,int interval, Node* p)
		{
		}

		void set_dens()
		{

		}
		
		void act_all_nodes()
		{
			for (int i = 0; i < nodeCount; i++)
				iMap[i].act();
		}
	
	};


};

