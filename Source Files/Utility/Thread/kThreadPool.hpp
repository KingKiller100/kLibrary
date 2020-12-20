#pragma once
#include <functional>
#include <mutex>
#include <queue>

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
		ThreadPool(size_t count);

		~ThreadPool();

		void Shutdown(size_t index);
		
		void ShutdownAll();

		void JoinAll();
		
		void JoinAndPopAll();

		void Join(size_t index);

		void Detach(size_t index);

		void DetachAll();

		std::thread::id GetID(size_t index);

		std::vector<std::thread::id> GetIDs();

		void DoJob(Job job);

		std::thread& GetThread(size_t index);

		static std::uint32_t MaxCores() noexcept;

	protected:
		void threadEntry(const bool& sd);

	protected:
		std::mutex mutex;
		std::condition_variable condVar;
		std::vector<bool> shutdowns;
		std::queue<Job> jobs;
		std::vector<std::thread> threads;
		std::string prevJob;
	};
}