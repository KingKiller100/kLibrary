#include "pch.hpp"
#include "kThreadPool.hpp"

namespace klib::kThread
{
	ThreadPool::Job::Job() noexcept
		: Job( nullptr, "" )
	{}

	ThreadPool::Job::Job( std::function<Func_t> taskToDo, std::string_view description )
		: desc( description )
		, task( taskToDo )
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
		, lastJob_( nullptr )
		, threadNotifier_()
	{ }

	ThreadPool::~ThreadPool()
	{
		JoinAndPopAll();
	}

	void ThreadPool::Launch( size_t count )
	{
		terminator_.store( false );

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

	void ThreadPool::JoinAndPopAll()
	{
		Shutdown();
		for ( auto& thread : pool_ )
		{
			if ( thread.joinable() )
				thread.join();
		}
		pool_.clear();
	}

	void ThreadPool::ClearJobs()
	{
		while ( !jobsQueue_.empty() )
		{
			PopJob();
		}
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

	void ThreadPool::QueueJob( const Job& job )
	{
		{
			std::unique_lock<std::mutex> l( jobsMutex_ );
			jobsQueue_.emplace( std::move( job ) );
		}

		threadNotifier_.notify_one();
	}

	void ThreadPool::ThreadLoop()
	{
		Job job;

		while ( true )
		{
			{
				std::unique_lock<std::mutex> lock( jobsMutex_ );

				threadNotifier_.wait( lock,
					[&]()
					{
						return !jobsQueue_.empty() || terminator_.load();
					}
				);

				if ( jobsQueue_.empty() )
				{
					// No jobs to do and we are shutting down
					break;
				}

				job = std::move( jobsQueue_.front() );
				ModifyAtomicString( lastJob_, job.desc );
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
