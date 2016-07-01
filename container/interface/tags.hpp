#ifndef NEWCONTAINERS_CONTAINER_TAGS_INCLUDED
#define NEWCONTAINERS_CONTAINER_TAGS_INCLUDED

#include <type_traits>

namespace container {
	namespace tags {
		
	}
	
	template<typename Tag, typename Container>
	using has_tag = std::is_same<Tag, typename Container::tag>;
}

#endif
