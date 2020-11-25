#pragma once

#include "TemplateTraits.hpp"
#include <memory>
#include <atomic>

namespace klib::type_trait
{
	template<typename T>
	using Atomic_Shared_Ptr = std::atomic<std::shared_ptr<T>>;
	
	template<typename T>
	using Atomic_Unique_Ptr = std::atomic<std::unique_ptr<T>>;
	
	template<typename T>
	using Atomic_Weak_Ptr = std::atomic<std::weak_ptr<T>>;

	template<typename T>
	struct Is_SmartPtrBase : public std::bool_constant<
		Is_Specialization_V<T, std::shared_ptr>
		|| Is_Specialization_V<T, Atomic_Shared_Ptr>
		|| Is_Specialization_V<T, std::weak_ptr>
		|| Is_Specialization_V<T, Atomic_Weak_Ptr>
		|| Is_Specialization_V<T, std::unique_ptr>
		|| Is_Specialization_V<T, Atomic_Unique_Ptr>> {};

	template<typename T>
	struct Is_SmartPtr : public Is_SmartPtrBase<std::remove_cv_t<T>> {};
	
	template<typename T>
	constexpr bool Is_SmartPtr_V = Is_SmartPtr<T>::value;

#if MSVC_PLATFORM_TOOLSET >= 142
	template<typename T>
	concept Is_SmartPtr_t = Is_SmartPtr_V<T> == true;
#endif
}