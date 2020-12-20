#include "pch.hpp"
#include "kThreadPool.hpp"
#include "../../Maths/kMaths.hpp"

namespace klib::kThread
{
	ThreadPool::ThreadPool(size_t count)
	{
		// Create the specified number of threads
		count = kmaths::Clamp(count, 0, std::numeric_limits<std::uint16_t>::max());

		threads.reserve(count);
		shutdowns.reserve(count);
		for (auto i = 0; i < count; ++i)
		{			
			const auto& sd = shutdowns.emplace_back(false);
			threads.emplace_back([this, sd] { threadEntry(sd); });
		}
	}

	ThreadPool::~ThreadPool()
	{
		{
			// Unblock any threads and tell them to stop
			std::unique_lock<std::mutex> l(mutex);

			ShutdownAll();
			condVar.notify_all();
		}

		// Wait for all threads to stop
		// std::cerr << "Joining threads" << std::endl;
		JoinAll();
	}

	void ThreadPool::Shutdown(size_t index)
	{
		if (index >= threads.size())
			throw std::out_of_range("Index >= number of threads");
		shutdowns[index] = true;
	}

	void ThreadPool::ShutdownAll()
	{
		for (auto&& sd : shutdowns)
			sd = true;
	}

	void ThreadPool::Join(size_t index)
	{
		if (index >= threads.size())
			throw std::out_of_range("Index >= number of threads");
		threads[index].join();
	}

	void ThreadPool::JoinAll()
	{
		for (auto&& thread : threads)
		{
			thread.join();
		}
	}

	void ThreadPool::Detach(size_t index)
	{
		if (index >= threads.size())
			throw std::out_of_range("Index >= number of threads");
		threads[index].join();
	}

	void ThreadPool::DetachAll()
	{
		for (auto&& thread : threads)
		{
			thread.detach();
		}
	}

	std::thread::id ThreadPool::GetID(size_t index)
	{
		if (index >= threads.size())
			throw std::out_of_range("Index >= number of threads");
		return threads[index].get_id();
	}

	std::vector<std::thread::id> ThreadPool::GetIDs()
	{
		std::vector<std::thread::id> ids;
		ids.reserve(threads.size());

		for (auto&& thread : threads)
		{
			ids.emplace_back(thread.get_id());
		}

		return ids;
	}

	void ThreadPool::DoJob(Func_t func)
	{
		// Place a job on the queue and unblock a thread
		std::unique_lock<std::mutex> l(mutex);

		jobs.emplace(std::move(func));
		condVar.notify_one();
	}

	std::thread& ThreadPool::GetThread(size_t index)
	{
		if (index >= threads.size())
			throw std::out_of_range("Index >= number of threads");
		return threads[index];
	}

	std::uint32_t ThreadPool::MaxCores() noexcept
	{
		return std::thread::hardware_concurrency();
	}

	void ThreadPool::threadEntry(const bool& sd)
	{
		Func_t job;

		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(mutex);

				while (!sd && jobs.empty())
					condVar.wait(lock);

				if (jobs.empty())
				{
					// No jobs to do and we are shutting down
					return;
				}

				job = std::move(jobs.front());
				jobs.pop();
			}

			// Do the job without holding any locks
			job();
		}
	}
}
