#include "pch.hpp"
#include "kThreadPool.hpp"

namespace klib::kThread
{
	ThreadPool::Job::Job() noexcept
		: Job( nullptr, "" )
	{}

	ThreadPool::Job::Job( std::function<Func_t> taskToDo, std::string_view description )
		: task( taskToDo )
		, desc( description )
	{}

	void ThreadPool::Job::operator()() const
	{
		if ( task )
			task();
	}

	ThreadPool::SafeThread::SafeThread() = default;

	ThreadPool::SafeThread::SafeThread( std::thread t, std::function<void()> exitEvent )
		: thread_( std::move( t ) )
		, onExitEvent_( exitEvent )
	{}

	ThreadPool::SafeThread::~SafeThread()
	{
		if ( thread_.joinable() )
		{
			if ( onExitEvent_ )
				onExitEvent_();
			thread_.join();
		}
	}

	bool ThreadPool::SafeThread::Active() const noexcept
	{
		return thread_.joinable();
	}

	void ThreadPool::SafeThread::SetThread( std::thread&& t )
	{
		thread_ = std::move( t );
	}

	void ThreadPool::SafeThread::SetExitEvent( std::function<void()> exitEvent )
	{
		onExitEvent_ = std::move( exitEvent );
	}

	auto ThreadPool::SafeThread::GetID() const
	{
		return thread_.get_id();
	}

	ThreadPool::ThreadPool( size_t count )
	{
		AddThread( count );
	}

	ThreadPool::~ThreadPool()
	{
		// Unblock any threads and tell them to stop
		std::scoped_lock<std::mutex> lock( mutex_ );

		// Wait for all threads to stop
		JoinAndPopAll();
	}

	void ThreadPool::AddThread( size_t count )
	{
		// Create the specified number of threads
		const auto currentSize = GetSize();
		const auto newSize = currentSize + count;

		threads_.reserve( newSize );
		shutdowns_.resize( newSize, false );

		for ( size_t i = 0; i < count; ++i )
		{
			auto& safeThr = threads_.emplace_back( std::make_unique<SafeThread>(  ) );
			// SafeThread safeThr;
			safeThr->SetExitEvent( [&,i]
			{
				shutdowns_[i] = true;
				threadNotifier_.notify_all();
			} );
			safeThr->SetThread( std::move( std::thread( [this, i]
				{
					ThreadLoop( i );
				} ) )
			);
		}
	}

	void ThreadPool::ShutdownAll()
	{
		std::ranges::for_each( shutdowns_, []( auto&& bw )
		{
			bw = true;
		} );

		threadNotifier_.notify_all();
	}

	bool ThreadPool::CanJoinAll() const
	{
		for ( const auto& thr : threads_ )
		{
			if ( !thr->Active() )
				return false;
		}

		return true;
	}

	void ThreadPool::JoinAndPopAll()
	{
		threads_.clear();
		shutdowns_.clear();
	}

	void ThreadPool::ClearJobs()
	{
		while ( !jobs_.empty() )
		{
			PopJob();
		}
	}

	void ThreadPool::PopJob()
	{
		jobs_.pop();
	}

	size_t ThreadPool::QueueSize() const
	{
		return jobs_.size();
	}

	bool ThreadPool::IsQueueEmpty() const
	{
		return jobs_.empty();
	}

	size_t ThreadPool::GetSize() const
	{
		return threads_.size();
	}

	std::vector<std::thread::id> ThreadPool::GetIDs() const
	{
		std::vector<std::thread::id> ids;
		ids.reserve( threads_.size() );

		for ( auto&& thread : threads_ )
		{
			ids.emplace_back( thread->GetID() );
		}

		return ids;
	}

	void ThreadPool::QueueJob( const Job& job )
	{
		// Place a job on the queue and unblock a thread
		std::unique_lock<std::mutex> l( mutex_ );

		jobs_.emplace( std::move( job ) );
		threadNotifier_.notify_one();
	}

	std::string_view ThreadPool::LatestJob() const noexcept
	{
		return prevJobDesc_;
	}

	void ThreadPool::ThreadLoop( size_t index )
	{
		Job job;
		const auto& sd = shutdowns_[index];

		while ( !sd )
		{
			{
				std::unique_lock<std::mutex> lock( mutex_ );

				threadNotifier_.wait( lock, [this, &sd]
				{
					return sd || !IsQueueEmpty();
				} );

				if ( IsQueueEmpty() )
				{
					// No jobs to do and we are shutting down
					return;
				}

				job = std::move( jobs_.front() );
				prevJobDesc_ = std::move( job.desc );
				jobs_.pop();
			}

			// Do the job without holding any locks
			job();
		}
	}
}
