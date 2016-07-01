#ifndef NEWCONTAINERS_CONTAINER_SIMPLEVECTOR_INCLUDED
#define NEWCONTAINERS_CONTAINER_SIMPLEVECTOR_INCLUDED

#include "container/simple_vector_fwd.hpp"
#include "container/contiguous_storage.hpp"
#include "container/interface/iteratable_range.hpp"

namespace container {

/* has begin and size, some known count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator>
struct simple_vector
{
	using tag = tags::simple_vector;
    using value_type = ElemType;
    using allocator_type = Allocator;
    using pointer = typename allocator_type::template pointer_model<value_type>;
    using const_pointer = typename allocator_type::template pointer_model<value_type const>;
    using size_type = typename allocator_type::size_type;
    using storage_type = contiguous_storage<value_type, allocator_type>;
private:
    static_assert(std::is_nothrow_copy_constructible<pointer>::value, "required");
    static_assert(std::is_nothrow_move_assignable<pointer>::value, "required");
    static_assert(std::is_nothrow_destructible<value_type>::value, "required");

    simple_vector(simple_vector const&) = delete;
    void operator = (simple_vector const&) = delete;

public:
    constexpr simple_vector() noexcept : size{} {}
	template<typename... Args, typename = typename std::enable_if<std::is_constructible<storage_type, Args&&...>::value>::type>
    simple_vector(Args&&...) noexcept(std::is_nothrow_constructible<storage_type, Args&&...>::value);

    ~simple_vector() noexcept;

    simple_vector(simple_vector&&) noexcept;
    simple_vector& operator = (simple_vector&&) noexcept;

    void swap(simple_vector&) noexcept;

    allocator_type& get_allocator() const noexcept { return storage.get_allocator(); }
    pointer get_storage() noexcept { return storage.get_storage(); }
    const_pointer get_storage() const noexcept { return storage.get_storage(); }
    size_type get_capacity() const noexcept { return storage.get_capacity(); }
    size_type get_size() const noexcept { return size; }

	template<typename... Args, typename = typename std::enable_if<std::is_constructible<value_type, Args&&...>::value>::type>
    void emplace_back(Args&&...) noexcept(std::is_nothrow_constructible<value_type, Args&&...>::value);
    void pop_back() noexcept;
	void clear() noexcept;

	storage_type detach_storage() noexcept;
private:
    storage_type storage;
    size_type size;
};

template<typename P, typename T> struct propagate_cvref;
template<typename P, typename T> struct propagate_cvref<P const&, T> { using type = T const&; };
template<typename P, typename T> struct propagate_cvref<P&&, T> { using type = T&&; };
template<typename P, typename T> struct propagate_cvref<P&, T> { using type = T&; };

template<typename C>
struct dismantle {
	using container_type = typename std::remove_reference<C>::type;
	using allocator_type = typename container_type::allocator_type;
	using elem_type = typename container_type::value_type;
	using forward_type = typename propagate_cvref<C, elem_type>::type;
	static forward_type forward(forward_type& e) { return static_cast<forward_type>(e); }
};

template<typename WantedElemType, typename Container>
auto make_iteratable_range(interface::hint<WantedElemType>, Container&&)
	-> typename std::enable_if<
			is_simple_vector<typename std::remove_cv<typename std::remove_reference<Container>::type>::type>::value
			&&
			std::is_convertible<typename dismantle<Container&&>::forward_type, WantedElemType>::value,
			typename container::interface::iteratable_range<WantedElemType, typename dismantle<Container&&>::allocator_type>::interface const&
		>::type
{
	using dis = dismantle<Container&&>;
	using container_type = typename dis::container_type;
	using size_type = typename container_type::size_type;
	using range_type = typename container::interface::template iteratable_range<WantedElemType, typename dismantle<Container&&>::allocator_type>;
	using interface_type = typename range_type::interface;
	using callback_type = typename interface_type::callback;
	struct Impl : interface_type
	{
		size_type get_size(void const* const p) const noexcept override
		{
			return static_cast<container_type*>(const_cast<void*>(p))->get_size();
		}
		void each_until(void const* const p, callback_type const& c) const override
		{
			auto const container = static_cast<container_type*>(const_cast<void*>(p));
			size_type const size = container->get_size();
			for(size_type i = 0u; i<size; ++i)
				if(c.on_element(dis::forward(*(container->get_storage() + i))))
					break;
		}
	};
	static constexpr Impl IMPL{};
	return IMPL;
}

template<
	typename ElemType,
	typename Allocator
	>
void save_move_or_copy(
		simple_vector<ElemType, Allocator>& target,
		simple_vector<ElemType, Allocator>&& source
	);

template<
	typename ElemType,
	typename Allocator
	>
void append(
		simple_vector<ElemType, Allocator>& target,
		typename interface::iteratable_range<ElemType const&, Allocator>::self source
	);

template<
	typename TargetElemType,
	typename Allocator,
	typename SourceElemType,
	typename = typename std::enable_if<std::is_constructible<TargetElemType, SourceElemType>::value>::type
	>
void append(
		simple_vector<TargetElemType, Allocator>& target,
		typename interface::iteratable_range<SourceElemType, Allocator> source
	);

template<
	typename ElemType,
	typename Allocator
	>
void reserve(simple_vector<ElemType, Allocator>& container, typename Allocator::size_type new_capacity);

template<
	typename ElemType,
	typename Allocator
	>
void grow(simple_vector<ElemType, Allocator>& container);

template<
	typename ElemType,
	typename Allocator,
	typename... Args,
	typename = typename std::enable_if<std::is_constructible<ElemType, Args&&...>::value>::type
	>
void emplace_back(simple_vector<ElemType, Allocator>& container, Args&&...)
	noexcept(std::is_nothrow_constructible<ElemType, Args&&...>::value);

template<
	typename ElemType,
	typename Allocator
	>
void pop_back(simple_vector<ElemType, Allocator>& container);

} //namespace container

#endif
