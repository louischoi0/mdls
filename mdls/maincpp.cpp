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
	densWeight.initialize_elem_to(2);
	
	t0.initialize_elem_to(1);

	Dense denseLayer(&t0, &densWeight);
	
	denseLayer.proceed();
	denseLayer.test();

	tfunc::print_object(denseLayer.get_tensor(Output));

	Node n;
	n.set_elemtp_by_col(t0.get_p(),0);	
	n.set_elemtp_by_col(t1.get_p(),1);

	NodeMap m(10);
	
	n.act();
	cout << n.get_out();
	int i = 0;


//	test_with_explain();
}