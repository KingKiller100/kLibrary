#pragma once

#include "kSafeQueue.hpp"

#include <atomic>
#include <functional>
#include <future>
#include <thread>


namespace klib::kThread
{
	class ThreadPool
	{
	public:
		using Task_t = void();

		struct Job
		{
			std::function<Task_t> task;

			Job( std::function<Task_t> taskToDo = nullptr ) noexcept;

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

		[[nodiscard]] std::int64_t IdleCount() const noexcept;

		[[nodiscard]] size_t QueueSize() const;

		[[nodiscard]] size_t GetSize() const;

		[[nodiscard]] std::vector<std::thread::id> GetIDs() const;

		// Place a job on the queue and unblock a thread
		template <typename Fn, typename ...Params>
		auto EnqueueJob( Fn&& jobImpl, Params&&... params )
		{
			using Ret_t = decltype( jobImpl( std::forward<Params>( params )... ) );

			auto task = std::make_shared<std::packaged_task<Ret_t()>>(
				std::bind( std::forward<Fn>( jobImpl ),
					std::forward<Params>( params )... )
			);

			auto future = task->get_future();

			{
				const std::function<Task_t> jobWrapper =
					[task]() mutable
				{
					( *task )();
				};
				
				jobsQueue_.Enqueue( Job{ jobWrapper } );
			}
			
			return future.share();
		}

	protected:
		void WorkLoop();

	protected:
		SafeQueue<Job> jobsQueue_;
		std::atomic_bool terminator_;
		std::vector<std::thread> pool_;
		std::atomic_int64_t waiting_;
		
	};
}
