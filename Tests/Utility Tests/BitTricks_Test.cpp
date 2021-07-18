﻿#include "pch.hpp"
#include "BitTricks_Test.hpp"

#include "../../Source/Utility/Bits/kBitTricks.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	BitTricksTester::BitTricksTester()
		: TesterBase("Bit Tricks Test")
	{}

	BitTricksTester::~BitTricksTester()
		= default;

	using namespace klib::kBits;
	
	void BitTricksTester::Prepare()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(SetTest());
		VERIFY_MULTI(ClearTest());
		VERIFY_MULTI(CheckTest());
		VERIFY_MULTI(ToggleTest());
		VERIFY_MULTI_END();
	}

	bool BitTricksTester::SetTest()
	{
		{
			int num = 0b0010;
			SetBit(num, 0);
			VERIFY(num == 0b0011);
		}
		
		{
			std::uint16_t num = 0b0010;
			SetBit(num, 3);
			VERIFY(num == 0b1010);
		}
		
		{
			std::uint64_t num = 0b1111;
			SetBit(num, 1, false);
			VERIFY(num == 0b1101);
		}
		
		{
			std::uint8_t num = 0b0000;
			SetBit(num, 2, true);
			VERIFY(num == 0b0100);
		}
		
		
	}

	bool BitTricksTester::ClearTest()
	{
		{
			int num = 0b1111;
			ClearBit(num, 1);
			VERIFY(num == 0b1101);
		}

		{
			std::uint16_t num = 0b11111111u;
			ClearBit(num, 7);
			VERIFY(num == 0b01111111u);
		}

		{
			std::uint64_t num = 0b0010u;
			ClearBit(num, 1);
			VERIFY(num == 0b0000u);
		}
		
		
	}

	bool BitTricksTester::CheckTest()
	{
		{
			const int num = 0b1000;
			const auto isSet = CheckBit(num, 1);
			VERIFY(isSet == false);
		}

		{
			const short num = 0b1000;
			const auto isSet = CheckBit(num, 3);
			VERIFY(isSet == true);
		}

		{
			const long long num = 0b0001;
			const auto isSet = CheckBit(num, 0);
			VERIFY(isSet == true);
		}
		
		
	}

	bool BitTricksTester::ToggleTest()
	{
		{
			int num = 0b1111;
			ToggleBit(num, 1);
			VERIFY(num == 0b1101);
		}

		{
			std::uint16_t num = 0b11111111u;
			ToggleBit(num, 7);
			VERIFY(num == 0b01111111u);
		}

		{
			std::uint64_t num = 0b0010u;
			ToggleBit(num, 1);
			VERIFY(num == 0b0000u);
		}
		
		
	}
}
#endif
