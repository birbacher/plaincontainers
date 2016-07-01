#ifndef NEWCONTAINERS_MEMORY_IMPLEMENTATION_MALLOCALLOCATOR_INCLUDED
#define NEWCONTAINERS_MEMORY_IMPLEMENTATION_MALLOCALLOCATOR_INCLUDED

#include "memory/implementation/malloc_allocator_fwd.hpp"

#include "memory/interface/allocator.hpp"

namespace memory {
namespace implementation {

struct malloc_allocator
    : interface::plain_allocator
{
    constexpr malloc_allocator() = default;

protected:
    pointer do_allocate_aligned(size_type, size_type) override;
    void do_deallocate(pointer, size_type) noexcept override;
};

} //namespace implementation
} //namespace memory

#endif
