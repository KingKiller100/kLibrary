#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>

namespace klib::kThread
{
	class ThreadPool
	{
	public:
		using Func_t = void();

		struct Job
		{
			std::string desc;
			std::function<Func_t> task;

			Job() noexcept;

			Job( std::function<Func_t> taskToDo, std::string_view description );

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

		void JoinAndPopAll();

		void PopJob();

		void ClearJobs();

		[[nodiscard]] size_t QueueSize() const;
		[[nodiscard]] bool IsQueueEmpty() const;

		[[nodiscard]] size_t GetSize() const;

		[[nodiscard]] std::vector<std::thread::id> GetIDs() const;

		// Place a job on the queue and unblock a thread
		void QueueJob( const Job& job );
		
	protected:
		void ThreadLoop();
		void ModifyAtomicString(std::atomic<const char*>& str, std::string_view text) const;

	protected:
		std::mutex jobsMutex_;
		std::condition_variable threadNotifier_;
		std::atomic_bool terminator_;
		std::queue<Job> jobsQueue_;
		std::atomic<const char*> lastJob_;
		std::vector<std::thread> pool_;
	};
}
