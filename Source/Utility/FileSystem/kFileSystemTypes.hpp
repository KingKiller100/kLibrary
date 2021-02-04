#pragma once

#include "../../TypeTraits/StringTraits.hpp"
#include "../String/kStringTypes.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

namespace klib::kFileSystem
{
	//Type aliases for STL containers --------------------------------------------------------

	// STL filesystem::path
	using Path = std::filesystem::path;

	// STL basic_ifstream
	template<typename Char_t, typename = std::enable_if_t<
		type_trait::Is_Char_V<Char_t>
		>>
	using FileReader = std::basic_ifstream<type_trait::Simplify_t<Char_t>, std::char_traits<type_trait::Simplify_t<Char_t>>>;

	// STL basic_ofstream
	template<typename Char_t, typename = std::enable_if_t<
		type_trait::Is_Char_V<Char_t>
		>>
	using FileWriter = std::basic_ofstream<type_trait::Simplify_t<Char_t>, std::char_traits<type_trait::Simplify_t<Char_t>>>;

	// STL vector of StringWriters
	template<typename Char_t, typename = std::enable_if_t<
		type_trait::Is_Char_V<Char_t>
		>>
	using FileLines = std::vector<kString::StringWriter<Char_t>>;

	// STL vector of STL filesystem paths
	using PathList = std::vector<std::filesystem::path>;
	
	// --------------------------------------------------------------------------------------
}
