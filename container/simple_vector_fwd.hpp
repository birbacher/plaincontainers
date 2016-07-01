#ifndef NEWCONTAINERS_CONTAINER_SIMPLEVECTOR_FWD_INCLUDED
#define NEWCONTAINERS_CONTAINER_SIMPLEVECTOR_FWD_INCLUDED

#include "container/interface/tags.hpp"
#include "memory/interface/allocator_fwd.hpp"

namespace container {

namespace tags { struct simple_vector; }

template<typename Container>
using is_simple_vector = has_tag<tags::simple_vector, Container>;
	
/* has begin and size, some known count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator = memory::interface::plain_allocator>
struct simple_vector;

} //namespace container

#endif
