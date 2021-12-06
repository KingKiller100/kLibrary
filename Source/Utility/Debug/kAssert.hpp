#pragma once

#include "Source/kSourceInfo.hpp"

#include <exception>
#include <functional>
#include <string>

namespace klib::kDebug
{
	class FailedConditionException final : public std::exception
	{
	public:
		using AssertFunc_t = std::function<void( std::string_view, std::string_view, const SourceInfo& )>;

	public:
		FailedConditionException(
			const std::string_view& expected
			, const std::string_view& msg
			, const SourceInfo& sourceInfo
			, const AssertFunc_t& cb
		);
		~FailedConditionException() throw() override;
	};
}
