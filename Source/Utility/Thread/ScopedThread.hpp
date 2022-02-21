#pragma once
#include <functional>
#include <thread>

namespace klib::kThread
{
	class ScopedThread
	{
	public:
		explicit ScopedThread();

		explicit ScopedThread( std::thread t, std::function<void()> exitEvent );

		~ScopedThread();

		[[nodiscard]] bool IsActive() const noexcept;

		void SetThread( std::thread&& t );

		void SetExitEvent( std::function<void()> exitEvent );

		[[nodiscard]] auto GetID() const;

	private:
		std::thread thread_;
		std::function<void()> onExitEvent_;
	};
}
