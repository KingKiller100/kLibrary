#include "pch.hpp"
#include "kFileLogger.hpp"

#include "../kLogEntry.hpp"
#include "../kLogProfile.hpp"

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
		FileLogger::FileLogger(const std::filesystem::path& path)
			: path(path)
		{
			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::TRC
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t");

			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::DBG
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t");

			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::NRM
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t");

			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::INF
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t");

			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::WRN
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t");

			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::ERR
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				"\n                  [Source]: &f [&l]");

			LogDestWithFormatSpecifier::SetFormat(
				LogLevel::FTL
				, "[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&p]: &t"
				"\n                  [File]: &f"
				"\n                  [Line]: &l");
		}

		FileLogger::~FileLogger() noexcept
			= default;

		std::string_view FileLogger::GetName() const
		{
			return "Console Logger";
		}

		std::string FileLogger::GetFileName() const
		{
			return path.stem().string();
		}

		void FileLogger::SetFileName(const std::string_view& newFilename)
		{
			Close();
			path.replace_filename(GetFileNameWithoutExtension(newFilename));
			Open();
		}

		std::string FileLogger::GetExtension() const
		{
			return path.extension().string();
		}

		void FileLogger::SetExtension(const std::string_view& newExtension)
		{
			path.replace_extension(newExtension);
		}

		std::string FileLogger::GetDirectory() const
		{
			return path.parent_path().string();
		}

		void FileLogger::SetDirectory(const std::filesystem::path& newDir)
		{
			Close();
			const auto filename = path.filename().string();
			path.clear();
			path = (newDir / filename);
			Open();
		}

		std::filesystem::path FileLogger::GetPath() const
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

		bool FileLogger::IsOpen() const
		{
			return fileStream.is_open() && fileStream.good();
		}

		void FileLogger::AddEntry(const LogEntry& entry)
		{
			if (!IsOpen())
				return;

			const auto logLine = CreateLogText(entry.GetProfile(), entry.GetMsg());

			Flush(logLine);
		}

		std::string FileLogger::CreateLogText( const LogProfile& profile, const LogMessage& msg ) const
		{
			// Message details
			const auto& t = msg.time;
			const auto& hour = t.GetHour();
			const auto& minute = t.GetMinute();
			const auto& second = t.GetSecond();
			const auto& milli = t.GetMillisecond();

			const auto& d = msg.date;
			const auto& day = d.GetDay();
			const auto& month = d.GetMonth();
			const auto& year = d.GetYear();

			const auto& text = msg.text;
			
			const auto& sourceInfo = msg.sourceInfo;

			// Description details
			const auto name = profile.GetName();
			
			const auto format = formatMap.at(lvl);

			std::string logLine = ToString(format,
				day,
				month,
				year,
				hour,
				minute,
				second,
				milli,
				name,
				text,
				sourceInfo.file,
				sourceInfo.line,
				sourceInfo.func
			);


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

				Flush(padding + logMsg + padding);
				Flush("\n");
			}

			fileStream.close();
		}

		void FileLogger::Flush(const std::string_view& msg)
		{
			std::scoped_lock scoped_lock(lock);

			if (!IsOpen())
				return;

			fileStream << msg;
			fileStream.flush();
		}
	}
}
