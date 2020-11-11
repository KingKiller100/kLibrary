#pragma once
#include "../../HelperMacros.hpp"

#include "../Length_Type.hpp"
#include "../kAlgorithms.hpp"

namespace kmaths
{
	template<typename T, Length_Type Rows, Length_Type Columns>
	struct Matrix;

	template<typename T, Length_Type N>
	struct Vector
	{
	public:
		static_assert(N > 0, "Must have at least one dimension to use mathematical vectors");

		using Type = T;
		inline static constexpr Length_Type Length = N;
		inline static constexpr size_t TypeSize = sizeof(T);
		inline static constexpr size_t TotalBytes = Length * TypeSize;
		
		constexpr Vector() noexcept
			= default;

		template< typename U, Length_Type C>
		constexpr Vector(const Vector<U, C>& other) noexcept
		{
			*this = other;
		}

		template< typename U, Length_Type C>
		constexpr Vector(Vector&& other) noexcept
		{
			*this = std::move(other);
		}

		constexpr Vector(const std::initializer_list<T> values) noexcept
		{
			const auto first_iter = values.begin();
			const auto loops = values.size() < N ? values.size() : N;
			for (auto i = 0; i < loops; ++i)
				dimensions[i] = first_iter[i];
		}

		explicit constexpr Vector(Type _x, Type _y, Type _z = Type(), Type _w = Type()) noexcept
		{
			dimensions[0] = _x;
			dimensions[1] = _y;
			if (_z) dimensions[2] = _z;
			if (_w) dimensions[3] = _w;
		}

		explicit constexpr Vector(const Type& _v) noexcept
		{
			for (auto& val : dimensions)
				val = _v;
		}

		explicit constexpr Vector(const T values[N])
		{
			for (auto i = 0; i < N; ++i)
				dimensions[i] = values[i];
		}

		~Vector() noexcept
			= default;

		USE_RESULT constexpr const Type& X() const noexcept
		{
			return dimensions[0];
		}

		USE_RESULT constexpr Type& X() noexcept
		{
			return dimensions[0];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 2, const U&> Y() const noexcept
		{
			return dimensions[1];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 2, U&> Y() noexcept
		{
			return dimensions[1];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 3, const U&> Z() const noexcept
		{
			return dimensions[2];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 3, U&> Z() noexcept
		{
			return dimensions[2];
		}


		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 4, const U&> W() const noexcept
		{
			return dimensions[3];
		}

		template<typename U = Type>
		USE_RESULT constexpr std::enable_if_t<N >= 4, U&> W() noexcept
		{
			return dimensions[3];
		}

		USE_RESULT constexpr Type Magnitude() const noexcept
		{
			const auto magSQ = MagnitudeSQ();
			const auto mag = Sqrt(magSQ);
			return mag;
		}

		USE_RESULT constexpr Type MagnitudeSQ() const noexcept
		{
			auto mag = Type();
			for (auto val : dimensions)
				mag += val * val;
			return mag;
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr T DotProduct(const Vector<U, C>& other) const noexcept
		{
			const auto size = (N < C) ? N : C;
			auto dp = Type();
			for (auto i = 0; i < size; ++i)
				dp += dimensions[i] * static_cast<Type>(other[i]);
			return dp;
		}

		USE_RESULT constexpr Vector Normalize() const noexcept
		{
			constexpr auto epsilon = constants::Epsilon<T>();

			const auto magSQ = MagnitudeSQ();

			if (magSQ <= epsilon)
				return Vector();

			const auto mag = constants::OneOver<Type>(Sqrt<Type>(magSQ));

			Type temp[N]{ };
			for (auto i = 0; i < N; ++i)
				temp[i] = dimensions[i] * mag;
			return Vector(temp);
		}

		// Restricts vector magnitude to max value
		constexpr void Truncate(const Type max) noexcept
		{
			if (Magnitude() > max)
			{
				*this = Normalize() * max;
			}
		}

		// Reassigns values to be positives
		constexpr void Abs() noexcept
		{
			for (auto& d : dimensions)
				if (d < 0)
					d = kmaths::Abs(d);
		}

		// Calculates distance between two 3D objects
		template<typename U, Length_Type C>
		USE_RESULT constexpr T Distance(const Vector<U, C>& v) const noexcept
		{
			const auto distanceVec = v - *this;
			return static_cast<Type>(distanceVec.Magnitude());
		}

		// Returns vector times by -1 - does not reassign values (except w element)
		USE_RESULT constexpr Vector Reverse() const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = 0; i < N; ++i)
				copy[i] = dimensions[i] * -1;
			return Vector(copy);
		}

		USE_RESULT constexpr Vector Inverse() const noexcept
		{
			if _CONSTEXPR_IF(std::is_integral_v<Type>)
				return *this;
			else
			{
				T copy[N]{ T() };
				for (auto i = 0; i < N; ++i)
					copy[i] = (constants::One<T>() / dimensions[i]);
				return Vector(copy);
			}
		}

		// Sets all values of the vector to zero
		constexpr void Zero() noexcept
		{
			for (auto i = 0; i < N; ++i)
				dimensions[i] = CAST(Type, 0);
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			for (auto val : dimensions)
				if (val != T())
					return false;
			return true;
		}

		// Compilers earlier than C++20 will not work in constexpr
		USE_RESULT constexpr Type* GetPointerToData() const
		{
			Type& first = (Type)dimensions[0];
			return std::addressof<Type>(first);
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
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& N == 2,
			Vector> Perpendicular() const noexcept
		{
			return Vector(-dimensions[1], dimensions[0]);
		}

		template<typename X, typename U = T>
		USE_RESULT constexpr std::enable_if_t<!std::is_unsigned_v<U>
			&& !std::is_unsigned_v<X>
			&& N == 3,
			Vector> CrossProduct(const Vector<X, N>& v) const noexcept
		{
			return Vector(
				(dimensions[1] * v[2] - dimensions[2] * v[1]),
				(dimensions[2] * v[0] - dimensions[0] * v[2]),
				(dimensions[0] * v[1] - dimensions[1] * v[0])
			);
		}

		// Gives a copy
		USE_RESULT constexpr Type At(const size_t index) const noexcept
		{
			return operator[](index);
		}

		// Gives a reference
		USE_RESULT constexpr Type& operator[](const size_t index)
		{
			if (index >= N) std::_Xout_of_range("Index must be between 0 and size of vector - 1!");
			return dimensions[index];
		}

		// Gives a const reference
		USE_RESULT constexpr const Type& operator[](const size_t index) const
		{
			if (index >= N) std::_Xout_of_range("Index must be between 0 and size of vector - 1!");
			return dimensions[index];
		}

		USE_RESULT constexpr Vector operator-() const noexcept
		{
			return Reverse();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator+(const Vector<U, C>& other) const noexcept
		{
			T copy[N]{ Type() };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (C > i)
					? static_cast<Type>(dimensions[i] + other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator-(const Vector<U, C>& other) const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (C > i)
					? static_cast<Type>(dimensions[i] - other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator*(const Vector<U, C>& other) const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (C > i)
					? static_cast<Type>(dimensions[i] * other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr Vector operator/(const Vector<U, C>& other) const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = size_t(0); i < N; ++i)
			{
				copy[i] = (C > i)
					? static_cast<Type>(dimensions[i] / other[i])
					: dimensions[i];
			}
			return Vector(copy);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Vector operator*(const U scalar) const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = CAST(Type, dimensions[i] * scalar);
			return Vector(copy);
		}

		template<typename U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Vector operator/(const U scalar) const noexcept
		{
			Type copy[N]{ Type() };
			for (auto i = size_t(0); i < N; ++i)
				copy[i] = CAST(Type, dimensions[i] / scalar);
			return Vector(copy);
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator+=(const Vector<U, C>& other) noexcept
		{
			*this = *this + other;
			return *this;
		}

		template<typename U, Length_Type C>
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

		template<typename U, Length_Type C>
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

		template<typename U, Length_Type C>
		constexpr Vector operator/=(const Vector<U, C>& other) noexcept
		{
			*this = *this / other;
			return *this;
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator<(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() < other.MagnitudeSQ();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator>(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() > other.MagnitudeSQ();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator<=(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() <= other.MagnitudeSQ();
		}

		template<typename U, Length_Type C>
		USE_RESULT constexpr bool operator>=(const Vector<U, C>& other) const noexcept
		{
			return MagnitudeSQ() >= other.MagnitudeSQ();
		}


		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(const Vector& v) const
		{
			for (size_t i = 0; i < N; ++i)
				if (dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator == returns true if both Vector values are equal
		USE_RESULT constexpr bool operator==(Vector&& v) const
		{
			for (size_t i = 0; i < N; ++i)
				if (dimensions[i] != v[i])
					return false;
			return true;
		}

		// bool operator != returns true if both Vector values are NOT equal
		USE_RESULT constexpr bool operator!=(const Vector& v) const
		{
			return !(*this == v);
		}

		// bool operator == returns true if both Vector values are equal
		template<typename U>
		USE_RESULT constexpr bool operator==(const U scalar) const
		{
			return MagnitudeSQ() == (scalar * scalar);
		}

		// bool operator == returns true if both Vector values are equal
		template<typename U>
		USE_RESULT constexpr bool operator!=(const U scalar) const
		{
			return !(*this == scalar);
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator=(const Vector<U, C>& other) noexcept // Copy
		{
			constexpr auto size = (N < C) ? N : C;

			for (auto i = 0; i < size; ++i)
				dimensions[i] = static_cast<Type>(other[i]);

			return *this;
		}

		template<typename U, Length_Type C>
		constexpr Vector& operator=(Vector<U, C>&& other) noexcept // Move
		{
			if (this == &other)
				return *this;
			
			constexpr auto size = (N < C) ? N : C;

			for (auto i = 0; i < size; ++i)
				dimensions[i] = std::move(other[i]);

			return *this;
		}

		// Deleted version of functions (under certain circumstances
		template<typename U = Type>
		std::enable_if_t < N < 2, const U&> Y() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 2, U&> Y() noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 3, U&> Z() noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 3, const U&> Z() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 4, const U&> W() const noexcept = delete;
		template<typename U = Type>
		std::enable_if_t < N < 4, U&> W() noexcept = delete;

		template<typename U = Type>
		USE_RESULT constexpr inline std::enable_if_t<std::is_unsigned_v<U>
			|| N != 2,
			Vector> Perpendicular() const
			= delete;

		template<typename X, typename U = Type>
		USE_RESULT constexpr std::enable_if_t<std::is_unsigned_v<U>
			|| std::is_unsigned_v<X>
			|| N != 3,
			Vector> CrossProduct(const Vector& v) const noexcept
			= delete;

		template<typename Type, Length_Type C>
		friend constexpr Vector<Type, C> operator*(const Vector<Type, N>& v, const Matrix<Type, N, C>& m) noexcept;

		template<typename Type, Length_Type C>
		friend constexpr Vector<Type, C> operator/(const Vector<Type, N>& v, const Matrix<Type, N, C>& m) noexcept;

	private:
		Type dimensions[N]{};
	};

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator>(const Vector<T, C>& lhs, const U& rhs) noexcept
	{
		return (lhs.MagnitudeSQ()) > (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator>=(const Vector<T, C>& lhs, const U& rhs) noexcept
	{
		return (lhs.MagnitudeSQ()) >= (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator<(const Vector<T, C>& lhs, const U& rhs) noexcept
	{
		return (lhs.MagnitudeSQ()) < (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator<=(const Vector<T, C>& lhs, const U& rhs) noexcept
	{
		return (lhs.MagnitudeSQ()) <= (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator==(const Vector<T, C>& lhs, const U& rhs) noexcept
	{
		return (lhs.MagnitudeSQ()) == (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator!=(const Vector<T, C>& lhs, const U& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator>(const U& rhs, const Vector<T, C>& lhs) noexcept
	{
		return (lhs.MagnitudeSQ()) > (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator<(const U& rhs, const Vector<T, C>& lhs) noexcept
	{
		return (lhs.MagnitudeSQ()) < (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator>=(const U& rhs, const Vector<T, C>& lhs) noexcept
	{
		return (lhs.MagnitudeSQ()) >= (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator<=(const U& rhs, const Vector<T, C>& lhs) noexcept
	{
		return (lhs.MagnitudeSQ()) <= (rhs*rhs);
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator==(const U& rhs, const Vector<T, C>& lhs) noexcept
	{
		return (rhs*rhs) == lhs.MagnitudeSQ();
	}

	template<typename T, typename U, Length_Type C, class = std::enable_if_t<std::is_arithmetic_v<U>>>
	USE_RESULT constexpr bool operator!=(const U& rhs, const Vector<T, C>& lhs) noexcept
	{
		return (rhs*rhs) != lhs.MagnitudeSQ();
	}

}
