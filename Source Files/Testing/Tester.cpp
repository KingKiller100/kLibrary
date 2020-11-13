#include "pch.hpp"
#include "Tester.hpp"

#include <iostream>

#ifdef TESTING_ENABLED

namespace kTest
{
	Tester::Tester(const char* name) noexcept
		: success(true), name(name)
	{	}

	Tester::Tester(Tester&& other) noexcept
	{
		*this = std::move(other);
	}

	Tester& Tester::operator=(Tester&& other) noexcept
	{
		this->success = std::move(other.success);
		this->name = std::move(other.name);
		this->failureData = std::move(other.failureData);
		return *this;
	}

	Tester::~Tester()
		{}

	const char* Tester::GetName() const noexcept
	{
		return name.c_str();
	}

	const std::string& Tester::GetFailureData() const noexcept
	{
		return failureData;
	}

	bool Tester::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			success = false;
		}

		return success;
	}
}

#endif