#ifndef NEWCONTAINERS_MEMORY_IMPLEMENTATION_FAILINGALLOCATOR_INCLUDED
#define NEWCONTAINERS_MEMORY_IMPLEMENTATION_FAILINGALLOCATOR_INCLUDED

#include "memory/implementation/failing_allocator_fwd.hpp"
#include "memory/interface/allocator.hpp"
#include <memory>
#include <cstdlib>

namespace memory {
namespace implementation {

template<template<typename> class PointerModel, typename SizeType>
struct failing_allocator
    : interface::allocator<PointerModel, SizeType>
{
    constexpr failing_allocator() = default;
protected:
    typedef interface::allocator<PointerModel, SizeType> base;
    typename base::pointer do_allocate_aligned [[ noreturn ]] (
            typename base::size_type,
            typename base::size_type
        ) override;

    void do_deallocate (
            typename base::pointer const p,
            typename base::size_type
        ) noexcept override;
};

extern template struct failing_allocator<interface::plain_pointer, std::size_t>;

extern failing_allocator<interface::plain_pointer, std::size_t> default_failing_allocator;

} //namespace implementation
} //namespace memory

#endif
