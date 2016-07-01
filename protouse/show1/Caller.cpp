#include "Caller.hpp"
#include "Printer.hpp"
#include "memory/implementation/malloc_allocator.hpp"
#include "container/simple_vector_impl.hpp"
#include "container/vector_impl.hpp"

#include <iostream>
#include <ostream>

namespace proto {
namespace show1 {

void test1(std::ostream& stream)
{
	container::simple_vector<std::string> values{memory::implementation::default_malloc_allocator, 10};
	values.emplace_back("Hello,");
	values.emplace_back("World!");
	show(values, stream);
}

void test2(std::ostream& stream)
{
	container::vector<std::string> values{memory::implementation::default_malloc_allocator};
	values.emplace_back("Hello,");
	values.emplace_back("World!");
	show(values, stream);
}

void test3(std::ostream& stream)
{
	struct my_alloc : memory::interface::allocator<>
	{
		char storage[1000];
		char *first;
		my_alloc() : first(storage) {}
	protected:
		virtual pointer do_allocate_aligned(size_type const bytes, size_type const alignment) override
		{
			std::cout << "alloc " << bytes << "/" << alignment << "\n";
			char* const limit = storage + 1000 - bytes;
			for(;first <= limit; ++first)
			{
				auto const n = reinterpret_cast<std::size_t>(first);
				if(n/alignment *alignment == n)
				{
					first += bytes;
					return first - bytes;
				}
			}
			throw std::bad_alloc();
		}
		virtual void do_deallocate(pointer, size_type) noexcept override
		{
		}
	};
	my_alloc local_alloc;
	container::vector<std::string> values{local_alloc};
	values.emplace_back("Hello,");
	values.emplace_back("World!");
	values.emplace_back("iae");
	values.emplace_back("nrdt");
	show(values, stream);
	auto v2 = values;
	std::cout << "copy done" << std::endl;
}

void run(std::ostream& stream)
{
	test1(stream);
	test2(stream);
	test3(stream);
}

} //namespace show1
} //namespace proto

