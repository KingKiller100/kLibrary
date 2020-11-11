#pragma once

#include <stdexcept>

namespace klib::kDebug
{
	class NotImplementedException final : public std::runtime_error
	{
	public:
		NotImplementedException(const char* what);
		~NotImplementedException() noexcept;
	};
}

#define FUNC_NO_IMPL() ::klib::kDebug::NotImplementedException(__FUNCSIG__ " Not Implemented")