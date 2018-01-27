#pragma once
#include "enumdef.h"
#include "element.h"



namespace mdls
{
	enum NodeType
	{
		Perceptron = 0,
		Unbound = 1,
		Collector = 2
	};

	typedef void(*layerRoutine)(elemt*, elemt*, elemt*);
	typedef void(*Activation)(elemt*, elemt*);

	class Node
	{
		friend class Placer;

		layerRoutine routine;
		Activation activation;

		int outIndex;
		int NodeCount;

		elemt** relNode;

		int index_in_layer;
		NodeType type;

		elemt out;
		int assign;
	public:
		inline void set_routine(layerRoutine routine)
		{
			this->routine = routine;
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
			}

		}
		void make_this_collector(int flow)
		{
			NodeCount = flow;
			free(relNode);
			relNode = new elemt*[NodeCount +1];
			outIndex = flow;
			relNode[outIndex] = &out;
			assign = 0;
		}

		void make_this_perceptron()
		{
			NodeCount = 3;
			relNode = new elemt*[3];
			outIndex = NodeCount - 1;
			relNode[outIndex] = &out;
		}
		void make_this_bounder()
		{
			NodeCount = 2;
			relNode = new elemt*[2];
			outIndex = 2 - 1;
			relNode[outIndex] = &out;
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
			routine(relNode[0], relNode[1], relNode[2]);
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
	class IPlacer
	{

	};


	class Placer
	{
		Node** relNode;
		Node* colNode;

		int flow;
		int nodes;

		placerRoutine routine;
		Activation activation;
	
	public:
		void pact()
		{
			Node** rN = relNode;
			Node* cN = colNode;
			for (int i = 0; i < nodes; i++ , rN += flow, cN++)
			{
				routine(rN, flow, cN->relNode[cN->outIndex]);
			}
		}
		void set_placer_routine(placerRoutine n) { this->routine = n; };

		void bind_out_node(Node* outNode, int ncount , int flow)
		{
			Node** fn = relNode;
			Node* on = outNode;
			
			colNode = new Node[ncount];
			int idx = 0;
			
			set_placer_routine([](Node** rN, int flow, elemt* cN) ->void {
				*cN = 0;
				for (int i = 0; i < flow; i++, rN++)
				{
					*cN += (*rN)->get_out();
				}
			});

			for (int i = 0; i < ncount; i++,on++)
			{
				colNode[i].make_this_collector(flow);

				for (int j = 0; j < flow; j++, fn++)
						colNode[i] += (*fn);

				on->relNode[0] = colNode[i].relNode[flow];
			}
		}
		void activate()
		{
			Node* oN = colNode;
			for (int i = 0; i < nodes; i++)
			{
			
			}

		}

		void fully_connected(Node* flowNode, int ncount)
		{
			Node** n = relNode;
			Node* fn = flowNode;

			for (int i = 0; i < ncount; i++)
			{
				for (int j = 0; j < flow; j++)
				{
					*(n++) = new Node(fn++);
				}
			}
		}

		void test()
		{
			Node** np = relNode;
			Node* op = colNode;

			for (int i = 0; i < flow; i++)
			{
				for (int j = 0; j < nodes; j++)
				{
					cout << "x : " << *((*np)->relNode[0]) << "  y : " << *((*np)->relNode[1]) << "  out : " << *(*np)->relNode[2] << endl;
					np++;
				}

			}

		}

		void act()
		{
		};

		Placer()
		{
		}
		Placer(int flow, int nodeCount) :
			nodes(nodeCount),
			flow(flow)
		{
			relNode = new Node*[flow * nodeCount];
			colNode = new Node[nodeCount];
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

		void set_out_node_dens(int ncount, int flowPerNode ,elemt* p)
		{
			oMap = new Node[ncount];
			for (int i = 0; i < ncount; i++)
			{
				oMap[i].make_this_bounder();
				oMap[i].set_elemtp_by_col(p++ , 1);
			}
		}

		void set_placer_routine(layerRoutine routine)
		{

		}
		
		void set_nodes_routine(layerRoutine routine)
		{
			for (int i = 0; i < nodeCount; i++)
				iMap[i].set_routine(routine);
		}
		void operate_nodes()
		{
			act_all_nodes();
			placer->pact();
			activate_all_nodes(8);
		}	

		void set_placer_dens(int flowPerNode ,int interval)
		{
			placer = new Placer(flowPerNode, interval);
			placer->fully_connected(iMap, interval);
			placer->bind_out_node(oMap, interval, flowPerNode);
		}

		void act_all_nodes()
		{
			for (int i = 0; i < nodeCount; i++)
				iMap[i].act();

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