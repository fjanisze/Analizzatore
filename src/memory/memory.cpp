
#include "memory.hpp"
#include <cstring>
/*
using namespace std;

namespace test
{
	struct foo
	{
		foo()
		{
			LOG("new foo!!");
		}
		~foo()
		{
			LOG("foo destroyed");
		}
		std::string mystr;
	};
}


int main()
{
	typedef memory::memory_manager< memory::Allocator<char* , char> >  memory_m;
	memory_m mm;
	LOG("Starting app");

	test::foo foo1;
	foo1.mystr = "This is the first string";
	std::string str;
	memory::memory_handler handler_100 = mm.allocate(100),
				handler_200 = mm.allocate(200),
				handler_foo = mm.allocate<test::foo>(),
				handler_str;

	LOG("the handler for the 100 byte chunk is :" , handler_100);
	std::cout<<"We have something to do!!\n";
	LOG("the handler for foo1 is :" , handler_foo);
	handler_str = mm.allocate<std::string>();
	LOG("the handler for str is :" , handler_str);

	mm.deallocate(handler_200);
	std::cout<<"Doing something!!\n";
	

	memory_m::mem_ptr ptr = mm[handler_100];
	strcpy(ptr,foo1.mystr.c_str());
	if(ptr!=nullptr)
	{
		std::string tmp;
		for(short i=0; i < mm.size(handler_100) ; i++)
		{
			tmp += ptr[i];
		}
		LOG(tmp);
	}
	ptr = mm[10];
	if(ptr==nullptr)
	{
		LOG_ERR("ptr is null!!");
	}
	LOG("Finish");
}*/
