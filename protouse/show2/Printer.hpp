#ifndef PROTO_SHOW1_PRINTER_HPP
#define PROTO_SHOW1_PRINTER_HPP

#include <iosfwd>
#include "container/interface/iteratable_range.hpp"
#include <string>

namespace proto {
namespace show1 {

void show(container::interface::iteratable_range<std::string const&>, std::ostream&);

} //namespace show1
} //namespace proto

#endif

