#ifndef NEWCONTAINERS_CONTAINER_VECTOR_IMPL_INCLUDED
#define NEWCONTAINERS_CONTAINER_VECTOR_IMPL_INCLUDED

#include "container/vector.hpp"
#include "container/simple_vector_impl.hpp"

#include <cassert>
#include <memory>
#include <utility>

namespace container {

template<typename ElemType, typename Allocator>
template<typename... Args, typename>
vector<ElemType, Allocator>::vector(
        Args&&... args
    ) noexcept(std::is_nothrow_constructible<storage_type, Args&&...>::value)
    : storage(std::forward<Args>(args)...)
{
}

template<typename ElemType, typename Allocator>
vector<ElemType, Allocator>::~vector() noexcept
{
	clear();
}

template<typename ElemType, typename Allocator>
vector<ElemType, Allocator>::vector(
		vector const& other
	)
	: storage(other.get_allocator(), other.get_size())
{
	interface::crange(other).for_each([this](ElemType const& elem){
		storage.emplace_back(elem);
	});
}
	
template<typename ElemType, typename Allocator>
auto vector<ElemType, Allocator>::operator = (
		vector const& other
	)
	-> vector&
{
	vector temp = other;
	swap(temp);
	return *this;
}
	
template<typename ElemType, typename Allocator>
vector<ElemType, Allocator>::vector(
        vector&& other
    ) noexcept
    : vector()
{
    swap(other);
}

template<typename ElemType, typename Allocator>
auto vector<ElemType, Allocator>::operator = (
        vector&& other
    ) noexcept
    -> vector&
{
    vector temp(std::move(other));
    swap(temp);
    return *this;
}

template<typename ElemType, typename Allocator>
void vector<ElemType, Allocator>::swap(
        vector& other
    ) noexcept
{
    using std::swap;
    swap(storage, other.storage);
}

template<typename ElemType, typename Allocator>
template<typename... Args, typename>
void vector<ElemType, Allocator>::emplace_back(
        Args&&... args
    ) noexcept(std::is_nothrow_constructible<value_type, Args&&...>::value)
{
	if(get_size() >= get_capacity())
		grow();
	storage.emplace_back(std::forward<Args>(args)...);
}

template<typename ElemType, typename Allocator>
void vector<ElemType, Allocator>::pop_back() noexcept
{
	storage.pop_back();
}

template<typename ElemType, typename Allocator>
void vector<ElemType, Allocator>::clear() noexcept
{
	storage.clear();
}

template<typename ElemType, typename Allocator>
void vector<ElemType, Allocator>::grow()
{
	storage_type new_storage{get_allocator(), get_size() + get_size()/2 + 2};
	interface::crange(*this).for_each([&new_storage](ElemType const& elem){
		new_storage.emplace_back(elem);
	});
	storage.swap(new_storage);
}

/*
template<typename ElemType, typename Allocator>
auto simple_vector<ElemType, Allocator>::detach_storage() noexcept
    -> storage_type
{
	clear();
	storage_type detached_storage = std::move(storage);
	return detached_storage;
}
*/
} //namespace container

#endif
