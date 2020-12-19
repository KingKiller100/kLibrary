#pragma once
#include "../../HelperMacros.hpp"

#include <thread>

namespace klib::kThread
{
	template<typename T>
	USE_RESULT T ThreadID(const std::thread::id& id)
	{
		return T(std::hash<std::thread::id>{}(id));
	}
	
	template<typename T>
	USE_RESULT T ThreadID()
	{
		return ThreadID<T>(std::this_thread::get_id());
	}
}
