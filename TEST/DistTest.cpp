#include "Distance.hpp"

int main(void)
{
	//std::function<double(std::vector<int>,std::vector<int>)> dist = Distance::levenstein2<int>;
	std::vector<int> a{1, 2, 3, 4, 5};
	std::vector<int> b{4, 3, 4, 1, 5};
	std::vector<int> c{1, 2, 7, 4, 5};
	std::vector<int> d{2, 3, 4, 5, 1};
	std::vector<int> e{6, 7, 8, 9, 1, 2, 3, 4, 5, 10};
	//dist2(a,b)=3,dist2(a,c)=1,dist2(a,d)=2,dist2(a,e)=5
	//dist(a,b)=3/5,dist(a,c)=1/5,dist(a,d)=2/5,dist(a,e)=5/10
	if (
		Distance::levenstein2(a, b) == 3 &&
		Distance::levenstein2(a, c) == 1 &&
		Distance::levenstein2(a, d) == 2 &&
		Distance::levenstein2(a, e) == 5 &&
		Distance::levenstein(a, b) == 0.6 &&
		Distance::levenstein(a, c) == 0.2 &&
		Distance::levenstein(a, d) == 0.4 &&
		Distance::levenstein(a, e) == 0.5 &&
		Distance::hamming(a, b) == 0.8 &&
		Distance::hamming(a, c) == 0.2)
	{
		return 0;
	}
	else
	{
		exit(1);
	}
}