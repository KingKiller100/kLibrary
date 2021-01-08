#pragma once

#include "../../HelperMacros.hpp"
#include "../../TypeTraits/IntegerTraits.hpp"

namespace klib::bits
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
	USE_RESULT constexpr void SetBit(Integer& num, size_t index, bool value)
	{
		if (value)
			SetBit<Integer>(num, index);
		else
			ClearBit<Integer>(num, index);
	}	
}
