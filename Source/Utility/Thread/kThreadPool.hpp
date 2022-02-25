#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <future>

namespace klib::kThread
{
	class ThreadPool
	{
	public:
		using Func_t = void();

		struct Job
		{
			std::function<Func_t> task;

			Job( std::function<Func_t> taskToDo = nullptr ) noexcept;

			void operator()() const;
		};

	public:
		ThreadPool();

		ThreadPool( const ThreadPool& other ) noexcept = delete;
		ThreadPool& operator=( const ThreadPool& other ) noexcept = delete;

		ThreadPool( ThreadPool&& other ) noexcept = delete;
		ThreadPool& operator=( ThreadPool&& other ) noexcept = delete;

		~ThreadPool();

		void Launch( size_t count );

		void Shutdown();

		bool CanJoinAll() const;

		void PopJob();

		void ClearJobs();

		std::int64_t IdleCount() const noexcept;

		[[nodiscard]] size_t QueueSize() const;
		[[nodiscard]] bool IsQueueEmpty() const;

		[[nodiscard]] size_t GetSize() const;

		[[nodiscard]] std::vector<std::thread::id> GetIDs() const;

		// Place a job on the queue and unblock a thread
		template <typename Fn, typename ...Params>
		auto QueueJob( Fn&& jobImpl, Params&&... params )
		{
			using Ret_t = decltype(jobImpl( std::forward<Params>( params )... ));

			auto task = std::make_shared<std::packaged_task<Ret_t( void )>>(
				std::bind( std::forward<Fn>( jobImpl ),
					std::forward<Params>( params )... )
			);

			auto future = task->get_future();

			{
				const std::function<void()> jobWrapper =
					[task]() mutable 
				{
					(*task)();
				};

				std::unique_lock<std::mutex> l( jobsMutex_ );
				jobsQueue_.emplace( Job{ jobWrapper } );
			}

			threadNotifier_.notify_one();
			return future;
		}

	protected:
		void ThreadLoop();
		void ModifyAtomicString( std::atomic<const char*>& str, std::string_view text ) const;

	protected:
		std::atomic_bool terminator_;
		std::queue<Job> jobsQueue_;
		std::vector<std::thread> pool_;
		std::atomic_int64_t waiting_;

		std::mutex jobsMutex_;
		std::condition_variable threadNotifier_;
	};
}
