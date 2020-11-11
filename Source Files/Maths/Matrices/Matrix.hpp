#pragma once

#include "../Length_Type.hpp"
#include "../kAlgorithms.hpp"
#include "../Vectors/Vector.hpp"

namespace kmaths
{
	template<typename T, Length_Type Rows, Length_Type Columns>
	struct Matrix;

	template<typename Type, Length_Type R, Length_Type C>
	USE_RESULT constexpr std::enable_if_t<R == C, Matrix<Type, R, C>> IdentityMatrix() noexcept;

	// Row Major Matrix
	template<typename T, Length_Type Rows, Length_Type Columns>
	struct Matrix
	{
	public:
		static_assert(!std::is_unsigned_v<T>, "Type entered for matrix cannot be an unsigned type");
		static_assert(Rows > 0 && Columns > 0, "Must have at least one row and one column to construct a matrix");

		using Type = T;
		using Column_Type = Vector<T, Rows>;
		using Row_Type = Vector<T, Columns>;
		inline static constexpr Length_Type Length = (Rows * Columns);
		inline static constexpr size_t TypeSize = sizeof(T);
		inline static constexpr size_t TotalBytes = Length * TypeSize;


		constexpr Matrix() noexcept
			= default;

		constexpr Matrix(const Matrix& other) noexcept
		{
			*this = other;
		}

		constexpr Matrix(Matrix&& other) noexcept
		{
			*this = std::move(other);
		}

		explicit constexpr Matrix(const Type newIndices[Rows][Columns]) noexcept
		{
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					elems[row][col] = newIndices[row][col];
		}

		explicit constexpr Matrix(const Vector<Type, Columns>& vec) noexcept
		{
			for (auto row = 0; row < Rows; ++row)
				elems[row][row] = vec[row];
		}

		explicit constexpr Matrix(Type&& initialVal) noexcept
		{
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					elems[row][col] = std::forward<Type&&>(initialVal);
		}

		constexpr Matrix(const std::initializer_list<Vector<Type, Columns>>& list)
		{
			const auto size = list.size();

			if (Rows < size)
				throw std::overflow_error("Attempting to create maths vector with more elements than dimensions");
			
			const auto first_iter = list.begin();

			for (auto row = 0u; row < Rows; ++row) {
				for (auto col = 0u; col < Columns; ++col)
				{
					const auto ptr = (first_iter + row);
					const auto val = (*ptr)[col];
					elems[row][col] = val;
				}
			}
		}

		explicit constexpr Matrix(const Type(&list)[Rows * Columns])
		{
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
				{
					const auto index = (row * Columns) + col;

					elems[row][col] = list[index];
				}
		}

		~Matrix()
			= default;

		USE_RESULT constexpr Matrix<Type, Columns, Rows> Transpose() const noexcept
		{
			if (IsZero())
				return Matrix();

			if (IsIdentity())
				return IdentityMatrix<Type, Rows, Columns>();

			Matrix<Type, Columns, Rows> transposeMat;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					transposeMat[col][row] = elems[row][col];
			return transposeMat;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> Inverse() const
		{
			if (IsZero() || !ValidColumns())
				return Matrix();

			if (IsIdentity())
				return IdentityMatrix<Type, R, C>();

			Matrix inverse;
			const auto determinant = this->GetDeterminant();

			if _CONSTEXPR_IF(Rows == 2)
			{
				Vector<Type, 2> copy[2];
				if (determinant != 0.f)
				{
					copy[0][0] = elems[1][1] / CAST(Type, determinant);
					copy[0][1] = -elems[0][1] / CAST(Type, determinant);
					copy[1][0] = -elems[1][0] / CAST(Type, determinant);
					copy[1][1] = elems[0][0] / CAST(Type, determinant);
				}
				inverse = { copy[0], copy[1] };
			}
			else if _CONSTEXPR_IF(Rows > 2)
			{
				auto positiveCoefficient = true;
				for (auto row = 0u; row < Rows; ++row)
				{
					positiveCoefficient = (row & 1) == 0;

					for (auto col = 0u; col < Columns; ++col)
					{
						const auto minorMatrix = CreateMinorMatrix(row, col);
						inverse[row][col] = CAST(Type, minorMatrix.GetDeterminant());
						if (!positiveCoefficient) inverse[row][col] *= CAST(Type, -1);

						positiveCoefficient = !positiveCoefficient;
					}
				}

				inverse = inverse.Mirror();

				inverse /= determinant;
			}
			else
			{
				if _CONSTEXPR_IF(std::is_floating_point_v<Type>)
					return Matrix(constants::OneOver<Type>(elems[0][0]));
				else
					return Matrix();
			}

			return inverse;
		}

		// Draws a mirror line down a square matrix through the identical row-column indices and swaps values
		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> Mirror() const noexcept
		{
			Matrix temp = *this;

			for (auto row = 0u; row < Rows; ++row) {
				for (auto col = 0u; col < Columns; ++col)
				{
					if (row == col)
						break;

					Swap(temp[row][col], temp[col][row]);
				}
			}

			return temp;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C && R == 2, Type> GetDeterminant() const noexcept
		{
			if (IsZero() || !ValidColumns())
				return CAST(Type, 0);

			if (IsIdentity())
				return CAST(Type, 1);

			const auto a = elems[0][0];
			const auto b = elems[0][1];
			const auto c = elems[1][0];
			const auto d = elems[1][1];
			const auto determinant = (a * d) - (b * c);
			return determinant;
		}

		// Rule of Sarrus Impl
		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C && R == 3, Type> GetDeterminant() const noexcept
		{
			if (IsZero() || !ValidColumns())
				return CAST(Type, 0);

			if (IsIdentity())
				return CAST(Type, 1);

			auto sum = CAST(Type, 1);
			auto determinant = CAST(Type, 0);
			bool isReverseColumnSearch = false;

			auto col = 0;
			const auto idxDiff = Columns - 1;

			do {
				for (auto row = 0u; row < Rows; ++row, isReverseColumnSearch ? --col : ++col)
				{
					const size_t colIdx = Modulus(col, CAST(int, Columns));
					sum *= elems[row][colIdx];
				}

				determinant += isReverseColumnSearch ? -sum : sum;
				sum = CAST(Type, 1);
				col += isReverseColumnSearch ? idxDiff : -idxDiff;

				if (!isReverseColumnSearch)
				{
					if (col == Columns)
					{
						col = idxDiff;
						isReverseColumnSearch = true;
					}
				}
			} while (col > -1);

			return determinant;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t < R == C && R >= 4, Type> GetDeterminant() const
		{
			if (IsZero() || !ValidColumns())
				return CAST(Type, 0);

			if (IsIdentity())
				return CAST(Type, 1);

			auto determinant = CAST(Type, 0);

			for (auto col = 0u; col < Columns; ++col)
			{
				const auto newSize = Rows - 1;
				Matrix<Type, newSize, newSize> minorMatrix = CreateMinorMatrix(0u, col);
				const auto subDeterminant = elems[0][col] * minorMatrix.GetDeterminant();
				determinant += (col & 1) == 0 ? subDeterminant : -subDeterminant;
			}

			return determinant;
		}

		USE_RESULT constexpr Matrix<Type, Rows - 1, Columns - 1> CreateMinorMatrix(Length_Type rowToSkip, Length_Type colToSkip) const
		{
			constexpr auto newSize = Rows - 1;
			if (rowToSkip > newSize || colToSkip > newSize)
				std::_Xout_of_range("Row/column entered is greater than the size of this matrix's row/column");

			Matrix<Type, newSize, newSize> minorMatrix;

			auto minorRowIndex = 0;
			for (auto r = 0u; r < Rows; ++r)
			{
				if (r == rowToSkip)
					continue;

				auto minorColIndex = 0;
				for (auto c = 0u; c < Columns; ++c)
				{
					if (c == colToSkip)
						continue;

					minorMatrix[minorRowIndex][minorColIndex++] =
						elems[r][c];
				}
				minorRowIndex++;
			}
			return minorMatrix;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Matrix> PowerOf(unsigned power) const
		{
			if (power == 0)
				return IdentityMatrix<Type, R, C>();

			Matrix temp = *this;
			for (auto i = 1u; i < power; ++i)
				temp *= *this;
			return temp;
		}

		USE_RESULT constexpr bool IsZero() const noexcept
		{
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					if (elems[row][col] != CAST(Type, 0))
						return false;
			return true;
		}

		USE_RESULT static constexpr bool IsSquare() noexcept
		{
			return Rows == Columns;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, Length_Type> Rank() const noexcept
		{
			if (IsIdentity())
				return Rows;

			if (this->GetDeterminant() != CAST(Type, 0))
				return Min(Rows, Columns);

			return 0;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R == C, bool> IsIdentity() const noexcept
		{
			for (auto row = 0; row < Rows; ++row)
				for (auto col = 0; col < Columns; ++col)
					if (elems[row][col] != ((row == col) ? CAST(Type, 1) : CAST(Type, 0)))
						return false;
			return true;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			bool> IsIdentity() const noexcept
		{
			return false;
		}

		USE_RESULT constexpr decltype(auto) GetRows() const noexcept
		{
			return Rows;
		}

		USE_RESULT constexpr decltype(auto) GetColumns() const noexcept
		{
			return Columns;
		}

		USE_RESULT constexpr decltype(auto) GetLength() const noexcept
		{
			return Rows * Columns;
		}

		USE_RESULT constexpr auto GetTotalBytes() const noexcept
		{
			return TotalBytes;
		}

		USE_RESULT constexpr Type* GetPointerToData() const
		{
			return elems[0].GetPointerToData();
		}

		// Operators
		template<typename U>
		USE_RESULT constexpr Matrix operator+(const Matrix<U, Rows, Columns>& other) const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					m[row][col] = CAST(Type, elems[row][col] + other[row][col]);
			return m;
		}

		template<typename U>
		USE_RESULT constexpr Matrix operator-(const Matrix<U, Rows, Columns>& other) const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					m.elems[row][col] = CAST(Type, elems[row][col] - other[row][col]);
			return m;
		}

		template<typename U>
		constexpr Matrix& operator+=(const Matrix<U, Rows, Columns>& other)
		{
			*this = *this + other;
			return *this;
		}

		template<typename U>
		constexpr Matrix& operator-=(const Matrix<U, Rows, Columns>& other) noexcept
		{
			*this = *this - other;
			return *this;
		}

		template<typename U, Length_Type C, Length_Type R = Columns>
		USE_RESULT constexpr Matrix<Type, Rows, C> operator*(const Matrix<U, R, C>& other) const noexcept
		{
			Matrix<Type, Rows, C> m;
			for (auto row = 0u; row < Rows; ++row) {
				for (auto col = 0u; col < C; ++col) {
					for (auto index = 0u; index < R; ++index)
					{
#ifdef KLIB_DEBUG
						const Type left = elems[row][index];
						const Type right = CAST(Type, other[index][col]);
						const Type res = left * right;
						m[row][col] += res;
#else
						m[row][col] += (elems[row][index] * CAST(Type, other[index][col]));
#endif
					}

					if _CONSTEXPR_IF(std::is_floating_point_v<Type>) // Round to reduce floating point precision error
						m[row][col] = HandleFloatingPointError<Type>(m[row][col]);
				}
			}
			return m;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
		USE_RESULT constexpr Matrix operator*(const U scalar) const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					m.elems[row][col] = elems[row][col] * scalar;

			return m;
		}

		// For Column Major Matrix -Vector operations /////////////////////////////////////////////////////////////////
		//		template<typename U>
		//		USE_RESULT constexpr Vector<U, Rows> operator*(const Vector<U, Columns>& v) const noexcept
		//		{
		//			Column_Type result;
		//
		//			for (auto row = 0; row < Rows; ++row) {
		//				for (auto col = 0u; col < Columns; ++col) {
		//#ifdef KLIB_DEBUG
		//					const auto left = elems[row][col];
		//					const auto right = v[col];
		//					const Type res = left * right;
		//					result[row] += res;
		//#else
		//					result[row] += (elems[row][col] * v[col]);
		//#endif
		//				}
		//
		//				if _CONSTEXPR_IF(std::is_floating_point_v<Type>) // Round to reduce floating point precision error
		//					result[row] = HandleFloatingPointError<Type>(result[row]);
		//			}
		//
		//			return result;
		//		}

				//template<typename U>
				//USE_RESULT constexpr Vector<U, Rows> operator/(const Vector<U, Columns>& v) const noexcept
				//{
				//	return Inverse() * v;
				//}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
		USE_RESULT constexpr Matrix operator/(const U scalar) const noexcept
		{
			//if _CONSTEXPR_IF(std::is_floating_point_v<Type>)
			//{
			//	const auto multipler = constants::OneOver<Type>(scalar);
			//	return *this * multipler;
			//}
			//else
			//{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					m.elems[row][col] = elems[row][col] / scalar;
			return m;
			//}
		}

		template<Length_Type C, Length_Type R = Columns>
		constexpr Matrix<Type, Rows, C> operator*=(const Matrix<Type, R, C>& other) noexcept
		{
			*this = *this * other;
			return *this;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
		constexpr Matrix& operator*=(const U obj)
		{
			*this = *this * obj;
			return *this;
		}

		template<typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>, U>>
		constexpr Matrix& operator/=(const U scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		USE_RESULT constexpr Matrix operator-() const noexcept
		{
			Matrix m;
			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					m.elems[row][col] = -elems[row][col];
			return m;
		}

		USE_RESULT constexpr decltype(auto) operator[](const size_t index) noexcept
		{
			if (index >= Rows) std::_Xout_of_range("Index must be between 0 and size of matrix's rows - 1!");
			return elems[index];
		}

		USE_RESULT constexpr decltype(auto) operator[](const size_t index) const noexcept
		{
			if (index >= Rows) std::_Xout_of_range("Index must be between 0 and size of matrix's rows - 1!");
			return elems[index];
		}

		USE_RESULT constexpr decltype(auto) At(const size_t rows, const size_t columns) const noexcept
		{
			if (rows >= Rows || columns >= Columns) std::_Xout_of_range("Index must be within matrix's boundaries!");
			return (elems[rows])[columns];
		}

		USE_RESULT constexpr decltype(auto) At(const size_t rows, const size_t columns) noexcept
		{
			if (rows >= Rows || columns >= Columns) std::_Xout_of_range("Index must be within matrix's boundaries!");
			return (elems[rows])[columns];
		}

		constexpr Matrix& operator=(const Matrix& other) noexcept
		{
			if (this == &other)
				return *this;

			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					elems[row][col] = other[row][col];

			return *this;
		}

		constexpr Matrix& operator=(Matrix&& other) noexcept
		{
			if (this == &other)
				return *this;

			for (auto row = 0u; row < Rows; ++row)
				for (auto col = 0u; col < Columns; ++col)
					elems[row][col] = std::move(other[row][col]);

			return *this;
		}

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Matrix> Mirror() const noexcept
			= delete;

		// Deleted version of functions (under certain conditions)
		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Type> GetDeterminant() const noexcept
			= delete;

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Matrix> Inverse() const noexcept
			= delete;

		template<Length_Type R = Rows, Length_Type C = Columns>
		USE_RESULT constexpr std::enable_if_t<R != C,
			Matrix> PowerOf(unsigned power)  const noexcept
			= delete;

	private:
		// TODO: Gauss-Jordan Elimination Method
	/*	constexpr Matrix RowEchalonMethod() noexcept
		{
			auto lhs = elems;
			auto rhs = Identity<Rows, Columns>();

			bool readyToStart = true;
			bool rowsStatus[Rows];

			for (auto i = 0u; i < lhs.size(); ++i)
			{
				const auto& vec = lhs[i];
				if (vec.IsZero())
					return Matrix();

				rowsStatus[i] = vec[i] != CAST(Type, 0);

				if (!rowsStatus[i])
					readyToStart = false;
			}

			if (!readyToStart)
			{
				for (auto i = 0u; i < Rows; ++i)
				{
					if (rowsStatus[i])
						continue;

					for (auto j = i + 1; j < Rows; ++j)
					{
						if (rowsStatus[j])
							continue;

						if (lhs[j][i] != CAST(Type, 0))
						{
							rowsStatus[j] == true;
							break;
						}

						if (elems[j][i] != CAST(Type, 0))
						{
							std::swap(lhs[i], lhs[j]);
							break;
						}
					}
				}

				for (auto& status : rowsStatus)
					if (!status)
						return Matrix();

				readyToStart = true;
			}

			if (lhs[0][0] != CAST(Type, 1))
			{

			}

			return rhs;
		}*/


		USE_RESULT constexpr bool ValidColumns() const noexcept
		{
			for (auto col = 0u; col < Columns; ++col)
			{
				bool valid = false;
				for (auto row = 0u; row < Rows; ++row)
				{
					if (elems[row][col] != CAST(Type, 0))
					{
						valid = true;
						break;
					}
				}
				if (!valid)
					return false;
			}
			return true;
		}

	private:
		Row_Type elems[Rows]{ {} };
	};

	template<typename Type, Length_Type R, Length_Type C>
	USE_RESULT constexpr std::enable_if_t<R == C, Matrix<Type, R, C>> IdentityMatrix() noexcept
	{
		Matrix<Type, R, C> identity;
		for (auto row = 0u; row < R; ++row)
			identity[row][row] = CAST(Type, 1);
		return identity;
	}

	template<typename Type, Length_Type R, Length_Type C>
	USE_RESULT constexpr Vector<Type, C> operator*(const Vector<Type, R>& v, const Matrix<Type, R, C>& m) noexcept
	{
		Vector<Type, C> result;

		for (auto col = 0u; col < C; ++col) {
			for (auto row = 0; row < R; ++row) {
#ifdef _DEBUG
				const auto left = v[col];
				const auto right = m[row][col];
				const Type res = left * right;
				result[col] += res;
#else
				result[col] += (v[col] * elems[row][col]);
#endif
			}

			if _CONSTEXPR_IF(std::is_floating_point_v<Type>) // Round to reduce floating point precision error
				result[col] = HandleFloatingPointError<Type>(result[col]);
		}

		return result;
	}

	template<typename Type, Length_Type R, Length_Type C>
	USE_RESULT constexpr Vector<Type, C> operator/(const Vector<Type, R>& v, const Matrix<Type, R, C>& m) noexcept
	{
		return operator*(v, m.Inverse());
	}
}
