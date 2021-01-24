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
		
		template<typename T>
		constexpr auto GetTypeHashCode() noexcept
		{
			return typeid(T).hash_code();
		}
		
		template<typename T>
		constexpr auto GetRawTypeName() noexcept
		{
			return typeid(T).raw_name();
		}
		
		template<typename T>
		constexpr auto GetRawTypeName(T&&) noexcept
		{
			return GetRawTypeName<T>();
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kTemplate;
#endif
}

