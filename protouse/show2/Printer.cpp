#include "Printer.hpp"
#include <ostream>

namespace proto {
namespace show1 {

void show(container::interface::iteratable_range<std::string const&> range, std::ostream& stream)
{
	range.for_each([&stream](std::string const& s) { stream << s << '\n'; });
}

} //namespace show1
} //namespace proto

