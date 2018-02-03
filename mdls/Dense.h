#pragma once
#include "layert.h"
#include "of.h"
#include "node.h"

namespace mdls
{
	class session;

	class Dense : public layer, public ILayer
	{
		int ncount;
		int flow;

	public:
		Dense(session& s, tensor* in, tensor* weight);
		Dense(session& s, tensor* input, int out);
		Dense(session& s, tensor* wei);
		
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
			for (int i = 0; i < flow; i++)
			{
				cout << map->get_node(i)->get_x() << " , " << map->get_node(i)->get_y() << endl;
			}
		}

		void set_up_nodes()
		{
			tensor* intens = _tensor_map[Input];
			shape inshape = intens->get_shape();
			shape wshape = _tensor_map[Weight]->get_shape();

			int flow = inshape.Alloc_size * ncount;
			int flowPerNode = intens->get_alloc_size();

			elemt* intensp = _tensor_map[Input]->get_p();
			elemt* wtensp = _tensor_map[Weight]->get_p();
			
			_tensor_map[Output]->reshape(shape(1, 1, 1, ncount));
			elemt* otensp = _tensor_map[Output]->get_p();

			map = new NodeMap(inshape, wshape, shape(1, 1, 1, ncount));
			map->convert_inodes_to_perceptron();
			
			map->set_node_address_x_by_interval(flowPerNode, intensp);
			map->set_node_address_y_by_interval(flowPerNode, wtensp);

			map->set_inodes_proutine([](elemt* p, elemt* pp, elemt* out)->void {
				*out = *p *  *pp;
			});

			map->set_wnodes_croutine([](elemt** p, elemt* out, int flow)->void{
				*out = 0;
				
				for (int i = 0; i < flow; i++, p++)
					*out += **p;
			});

			map->set_out_node_dens(otensp);
			map->set_placer_dens(flowPerNode, ncount);
					
			map->set_activation(ncount, [](elemt* p, elemt* pp)->void {
				*pp = *p;
			});
		}

		void tanh(elemt* p, elemt* pp)
		{

		}

		void proceed()
		{
			elemt* wp = _tensor_map[Weight]->get_p();

			tfunc::reduced_sum(target_dm::Batch, _tensor_map[Input], *_tensor_map[Output]);
			tfunc::channel_times(*_tensor_map[Output], ncount);

			_tensor_map[Output]->apply_to_elem_by_channel([](elemt* p, elemt* pp)->void {
				*p *= *pp;
			}, wp);

		};

		void proceed_inverse() {};
	};

};