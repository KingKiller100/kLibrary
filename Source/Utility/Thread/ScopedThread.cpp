#include "pch.hpp"
#include "ScopedThread.hpp"

namespace klib::kThread
{
	ScopedThread::ScopedThread() = default;

	ScopedThread::ScopedThread( std::thread t, std::function<void()> exitEvent )
		: thread_( std::move( t ) )
		, onExitEvent_( std::move( exitEvent ) )
	{}

	ScopedThread::~ScopedThread()
	{
		if ( thread_.joinable() )
		{
			if ( onExitEvent_ )
				onExitEvent_();
			thread_.join();
		}
	}

	bool ScopedThread::IsActive() const noexcept
	{
		return thread_.joinable();
	}

	void ScopedThread::SetThread( std::thread&& t )
	{
		thread_ = std::move( t );
	}

	void ScopedThread::SetExitEvent( std::function<void()> exitEvent )
	{
		onExitEvent_ = std::move( exitEvent );
	}

	auto ScopedThread::GetID() const
	{
		return thread_.get_id();
	}
}
