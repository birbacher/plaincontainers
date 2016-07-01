#include "memory/implementation/failing_allocator_impl.hpp"

namespace memory {
namespace implementation {

template struct failing_allocator<interface::plain_pointer, std::size_t>;

failing_allocator<interface::plain_pointer, std::size_t> default_failing_allocator{};

} //namespace implementation
} //namespace memory
