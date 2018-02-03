#pragma once
#include "enumdef.h"
#include "element.h"
#include <list>

namespace mdls
{
	enum NodeType
	{
		Perceptron = 0,
		Unbound = 1,
		Collector = 2
	};

	typedef void(*CollectorRoutine)(elemt** in , elemt* out, int flow);
	typedef void(*PerceptronRoutine)(elemt*, elemt*, elemt*);
	typedef void(*Activation)(elemt*, elemt*);
	class vHolder
	{
		elemt *nd;

		int now;
		int epochs;
		int ops;

		std::list<elemt> eList;

	public:
		vHolder(elemt* n) :
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
		};

		void set_epochs(int epc)
		{
			epochs = epc;
		}

		void avante()
		{
			now++;
			eList.push_back(*nd);
		}

	};
	class Node
	{
		friend class Placer;

		Node* back;

		CollectorRoutine cRoutine;
		PerceptronRoutine pRoutine;
		
		Activation activation;

		int outIndex;
		int NodeCount;

		elemt iDiff;
		elemt wDiff;

		elemt** relNode;
		vHolder* hd;

		int index_in_layer;
		NodeType type;

		elemt out;
		int assign;

	public:
		inline void set_proutine(PerceptronRoutine routine)
		{
			this->pRoutine = routine;
		}
		inline void set_croutine(CollectorRoutine routine)
		{
			this->cRoutine = routine;
		}

		void set_flow(int flow) {
			NodeCount = flow;
		};

		Node(NodeType t) :
			assign(0),
			type(t)
		{
			if (t == NodeType::Perceptron)
			{
				NodeCount = 3;
				relNode = new elemt*[3];
				outIndex = NodeCount - 1;
				relNode[outIndex] = &out;
				hd = new vHolder[3];
			}
		}

		void make_this_collector(int flow)
		{
			NodeCount = flow;
			free(relNode);
			relNode = new elemt*[NodeCount + 1];
			outIndex = flow;
			relNode[outIndex] = &out;
			assign = 0;
			type = NodeType::Collector;
		}

		void make_this_perceptron()
		{
			NodeCount = 3;
			relNode = new elemt*[3];
			outIndex = NodeCount - 1;
			relNode[outIndex] = &out;
			type = NodeType::Perceptron;
		}
		void make_this_bounder()
		{
			NodeCount = 2;
			relNode = new elemt*[2];
			outIndex = 2 - 1;
			relNode[outIndex] = &out;
			type = NodeType::Unbound;
		}

		Node(const Node* n) :
			NodeCount(n->NodeCount),
			outIndex(n->outIndex),
			assign(0)
		{
			relNode = new elemt*[NodeCount];

			for (int i = 0; i < NodeCount; i++)
			{
				relNode[i] = n->relNode[i];
			}
		}

		Node(const Node& n) :
			NodeCount(n.NodeCount),
			outIndex(n.outIndex),
			assign(0)
		{
			relNode = new elemt*[NodeCount];

			for (int i = 0; i < NodeCount; i++)
			{
				memcpy(relNode[i], n.relNode[i], sizeof(elemt));
			}
		}
		
		Node& operator=(elemt value)
		{
			*relNode[2] = value;
			return *this;
		}

		Node& operator+=(const Node& n)
		{
			if (assign >= NodeCount)
			{
				cout << "Node overflow exeption" << endl;
				return *this;
			}
			relNode[assign++] = n.relNode[n.outIndex];
			return *this;
		}

		Node(int count) :
			NodeCount(count),
			outIndex(count),
			assign(0)
		{
			relNode = new elemt*[count + 1];
			relNode[count] = &out;
		}

		Node() :
			NodeCount(3),
			outIndex(2)
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
			if (type == NodeType::Perceptron)
				pRoutine(relNode[0], relNode[1], relNode[2]);
			else if (type == NodeType::Collector)
				cRoutine(relNode, relNode[outIndex], NodeCount - 1);
			else if (type == NodeType::Unbound)
				activation(relNode[0], relNode[1]);
		}

		inline void set_node_count(int c)
		{
			NodeCount = c;
		}

		elemt get_out()
		{
			return *relNode[outIndex];
		}
		elemt* get_out_addres()
		{
			return relNode[outIndex];
		}

		void set_elemtp_by_col(elemt* p, int idx)
		{
			relNode[idx] = p;
		};
			
		typedef void(*nCall)( elemt** te, Node* narr);

		void call_by_interval(int interval,int rinterval, Node* p, nCall n)
		{
			elemt** ep = relNode;

			for(int i = 0; i < interval; i++, p++)
				for (int j = 0; j < rinterval; j++, ep++)
				{
					n(ep++, p);
				}
		}
		void call_by_rinterval(int interval, int rinterval, Node* p, nCall n)
		{
			elemt** ep = relNode;

			for (int i = 0; i < interval; i++, ep++)
				for(int j = 0; j < rinterval;  j++, p++)
			{
					n(ep, p);
			}
		
		}
		void set_activation(Activation a)
		{
			activation = a;
		}
		void activate()
		{
			activation(relNode[0], relNode[1]);
		}

	};

	typedef void(*placerRoutine) (Node** p, int nodeCount, elemt* out);
		
	class Placer
	{
		int flow;
		int nodes;

		placerRoutine routine;
		Activation activation;

	public:
		void set_placer_routine(placerRoutine n) { this->routine = n; };
		void set_back_dens(Node* inNode, Node* colNode, Node* outNode, int ncount , int flow)
		{
			int j = 0;
			for (int i = 0; i < ncount; i++)
			{
				for (int k = 0; k < flow; k++, j++)
				{
					inNode[j].back = &colNode[i];
				}

				colNode[i].back = &outNode[i];
			}
		}
		
		void bind_out_node_dens(Node* inNode, Node* colNode, Node* outNode, int ncount, int flow)
		{
			Node* fn = inNode;
			Node* on = outNode;

			int idx = 0;

			for (int i = 0; i < ncount; i++, on++)
			{
				colNode[i].make_this_collector(flow);

				for (int j = 0; j < flow; j++, fn++)
				{
					colNode[i] += (*fn);
				}

				on->relNode[0] = colNode[i].relNode[flow];
				colNode[i].back = on;
			}
		}


		Placer()
		{
		}
		
		Placer(int flow, int nodeCount) :
			nodes(nodeCount),
			flow(flow)
		{
		}

		Placer(int flow) :
			flow(flow)
 		{
		}


		void set_out_node(elemt* p)
		{

		}
	};

	class NodeMap
	{
		NodeMap* prev;
		NodeMap* next;
		
		int idxTensor;
		int pCount;
		
		Node* iMap;
		Node* wMap;
		Node* oMap;

		Placer* placer;

		shape nodeShape;
		shape nodeWShape;
		shape nodeOShape;
		
		int nodeCount;
		int epoch;

	public:
		Node * get_node(int idx) { return &iMap[idx]; };
		int get_node_count() { return nodeCount; };
		NodeMap() :
			nodeShape(shape()),
			nodeWShape(shape()),
			nodeOShape(shape()),
			epoch(0)
		{

		}

		NodeMap(shape in , shape wei , shape out) :
			nodeShape(in),
			nodeWShape(wei),
			nodeOShape(out),
			nodeCount(wei.Alloc_size * in.Alloc_size),
			epoch(0)
		{
			iMap = new Node[nodeShape.Alloc_size * wei.Alloc_size];
			wMap = new Node[nodeWShape.Alloc_size];
			oMap = new Node[nodeOShape.Alloc_size];
		}

		NodeMap(shape sh) :
			nodeShape(sh),
			nodeCount(sh.Alloc_size),
			epoch(0)

		{

		}

		NodeMap(int count) :
			nodeCount(count),
			nodeShape(1, 1, 1, count)
		{
			iMap = new Node[count];
		}

		void set_node_info()
		{

		}

		void convert_inodes_to_perceptron()
		{
			for (int i = 0; i < nodeCount; i++)
			{
				iMap[i].make_this_perceptron();
			}
		
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

		void set_out_node_dens(elemt* p)
		{
			for (int i = 0; i < nodeOShape.Alloc_size; i++)
			{
				oMap[i].make_this_bounder();
				oMap[i].set_elemtp_by_col(p++ , 1);
			}
		}

		
		void set_inodes_proutine(PerceptronRoutine routine)
		{
			for (int i = 0; i < nodeCount; i++)
				iMap[i].set_proutine(routine);
		}
		void set_wnodes_croutine(CollectorRoutine routine)
		{
			for (int i = 0; i < nodeWShape.Alloc_size; i++)
				wMap[i].set_croutine(routine);
		}

		void operate_nodes()
		{
			epoch++;
			act_all_nodes();
			activate_all_nodes(nodeOShape.Alloc_size);
		}

		void set_placer_dens(int flowPerNode ,int interval)
		{
			placer = new Placer(flowPerNode, interval);
			placer->bind_out_node_dens(iMap, wMap, oMap, interval, flowPerNode);
		}

		void act_all_nodes()
		{
			for (int i = 0; i < nodeCount; i++)
				iMap[i].act();
			act_col_node();
		}


		void act_col_node()
		{
			for (int i = 0; i < nodeWShape.Alloc_size; i++)
			{
				wMap[i].act();
			}
		}

		void activate_all_nodes(int ncount)
		{
			for (int i = 0; i < ncount; i++)
			{
				oMap[i].activate();
			}

		}
		void set_activation(int ncount, Activation a)
		{
			for (int i = 0; i < ncount; i++)
				oMap[i].set_activation(a);
		}

	};

};

