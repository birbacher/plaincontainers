#ifndef NEWCONTAINERS_CONTAINER_INTERFACE_ITERATABLERANGE_HPP
#define NEWCONTAINERS_CONTAINER_INTERFACE_ITERATABLERANGE_HPP

#include "memory/interface/allocator.hpp"
#include <utility>
#include <type_traits>

namespace container {
namespace interface {

template<typename>
struct hint { constexpr hint() = default; };

template<
	typename ElemType,
	typename Allocator = memory::interface::plain_allocator
	>
struct iteratable_range;

template<typename ElemType,	typename Allocator>
struct iteratable_range
{
	using self = iteratable_range;
	using value_type = ElemType;
	using allocator_type = Allocator;
	template<typename T>
	using pointer_model = typename allocator_type::template pointer_model<T>;
	using size_type = typename allocator_type::size_type;
	
	struct interface
	{
		struct callback
		{
			virtual bool on_element(value_type) const =0;
		};
	
		virtual size_type get_size(void const*) const noexcept =0;
		virtual void each_until(void const*, callback const&) const =0;
	};
	
	template<
			typename Range,
			typename... Args,
			typename = typename std::enable_if<
					std::is_nothrow_constructible<
							interface const*,
							decltype( & make_iteratable_range(
									hint<ElemType>{},
									std::declval<Range&&>(),
									std::declval<Args&&>()...
								))
						>::value
				>::type
		>
	constexpr iteratable_range(Range&&, Args&&...) noexcept;
	
	size_type get_size() const noexcept { return impl->get_size(instance); }
	template<typename F>
	void for_each(F&&);

private:
	interface const* impl;
	void const* instance;
};

template<typename ElemType,	typename Allocator>
template<typename Range, typename... Args, typename>
constexpr iteratable_range<ElemType, Allocator>::iteratable_range(
		Range&& range,
		Args&&... args
	) noexcept
	: impl(&make_iteratable_range(
			hint<ElemType>{},
			std::forward<Range>(range),
			std::forward<Args>(args)...
		))
	, instance(static_cast<void const*>(&range))
{}

template<typename ElemType,	typename Allocator>
template<typename F>
void iteratable_range<ElemType, Allocator>::for_each(F&& f)
{
	using Fmem = typename std::remove_reference<F>::type &;
	struct S : interface::callback {
		S(Fmem fmem) : fmem(fmem) {}
		Fmem fmem;
		bool on_element(value_type v) const override
		{
			fmem(std::forward<value_type>(v));
			return false;
		}
	} s{f};
	impl->each_until(instance, s);
}

template<typename Container>
auto vrange(Container const& container)
	-> iteratable_range<typename std::decay<typename Container::value_type>::type, typename Container::allocator_type>
{
	return container;
}
	
template<typename Container>
auto lrange(Container& container)
	-> iteratable_range<typename std::decay<typename Container::value_type>::type&, typename Container::allocator_type>
{
	return container;
}
	
template<typename Container>
auto rrange(Container&& container)
	-> iteratable_range<typename std::decay<typename Container::value_type>::type&&, typename Container::allocator_type>
{
	return std::move(container);
}
	
template<typename Container>
auto crange(Container const& container)
	-> iteratable_range<typename std::decay<typename Container::value_type>::type const&, typename Container::allocator_type>
{
	return container;
}

} //namespace interface
} //namespace container

#endif
