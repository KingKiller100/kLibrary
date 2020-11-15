#pragma once

#include "TemplateTraits.hpp"
#include <memory>

namespace klib::type_trait
{
	template<typename T>
	constexpr bool Is_SmartPtr_V =
		Is_Specialization_V<T, std::shared_ptr>
		|| Is_Specialization_V<T, std::weak_ptr>
		|| Is_Specialization_V<T, std::unique_ptr>;

	template<typename T>
	struct Is_SmartPtrBase : public std::bool_constant<Is_SmartPtr_V<T>> {};

	template<typename T>
	struct Is_SmartPtr : public Is_SmartPtrBase<std::remove_cv_t<T>> {};
}