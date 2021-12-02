#pragma once

#include "../Source/kSourceInfo.hpp"
#include "../../String/kToString.hpp"

namespace klib::kDebug
{
	template <typename Exception>
	struct ExceptionWrapper : std::runtime_error
	{
		Exception e;
		SourceInfo src;

		constexpr std::string_view TName() const noexcept
		{
			return typeid( Exception ).name();
		}

		constexpr ExceptionWrapper( const Exception& exception, const SourceInfo& sourceInfo )
			: runtime_error( kString::ToString( "[{0}]: {1} {2}", TName(), exception.what(), sourceInfo ) )
			, e( exception )
			, src( sourceInfo )
		{ }
	};

	class ExceptionWithSource
	{
	public:
		template <typename Exception>
		static auto Generate( const Exception& e, const SourceInfo& sourceInfo )
		{
			ExceptionWrapper<Exception> wrapper{e, sourceInfo};
			return wrapper;
		}

		template <typename Exception>
		[[noreturn]] static void Throw( const Exception& e, const SourceInfo& sourceInfo )
		{
			const auto wrapper = Generate( e, sourceInfo );
			throw wrapper.what();
		}
	};
}

#define THROW_WITH_SRC(e) klib::kDebug::ExceptionWithSource::Throw(e, SOURCE_INFO())

