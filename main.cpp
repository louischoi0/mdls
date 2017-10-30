#include ".\objects\element.h"
#include ".\objects\tensor.h"
#include ".\objects\layert.h"
#include <iostream>

int main()
{
  mdls::element a(1,2,3,4);
  mdls::tensor bb(mdls::shape(2,4,2,1));

  std::cout << a.get_batch_offset() << std::endl;

}
