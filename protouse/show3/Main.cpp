#include "container/simple_vector_impl.hpp"
#include "container/interface/iteratable_range.hpp"
#include "memory/implementation/malloc_allocator.hpp"

namespace container {
	template<typename ElemType, typename Allocator = memory::interface::plain_allocator, typename Container>
	auto mk_simple_vector(
			//interface::iteratable_range<Container::value_type&&> source_range,
			Container&& c,
			Allocator& allocator = memory::implementation::default_malloc_allocator
		)
		-> simple_vector<ElemType, Allocator>
	{
		interface::iteratable_range<typename Container::value_type&&> source_range = std::move(c);
		simple_vector<ElemType, Allocator> result{allocator, source_range.get_size()};
		source_range.for_each([&result](ElemType&& elem) { result.emplace_back(std::move(elem)); });
		return result;
	}
}

int main()
{
	container::simple_vector<int> v{memory::implementation::default_malloc_allocator, 10};
	emplace_back(v, 5);
	emplace_back(v, 4);
	auto v2 = container::mk_simple_vector<int>(std::move(v));
}

