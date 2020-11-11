#include "pch.hpp"
#include "Tester.hpp"

#include "TesterManager.hpp"

#ifdef TESTING_ENABLED

namespace kTest
{
	Tester::Tester(const char* name) noexcept
		: success(true), name(name)
	{	}

	Tester::Tester(Tester&& other) noexcept
		: success(true)
	{
		*this = std::move(other);
	}

	Tester& Tester::operator=(Tester&& other) noexcept
	{
		this->name = std::move(other.name);
		this->failureData = std::move(other.failureData);
		return *this;
	}

	Tester::~Tester()
		= default;

	const char* Tester::GetName() const noexcept
	{
		return name.c_str();
	}

	const std::string& Tester::GetFailureData() const noexcept
	{
		return failureData;
	}

	void Tester::Add(Tester* test)
	{
		TesterManager::Get().Add(test);
	}

	bool Tester::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (...)
		{
			success = false;
		}

		return success;
	}
}

#endif