#include "Caller.hpp"
#include "Printer.hpp"
#include "memory/implementation/malloc_allocator.hpp"
#include "container/vector_impl.hpp"

namespace proto {
namespace show1 {

void run(std::ostream& stream)
{
	container::vector<std::string> values{memory::implementation::default_malloc_allocator, 10};
	values.emplace_back("Hello,");
	values.emplace_back("World!");
	show(values, stream);
}

} //namespace show1
} //namespace proto

