#ifndef NEWCONTAINERS_MEMORY_ADAPTER_TYPEDALLOCATOR_INCLUDED
#define NEWCONTAINERS_MEMORY_ADAPTER_TYPEDALLOCATOR_INCLUDED

#include <cstdint>
#include <utility>
#include <cassert>

#include "memory/interface/allocator.hpp"

namespace memory {
namespace adapter {

template<
        typename ElemType,
        typename Allocator = interface::plain_allocator
    >
struct typed_allocator
{
    using value_type = ElemType;
    using allocator_type = Allocator;
    static_assert(noexcept(std::declval<value_type*>()->~value_type()), "must have no-throw dtor");

private:
    template<typename T>
    using pointer_model = typename allocator_type::template pointer_model<T>;
    using size_type = typename allocator_type::size_type;

public:
    typedef pointer_model<value_type> pointer;
    typedef pointer_model<value_type const> const_pointer;

    constexpr typed_allocator(allocator_type& a) noexcept
        : allocator_instance(&a)
    {}

    allocator_type& get_allocator() const noexcept
    {
        assert(allocator_instance);
        return *allocator_instance;
    }
    pointer allocate(size_type const n)
    {
        assert(allocator_instance);
        return static_cast<pointer>(allocator_instance->allocate_aligned(n * sizeof(value_type), alignof(value_type)));
    }
    void deallocate(pointer const p, size_type const n) noexcept
    {
        assert(allocator_instance);
        allocator_instance->deallocate(p, n * sizeof(value_type));
    }

private:
    allocator_type* allocator_instance = nullptr;
};

} //namespace adapter
} //namespace memory

#endif
