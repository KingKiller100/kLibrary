#include "pch.hpp"
#include "kThreadPool.hpp"

namespace klib::kThread
{
	ThreadPool::Job::Job( std::function<Task_t> taskToDo ) noexcept
		: task( taskToDo )
	{}

	void ThreadPool::Job::operator()() const
	{
		if ( task )
			task();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Thread Pool ///////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ThreadPool::ThreadPool()
		: terminator_( false )
		, waiting_( 0 )
	{ }

	ThreadPool::~ThreadPool()
	{
		if ( !pool_.empty() )
		{
			std::terminate();
		}
	}

	void ThreadPool::Launch( size_t count )
	{
		terminator_.store( false );
		waiting_.store( 0 );

		// Create the specified number of threads
		const auto currentSize = GetSize();
		const auto newSize = currentSize + count;

		pool_.resize( newSize );

		for ( auto& thread : pool_ )
		{
			thread = std::thread( [this]
			{
				WorkLoop();
			} );
		}
	}

	void ThreadPool::Shutdown()
	{
		jobsQueue_.Terminate();
		terminator_.store( true );
		for ( auto& thread : pool_ )
		{
			if ( thread.joinable() )
				thread.join();
		}
		pool_.clear();
	}

	bool ThreadPool::CanJoinAll() const
	{
		for ( const auto& thr : pool_ )
		{
			if ( !thr.joinable() )
				return false;
		}

		return true;
	}

	void ThreadPool::ClearJobs()
	{
		while ( !jobsQueue_.IsEmpty() )
		{
			PopJob();
		}
	}

	std::int64_t ThreadPool::IdleCount() const noexcept
	{
		return waiting_.load();
	}

	void ThreadPool::PopJob()
	{
		( void )jobsQueue_.Dequeue();
	}

	size_t ThreadPool::QueueSize() const
	{
		return jobsQueue_.Size();
	}

	size_t ThreadPool::GetSize() const
	{
		return pool_.size();
	}

	std::vector<std::thread::id> ThreadPool::GetIDs() const
	{
		std::vector<std::thread::id> ids;
		ids.reserve( pool_.size() );

		for ( auto&& thread : pool_ )
		{
			ids.emplace_back( thread.get_id() );
		}

		return ids;
	}

	void ThreadPool::WorkLoop()
	{
		std::optional<Job> job;

		while ( !terminator_.load() )
		{
			{
				waiting_.fetch_add( 1 );
				job = jobsQueue_.Dequeue();
				waiting_.fetch_sub( 1 );

				if ( job == std::nullopt || terminator_.load() )
				{
					// No jobs to do and/or we are shutting down
					break;
				}
			}

			// Do the job without holding any locks
			( *job )();
		}
	}
}
