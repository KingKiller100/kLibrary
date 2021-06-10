#pragma once

#include "../Platform/kPlatform.hpp"
#include "../String/Tricks/kStringCases.hpp"

namespace klib
{
	namespace kFileSystem
	{
		template<typename CharT>
		struct UnixPathTraits : public std::char_traits<CharT>
		{
		public:
			static constexpr auto IsDOS = false;
			static constexpr auto IsUnix = true;
			static constexpr auto CaseSensitivity = kString::CaseSensitivity::SENSITIVE;
			using Char_t = CharT;
			using Base_t = std::char_traits<Char_t>;

			USE_RESULT static constexpr const char* find(const Char_t* haystack, size_t count,
				const Char_t& needle) noexcept
			{
				for (; 0 < count; --count, ++haystack) {
					if (ResolveCharacter(*haystack) == ResolveCharacter(needle)) {
						return haystack;
					}
				}

				return nullptr;
			}

			USE_RESULT static constexpr int compare(const Char_t* strA,
				const Char_t* strB, size_t count) noexcept
			{
				for (; 0 < count; --count, ++strA, ++strB)
				{
					const auto letterA = ResolveCharacter(*strA);
					const auto letterB = ResolveCharacter(*strB);
					if (letterA != letterB)
					{
						return letterA < letterB ? -1 : 1;
					}
				}
				return 0;
			}

			USE_RESULT static constexpr bool eq(const Char_t& lhs, const Char_t& rhs) noexcept
			{
				return ResolveCharacter(lhs) == ResolveCharacter(rhs);
			}

			USE_RESULT static constexpr bool lt(const Char_t& lhs, const Char_t& rhs) noexcept
			{
				return ResolveCharacter(lhs) < ResolveCharacter(rhs);
			}

		private:
			static constexpr Char_t ResolveCharacter(const Char_t ch)
			{
				if (ch == static_cast<Char_t>('\\'))
					return static_cast<Char_t>('/');

				return ch;

			}
		};

		template<typename CharT>
		struct DOSPathTraits : public std::char_traits<CharT>
		{
		public:
			static constexpr auto IsDOS = true;
			static constexpr auto IsUnix = false;
			static constexpr auto CaseSensitivity = kString::CaseSensitivity::SENSITIVE;
			using Char_t = CharT;
			using Base_t = std::char_traits<Char_t>;

			USE_RESULT static constexpr const char* find(const Char_t* haystack, size_t count,
				const Char_t& needle) noexcept
			{
				for (; 0 < count; --count, ++haystack) {
					if (ResolveCharacter(*haystack) == ResolveCharacter(needle)) {
						return haystack;
					}
				}

				return nullptr;
			}

			USE_RESULT static constexpr int compare(const Char_t* strA,
				const Char_t* strB, size_t count) noexcept
			{
				for (; 0 < count; --count, ++strA, ++strB)
				{
					const auto letterA = ResolveCharacter(*strA);
					const auto letterB = ResolveCharacter(*strB);
					if (letterA != letterB)
					{
						return letterA < letterB ? -1 : 1;
					}
				}
				return 0;
			}

			USE_RESULT static constexpr bool eq(const Char_t& lhs, const Char_t& rhs) noexcept
			{
				return ResolveCharacter(lhs) == ResolveCharacter(rhs);
			}

			USE_RESULT static constexpr bool lt(const Char_t& lhs, const Char_t& rhs) noexcept
			{
				return ResolveCharacter(lhs) < ResolveCharacter(rhs);
			}

		private:
			static constexpr Char_t ResolveCharacter(const Char_t ch)
			{
				if (ch == static_cast<Char_t>('\\'))
					return static_cast<Char_t>('/');

				return kString::ToUpper(ch);
			}
		};

		namespace secret::impl
		{
			constexpr auto s_IsDos = (kPlatform::GetPlatform() & kPlatform::PlatformOS::WINDOWS) != 0;
		}

		template<typename CharT>
		using PathTraits = std::conditional_t<secret::impl::s_IsDos, DOSPathTraits<CharT>, UnixPathTraits<CharT>>;
	}
}
