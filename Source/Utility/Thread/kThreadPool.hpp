#pragma once
#include "../../TypeTraits/BooleanTraits.hpp"

#include <functional>
#include <mutex>
#include <queue>
#include <thread>

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

			Job( std::function<Func_t> taskToDo, std::string_view description );

			void operator()() const;
		};

		class SafeThread
		{
		public:
			explicit SafeThread();

			explicit SafeThread( std::thread t, std::function<void()> exitEvent );

			~SafeThread();

			[[nodiscard]] bool Active() const noexcept;

			void SetThread( std::thread&& t );

			void SetExitEvent( std::function<void()> exitEvent );

			[[nodiscard]] auto GetID() const;

		private:
			std::thread thread_;
			std::function<void()> onExitEvent_;
		};

	public:
		ThreadPool( size_t count );

		ThreadPool( const ThreadPool& other ) noexcept = delete;
		ThreadPool& operator=( const ThreadPool& other ) noexcept = delete;

		ThreadPool( ThreadPool&& other ) noexcept = delete;
		ThreadPool& operator=( ThreadPool&& other ) noexcept = delete;

		~ThreadPool();

		void AddThread( size_t count );

		void ShutdownAll();

		bool CanJoinAll() const;

		void JoinAndPopAll();

		void PopJob();

		void ClearJobs();

		[[nodiscard]] size_t QueueSize() const;
		[[nodiscard]] bool IsQueueEmpty() const;

		[[nodiscard]] size_t GetSize() const;

		[[nodiscard]] std::vector<std::thread::id> GetIDs() const;

		void QueueJob( const Job& job );

		std::string_view LatestJob() const noexcept;

	protected:
		void ThreadLoop( size_t index );

	protected:
		std::mutex mutex_;
		std::condition_variable threadNotifier_;
		std::vector<type_trait::BooleanWrapper> shutdowns_;
		std::queue<Job> jobs_;
		std::vector<std::unique_ptr<SafeThread>> threads_;
		std::string prevJobDesc_;
	};
}
