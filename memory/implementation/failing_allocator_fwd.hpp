#ifndef NEWCONTAINERS_MEMORY_IMPLEMENTATION_FAILINGALLOCATOR_FWD_INCLUDED
#define NEWCONTAINERS_MEMORY_IMPLEMENTATION_FAILINGALLOCATOR_FWD_INCLUDED

#include "memory/interface/allocator.hpp"
#include <memory>
#include <cstdlib>

namespace memory {
namespace implementation {

template<template<typename> class PointerModel, typename SizeType>
struct failing_allocator;

extern failing_allocator<interface::plain_pointer, std::size_t> default_failing_allocator;

} //namespace implementation
} //namespace memory

#endif
