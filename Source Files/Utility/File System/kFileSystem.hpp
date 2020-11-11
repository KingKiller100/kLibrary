#pragma once

#include "../../HelperMacros.hpp"

#include "../String/kStringConverter.hpp"
#include "../String/kStringManipulation.hpp"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#ifdef KLIB_WINDOWS_OS
#	include <corecrt_wdirect.h>
#	include <direct.h>
#	include <sys/stat.h>

#	define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#		include <Windows.h>
#	undef WIN32_LEAN_AND_MEAN
#endif

namespace klib::kFileSystem
{
	//Type aliases for STL containers --------------------------------------------------------

	// STL filesystem::path
	using Path = std::filesystem::path;

	// STL basic_ifstream
	template<class Char>
	using FileReader = std::basic_ifstream<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>>;

	// STL basic_ofstream
	template<class Char>
	using FileWriter = std::basic_ofstream<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>>;

	// STL vector of StringWriters
	template<typename Char>
	using FileLines = std::vector<kString::StringWriter<Char>>;

	// --------------------------------------------------------------------------------------

	template<class CharType, class = std::enable_if_t<type_trait::Is_CharType_V<CharType>>>
	constexpr auto pathSeparator = CharType('\\');

	template<class SourceType, class = std::enable_if_t<type_trait::Is_StringType_V<SourceType>>>
	USE_RESULT constexpr auto CorrectFilePathSeparators(const SourceType& src)
	{
		using CharType = typename SourceType::value_type;
		return kString::Replace<CharType>(src, CharType('/'), pathSeparator<CharType>);
	}

	template<class SourceType, class = std::enable_if_t<type_trait::Is_CharType_V<SourceType>>>
	USE_RESULT constexpr auto CorrectFilePathSeparators(const SourceType* src)
	{
		using CharType = SourceType;
		return kString::Replace(src, CharType('/'), pathSeparator<CharType>);
	}

	namespace secret::helper
	{
		inline bool update_cwd = true;
	}

	/**
	 * \brief
	 *		Outputs a file to the specified directory and fills it with the given data
	 * \param filePath
	 *		The full file directory with the final filename and file extension
	 * \param content
	 *		The data to fill the file with.
	 * \param mode
	 *		File mode i.e. out/append/binary/etc...
	 */
	template<class CharType = char>
	constexpr bool WriteFile(const kString::StringWriter<CharType>& filePath,
		const kString::StringReader<CharType>& content, std::ios::openmode mode = std::ios::out | std::ios::app)
	{
		FileWriter<CharType> outFile(filePath, mode);

		if (outFile.is_open())
		{
			if (mode & std::ios::binary)
				outFile.write(
					reinterpret_cast<const CharType*>(content.data())
					, content.size());
			else
				outFile << content.data();

			outFile.close();
			return true;
		}

#if defined(_DEBUG) || defined(KLIB_DEBUG)
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			const auto failMsg = filePath + "Cannot create/open file: ";
			OutputDebugStringA(failMsg.c_str());
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			const auto failMsg = filePath + L"Cannot create/open file: ";
			OutputDebugStringW(failMsg.c_str());
		}
		else
		{
			const auto wFileName = kString::Convert<wchar_t>(filePath);
			const auto failMsg = L"Cannot create/open file: " + wFileName;
			OutputDebugStringW(failMsg.c_str());
		}

		throw std::runtime_error("Unable to write to file: " + kString::Convert<char>(filePath));
#endif // DEBUG


		return false;
	}

	/**
	 * \brief
	 *		Creates a new folder to the directory, if it does not exist already
	 * \param directory
	 *		Full folder path of the new folder to be created
	 * \return
	 *		Boolean representing whether the directory has been created (TRUE) or not (FALSE)
	 */
	inline bool CreateNewDirectory(const Path& directory) noexcept
	{
		return _wmkdir(directory.wstring().data()) == 0; // 0 == SUCCESS
	}

	/**
	 * \brief
	 *		Creates multiple new folder to the directory paths, if the directory does not exist already;
	 *		from the current working directory
	 * \param directory
	 *		Full folder path of the directories to be created
	 * \return
	 *		Boolean representing whether the directories has been created (TRUE) or not (FALSE)
	 *	\note
	 *		The path must be completely unique otherwise the path will not be created. If parts of the
	 *		path already exist, only
	 */
	inline bool CreateNewDirectories(const Path& directory)
	{
		using Char = wchar_t;

		Path dir(directory);

		if (dir.wstring().back() != pathSeparator<Char>)
			dir += pathSeparator<Char>; // Final suffix of directory char type must end with '\\'

		bool isDirCreated = std::filesystem::create_directories(dir);

		if (!isDirCreated)
			isDirCreated = std::filesystem::exists(dir);

		return isDirCreated;
	}


	/**
	* \brief
	*		Deletes file from system
	* \tparam CharType
	*		Char type i.e. must be char/wchar_t/u32char_t/etc...
	* \param[in] filepath
	*		Full file path of the file to delete
	* \return
	*		TRUE if file is found and deleted, else FALSE if file cannot be found or deleted
	*/
	inline bool RemoveFile(const Path& filepath)
	{
		return std::filesystem::remove(filepath);
	}

	/**
	 * \brief
	 *		Deletes a directory
	 * \param directory
	 *		The full folder directory
	 * \return
	 *		Boolean representing whether the directory could be deleted or not
	 * \note
	 *		Please note that this may only work if these two requirements are met:
	 *		- The folder is completely empty (including empty of system and hidden folder files)
	 *		- This directory is not the current directory of this application.
	 */
	template<class CharType = char>
	bool DeleteDirectory(const kString::StringReader<CharType>& directory)
	{
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			return _rmdir(directory.data()) == 0; // 0 == SUCCESS
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			return _wrmkdir(directory.data()) == 0; // 0 == SUCCESS
		}
		else
		{
			kString::StringWriter<wchar_t> temp;
			for (auto& c : directory)
				temp += CAST(wchar_t, c);
			return DeleteDirectory<wchar_t>(temp);
		}
		return false;
	}

	/**
	 * \brief
	 *		Checks (from folder holding the executable file in current directory) if a file exists
	 * \param fullFilePath
	 *		filename (or full directory to the file)
	 * \return
	 *		TRUE if file exist or FALSE if file does not exist
	 */

	template<class CharType = char>
	constexpr bool CheckFileExists(const kString::StringReader<CharType>& fullFilePath) noexcept
	{
		FILE* file;
		errno_t result;

		const auto path = CorrectFilePathSeparators(fullFilePath);

		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			result = fopen_s(&file, path.data(), "r");
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			result = _wfopen_s(&file, path.data(), L"r");
		}
		else
		{
			kString::StringWriter<wchar_t> temp;
			for (auto& c : fullFilePath)
				temp += CAST(wchar_t, c);
			result = CheckFileExists<wchar_t>(temp);
		}

		if (file)
			fclose(file);

		return result == 0;
	}

	/**
	 * \brief
	 *		Checks (from folder holding the executable file in current directory) if a directory exists
	 * \param directoryPath
	 *		folder (or full directory)
	 * \return
	 *		TRUE if it exists or FALSE if it does not exist
	 */

	template<class CharType = char>
	constexpr bool CheckDirectoryExists(const kString::StringReader<CharType>& directoryPath) noexcept
	{
#if MSVC_PLATFORM_TOOLSET > 140
		const auto exists = std::filesystem::exists(directoryPath);
		return exists;
#else
		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			struct stat info {};
			const auto statResult = stat(GetParentPath(directoryPath).data(), &info);

			if (statResult != KLIB_FALSE)
				return false;

			const auto result = (info.st_mode & S_IFDIR);
			return result != KLIB_FALSE;
		}
		else
		{
			const auto temp = kString::Convert<char>(directoryPath);
			return CheckDirectoryExists<char>(temp.data());
		}
#endif
	}

	/**
	 * \brief
	 *		Collects every line of text in the file and returns it
	 * \param fullFilePath
	 *		Full directory of the file you wish to collect data from
	 * \return
	 *		A vector of every line of data in the file, as a string
	 */
	template<class CharType = char>
	USE_RESULT constexpr auto ReadFile(const kString::StringReader<CharType>& fullFilePath)
	{
		using Char = ONLY_TYPE(CharType);

		FileLines<Char> fileData;

		if (!CheckFileExists<Char>(fullFilePath))
			return fileData;

		FileReader<CharType> inFile(fullFilePath.data());

		if (!inFile.is_open())
			return fileData;

		kString::StringWriter<Char> data;
		while (std::getline(inFile, data))
		{
			fileData.push_back(data);
		}
		inFile.close();

		return fileData;
	}

	/**
	 * \brief
	 *		Returns the current working directory
	 * \return
	 *		Current working directory as a string
	 */
	template<class CharType = char>
	USE_RESULT kString::StringReader<CharType> GetCurrentWorkingDirectory()
	{
		using Char = ONLY_TYPE(CharType);

		static kString::StringWriter<Char> cwdFullPath;

		if (cwdFullPath.empty() || secret::helper::update_cwd)
		{
			Char* cwdBuffer = nullptr;
			DWORD length = 0;
			if _CONSTEXPR_IF(std::is_same_v<Char, char>)
			{
				length = ::GetCurrentDirectoryA(0, nullptr);
				cwdBuffer = new Char[length]{};
				::GetCurrentDirectoryA(length, cwdBuffer);
			}
			else if _CONSTEXPR_IF(std::is_same_v<Char, wchar_t>)
			{
				length = ::GetCurrentDirectoryW(0, nullptr);
				cwdBuffer = new Char[length]{};
				::GetCurrentDirectoryW(length, cwdBuffer);
			}
			else
			{
				const auto dummyBuffer = GetCurrentWorkingDirectory<wchar_t>();
				for (auto& c : dummyBuffer)
					cwdFullPath += CAST(Char, c);
				return cwdFullPath;
			}

			cwdFullPath = kString::StringWriter<Char>(cwdBuffer, cwdBuffer + (length - 1));
			cwdFullPath += pathSeparator<Char>;

			delete[] cwdBuffer;
			secret::helper::update_cwd = false;
		}

		return cwdFullPath;
	}

	/**
	 * \brief
	 *		Changes the current working directory to a new path
	 * \tparam CharType
	 *		character type
	 * \param path
	 *		Path to change to as an STL string
	 * \return
	 *		TRUE if path is valid and change is attempted, FALSE if bad path
	 */
	template<class CharType = char>
	bool SetCurrentWorkingDirectory(const kString::StringReader<CharType>& path)
	{
		if (!CheckDirectoryExists(path))
			return false;

		std::filesystem::current_path(path);

		// Flag to notify current working directory to update
		secret::helper::update_cwd = true;

		return std::filesystem::current_path() == path;
	}

	/**
	 * \brief
	 *		Returns the current exe directory
	 * \return
	 *		Current working directory as a string
	 */
	template<class CharType = char>
	USE_RESULT kString::StringReader<CharType> GetExeDirectory()
	{
		using Char = ONLY_TYPE(CharType);

		static kString::StringWriter<Char> exeFullPath;

		if (exeFullPath.empty())
		{
			constexpr DWORD bufferSize = 1024 * 2;
			Char* exeBuffer = new Char[bufferSize]{};

			DWORD length = 0;
			if _CONSTEXPR_IF(std::is_same_v<Char, char>)
			{
				length = ::GetModuleFileNameA(nullptr, exeBuffer, bufferSize);
			}
			else if _CONSTEXPR_IF(std::is_same_v<Char, wchar_t>)
			{
				length = ::GetModuleFileNameW(nullptr, exeBuffer, bufferSize);
			}
			else
			{
				const auto dummyBuffer = GetExeDirectory<wchar_t>();
				for (auto& c : dummyBuffer)
					exeFullPath += CAST(Char, c);
				return exeFullPath;
			}

			exeFullPath = kString::StringWriter<Char>(exeBuffer, exeBuffer + (length - 1));
			exeFullPath.erase(exeFullPath.find_last_of(pathSeparator<Char>) + 1);

			delete[] exeBuffer;
		}

		return exeFullPath;
	}

	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType> GetFileName(const kString::StringReader<CharType>& path) noexcept
	{
		using Char = std::decay_t<std::remove_pointer_t<CharType>>;
		const auto p = kString::Replace<Char>(path, Char('/'), pathSeparator<Char>);
		const auto filename = p.substr(p.find_last_of(pathSeparator<Char>) + 1);
		return filename;
	}

	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType> GetFileNameWithoutExtension(const kString::StringReader<CharType>& path) noexcept
	{
		kString::StringWriter<CharType> filename = GetFileName<ONLY_TYPE(CharType)>(path);
		filename = filename.erase(filename.find_first_of('.'));
		return filename;
	}

	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType> GetExtension(const kString::StringReader<CharType>& path)
	{
		kString::StringWriter<CharType> extension = GetFileName<ONLY_TYPE(CharType)>(path);
		const size_t dotPos = extension.find_first_of('.');
		const auto start = extension.cbegin();
		const auto end = start + dotPos;
		extension.erase(start, end);
		return extension;
	}

	template<class CharType = char>
	USE_RESULT constexpr kString::StringWriter<CharType> GetParentPath(const kString::StringWriter<CharType>& path)
	{
		using Char = ONLY_TYPE(CharType);
		auto parentPath = kString::Replace<ONLY_TYPE(Char)>(path, Char('/'), pathSeparator<Char>);
		parentPath = parentPath.substr(0, parentPath.find_last_of(pathSeparator<Char>));
		return parentPath;
	}

	template<class Char = char>
	USE_RESULT constexpr kString::StringWriter<Char> AppendFileExtension(const kString::StringReader<Char>& fname, const kString::StringReader<Char>& extension) noexcept
	{
		using StrW = kString::StringWriter<Char>;
		using StrR = kString::StringReader<Char>;

		StrR filename = fname;
		const auto isDotAtStartOFExtension = extension[0] == '.';

		const auto dotPos = filename.find_first_of('.');
		if (dotPos != StrR::npos) // Dot is in the filename
		{
			const auto extPos = filename.find(extension);

			if (extPos != StrR::npos) // Extension is present
			{
				if (dotPos <= extPos) // Extension is present after the dot
				{
					return filename.data(); // Assume it's all good
				}
			}

			StrW appendedFilename = filename.data();
			if (dotPos == filename.length() - 1)
			{
				appendedFilename += extension;
			}
			else
			{
				if (!isDotAtStartOFExtension)
					appendedFilename += '.';
				appendedFilename += extension;
			}

			return appendedFilename;
		}

		StrW newFilename = filename.data();
		if (!isDotAtStartOFExtension)
			newFilename += '.';
		newFilename += extension;

		return newFilename;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kFileSystem;
#endif

}


