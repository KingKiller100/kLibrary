#include "pch.hpp"
#include "kFileLogger.hpp"

#include "../kLogEntry.hpp"

#include "../../Calendar/kCalendar.hpp"
#include "../../String/kToString.hpp"
#include "../../FileSystem/kFileSystem.hpp"

#include <mutex>

namespace klib
{
	using namespace kString;
	using namespace kCalendar;
	using namespace kFileSystem;

	namespace kLogs
	{
		std::mutex g_kFileLoggerMutex;

		FileLogger::FileLogger(const std::string_view& newName, const std::filesystem::path& path)
			: name(newName)
			, path(path)
		{}

		FileLogger::~FileLogger() noexcept
			= default;

		std::string_view FileLogger::GetName() const
		{
			return name;
		}

		void FileLogger::SetName(const std::string_view& newName)
		{
			name = newName;
		}

		std::string_view FileLogger::GetFileName() const
		{
			return path.stem().string();
		}

		void FileLogger::SetFileName(const std::string_view& newFilename)
		{
			Close(true);
			path.replace_filename(GetFileNameWithoutExtension(newFilename));
			Open();
		}

		std::string_view FileLogger::GetExtension() const
		{
			return path.extension().string();
		}

		void FileLogger::SetExtension(const std::string_view& newExtension)
		{
			path.replace_extension(newExtension);
		}

		std::string_view FileLogger::GetDirectory() const
		{
			return path.parent_path().string();
		}

		void FileLogger::SetDirectory(const std::string_view& newDir)
		{
			Close(true);
			const auto filename = kFileSystem::AppendFileExtension(GetFileName(), GetExtension());
			const auto pathSep = kFileSystem::pathSeparator<char>;
			path.clear();
			path.assign(ToString(newDir, pathSep, filename));
			Open();
		}

		std::string_view FileLogger::GetPath() const
		{
			return path.string();
		}

		void FileLogger::SetPath(const std::filesystem::path& path)
		{
			this->path = path;
		}

		void FileLogger::OutputInitialized(const std::string_view& openingMsg)
		{
			if (!IsOpen())
			{
				const auto msg = ToString("{0}: file logger not open", name);
				throw std::runtime_error(msg);
			}

			const std::string spacing(5, ' ');
			const std::string padding(73, '*');
			constexpr auto nl = "\n";

			std::string opener = padding + nl;
			opener += spacing + "File logger activated: ";
			opener += spacing + GetDateInTextFormat(Date::DateTextLength::SHORT) + spacing + GetTimeText() + nl;
			opener += openingMsg;
			opener += nl + padding + nl;

			Flush(opener);
		}

		bool FileLogger::Open()
		{
			std::scoped_lock<decltype(g_kFileLoggerMutex)> scoped_lock(g_kFileLoggerMutex);

			if (!IsOpen())
			{
				CreateNewDirectories(path.parent_path());
				fileStream.open(path, std::ios::out | std::ios::in | std::ios::app);
			}

			return fileStream.good();
		}

		bool FileLogger::IsOpen()
		{
			return fileStream.is_open() && fileStream.good();
		}

		void FileLogger::AddEntry(const LogEntry& entry)
		{
			if (!IsOpen())
				return;

			const auto logLine = CreateLogText(entry);

			Flush(logLine);
		}

		std::string FileLogger::CreateLogText(const LogEntry& entry) const
		{
			std::string logLine;

			const auto& message = entry.GetMsg();
			const auto& desc = entry.GetDescriptor();

			if (entry.HasDescription(LogDescriptor(LogLevel::VBAT)))
			{
				logLine = message.text;
			}
			else
			{
				const auto timeStr = message.time.ToString();
				const auto dateStr = message.date.ToString(Date::SLASH);

				logLine = ToString("[{0}] [{1}] [{2}] [{3}]:  {4}",
					dateStr,
					timeStr,
					name,
					desc.info,
					message.text);
			}

			if (desc.lvl >= LogLevel::ERRR)
			{
				logLine.append(ToString(R"(
               [FILE]: {0}
               [LINE]: {1}
               [FUNC]: {2})",
					message.sourceInfo.file
					, message.sourceInfo.line
					, message.sourceInfo.func)
				);
			}

			logLine.push_back('\n');

			return logLine;
		}

		void FileLogger::Close(const bool outputClosingMsg)
		{
			if (!IsOpen())
				return;

			if (outputClosingMsg)
			{
				static constexpr char msg[] = "File Logging Concluded";

				const std::string padding(72, '*');
				const std::string spacing(25, ' ');
				const auto logMsg = spacing + msg + spacing;

				Flush(padding);
				Flush(logMsg);
				Flush(padding);
			}

			fileStream.close();
		}

		void FileLogger::Flush(const std::string_view& msg)
		{
			std::scoped_lock<decltype(g_kFileLoggerMutex)> scoped_lock(g_kFileLoggerMutex);

			if (!IsOpen())
				return;

			fileStream << msg;
			fileStream.flush();
		}
	}
}
