#ifndef NEWCONTAINERS_MEMORY_IMPLEMENTATION_FAILINGALLOCATOR_IMPL_INCLUDED
#define NEWCONTAINERS_MEMORY_IMPLEMENTATION_FAILINGALLOCATOR_IMPL_INCLUDED

#include "memory/implementation/failing_allocator.hpp"
#include <memory>
#include <cstdlib>

namespace memory {
namespace implementation {

template<template<typename> class PointerModel, typename SizeType>
auto failing_allocator<PointerModel, SizeType>::do_allocate_aligned(
		typename base::size_type,
		typename base::size_type
	)
	-> typename base::pointer
{
	throw std::bad_alloc();
}

template<template<typename> class PointerModel, typename SizeType>
void failing_allocator<PointerModel, SizeType>::do_deallocate (
		typename base::pointer const p,
		typename base::size_type
	) noexcept
{
	if(p) std::abort();
}

} //namespace implementation
} //namespace memory

#endif
