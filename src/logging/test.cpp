#include <iostream>
#include <vector>
using namespace std;

template<int index>
class myclass
{
public:
	myclass()
	{
	//	T t = 69;
	//	vec.push_back(t);
	}
};

static std::vector<int> merda;

int main()
{
	myclass<110> mc;
	std::cout<<merda.size()<<std::endl;
}
