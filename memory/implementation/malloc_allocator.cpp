#include "memory/implementation/malloc_allocator.hpp"

#include <cstdlib>
#include <memory>

namespace memory {
namespace implementation {

malloc_allocator default_malloc_allocator{};

auto malloc_allocator::do_allocate_aligned(
        size_type const bytes,
        size_type
    )
    -> pointer
{
    if(auto result = std::malloc(bytes))
        return result;

    throw std::bad_alloc();
}

auto malloc_allocator::do_deallocate(
        pointer const where,
        size_type
    ) noexcept
    -> void
{
    std::free(where);
}

} //namespace implementation
} //namespace memory
