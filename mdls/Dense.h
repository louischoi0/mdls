#pragma once
#include "layert.h"
#include "of.h"
#include "node.h"

namespace mdls
{


	class Dense : public layer, public layerITF
	{
		int ncount;

	public:
		Dense(tensor* in, tensor* weight)
		{
			ncount = weight->get_row_count();
			_tensor_map[Input] = in;
			_tensor_map[Weight] = weight;
			initialize();


		}
		Dense(tensor* input, int out) :
			ncount(out)
		{
			_tensor_map[Input] = input;
			initialize();
		}

		void initialize()
		{
			set_up_nodes();
		}
		void procced_inverse()
		{


		}

		void test()
		{
			tensor* intens = _tensor_map[Input];
			shape inshape = intens->get_shape();

			int flow = inshape.Alloc_size* ncount;
			for (int i = 0; i < flow ; i ++)
			{
				cout << map->get_node(i)->get_x() << " , " << map->get_node(i)->get_y()<< endl;
			}
		}

		void set_up_nodes()
		{
			tensor* intens = _tensor_map[Input];			
			shape inshape = intens->get_shape();
			
			int flow =  inshape.Alloc_size* ncount;
			int flowPerNode = intens->get_alloc_size();

			elemt* intensp = _tensor_map[Input]->get_p();
			elemt* wtensp = _tensor_map[Weight]->get_p();
			_tensor_map[Output]->reshape(shape(1, 1, 1, ncount));

			elemt* otensp = _tensor_map[Output]->get_p();

			map = new NodeMap(flow);

			map->set_node_address_x_by_interval(flowPerNode, intensp);
			map->set_node_address_y_by_interval(flowPerNode, wtensp);
			map->set_nodes_routine([](elemt* p, elemt* pp, elemt* out)->void {
				*out = *p *  *pp;
			});
		
			map->set_out_node_dens(ncount, otensp);
			//map->set_placer_dens(flowPerNode, ncount, );
		}
		
		void tanh(elemt* p , elemt* pp )
		{

		}


		void proceed()
		{
			initialize();

			elemt* wp = _tensor_map[Weight]->get_p();

			tfunc::reduced_sum(target_dm::Batch, _tensor_map[Input], *_tensor_map[Output]);
			tfunc::channel_times(*_tensor_map[Output], ncount);

			_tensor_map[Output]->apply_to_elem_by_channel([](elemt* p, elemt* pp)->void {
				*p *= *pp;
			}, wp);

		};

		void proceed_inverse() {};






	};






}