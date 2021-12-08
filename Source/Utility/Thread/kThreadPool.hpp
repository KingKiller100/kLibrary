#pragma once
#include "../../TypeTraits/BooleanTraits.hpp"

#include <functional>
#include <mutex>
#include <queue>

namespace klib::kThread
{
	class ThreadPool
	{
	public:
		using Func_t = void();

		struct Job
		{
			std::function<Func_t> task;
			std::string desc;

			Job() noexcept;

			Job(std::function<Func_t> taskToDo, std::string_view description);

			void operator()() const;
		};

	public:
		ThreadPool(size_t count);

		ThreadPool(const ThreadPool& other) noexcept = delete;
		ThreadPool& operator=(const ThreadPool& other) noexcept = delete;

		ThreadPool(ThreadPool&& other) noexcept = delete;
		ThreadPool& operator=(ThreadPool&& other) noexcept = delete;

		~ThreadPool();

		void AddThread(size_t count);

		void ShutdownAll();
		
		bool CanJoinAll() const;
		
		void JoinAndPopAll();

		void PopJob();

		void ClearJobs();

		[[nodiscard]] size_t GetSize() const;

		[[nodiscard]] std::vector<std::thread::id> GetIDs() const;

		void QueueJob(const Job& job);

	protected:
		void ThreadLoop(const type_trait::BooleanWrapper& shuttingDown);

	protected:
		std::mutex mutex;
		std::condition_variable condVar;
		std::vector<type_trait::BooleanWrapper> shutdowns;
		std::queue<Job> jobs;
		std::vector<std::thread> threads;
		std::string prevJobDesc;
	};
}
