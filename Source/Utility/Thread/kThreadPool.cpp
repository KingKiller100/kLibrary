#include "pch.hpp"
#include "kThreadPool.hpp"

namespace klib::kThread
{
	
	ThreadPool::Job::Job( std::function<Func_t> taskToDo ) noexcept
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
		, threadNotifier_()
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
				ThreadLoop();
			} );
		}
	}

	void ThreadPool::Shutdown()
	{
		terminator_.store( true );
		threadNotifier_.notify_all();
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
		while ( !jobsQueue_.empty() )
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
		jobsQueue_.pop();
	}

	size_t ThreadPool::QueueSize() const
	{
		return jobsQueue_.size();
	}

	bool ThreadPool::IsQueueEmpty() const
	{
		return jobsQueue_.empty();
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

	void ThreadPool::ThreadLoop()
	{
		Job job;

		while ( !terminator_.load() )
		{
			{
				std::unique_lock<std::mutex> lock( jobsMutex_ );

				waiting_.fetch_add( 1 );
				threadNotifier_.wait( lock,
					[&]()
					{
						return !jobsQueue_.empty() || terminator_.load();
					}
				);
				waiting_.fetch_sub( 1 );

				if ( terminator_.load() || jobsQueue_.empty() )
				{
					// No jobs to do and we are shutting down
					break;
				}

				job = std::move( jobsQueue_.front() );
				jobsQueue_.pop();
			}

			// Do the job without holding any locks
			job();
		}
	}

	void ThreadPool::ModifyAtomicString( std::atomic<const char*>& str, std::string_view text ) const
	{
		auto tempStr = std::make_unique<char[]>( text.size() + 1 );
		std::memcpy( tempStr.get(), text.data(), sizeof( char ) * text.size() );
		delete[] str.exchange( tempStr.release() );
	}
}
