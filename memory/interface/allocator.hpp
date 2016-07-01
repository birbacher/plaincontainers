#ifndef NEWCONTAINERS_INTERFACE_ALLOCATOR_INCLUDED
#define NEWCONTAINERS_INTERFACE_ALLOCATOR_INCLUDED

#include "memory/interface/allocator_fwd.hpp"

#include <type_traits>

namespace memory {
namespace interface {

template<
        template<typename> class PointerModel,
        typename SizeType
    >
struct allocator
{
    template<typename T>
	using pointer_model = PointerModel<T>;
    typedef pointer_model<void> pointer;
    typedef SizeType size_type;
	
	static_assert(std::is_nothrow_copy_constructible<pointer>::value, "required");
	static_assert(std::is_nothrow_move_constructible<pointer>::value, "required");

	constexpr allocator() {} /* = default will break on shared lib export */
	
    pointer allocate_aligned(size_type const bytes, size_type const alignment)
        { return do_allocate_aligned(bytes, alignment); }
    void deallocate(pointer const p, size_type const bytes) noexcept
        { do_deallocate(p, bytes); }
protected:
    virtual pointer do_allocate_aligned(size_type, size_type) =0;
    virtual void do_deallocate(pointer, size_type) noexcept =0;
};

extern template struct allocator<>;

} //namespace interface
} //namespace memory

#endif
