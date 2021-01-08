#pragma once

#include "../../HelperMacros.hpp"
#include "../../TypeTraits/IntegerTraits.hpp"

namespace klib::kBits
{
	template<class Integer, class = std::enable_if_t<type_trait::Is_Integer_V<Integer>>>
	constexpr void SetBit(Integer& num, size_t index)
	{
		if (index > std::numeric_limits<Integer>::digits)
			std::_Xout_of_range("Index greater than bits in this type");
		num |= 1ull << index;
	}
	
	template<class Integer, class = std::enable_if_t<type_trait::Is_Integer_V<Integer>>>
	constexpr void ClearBit(Integer& num, size_t index)
	{
		if (index > std::numeric_limits<Integer>::digits)
			std::_Xout_of_range("Index greater than bits in this type");
		num &= ~(1ull << index);
	}
	
	template<class Integer, class = std::enable_if_t<type_trait::Is_Integer_V<Integer>>>
	constexpr void ToggleBit(Integer& num, size_t index)
	{
		if (index > std::numeric_limits<Integer>::digits)
			std::_Xout_of_range("Index greater than bits in this type");
		num ^= 1ull << index;
	}
	
	template<class Integer, class = std::enable_if_t<type_trait::Is_Integer_V<Integer>>>
	USE_RESULT constexpr bool CheckBit(Integer num, size_t index)
	{
		if (index > std::numeric_limits<Integer>::digits)
			std::_Xout_of_range("Index greater than bits in this type");
		return (num >> index) & 1;
	}
	
	template<class Integer, class = std::enable_if_t<type_trait::Is_Integer_V<Integer>>>
	constexpr void SetBit(Integer& num, size_t index, bool value)
	{
		const auto bit = value ? 0b1ull : 0b0ull;
		num = static_cast<Integer>(num & ~(1ull << index) | bit << index);
	}	
}
