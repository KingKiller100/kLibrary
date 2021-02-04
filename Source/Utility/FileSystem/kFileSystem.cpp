#include "pch.hpp"
#include "kFileSystem.hpp"

namespace klib::kFileSystem
{
	bool CreateNewDirectory(const Path& directory) noexcept
	{
		return _wmkdir(directory.wstring().data()) == 0; // 0 == SUCCESS
	}

	bool CreateNewDirectories(const Path& directory)
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

	bool Remove(const Path& filepath)
	{
		return std::filesystem::remove(filepath);
	}

	bool RemoveAll(const Path& filepath)
	{
		return std::filesystem::remove_all(filepath);
	}

	bool CheckFileExists(const Path& path) noexcept
	{
		return CheckFileExists(path.wstring());
	}

	bool CheckDirectoryExists(const Path& path) noexcept
	{
		const auto exists = std::filesystem::exists(path);
		return exists;
	}
}
