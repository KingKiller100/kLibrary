#include "pch.hpp"
#include "kFileLogger.hpp"

#include "../kLogEntry.hpp"

#include "../../Calendar/kCalendar.hpp"
#include "../../Calendar/kCalendarToString.hpp"
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
		FileLogger::FileLogger(std::string* newName, const std::filesystem::path& path)
			: name(newName)
			, path(path)
		{
			// LogDestWithFormatSpecifier::SetFormat("&t", LogLevel::RAW);

			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				, LogLevel::BNR);
			
			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				, LogLevel::DBG);
			
			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				, LogLevel::NRM);
			
			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				, LogLevel::INF);
			
			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				, LogLevel::WRN);
			
			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				"\n                  [File]: &q [&l]"
				, LogLevel::ERR);
			
			LogDestWithFormatSpecifier::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				"\n                  [File]: &q"
				"\n                  [Line]: &l"
				, LogLevel::FTL);
		}

		FileLogger::~FileLogger() noexcept
			= default;

		void FileLogger::SetName(std::string* newName)
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

		bool FileLogger::Open()
		{
			std::scoped_lock<decltype(lock)> scoped_lock(lock);

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

			const auto& msg = entry.GetMsg();
			const auto& desc = entry.GetDescriptor();

			if (entry.HasDescription(LogDescriptor(LogLevel::RAW)))
			{
				logLine = msg.text;
			}
			else
			{
				const auto& t = msg.time;
				const auto& hour = t.GetHour();
				const auto& minute = t.GetMinute();
				const auto& second = t.GetSecond();
				const auto& milli = t.GetMillisecond();

				const auto& d = msg.date;
				const auto& day = d.GetDay();
				const auto& month = d.GetMonth();
				const auto& year = d.GetYear();

				const auto format = formatMap.at(desc.lvl);

				logLine = ToString(format,
					day,
					month,
					year,
					hour,
					minute,
					second,
					milli,
					*name,
					desc.info,
					desc.lvl.ToUnderlying(),
					msg.text,
					msg.sourceInfo.file,
					msg.sourceInfo.line,
					msg.sourceInfo.func
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

				const std::string padding(81, '*');
				const std::string spacing(25, ' ');
				const auto logMsg = spacing + msg + spacing;

				Flush(padding);
				Flush(logMsg);
				Flush(padding);
				Flush("\n");
			}

			fileStream.close();
		}

		void FileLogger::Flush(const std::string_view& msg)
		{
			std::scoped_lock<decltype(lock)> scoped_lock(lock);

			if (!IsOpen())
				return;

			fileStream << msg;
			fileStream.flush();
		}
	}
}
