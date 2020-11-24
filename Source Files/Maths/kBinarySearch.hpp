#pragma once
#include "../HelperMacros.hpp"
#include <type_traits>

namespace kmaths
{
	namespace secret::impl
	{
		template<typename List, typename T>
		USE_RESULT constexpr size_t BinarySearchImpl(const List& list, T&& value, size_t lbIdx, size_t ubIdx, size_t size)
		{
			if (lbIdx > ubIdx
				|| value < list[0]
				|| value > list[size - 1])
				return -1;

			const size_t midIdx = lbIdx + ((ubIdx - lbIdx) >> 1);
			const auto mid = list[midIdx];

			if (mid > value)
				return BinarySearchImpl(list, value, lbIdx, midIdx - 1, size);
			if (mid < value)
				return BinarySearchImpl(list, value, midIdx + 1, ubIdx, size);

			return midIdx;
		}

		template<typename List, typename T>
		USE_RESULT constexpr size_t BinarySearchClosestImpl(const List& list, T&& value, size_t lbIdx, size_t ubIdx, size_t size)
		{
			if (lbIdx > ubIdx
				|| value < list[0]
				|| value > list[size - 1])
				return -1;

			const size_t midIdx = lbIdx + ((ubIdx - lbIdx) >> 1);

			const auto lowerBound = list[midIdx];
			const auto upperBound = list[midIdx + 1];

			if (lowerBound == value)
				return midIdx;
			else if (value < lowerBound)
				return BinarySearchClosestImpl(list, value, lbIdx, midIdx - 1, size);
			else if (value > upperBound)
				return BinarySearchClosestImpl(list, value, midIdx + 1, ubIdx, size);

			const auto lbDiff = value - lowerBound;
			const auto ubDiff = upperBound - value;

			return lbDiff < ubDiff ? midIdx : midIdx + 1;
		}
	}

	template<typename T>
	USE_RESULT constexpr size_t BinarySearch(const T* const list, T&& value, size_t size) noexcept
	{
		return kmaths::secret::impl::BinarySearchImpl(list, value, 0, size - 1, size);
	}

	template< typename T, size_t N, class = std::enable_if_t<!std::is_pointer_v<T>>>
	USE_RESULT constexpr size_t BinarySearch(const T(&list)[N], T&& value) noexcept
	{
		return kmaths::secret::impl::BinarySearchImpl(list, value, 0, N - 1, N);
	}

	template< typename T>
	USE_RESULT constexpr size_t BinarySearchClosest(const T* const list, T&& value, size_t size)
	{
		return kmaths::secret::impl::BinarySearchClosestImpl(list, value, 0, size - 1, size);
	};

	template< typename T, size_t N, class = std::enable_if_t<!std::is_pointer_v<T>>>
	USE_RESULT constexpr size_t BinarySearchClosest(const T(&list)[N], T&& value)
	{
		return kmaths::secret::impl::BinarySearchClosestImpl(list, value, 0, N - 1, N);
	};

}
