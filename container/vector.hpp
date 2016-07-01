#ifndef NEWCONTAINERS_CONTAINER_VECTOR_INCLUDED
#define NEWCONTAINERS_CONTAINER_VECTOR_INCLUDED

#include "container/vector_fwd.hpp"
#include "container/simple_vector.hpp"
#include "container/contiguous_storage.hpp"
#include "container/interface/iteratable_range.hpp"

namespace container {

/* has begin and size, some known count of
 * elements are constructed
 */
template<typename ElemType, typename Allocator>
struct vector
{
	using tag = tags::vector;
    using value_type = ElemType;
    using allocator_type = Allocator;
    using pointer = typename allocator_type::template pointer_model<value_type>;
    using const_pointer = typename allocator_type::template pointer_model<value_type const>;
    using size_type = typename allocator_type::size_type;
    using storage_type = simple_vector<value_type, allocator_type>;
private:
    static_assert(std::is_nothrow_copy_constructible<pointer>::value, "required");
    static_assert(std::is_nothrow_move_assignable<pointer>::value, "required");
    static_assert(std::is_nothrow_destructible<value_type>::value, "required");

public:
    constexpr vector() noexcept {}
	template<typename... Args, typename = typename std::enable_if<std::is_constructible<storage_type, Args&&...>::value>::type>
    vector(Args&&...) noexcept(std::is_nothrow_constructible<storage_type, Args&&...>::value);

    ~vector() noexcept;

	vector(vector const&);
	vector& operator = (vector const&);
    vector(vector&&) noexcept;
    vector& operator = (vector&&) noexcept;

    void swap(vector&) noexcept;

    allocator_type& get_allocator() const noexcept { return storage.get_allocator(); }
    pointer get_storage() noexcept { return storage.get_storage(); }
    const_pointer get_storage() const noexcept { return storage.get_storage(); }
    size_type get_capacity() const noexcept { return storage.get_capacity(); }
    size_type get_size() const noexcept { return storage.get_size(); }

	template<typename... Args, typename = typename std::enable_if<std::is_constructible<value_type, Args&&...>::value>::type>
    void emplace_back(Args&&...) noexcept(std::is_nothrow_constructible<value_type, Args&&...>::value);
    void pop_back() noexcept;
	void clear() noexcept;

	//storage_type detach_storage() noexcept;
private:
    storage_type storage;
	
	void grow();
};

template<typename WantedElemType, typename Container>
auto make_iteratable_range(interface::hint<WantedElemType>, Container&&)
	-> typename std::enable_if<
			is_vector<typename std::remove_cv<typename std::remove_reference<Container>::type>::type>::value
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

} //namespace container

#endif
