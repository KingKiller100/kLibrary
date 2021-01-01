#pragma once

#include "Vector.hpp"

namespace kmaths
{
	template<typename T>
	struct Vector<T, 2>
	{
	public:
		using Type = T;
		inline static constexpr Length_t Length = 2;
		inline static constexpr size_t TypeSize = sizeof(T);
		inline static constexpr size_t TotalBytes = Length * TypeSize;

		constexpr Vector() noexcept
			= default;

		template< typename U, Length_t C>
		constexpr Vector(const Vector<U, C>& other) noexcept
		{
			*this = other;
		}

		template< typename U, Length_t C>
		constexpr Vector(Vector&& other) noexcept
		{
			*this = std::move(other);
		}

		constexpr Vector(const std::initializer_list<T> values) noexcept
		{
			const auto first_iter = values.begin();

			const auto loops = values.size() < 2 ? values.size() : 2;
			for (size_t i = 0; i < loops; ++i)
			{
				operator[](i) = first_iter[i];
			}
		}

		explicit constexpr Vector(Type _x, Type _y) noexcept
			: x(_x), y(_y)
		{}

		explicit constexpr Vector(const Type& _v) noexcept
			: x(_v), y(_v)
		{}

		explicit constexpr Vector(const T values[2]) noexcept
			: x(values[0]), y(values[1])
		{}

		~Vector() noexcept
			= default;

		USE_RESULT constexpr Type& X() noexcept { return x; }
		USE_RESULT constexpr const Type& X() const noexcept { return x; }

		USE_RESULT constexpr Type& Y() noexcept { return y; }
		USE_RESULT constexpr const Type& Y() const noexcept { return y; }

		USE_RESULT constexpr Type MagnitudeSQ() const noexcept
		{
			const auto mag = (x * x) + (y * y);
			return mag;
		}

		USE_RESULT constexpr Type Magnitude() const noexcept
		{
			const auto magSQ = MagnitudeSQ();
			const auto mag = Sqrt(magSQ);
			return mag;
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr T DotProduct(const Vector<U, C>& other) const noexcept
		{
			constexpr Length_t size = C < Length ? C : Length;

			Type dp = Type();
			for (size_t i = 0; i < size; ++i)
			{
				dp += (operator[](i) * other[i]);
			}
			return dp;
		}

		USE_RESULT constexpr Vector Normalize() const noexcept
		{
			constexpr auto epsilon = constants::Epsilon<Type>();

			const auto magSQ = MagnitudeSQ();

			if (magSQ <= epsilon)
				return Vector();

			const auto mag = constants::OneOver<Type>(Sqrt<T>(magSQ));

			return Vector(x * mag, y * mag);
		}

		// Restricts vector magnitude to max value
		constexpr void Truncate(const Type max) noexcept
		{
			if (Magnitude() > max)
				*this = Normalize() * max;
		}

		// Reassigns values to be positives
		constexpr void Abs() noexcept
		{
			x = kmaths::Abs(x);
			y = kmaths::Abs(y);
		}

		// Calculates distance between two 3D objects
		template<typename U, Length_t C>
		USE_RESULT constexpr T Distance(const Vector<U, C>& v) const noexcept
		{
			const auto distanceVec = v - *this;
			return CAST(Type, distanceVec.Magnitude());
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		USE_RESULT constexpr Vector Reverse() const noexcept
		{
			return Vector(-x, -y);
		}

		USE_RESULT constexpr Vector Inverse() const noexcept
		{
			if _CONSTEXPR_IF(std::is_integral_v<Type>)
				return *this;

			return Vector(constants::OneOver<Type>(x), constants::OneOver<Type>(y));
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			x = y = Type();
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			constexpr auto zero = constants::Zero<Type>();
			return (x == zero && y == zero);
		}

		// Compilers earlier than C++20 features will not work in constexpr
		USE_RESULT constexpr Type* GetPointerToData() const
		{
			return REINTERPRET(Type*, (void *)this);
		}

		USE_RESULT constexpr auto GetTotalBytes() const noexcept
		{
			return TotalBytes;
		}

		USE_RESULT constexpr auto GetLength() const noexcept
		{
			return Length;
		}

		template<typename U = Type>
		USE_RESULT constexpr Vector Perpendicular() const noexcept
		{
			return Vector(-y, x);
		}

		// Gives a copy
		USE_RESULT constexpr Type At(const size_t index) const noexcept
		{
			return operator[](index);
		}

		// Gives a reference
		USE_RESULT constexpr Type& operator[](const size_t index)
		{
			if (index >= 2) std::_Xout_of_range("Index allowed must be 0 or 1!");
			return index ? y : x;
		}

		// Gives a const reference
		USE_RESULT constexpr const Type& operator[](const size_t index) const
		{
			if (index >= 2) std::_Xout_of_range("Index allowed must be 0 or 1!");
			return index ? y : x;
		}

		USE_RESULT constexpr Vector operator-() const noexcept
		{
			return Reverse();
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr Vector operator+(const Vector<U, C>& other) const noexcept
		{
			Type temp[Length]{ Type() };
			for (auto i = size_t(0); i < Length; ++i)
			{
				temp[i] = (C > i)
					? CAST(Type, operator[](i) + other[i])
					: operator[](i);
			}
			return Vector(temp);
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr Vector operator-(const Vector<U, C>& other) const noexcept
		{
			Type temp[Length]{ Type() };
			for (auto i = size_t(0); i < Length; ++i)
			{
				temp[i] = (C > i)
					? CAST(Type, operator[](i) - other[i])
					: operator[](i);
			}
			return Vector(temp);
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr Vector operator*(const Vector<U, C>& other) const noexcept
		{
			Type temp[Length]{ Type() };
			for (auto i = size_t(0); i < Length; ++i)
			{
				temp[i] = (C > i)
					? CAST(Type, operator[](i) * other[i])
					: operator[](i);
			}
			return Vector(temp);
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr Vector operator/(const Vector<U, C>& other) const noexcept
		{
			Type temp[Length]{ Type() };
			for (auto i = size_t(0); i < Length; ++i)
			{
				temp[i] = (C > i)
					? CAST(Type, operator[](i) / other[i])
					: operator[](i);
			}
			return Vector(temp);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Vector operator*(const U scalar) const noexcept
		{
			return Vector(
				CAST(Type, x * scalar),
				CAST(Type, y * scalar)
			);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Vector operator/(const U scalar) const noexcept
		{
			return Vector(
				CAST(Type, x / scalar),
				CAST(Type, y / scalar)
			);
		}

		template<typename U, Length_t C>
		constexpr Vector& operator+=(const Vector<U, C>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<typename U, Length_t C>
		constexpr Vector operator-=(const Vector<U, C>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator*=(const U scalar) noexcept
		{
			*this = *this * scalar;
			return *this;
		}

		template<typename U, Length_t C>
		constexpr Vector operator*=(const Vector<U, C>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		template<typename U>
		constexpr Vector operator/=(const U scalar) noexcept
		{
			*this = *this / scalar;
			return *this;
		}

		template<typename U, Length_t C>
		constexpr Vector operator/=(const Vector<U, C>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr bool operator<(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() < other.MagnitudeSQ();
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr bool operator>(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() > other.MagnitudeSQ();
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr bool operator<=(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() <= other.MagnitudeSQ();
		}

		template<typename U, Length_t C>
		USE_RESULT constexpr bool operator>=(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() >= other.MagnitudeSQ();
		}


		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(const Vector& v) const
		{
			return (x == v.x) && (y == v.y);
		}

		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(Vector&& v) const
		{
			return (x == v.x) && (y == v.y);
		}

		// bool operator != returns true if both Vector values are NOT equal
		USE_RESULT constexpr bool operator!=(const Vector& v) const
		{
			return !(*this == v);
		}

		template<typename U, Length_t C>
		constexpr Vector& operator=(const Vector<U, C>& other) noexcept // Copy
		{
			constexpr size_t size = C < Length ? C : Length;
			for (size_t i = 0; i < size; ++i)
				operator[](i) = CAST(Type, other[i]);
			return *this;
		}

		template<typename U, Length_t C>
		constexpr Vector& operator=(Vector<U, C>&& other) noexcept // Move
		{
			constexpr size_t size = C < Length ? C : Length;
			for (size_t i = 0; i < size; ++i)
				operator[](i) = CAST(Type, other[i]);
			return *this;
		}

	public:
		Type x = Type();
		Type y = Type();
	};

	template<typename T>
	using Vector2 = Vector<T, 2>;

	using Vector2s = Vector2 <   int    >; // signed integer
	using Vector2f = Vector2 <  float   >; // floating point
	using Vector2d = Vector2 <  double  >; // double floating point
	using Vector2u = Vector2 < unsigned >; // unsigned integer
}

