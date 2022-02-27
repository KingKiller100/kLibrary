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
		using Job_t = std::function<void()>;

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

			const auto jobWrapper =
				[task]() 
			{
				( *task )();
			};

			jobsQueue_.Enqueue( std::move( jobWrapper ) );

			return future.share();
		}

	protected:
		void WorkLoop();

	protected:
		SafeQueue<Job_t> jobsQueue_;
		std::atomic_bool terminator_;
		std::vector<std::thread> pool_;
		std::atomic_int64_t waiting_;
	};
}
