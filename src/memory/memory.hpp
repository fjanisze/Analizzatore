#include <iostream>
#include <map>
#include <memory>
#include "..\configurations.hpp"

#define INVALID_MEMORY_HANDLER 0

namespace memory
{
	template<typename mem_ptr,typename mem_block>
	class Allocator
	{
	public:
		typedef mem_ptr   memory_pointer;
		typedef mem_block memory_unit;
	public:
		mem_ptr	alloc(std::size_t size)
		{
			return new mem_block[size];
		}
		template<typename T>
		T* alloc()
		{
			return new T;
		}
		void dealloc(mem_ptr mem)
		{
			delete[] mem;
		}
	};

	typedef long memory_handler;

	template< class alType >
	class memory_manager
	{
		enum memory_state { OWNED = 0, FREE };
		typedef std::shared_ptr< typename alType::memory_unit > mem_ptr_type;
		std::map< memory_handler, mem_ptr_type > memory_slabs; 
		std::map< memory_handler, std::size_t >  memory_size;
		std::map< memory_handler, unsigned >     memory_state;
		memory_handler cur_handler_id;
		alType allocator;
	private: //Management utility
		memory_handler find_compatible(std::size_t size);
	public:
		memory_manager() : cur_handler_id(1) {}
		memory_handler allocate(std::size_t size) throw();
		template<typename T>
		memory_handler allocate() throw();
		void deallocate(const memory_handler& mh) throw();
	public: //Access function
		typename alType::memory_pointer operator[](const memory_handler& h); 
	public: //Utility
		typedef typename alType::memory_pointer mem_ptr;
		std::size_t size(const memory_handler& mh);
	};

	template<class alType>
	std::size_t memory_manager<alType>::size(const memory_handler& mh)
	{
		if(memory_size.find(mh) != memory_size.end())
		{
			return memory_size[mh];
		}
		return 0;
	}

	template<class alType>
	void memory_manager<alType>::deallocate(const memory_handler& mh) throw()
	{
		if(memory_state.find(mh) != memory_state.end())
		{
			memory_state[mh] = FREE;
		}
	}

	template<class alType>
	memory_handler memory_manager<alType>::find_compatible(std::size_t size)
	{
		for(auto it : memory_state)
		{
			if( it.second == FREE && memory_size[it.first] >= size )
			{	
				return it.first;
			}
		}
		return INVALID_MEMORY_HANDLER;
	}

	template<class alType>
	memory_handler memory_manager<alType>::allocate(std::size_t size) throw()
	try{
		memory_handler mh;
		if(( mh = find_compatible( size ) ) != INVALID_MEMORY_HANDLER )
		{
			memory_state[mh] = OWNED;
			return mh;
		}
		//new handler
		mh = ++cur_handler_id;

		mem_ptr_type pointer( allocator.alloc(size) );
		memory_slabs[mh] = pointer;
		memory_size[mh] = size;
		memory_state[mh] = OWNED;

		return mh;
	}catch(std::bad_alloc& xa)
	{
		return INVALID_MEMORY_HANDLER;
	}

	template<class alType> 
		template<typename T>
	memory_handler memory_manager<alType>::allocate() throw()
	try{
		memory_handler mh = ++cur_handler_id;
		mem_ptr_type pointer( allocator.alloc( sizeof(T) ) );
		memory_slabs[mh] = pointer;
		memory_size[mh] = sizeof(T);
		return mh;
	}catch(std::bad_alloc& xa)
	{
		return INVALID_MEMORY_HANDLER;
	}

	template<class alType>
	typename alType::memory_pointer memory_manager<alType>::operator[](const memory_handler& h)
	{
		if(memory_slabs.find(h)!=memory_slabs.end())
		{
			return memory_slabs[h].get();
		}
		return nullptr;
	}
}
