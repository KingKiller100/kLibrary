﻿#pragma once
#include "../HelperMacros.hpp"

namespace klib::type_trait
{
	struct BooleanWrapper
	{
		bool val;

		constexpr BooleanWrapper() : val(false) {}
		constexpr BooleanWrapper(bool v) : val(v) {}

		template<typename Target_t>
		constexpr operator Target_t() const
		{
			return val;
		}

		constexpr operator bool() const
		{
			return val;
		}

	};
	
	struct BooleanTraits
	{
		static constexpr bool True = true;
		static constexpr bool False = false;

		template<class T>
		USE_RESULT static constexpr T Convert(bool b) noexcept {
			return static_cast<T>(b);
		}
	};

	template<typename T>
	struct Is_BoolBase : std::false_type
	{};

	template<>
	struct Is_BoolBase<bool> : std::true_type
	{};

	template<typename T>
	struct Is_Bool : Is_BoolBase<std::remove_cv_t<T>>
	{};

	template<typename T>
	constexpr auto Is_Bool_V = Is_Bool<T>::value;

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_Bool_t = Is_Bool_V<T> == true;
#endif
	
}
