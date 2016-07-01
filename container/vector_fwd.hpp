#ifndef NEWCONTAINERS_CONTAINER_VECTOR_FWD_INCLUDED
#define NEWCONTAINERS_CONTAINER_VECTOR_FWD_INCLUDED

#include "container/interface/tags.hpp"
#include "memory/interface/allocator_fwd.hpp"

namespace container {

namespace tags { struct vector; }

template<typename Container>
using is_vector = has_tag<tags::vector, Container>;

/* has begin and size, some known count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator = memory::interface::plain_allocator>
struct vector;

} //namespace container

#endif
