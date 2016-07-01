#ifndef NEWCONTAINERS_INTERFACE_ALLOCATOR_FWD_INCLUDED
#define NEWCONTAINERS_INTERFACE_ALLOCATOR_FWD_INCLUDED

#include <cstddef>

namespace memory {
namespace interface {

template<typename T>
using plain_pointer = T*;

template<
        template<typename> class PointerModel = plain_pointer,
        typename SizeType = std::size_t
    >
struct allocator;

typedef allocator<plain_pointer, std::size_t> plain_allocator;

} //namespace interface
} //namespace memory

#endif
