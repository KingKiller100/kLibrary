#pragma once

namespace klib {
	namespace kTemplate
	{
		template<typename T>
		constexpr auto GetTypeName()
		{
			return typeid(T).name();
		}

		template<typename T>
		constexpr auto GetTypeName(T&&)
		{
			return typeid(T).name();
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kTemplate;
#endif
}

