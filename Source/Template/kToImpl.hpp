#pragma once

#include <memory>
#include <optional>

namespace klib
{
	namespace kTemplate
	{
		namespace secret::impl
		{
			template<typename Impl_t, typename Base_t,
				typename = std::enable_if_t<std::is_base_of_v<Base_t, Impl_t>>>
				Impl_t* ToImplPtr(Base_t* base)
			{
				if (!base)
					throw std::runtime_error("Null ptr given to function: " __FUNCTION__);

				Impl_t* derived = reinterpret_cast<Impl_t*>(base);

				if (!derived)
				{
					const std::string baseName = typeid(Base_t).name();
					const std::string implName = typeid(Impl_t).name();
					const std::string msg = "Bad cast of " + baseName + " ptr to " + implName + " in function: " __FUNCTION__;
					throw std::runtime_error(msg);
				}

				return derived;
			}
		}


		template<typename Impl_t, typename Base_t>
		std::enable_if_t<
			std::is_base_of_v<Base_t, Impl_t>
			, const Impl_t&> ToImpl(const Base_t& base)
		{
			const auto& derived = dynamic_cast<const Impl_t&>(base);
			return derived;
		}

		template<typename Impl_t, typename Base_t>
		std::enable_if_t<
			std::is_base_of_v<Base_t, Impl_t>
			, Impl_t&> ToImpl(Base_t& base)
		{
			Impl_t& derived = dynamic_cast<Impl_t&>(base);
			return derived;
		}

		template<typename Impl_t, typename Base_t>
		std::enable_if_t<
			std::is_base_of_v<typename Base_t::element_type, Impl_t>
			&& type_trait::Is_SmartPtr_V<Base_t>
			, Impl_t&> ToImpl(const Base_t& base)
		{
			Impl_t* impl = secret::impl::ToImplPtr<Impl_t, typename Base_t::element_type>(base.get());
			return *impl;
		}

		template<typename Impl_t, typename Base_t>
		std::enable_if_t<
			std::is_base_of_v<Base_t, Impl_t>
			, Impl_t&> ToImpl(Base_t* base)
		{
			Impl_t* impl = secret::impl::ToImplPtr<Impl_t, Base_t>(base);
			return *impl;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kTemplate;
#endif

}