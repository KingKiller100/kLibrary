#pragma once
#include <functional>
#include <mutex>
#include <queue>

#include "../../TypeTraits/BooleanTraits.hpp"

namespace klib::kThread
{
	class ThreadPool
	{
	public:
		using Func_t = std::function<void()>;

		struct Job
		{
			Func_t work;
			std::string desc;

			void operator()() const
			{
				work();
			}
		};
		
	public:
		ThreadPool();
		ThreadPool(size_t count);

		ThreadPool(const ThreadPool& other) noexcept = delete;
		ThreadPool& operator=(const ThreadPool& other) noexcept = delete;

		ThreadPool(ThreadPool&& other) noexcept = delete;
		ThreadPool& operator=(ThreadPool&& other) noexcept = delete;
		
		~ThreadPool();

		void AddThread(size_t count);
		
		void Shutdown(size_t index);
		
		void ShutdownAll();

		bool CanJoin(size_t index) const;
		
		bool CanJoinAll() const;

		void Join(size_t index);

		void JoinAll();
		
		void JoinAndPopAll();

		void Detach(size_t index);

		void DetachAll();

		void PopJob();
		
		void ClearJobs();
		
		size_t GetSize() const;
		
		std::thread::id GetID(size_t index) const;

		std::vector<std::thread::id> GetIDs() const;

		void QueueJob(Job job);

		std::thread& GetThread(size_t index);
		const std::thread& GetThread(size_t index) const;

	protected:
		void ThreadLoop(const type_trait::BooleanWrapper& sd);
		
	protected:
		std::mutex mutex;
		std::condition_variable condVar;
		std::vector<type_trait::BooleanWrapper> shutdowns;
		std::queue<Job> jobs;
		std::vector<std::thread> threads;
		std::string prevJob;
	};
}
