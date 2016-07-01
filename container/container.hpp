#ifndef CONTAINER_INCLUDED
#define CONTAINER_INCLUDED

#include "memory/interface/allocator.hpp"
#include "memory/adapter/typed_allocator.hpp"
#include "memory/implementation/failing_allocator_fwd.hpp"
#include <cassert>
#include <memory>
#include <utility>

namespace container {

/* has begin and size, unknown count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator = memory::interface::plain_allocator>
struct contiguous_memory
{
    using value_type = ElemType;
    using allocator_type = Allocator;
    using pointer = typename allocator_type::template pointer_model<value_type>;
    using size_type = typename allocator_type::size_type;

private:
    static_assert(std::is_nothrow_copy_constructible<pointer>::value, "required");
    static_assert(std::is_nothrow_move_assignable<pointer>::value, "required");

    contiguous_memory(contiguous_memory const&) = delete;
    void operator = (contiguous_memory const&) = delete;

public:
    contiguous_memory() noexcept;
    contiguous_memory(
            allocator_type&,
            size_type
        ) noexcept;

    ~contiguous_memory() noexcept;

    contiguous_memory(contiguous_memory&&) noexcept;
    contiguous_memory& operator = (contiguous_memory&&) noexcept;

    void swap(contiguous_memory&) noexcept;

    allocator_type& get_allocator() const noexcept { return allocator_adapter_instance.get_allocator(); }
    pointer get_storage() const noexcept { return storage; }
    size_type get_capacity() const noexcept { return capacity; }

private:
    typedef memory::adapter::typed_allocator<value_type> allocator_adapter;

    allocator_adapter allocator_adapter_instance;
    pointer storage;
    size_type capacity;
};

template<typename ElemType, typename Allocator>
contiguous_memory<ElemType, Allocator>::contiguous_memory() noexcept
    : allocator_adapter_instance(memory::implementation::default_failing_allocator)
    , storage{}
    , capacity{}
{
}

template<typename ElemType, typename Allocator>
contiguous_memory<ElemType, Allocator>::contiguous_memory(
        allocator_type& allocator_instance,
        size_type const element_count
    ) noexcept
    : allocator_adapter_instance(allocator_instance)
    , storage(allocator_adapter_instance.allocate(element_count))
    , capacity(element_count)
{
}

template<typename ElemType, typename Allocator>
contiguous_memory<ElemType, Allocator>::~contiguous_memory() noexcept
{
    if(storage)
        allocator_adapter_instance.deallocate(storage, capacity);
}

template<typename ElemType, typename Allocator>
contiguous_memory<ElemType, Allocator>::contiguous_memory(
        contiguous_memory&& other
    ) noexcept
    : contiguous_memory()
{
    swap(other);
}

template<typename ElemType, typename Allocator>
auto contiguous_memory<ElemType, Allocator>::operator = (
        contiguous_memory&& other
    ) noexcept
    -> contiguous_memory&
{
    contiguous_memory temp(std::move(other));
    swap(temp);
    return *this;
}

template<typename ElemType, typename Allocator>
void contiguous_memory<ElemType, Allocator>::swap(
        contiguous_memory& other
    ) noexcept
{
    using std::swap;
    swap(storage, other.storage);
    swap(capacity, other.capacity);
    swap(allocator_adapter_instance, other.allocator_adapter_instance);
}


/* has begin and size, some known count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator = memory::interface::plain_allocator>
struct simple_vector
{
    using value_type = ElemType;
    using allocator_type = Allocator;
    using pointer = typename allocator_type::template pointer_model<value_type>;
    using size_type = typename allocator_type::size_type;
    using storage_type = contiguous_memory<value_type, allocator_type>;
private:
    static_assert(std::is_nothrow_copy_constructible<pointer>::value, "required");
    static_assert(std::is_nothrow_move_assignable<pointer>::value, "required");
    static_assert(std::is_nothrow_destructible<value_type>::value, "required");

    simple_vector(simple_vector const&) = delete;
    void operator = (simple_vector const&) = delete;

public:
    simple_vector() noexcept;
    template<typename... Args>
    simple_vector(Args&&...) noexcept(std::is_nothrow_constructible<storage_type, Args&&...>::value);

    ~simple_vector() noexcept;

    simple_vector(simple_vector&&) noexcept;
    simple_vector& operator = (simple_vector&&) noexcept;

    void swap(simple_vector&) noexcept;

    allocator_type& get_allocator() const noexcept { return storage.get_allocator(); }
    pointer get_storage() const noexcept { return storage.get_storage(); }
    size_type get_capacity() const noexcept { return storage.get_capacity(); }
    size_type get_size() const noexcept { return size; }

    template<typename... Args>
    void emplace_back(Args&&...) noexcept(std::is_nothrow_constructible<value_type, Args&&...>::value);
    void pop_back() noexcept;
	void clear() noexcept;

	storage_type detach_storage() noexcept;
private:
    storage_type storage;
    size_type size;
};

template<typename ElemType, typename Allocator>
simple_vector<ElemType, Allocator>::simple_vector() noexcept
    : size{}
{
}

template<typename ElemType, typename Allocator>
template<typename... Args>
simple_vector<ElemType, Allocator>::simple_vector(
        Args&&... args
    ) noexcept(std::is_nothrow_constructible<storage_type, Args&&...>::value)
    : storage(std::forward<Args>(args)...)
    , size{}
{
}

template<typename ElemType, typename Allocator>
simple_vector<ElemType, Allocator>::~simple_vector() noexcept
{
	clear();
}

template<typename ElemType, typename Allocator>
simple_vector<ElemType, Allocator>::simple_vector(
        simple_vector&& other
    ) noexcept
    : simple_vector()
{
    swap(other);
}

template<typename ElemType, typename Allocator>
auto simple_vector<ElemType, Allocator>::operator = (
        simple_vector&& other
    ) noexcept
    -> simple_vector&
{
    simple_vector temp(std::move(other));
    swap(temp);
    return *this;
}

template<typename ElemType, typename Allocator>
void simple_vector<ElemType, Allocator>::swap(
        simple_vector& other
    ) noexcept
{
    using std::swap;
    swap(storage, other.storage);
    swap(size, other.size);
}

template<typename ElemType, typename Allocator>
template<typename... Args>
void simple_vector<ElemType, Allocator>::emplace_back(
        Args&&... args
    ) noexcept(std::is_nothrow_constructible<value_type, Args&&...>::value)
{
    assert(get_size() < get_capacity());
    ::new(&*(get_storage() + get_size())) value_type(std::forward<Args>(args)...);
    ++size;
}

template<typename ElemType, typename Allocator>
void simple_vector<ElemType, Allocator>::pop_back() noexcept
{
    assert(get_size() > 0);
    --size;
	assert(get_storage() != nullptr);
    (&*(get_storage() + get_size()))->~value_type();
}

template<typename ElemType, typename Allocator>
void simple_vector<ElemType, Allocator>::clear() noexcept
{
	while(get_size())
		pop_back();
}

template<typename ElemType, typename Allocator>
auto simple_vector<ElemType, Allocator>::detach_storage() noexcept
    -> storage_type
{
	clear();
	storage_type detached_storage = std::move(storage);
	return detached_storage;
}


} //namespace container

#endif
