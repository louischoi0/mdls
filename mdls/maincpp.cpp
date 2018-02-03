#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include "node.h"

#include "padding.h"
#include "session.h"
#include "statistics.h"

using namespace mathm;
using namespace std;
using namespace mdls;

int main()
{
	tensor t0(1, 2, 4, 4);
	tensor t1(4, 3, 4, 4);
	
	tensor out(1, 2, 4, 4);

	tensor densWeight(1, 1, 1, 8);
	tensor densWeight0(1, 1, 1, 8);
	
	session s = session();

	tfunc::init_tensor_ladder(t0);

	tfunc::init_tensor_with_random(densWeight,0,1);
	tfunc::init_tensor_with_random(densWeight0,0,1);

	Dense denseLayer(s , &t0, &densWeight);
	Dense denseLayer0(s, &densWeight0);

	densWeight.apply_to_all_elem([](elemt* p)->void {
		*p += 1;
	});
	
	denseLayer.set_up_nodes();
	denseLayer.proceed_by_nodes();

	s.add_layer(&denseLayer);
	s.add_layer(&denseLayer0);
	
	s.preprocessing();
	s.run();
	tfunc::print_object(denseLayer.get_tensor(Output));
	tfunc::print_object(denseLayer0.get_tensor(Output));
	
	Node n;
	n.set_elemtp_by_col(t0.get_p(),0);	
	n.set_elemtp_by_col(t1.get_p(),1);

	NodeMap m(10);
	
	n.act();
	cout << n.get_out();
	int i = 0;


//	test_with_explain();
}