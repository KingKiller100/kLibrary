#pragma once

#include <xtr1common>

namespace klib::type_trait
{
	// Applies a reference type depending on if the test conditions matches up to what's desired
	template<bool Test, typename T>
	struct ConditionalReference
	{
		using Real_t = T;
		using Type = std::conditional_t<Test
			, std::conditional_t<std::is_array_v<T>, std::remove_all_extents_t<std::remove_cv_t<Real_t>>*, Real_t&>
			, std::conditional_t<std::is_trivially_copyable_v<T>, Real_t, Real_t&>>;
	};
	
	template<bool Test, typename T>
	struct ConditionalConstReference
	{
		using Real_t = T;
		using Type = std::conditional_t<Test
			, std::conditional_t<std::is_array_v<T>, const std::remove_all_extents_t<std::remove_cv_t<Real_t>>*, const Real_t&>
			, std::conditional_t<std::is_trivially_copyable_v<T>, const Real_t, const Real_t&>>;
	};
	
	template<bool Test, typename T>
	using CondRef_t = typename ConditionalReference<Test, T>::Type;
	
	template<bool Test, typename T>
	using CondConstRef_t = typename ConditionalConstReference<Test, T>::Type;

	// Applies a reference type depending on if the sizeof T is greater than a pointer
	template<typename T>
	struct SizeConditionalReference : ConditionalReference<(sizeof(T) > sizeof(uintptr_t)), T>
	{};

	// Applies a reference type depending on if the sizeof T is greater than a pointer
	template<typename T>
	struct SizeConditionalConstReference : ConditionalConstReference<(sizeof(T) > sizeof(uintptr_t)), T>
	{};

	template<typename T>
	using SizeCondRef_t = typename SizeConditionalReference<T>::Type;

	template<typename T>
	using SizeCondConstRef_t = typename SizeConditionalConstReference<T>::Type;
}
