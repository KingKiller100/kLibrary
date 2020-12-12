#include "pch.hpp"
#include "TesterBase.hpp"

#include <iostream>

#ifdef TESTING_ENABLED

namespace kTest
{
	TesterBase::TesterBase(const char* name) noexcept
		: success(true), name(name)
	{	}

	TesterBase::TesterBase(TesterBase&& other) noexcept
	{
		*this = std::move(other);
	}

	TesterBase& TesterBase::operator=(TesterBase&& other) noexcept
	{
		this->success = std::move(other.success);
		this->name = std::move(other.name);
		this->failureData = std::move(other.failureData);
		return *this;
	}

	TesterBase::~TesterBase()
		{}

	const char* TesterBase::GetName() const noexcept
	{
		return name.c_str();
	}

	const std::string& TesterBase::GetFailureData() const noexcept
	{
		return failureData;
	}

	bool TesterBase::Run() noexcept
	{
		try
		{
			Test();
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			failureData.append(klib::kString::Sprintf("\tCondition: %s\n", e.what())); 
			success = false;
		}

		return success;
	}
}

#endif