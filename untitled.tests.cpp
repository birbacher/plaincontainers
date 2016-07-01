#include "memory/interface/allocator.hpp"
#include "container/contiguous_storage_impl.hpp"
#include "container/simple_vector_impl.hpp"
#include "memory/implementation/failing_allocator.hpp"
#include "memory/implementation/malloc_allocator.hpp"
#include "container/interface/iteratable_range.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <type_traits>
#include <string>

using namespace memory;

static_assert(std::is_nothrow_move_assignable<container::contiguous_storage<int>>::value, "required");

BOOST_AUTO_TEST_CASE( example )
{
    container::contiguous_storage<int> mem;
    container::contiguous_storage<int> mem2{implementation::default_malloc_allocator, 10};
    mem = std::move(mem2);
}

static_assert(std::is_nothrow_constructible<
        container::simple_vector<int>,
        container::contiguous_storage<int>&&
    >::value, "required");

BOOST_AUTO_TEST_CASE( example_simple_vector )
{
    container::contiguous_storage<int> mem{implementation::default_malloc_allocator, 10};
    container::simple_vector<int> v(std::move(mem));
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(0, v.get_size());

    static_assert(noexcept(v.emplace_back()), "nothrow");
    v.emplace_back();
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(1, v.get_size());

    static_assert(noexcept(v.emplace_back(5)), "nothrow");
    v.emplace_back(5);
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(2, v.get_size());
}

BOOST_AUTO_TEST_CASE( example_simple_vector2 )
{
    container::simple_vector<int> v{implementation::default_malloc_allocator, 10};
    static_assert(noexcept(v.emplace_back()), "nothrow");
    v.emplace_back();
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(1, v.get_size());

    static_assert(noexcept(v.emplace_back(5)), "nothrow");
    v.emplace_back(5);
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(2, v.get_size());
}

BOOST_AUTO_TEST_CASE( example_simple_vector_string )
{
    container::contiguous_storage<std::string> mem{implementation::default_malloc_allocator, 10};
    container::simple_vector<std::string> v(std::move(mem));
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(0, v.get_size());

    v.emplace_back();
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(1, v.get_size());

    v.emplace_back("hello, world!");
    BOOST_CHECK_EQUAL(10, v.get_capacity());
    BOOST_CHECK_EQUAL(2, v.get_size());

    container::simple_vector<std::string> v2(std::move(v));
    BOOST_CHECK_EQUAL(10, v2.get_capacity());
    BOOST_CHECK_EQUAL(2, v2.get_size());

    auto s = v2.detach_storage();
    BOOST_CHECK_EQUAL(0, v2.get_capacity());
    BOOST_CHECK_EQUAL(0, v2.get_size());
    BOOST_CHECK_EQUAL(10, s.get_capacity());
}

BOOST_AUTO_TEST_CASE( example_simple_vector_iterate )
{
    container::contiguous_storage<int> mem{implementation::default_malloc_allocator, 10};
	container::simple_vector<int> v(std::move(mem));
	
    v.emplace_back(1);
    v.emplace_back(1);
    v.emplace_back(1);
	
	int sum = 0;
	container::interface::iteratable_range<int> r = v;
	r.for_each([&sum](int i) { sum+=i; });
	BOOST_CHECK_EQUAL(sum, 3);
}

template<typename T> using range = container::interface::iteratable_range<T>;
template<typename T> using vec = container::simple_vector<T>;

BOOST_AUTO_TEST_CASE( example_simple_vector_iterate2 )
{
	static_assert(std::is_nothrow_constructible<range<int>, vec<int>&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int>, vec<int>const&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int>, vec<int>&&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int const&>, vec<int>&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int const&>, vec<int>const&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int const&>, vec<int>&&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int&>, vec<int>&>::value, "required");
	static_assert( !      std::is_constructible<range<int&>, vec<int>const&>::value, "required");
	static_assert( !      std::is_constructible<range<int&>, vec<int>&&>::value, "required");
	static_assert( !      std::is_constructible<range<int&&>, vec<int>&>::value, "required");
	static_assert( !      std::is_constructible<range<int&&>, vec<int>const&>::value, "required");
	static_assert(std::is_nothrow_constructible<range<int&&>, vec<int>&&>::value, "required");

    container::contiguous_storage<std::string> mem{implementation::default_malloc_allocator, 10};
	container::simple_vector<std::string> v;
	
    mem = container::contiguous_storage<std::string>{implementation::default_malloc_allocator, 10};
	v = container::simple_vector<std::string>(std::move(mem));
	v.emplace_back("hello");
	v.emplace_back(", ");
	v.emplace_back("world");

	{
	container::interface::iteratable_range<std::string&&> r = std::move(v);
	std::string tmp;
	r.for_each([&tmp](std::string s) {
			tmp = std::move(s);
		});
	int i = 0;
	r.for_each([&i](std::string&& s) {
			if(s.empty()) ++i;
		});
	BOOST_CHECK_EQUAL(i, 3);
	}

/*
    container::contiguous_storage<int> mem{implementation::default_malloc_allocator, 10};
	container::simple_vector<int> v(std::move(mem));
	
    v.emplace_back(1);
    v.emplace_back(1);
    v.emplace_back(1);
	
	using vec = container::simple_vector<int>;
	container::simple_vector<int> const& vcref = v;
	
	static_cast<range<int>>(v);
	static_cast<range<int const&>>(v);
	int sum = 0;
	container::interface::iteratable_range<int> r2 = vcref;
	r2.for_each([&sum](int i) { sum+=i; });
	*/
}
