#pragma once

#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"

#include "../../kStringConverter.hpp"
#include "../../../Debug/Exceptions/StringExceptions.hpp"

#include "../../../../TypeTraits/FloatTraits.hpp"
#include "../../../../HelperMacros.hpp"
#include "../../../../Maths/Length_Type.hpp"
#include "../../../../Maths/kMathsFundamentals.hpp"
#include "../../../../Maths/kModulus.hpp"
#include "../../../../Maths/kRound.hpp"

#include <utility>

namespace klib::kString::stringify
{
	namespace secret::impl
	{
		constexpr auto g_MaxPrecision = 25;

		template<class T, class = std::enable_if_t<std::is_floating_point_v<T>>>
		struct Figures
		{
			size_t integers;
			T decimals;
			size_t dpShifts;
			bool isNeg;
		};

		template<typename T>
		USE_RESULT constexpr size_t GetDpShifts(T decimals)
		{
			size_t count = 0;
			if (!kmaths::constants::ApproximatelyZero<T>(decimals))
			{
				auto iter = g_MaxPrecision;
				size_t magnitude = 1;
				auto val = decimals * magnitude;
				while (iter-- > 0 && val < 1)
				{
					++count;
					magnitude *= 10;
					val = decimals * magnitude;
				}
			}
			return count;
		}

		template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
		constexpr void TrimZeros(T& num)
		{
			while (num != 0 && (num % 10 == 0))
			{
				num /= static_cast<T>(10);
			}
		}

		template<typename T>
		USE_RESULT constexpr Figures<T> GetFigures(T val, size_t decimalPlaces)
		{
			const auto isNeg = kmaths::IsNegative(val);
			val = kmaths::Abs(val);
			const auto justIntegers = static_cast<size_t>(kmaths::Floor(val));
			const auto justDecimals = kmaths::GetDecimals(val);
			const auto dpShifts = GetDpShifts(justDecimals);
			return { justIntegers, justDecimals, dpShifts, isNeg };
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* FixedNotation(T val, size_t decimalPlaces, Figures<T>& figs)
		{
			using namespace type_trait;

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			if (decimalPlaces > 0)
			{
				const size_t decimals = static_cast<size_t>(figs.decimals * kmaths::PowerOf10(decimalPlaces));
				current = UintToStr(current, decimals);
				const auto desiredCharacterCount = (end - current) + (figs.dpShifts - 1);
				PrependPadding(current, desiredCharacterCount, Char_t('0'));
				*(--current) = Char_t('.');
			}

			current = UintToStr(current, figs.integers, 10);

			if (figs.isNeg)
				PrependMinusSign(current);

			return CreateNewCString(current);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* ScientificNotation(T val, size_t sigFigs, Figures<T>& figs)
		{
			using namespace type_trait;
			if (figs.integers == 0 && figs.decimals == 0)
				return Convert<Char_t>("0");

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			const auto isZeroInt = figs.integers == 0;

			const auto direction = kmaths::IsDecimal(val)
				? Char_t('-')
				: Char_t('+');

			const auto intSize = kmaths::CountIntegerDigits(figs.integers);
			const auto sizeMet = !isZeroInt && intSize >= sigFigs;
			auto exponent = intSize - 1;

			if (sizeMet) // Only need integer significant figures
			{
				current = UintToStr(current, exponent);
				if (exponent < 10) current = UintToStr(current, 0);
				*(--current) = direction;
				*(--current) = g_ScientificFloatToken<Char_t>;
				if (intSize > sigFigs)
				{
					const auto limit = kmaths::PowerOf10(sigFigs + 1);
					while (figs.integers >= limit)
					{
						figs.integers = kmaths::Demote(figs.integers);
					}
					figs.integers += 5;
					figs.integers = kmaths::Demote(figs.integers);
				}
			}
			else // Need integer and decimals significant figures
			{
				const auto remaining = isZeroInt ? sigFigs : (sigFigs - intSize);
				const auto mag = kmaths::PowerOf10(remaining - 1);
				size_t power = 1;
				size_t shifts = 1;
				size_t decimals = 0;

				while (decimals < mag)
				{
					decimals = static_cast<size_t>(figs.decimals * kmaths::PowerOf10(power++));

					if (decimals == 0)
						++shifts;
				}

				exponent += isZeroInt ? shifts : shifts - 1;

				current = UintToStr(current, exponent);
				if (exponent < 10) current = UintToStr(current, 0);
				*(--current) = direction;
				*(--current) = g_ScientificFloatToken<Char_t>;

				current = UintToStr(current, decimals);

				while (!isZeroInt && shifts-- > 1)
				{
					*(--current) = Char_t('0');
				}
			}

			if (!isZeroInt)
				current = UintToStr(current, figs.integers);

			if (current[1] != g_ScientificFloatToken<Char_t>)
			{
				*(--current) = Char_t('.');
				kmaths::Swap(current[0], current[1]);
			}

			if (figs.isNeg)
				PrependMinusSign(current);

			return CreateNewCString(current);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* GeneralNotation(T val, size_t sigFigs, Figures<T>& figs)
		{
			return figs.dpShifts > 5 || sigFigs > 5
				? ScientificNotation<Char_t>(val, sigFigs, figs)
				: FixedNotation<Char_t>(val, sigFigs, figs);
		}

		// Big Endian
		template<class Char_t, typename Floating_t, typename Unsigned_t, typename = std::enable_if_t<
			std::is_unsigned_v<Unsigned_t>
			|| type_trait::Is_CharType_V<Char_t>
			>>
			const Char_t* BinaryNotation(Unsigned_t val)
		{
			constexpr auto dotIndex = type_trait::FloatTraits<Floating_t>::DotIndex;

			Char_t buff[g_MaxBits<Unsigned_t> +1]{ type_trait::g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			while (val > 0)
			{
				const auto binVal = val % 2;
				const Char_t digit = static_cast<Char_t>('0' + binVal);
				*(--current) = digit;
				val >>= 1;

				if (std::distance(current, end) == dotIndex)
					*(--current) = Char_t('.');
			}

			if (const auto distance = std::distance(current, end);
				distance < dotIndex)
			{
				PrependPadding(current, dotIndex, Char_t('0'));
				*(--current) = Char_t('.');
				*(--current) = Char_t('0');
			}

			if (!kmaths::IsNegative(val))
				*current = Char_t('0');

			auto cstr = CreateNewCString(current);
			return std::move(cstr);
		}

	}

	/// <summary>
	/// Floating point formatting for string
	/// </summary>
	/// <param name="FIX">Fixed notation</param>
	/// <param name="SCI">Scientific notation</param>
	/// <param name="GEN">General notation (either scientific or fixed)</param>
	/// <param name="HEX">Hexadecimal format</param>
	/// <param name="BIN">Binary format</param>
	ENUM_CLASS(FloatingPointFormat, std::uint8_t,
		FIX = BIT_SHIFT(0),
		SCI = BIT_SHIFT(1),
		GEN = FIX | SCI,
		HEX = BIT_SHIFT(2),
		BIN = BIT_SHIFT(3)
	);

	template<class Char_t, typename T, typename = std::enable_if_t<
		std::is_floating_point_v<T>
		|| type_trait::Is_CharType_V<Char_t>>
		>
		const Char_t* StringFloatingPoint(T val, size_t significantFigures = s_NoSpecifier
			, FloatingPointFormat fmt = FloatingPointFormat::FIX)
	{
		using namespace secret::impl;
		using Traits = type_trait::FloatTraits<T>;

		if (std::isnan(val)) return Convert<Char_t>("nan");
		if (std::isinf(val)) return Convert<Char_t>("inf");

		if (significantFigures == s_NoSpecifier)
			significantFigures = 1;
		else
			significantFigures = kmaths::Min<size_t>(significantFigures, g_MaxPrecision);

		if (fmt.MaskCmp(FloatingPointFormat::GEN))
		{
			Figures figs = GetFigures(val, significantFigures);

			switch (fmt.ToEnum()) {
			case FloatingPointFormat::FIX: return FixedNotation<Char_t>(val, significantFigures, figs);
			case FloatingPointFormat::SCI: return ScientificNotation<Char_t>(val, significantFigures + 1, figs);
			case FloatingPointFormat::GEN: return GeneralNotation<Char_t>(val, significantFigures, figs);
			default: throw kDebug::FormatError("Unknown floating point notation: " + ToWriter(fmt.ToString()));
			}
		}

		return fmt.Compare(FloatingPointFormat::HEX
			, StringIntegralHex<Char_t>(Traits::UintBitCast(val))
			, BinaryNotation<Char_t, T>(Traits::UintBitCast(val))
		);
	}

}

