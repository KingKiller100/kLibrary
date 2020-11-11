#pragma once

#include <memory>
#include <optional>

namespace klib
{
	namespace type_trait
	{
		namespace secret::helper
		{
			template<typename Impl, typename Base,
				typename = std::enable_if_t<std::is_base_of_v<Base, Impl>>>
				Impl* ToImplPtr(const std::unique_ptr<Base>& base)
			{
				if (!base)
					throw std::runtime_error("Null ptr given to function: " __FUNCSIG__);

				Impl* impl = dynamic_cast<Impl*>(base.get());

				if (!impl)
				{
					const std::string baseName = typeid(Base).name();
					const std::string implName = typeid(impl).name();
					const std::string msg = "Bad cast of " + baseName + " ptr to " + implName + " in function: " __FUNCSIG__;
					throw std::runtime_error(msg);
				}

				return impl;
			}
		}


		template<typename Impl, typename Base,
			typename = std::enable_if_t<std::is_base_of_v<Base, Impl>>>
			Impl& ToImpl(const std::unique_ptr<Base>& base)
		{
			Impl* impl = secret::helper::ToImplPtr<Impl, Base>(base);
			return *impl;
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace type_trait;
#endif

}