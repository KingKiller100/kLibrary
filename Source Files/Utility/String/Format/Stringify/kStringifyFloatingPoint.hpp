#pragma once


#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"

#include "../../kStringConverter.hpp"

#include "../../../Debug/Exceptions/StringExceptions.hpp"
#include "../../../../Maths/Length_Type.hpp"
#include "../../../../Maths/kMathsFundamentals.hpp"
#include "../../../../Maths/kModulus.hpp"
#include "../../../../Maths/kRound.hpp"

#include <utility>

namespace klib::kString::stringify
{
	namespace secret::impl
	{
		struct Figures
		{
			size_t integers;
			size_t decimals;
			size_t dpShifts;
			bool isNeg;
		};

		template <class T>
		USE_RESULT constexpr kmaths::constants::Accuracy_t GetSignificantFigures(T val, size_t precision)
		{
			using namespace kmaths;
			using namespace kmaths::secret::impl;
			using namespace kmaths::constants;

			const auto accuracy = kmaths::secret::impl::PowerOfImpl<Accuracy_t>(CAST(Accuracy_t, 10), precision);
			const auto accuracyInverse = constants::OneOver<Accuracy_t>(accuracy);
			const auto dpShifts = constants::ZeroPointFive<Accuracy_t>() * accuracyInverse;

			const auto valuePlusDpsByAcc = (CAST(constants::Accuracy_t, val) + dpShifts) * accuracy;
			const auto sigFigs = Floor(valuePlusDpsByAcc);
			return static_cast<Accuracy_t>(sigFigs);
		}

		template<typename T>
		USE_RESULT constexpr size_t GetDpShifts(T decimals)
		{
			size_t count = 0;
			if (!kmaths::constants::ApproximatelyZero<T>(decimals))
			{
				size_t magnitude = 1;
				auto val = decimals * magnitude;
				while (val < 1)
				{
					++count;
					magnitude *= 10;
					val = decimals * magnitude;
				}
			}
			return count;
		}

		inline USE_RESULT size_t TrimZeros(size_t num)
		{
			while (num != 0 && kmaths::Modulus<size_t>(num, 10) == 0)
			{
				num /= static_cast<size_t>(10);
			}
			return num;
		}

		template<typename T>
		USE_RESULT constexpr Figures GetFigures(T val, size_t decimalPlaces)
		{
			const auto isNeg = kmaths::IsNegative(val);
			val = kmaths::Abs(val);
			const auto justIntegers = static_cast<size_t>(kmaths::Floor(val));
			const auto justDecimals = val - justIntegers;
			const auto dpShifts = GetDpShifts(justDecimals);
			const auto rawDecimalsToUse = GetSignificantFigures(justDecimals, decimalPlaces);
			const auto decimalsToUse = static_cast<size_t>(rawDecimalsToUse);
			return { justIntegers, decimalsToUse, dpShifts, isNeg };
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* FixedNotation(T val, size_t decimalPlaces, Figures& figs)
		{
			using namespace type_trait;

			if (decimalPlaces == s_NoSpecifier)
				decimalPlaces = 6;

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			if (decimalPlaces > 0)
			{
				const auto remaining = static_cast<long long>(decimalPlaces) - static_cast<long long>(figs.dpShifts);
				if (remaining >= 0)
				{
					current = UintToStr(current, figs.decimals);
					PrependPadding(current, GetSize(current) + figs.dpShifts - 1, Char_t('0'));
				}
				else
				{
					PrependPadding(current, decimalPlaces, Char_t('0'));
				}

				*(--current) = Char_t('.');
			}

			current = UintToStr(current, figs.integers);

			if (figs.isNeg)
				PrependMinusSign(current);

			auto cstr = CreateNewCString(current);
			return std::move(cstr);
		}

		template<class Char_t, typename T, typename = std::enable_if_t<
			std::is_floating_point_v<T>
			|| type_trait::Is_CharType_V<Char_t>>
			>
			const Char_t* ScientificNotation(T val, size_t decimalPlaces, Figures& figs)
		{
			using namespace type_trait;
			using String_t = std::basic_string<Char_t>;

			figs.decimals = TrimZeros(figs.decimals);
			
			if (figs.integers != 0 && kmaths::Abs(val) < 10)
				return FixedNotation<Char_t>(val, figs.dpShifts + kmaths::CountIntegerDigits(figs.decimals), figs);

			if (decimalPlaces == s_NoSpecifier)
				decimalPlaces = 1;

			Char_t buff[g_MaxFloatDigits<T>]{ g_NullTerminator<Char_t> };
			Char_t* const end = std::end(buff) - 1;
			Char_t* current = end;

			size_t totalShifts = 0;

			if (figs.integers > 0)
			{
				if (figs.integers > 9)
				{
					totalShifts += kmaths::CountIntegerDigits(figs.integers);
					--totalShifts;
				}
				figs.integers = TrimZeros(figs.integers);
			}

			if (figs.decimals > 0)
			{
				if (figs.decimals > 9
					|| (figs.integers == 0 && figs.decimals > 0))
					totalShifts += figs.dpShifts;
			}

			if (totalShifts > 0)
			{
				current = UintToStr(current, totalShifts);
				if (val < 1)
					*(--current) = Char_t('-');
				*(--current) = g_ScientificFloatToken<Char_t>;
			}

			if (figs.decimals > 0)
			{
				current = UintToStr(current, figs.decimals);
			}

			if (figs.integers > 0)
			{
				if (figs.dpShifts > 1)
				{
					auto dpShifts = figs.dpShifts;
					while (dpShifts-- > 0)
					{
						*(--current) = s_DefaultPlaceHolder<Char_t>;
					}
				}
				current = UintToStr(current, figs.integers);
			}

			const auto ePos = Find_First_Of(current, g_ScientificFloatToken<Char_t>);
			if (ePos > 1)
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
			const Char_t* GeneralNotation(T val, size_t decimalPlaces, Figures& figs)
		{
			const auto cstr = figs.dpShifts > 6
				? ScientificNotation<Char_t>(val, decimalPlaces, figs)
				: FixedNotation<Char_t>(val, decimalPlaces, figs);
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
		const Char_t* StringFloatingPoint(T val, size_t decimalPlaces = s_NoSpecifier
			, FloatingPointFormat fmt = FloatingPointFormat::FIX)
	{
		using namespace secret::impl;
		using IntegralSizeMatch_t = std::conditional_t<sizeof(T) == 4, std::int32_t, std::int64_t>;

		constexpr auto defaultDps = std::numeric_limits<size_t>::digits10;

		if (std::isnan(val))
			return Convert<Char_t>("nan");
		if (std::isinf(val))
			return Convert<Char_t>("inf");
		if (g_MaxFloatDigits<T> - 2 < decimalPlaces)
			throw kDebug::FormatError("value is too large for buffer");
		
		Figures figs = decimalPlaces < defaultDps
			? GetFigures(val, defaultDps)
			: GetFigures(val, decimalPlaces);

		const auto limit = kmaths::PowerOf10(decimalPlaces);
		while (figs.decimals > limit)
		{
			figs.decimals = kmaths::Demote(figs.decimals);
		}

		switch (fmt.ToEnum()) {
		case FloatingPointFormat::FIX: return FixedNotation<Char_t>(val, decimalPlaces, figs);
		case FloatingPointFormat::SCI: return ScientificNotation<Char_t>(val, decimalPlaces, figs);
		case FloatingPointFormat::GEN: return GeneralNotation<Char_t>(val, decimalPlaces, figs);
		case FloatingPointFormat::HEX: return StringIntegralBinary<Char_t>(*(IntegralSizeMatch_t*)&val, decimalPlaces);
		case FloatingPointFormat::BIN: return StringIntegralHex<Char_t>(*(IntegralSizeMatch_t*)&val, decimalPlaces);
		default:
			throw kDebug::FormatError("Unknown floating point notation: " + ToWriter(fmt.ToString()));
		}
	}

}
