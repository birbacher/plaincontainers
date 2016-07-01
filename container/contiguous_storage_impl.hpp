#ifndef NEWCONTAINER_CONTAINER_CONTIGUOUSSTORAGE_IMPL_INCLUDED
#define NEWCONTAINER_CONTAINER_CONTIGUOUSSTORAGE_IMPL_INCLUDED

#include "container/contiguous_storage.hpp"
#include "memory/interface/allocator.hpp"
#include "memory/adapter/typed_allocator.hpp"
#include <cassert>
#include <memory>
#include <utility>

namespace container {

template<typename ElemType, typename Allocator>
contiguous_storage<ElemType, Allocator>::contiguous_storage(
        allocator_type& allocator_instance,
        size_type const element_count
    ) noexcept
    : allocator_adapter_instance(allocator_instance)
    , storage(allocator_adapter_instance.allocate(element_count))
    , capacity(element_count)
{
}

template<typename ElemType, typename Allocator>
contiguous_storage<ElemType, Allocator>::~contiguous_storage() noexcept
{
    if(storage)
        allocator_adapter_instance.deallocate(storage, capacity);
}

template<typename ElemType, typename Allocator>
contiguous_storage<ElemType, Allocator>::contiguous_storage(
        contiguous_storage&& other
    ) noexcept
    : contiguous_storage()
{
    swap(other);
}

template<typename ElemType, typename Allocator>
auto contiguous_storage<ElemType, Allocator>::operator = (
        contiguous_storage&& other
    ) noexcept
    -> contiguous_storage&
{
    contiguous_storage temp(std::move(other));
    swap(temp);
    return *this;
}

template<typename ElemType, typename Allocator>
void contiguous_storage<ElemType, Allocator>::swap(
        contiguous_storage& other
    ) noexcept
{
    using std::swap;
    swap(storage, other.storage);
    swap(capacity, other.capacity);
    swap(allocator_adapter_instance, other.allocator_adapter_instance);
}

} //namespace container

#endif
