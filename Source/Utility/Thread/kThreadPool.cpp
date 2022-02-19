#include "pch.hpp"
#include "kThreadPool.hpp"
#include "../../Maths/kAlgorithms.hpp"

namespace klib::kThread
{
	ThreadPool::Job::Job() noexcept
		: task( nullptr )
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

	ThreadPool::ThreadPool( size_t count )
	{
		AddThread( count );
	}

	ThreadPool::~ThreadPool()
	{
		{
			// Unblock any threads and tell them to stop
			std::scoped_lock<std::mutex> l( mutex );

			ShutdownAll();
			condVar.notify_all();
		}

		// Wait for all threads to stop
		JoinAndPopAll();
	}

	void ThreadPool::AddThread( size_t count )
	{
		// Create the specified number of threads
		count = kmaths::Min( count, std::numeric_limits<std::uint8_t>::max() );

		const auto currentSize = GetSize();
		const auto newSize = currentSize + count;
		threads.reserve( newSize );
		shutdowns.reserve( newSize );

		for ( auto i = 0; i < count; ++i )
		{
			const auto& sd = shutdowns.emplace_back( false );
			threads.emplace_back( [this, &sd]
			{
				ThreadLoop( sd );
			} );
		}
	}

	void ThreadPool::ShutdownAll()
	{
		for ( auto& sd : shutdowns )
			sd = true;
	}

	bool ThreadPool::CanJoinAll() const
	{
		for ( const auto& thr : threads )
		{
			if ( !thr.joinable() )
				return false;
		}

		return true;
	}

	void ThreadPool::JoinAndPopAll()
	{
		while ( !threads.empty() )
		{
			auto& thr = threads.back();
			if ( thr.joinable() )
			{
				shutdowns.back() = true;
				thr.join();
			}

			threads.pop_back();
			shutdowns.pop_back();
		}
	}

	void ThreadPool::PopJob()
	{
		jobs.pop();
	}

	void ThreadPool::ClearJobs()
	{
		while ( !jobs.empty() )
		{
			PopJob();
		}
	}

	size_t ThreadPool::GetSize() const
	{
		return threads.size();
	}

	std::vector<std::thread::id> ThreadPool::GetIDs() const
	{
		std::vector<std::thread::id> ids;
		ids.reserve( threads.size() );

		for ( auto&& thread : threads )
		{
			ids.emplace_back( thread.get_id() );
		}

		return ids;
	}

	void ThreadPool::QueueJob( const Job& job )
	{
		// Place a job on the queue and unblock a thread
		std::unique_lock<std::mutex> l( mutex );

		jobs.emplace( std::move( job ) );
		condVar.notify_one();
	}

	std::string_view ThreadPool::LatestJob() const noexcept
	{
		return prevJobDesc;
	}

	void ThreadPool::ThreadLoop( const type_trait::BooleanWrapper& shuttingDown )
	{
		Job job;

		// Infinite loop
		while ( true )
		{
			{
				std::unique_lock<std::mutex> lock( mutex );

				condVar.wait( lock, [&shuttingDown, this]
				{
					return shuttingDown || !jobs.empty();
				} );

				if ( jobs.empty() )
				{
					// No jobs to do and we are shutting down
					return;
				}

				job = std::move( jobs.front() );
				prevJobDesc = std::move( job.desc );
				jobs.pop();
			}

			// Do the job without holding any locks
			job();
		}
	}
}
