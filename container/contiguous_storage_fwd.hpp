#ifndef NEWCONTAINER_CONTAINER_CONTIGUOUSSTORAGE_FWD_INCLUDED
#define NEWCONTAINER_CONTAINER_CONTIGUOUSSTORAGE_FWD_INCLUDED

#include "memory/interface/allocator_fwd.hpp"

namespace container {

/* has begin and size, unknown count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator = memory::interface::plain_allocator>
struct contiguous_storage;

} //namespace container

#endif
