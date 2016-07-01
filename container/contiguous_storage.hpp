#ifndef NEWCONTAINER_CONTAINER_CONTIGUOUSSTORAGE_INCLUDED
#define NEWCONTAINER_CONTAINER_CONTIGUOUSSTORAGE_INCLUDED

#include "container/contiguous_storage_fwd.hpp"
#include "memory/implementation/failing_allocator.hpp"
#include "memory/adapter/typed_allocator.hpp"
#include <type_traits>

namespace container {

/* has begin and size, unknown count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator>
struct contiguous_storage
{
    using value_type = ElemType;
    using allocator_type = Allocator;
    using pointer = typename allocator_type::template pointer_model<value_type>;
    using size_type = typename allocator_type::size_type;

private:
    static_assert(std::is_nothrow_copy_constructible<pointer>::value, "required");
    static_assert(std::is_nothrow_move_assignable<pointer>::value, "required");

    contiguous_storage(contiguous_storage const&) = delete;
    void operator = (contiguous_storage const&) = delete;

public:
    explicit constexpr contiguous_storage(
			allocator_type& allocator = memory::implementation::default_failing_allocator
		) noexcept
	    : allocator_adapter_instance(allocator)
		, storage{}
		, capacity{}
	{}

    contiguous_storage(
            allocator_type&,
            size_type
        ) noexcept;

    ~contiguous_storage() noexcept;

    contiguous_storage(contiguous_storage&&) noexcept;
    contiguous_storage& operator = (contiguous_storage&&) noexcept;

    void swap(contiguous_storage&) noexcept;

    allocator_type& get_allocator() const noexcept { return allocator_adapter_instance.get_allocator(); }
    pointer get_storage() const noexcept { return storage; }
    size_type get_capacity() const noexcept { return capacity; }

private:
    typedef memory::adapter::typed_allocator<value_type> allocator_adapter;

    allocator_adapter allocator_adapter_instance;
    pointer storage;
    size_type capacity;
};

} //namespace container

#endif
