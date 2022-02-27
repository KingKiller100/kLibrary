#pragma once

#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace klib::kThread
{
	template <typename T, typename Container = std::deque<T>>
	class SafeQueue
	{
	public:
		SafeQueue()
			: terminateFlag_( false )
		{ }

		SafeQueue( const SafeQueue& ) = delete;
		SafeQueue& operator=( const SafeQueue& ) const = delete;

		SafeQueue( SafeQueue&& ) = default;
		SafeQueue& operator=( SafeQueue&& ) = default;


		~SafeQueue()
		{
			if ( !terminateFlag_ )
				Terminate();
		}

		void Enqueue( T&& val )
		{
			std::scoped_lock lock( mutex_ );
			container_.push_front( std::forward<T>( val ) );
			threadNotifier_.notify_one();
		}

		[[nodiscard]] bool IsEmpty() const noexcept
		{
			std::unique_lock lock( mutex_ );
			return container_.empty();
		}

		auto Size() const noexcept
		{
			std::unique_lock lock( mutex_ );
			return container_.size();
		}

		std::optional<T> Dequeue()
		{
			std::unique_lock lock( mutex_ );
			threadNotifier_.wait( lock, [&]
			{
				return !container_.empty() || terminateFlag_;
			} );

			std::optional<T> val;

			if ( container_.empty() )
				val = std::nullopt;
			else
			{
				val = std::move( container_.front() );
				container_.pop_front();
			}

			return val;
		}

		void Clear()
		{
			std::unique_lock lock( mutex_ );
			container_.clear();
			threadNotifier_.notify_all();
		}

		void Terminate()
		{
			std::unique_lock lock( mutex_ );
			terminateFlag_ = true;
			threadNotifier_.notify_all();
		}

	private:
		Container container_;
		mutable std::mutex mutex_;
		std::condition_variable threadNotifier_;
		bool terminateFlag_;
	};
}
