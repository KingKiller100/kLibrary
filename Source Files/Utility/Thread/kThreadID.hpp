#pragma once
#include "../../HelperMacros.hpp"

#include <thread>

namespace klib::kThread
{
	template<typename T>
	USE_RESULT T GetThreadID(const std::thread::id& id)
	{
		return T(std::hash<std::thread::id>{}(id));
	}
	
	template<typename T>
	USE_RESULT T GetThreadID(const std::thread& thr)
	{
		return T(std::hash<std::thread::id>{}(thr.get_id()));
	}
	
	template<typename T>
	USE_RESULT T GetThreadID()
	{
		return GetThreadID<T>(std::this_thread::get_id());
	}
}
