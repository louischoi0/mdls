#pragma once
#include "element.h"

namespace mdls
{
	class tensor : public element
	{
	public:
		tensor();
		tensor(const tensor& t);

		tensor(const shape& s);
		tensor(int n0, int n1, int n2, int n3);

		tensor& operator=(const tensor& k);

	};

}