#include "Distance.hpp"

int main(void){
	//std::function<double(std::vector<int>,std::vector<int>)> dist = Distance::levenstein2<int>;
	std::vector<int> a{1,2,3,4,5};
	std::vector<int> b{4,3,4,1,5};
	std::vector<int> c{1,2,7,4,5};
	std::vector<int> d{2,3,4,5,1};
	std::vector<int> e{6,7,8,9,1,2,3,4,5,10};
	//dist(a,b)=3,dist(a,c)=1,dist(a,d)=2,dist(a,e)=5
	if(
		Distance::levenstein(a, b) == 3 &&
		Distance::levenstein(a, c) == 1 &&
		Distance::levenstein(a, d) == 2 &&
		Distance::levenstein(a, e) == 5
		){
		return 0;
	}else{
		exit(1);
	}
}