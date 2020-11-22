#pragma once

#include <memory>
#include <optional>

namespace klib
{
	namespace type_trait
	{
		namespace secret::impl
		{
			template<typename Impl_t, typename Base_t,
				typename = std::enable_if_t<std::is_base_of_v<Base_t, Impl_t>>>
				Impl_t* ToImplPtr(const std::unique_ptr<Base_t>& base)
			{
				if (!base)
					throw std::runtime_error("Null ptr given to function: " __FUNCSIG__);

				Impl_t* derived = dynamic_cast<Impl_t*>(base.get());

				if (!derived)
				{
					const std::string baseName = typeid(Base_t).name();
					const std::string implName = typeid(Impl_t).name();
					const std::string msg = "Bad cast of " + baseName + " ptr to " + implName + " in function: " __FUNCSIG__;
					throw std::runtime_error(msg);
				}

				return derived;
			}
		}


		template<typename Impl_t, typename Base_t,
			typename = std::enable_if_t<std::is_base_of_v<Base_t, Impl_t>>>
			Impl_t& ToImpl(const std::unique_ptr<Base_t>& base)
		{
			Impl_t* impl = secret::impl::ToImplPtr<Impl_t, Base_t>(base);
			return *impl;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace type_trait;
#endif

}