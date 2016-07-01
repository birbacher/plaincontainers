#ifndef NEWCONTAINERS_CONTAINER_SIMPLEVECTOR_IMPL_INCLUDED
#define NEWCONTAINERS_CONTAINER_SIMPLEVECTOR_IMPL_INCLUDED

#include "container/simple_vector.hpp"
#include "container/contiguous_storage_impl.hpp"

#include <cassert>
#include <memory>
#include <utility>

namespace container {

template<typename ElemType, typename Allocator>
template<typename... Args, typename>
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
template<typename... Args, typename>
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


template<
	typename ElemType,
	typename Allocator
	>
void save_move_or_copy(
		simple_vector<ElemType, Allocator>& target,
		simple_vector<ElemType, Allocator>&& source
	)
{
	using sourceType = typename std::conditional<
			std::is_nothrow_constructible<ElemType, ElemType&&>::value,
			ElemType&&,
			ElemType const&
		>::type;
	
	interface::iteratable_range<sourceType, Allocator> sourceRange{std::move(source)};
	sourceRange.for_each(
			[&target]
			(sourceType t)
			{
				emplace_back(target, std::forward<sourceType>(t));
			}
		);
	source = {};
}

template<
	typename ElemType,
	typename Allocator
	>
void append(
		simple_vector<ElemType, Allocator>& target,
		typename interface::iteratable_range<ElemType const&, Allocator>::self source
	)
{
	source.for_each([&target](ElemType const& elem) { emplace_back(target, elem); });
}

template<
	typename TargetElemType,
	typename Allocator,
	typename SourceElemType,
	typename
	>
void append(
		simple_vector<TargetElemType, Allocator>& target,
		interface::iteratable_range<SourceElemType, Allocator> source
	)
{
	source.for_each([&target](SourceElemType elem) { emplace_back(target, std::forward<SourceElemType>(elem)); });
}

template<
	typename ElemType,
	typename Allocator
	>
void reserve(
		simple_vector<ElemType, Allocator>& container,
		typename Allocator::size_type new_capacity
	)
{
	if(new_capacity < container.get_size())
		new_capacity = container.get_size();
		
	simple_vector<ElemType, Allocator> new_storage{container.get_allocator(), new_capacity};
	save_move_or_copy(new_storage, std::move(container));
	container.swap(new_storage);
}

template<
	typename ElemType,
	typename Allocator
	>
void grow(simple_vector<ElemType, Allocator>& container)
{
	auto const new_capacity = container.get_capacity() + container.get_capacity()/2 + 2;
	reserve(container, new_capacity);
}

template<
	typename ElemType,
	typename Allocator,
	typename... Args,
	typename
	>
void emplace_back(simple_vector<ElemType, Allocator>& container, Args&&... args)
	noexcept(std::is_nothrow_constructible<ElemType, Args&&...>::value)
{
	if(container.get_size() >= container.get_capacity())
		grow(container);
	container.emplace_back(std::forward<Args>(args)...);
}

template<
	typename ElemType,
	typename Allocator
	>
void pop_back(simple_vector<ElemType, Allocator>& container)
{
	container.pop_back();
}

} //namespace container

#endif
