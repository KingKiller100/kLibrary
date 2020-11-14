#include "pch.hpp"
#include "NotImplementedException.hpp"

namespace klib::kDebug
{
	NotImplementedException::NotImplementedException(const char* what)
		: runtime_error(what)
	{ }

	NotImplementedException::~NotImplementedException() noexcept
		= default;
}
