#include "pch.hpp"
#include "kFileLogger.hpp"

#include "../kLogEntry.hpp"
#include "../kLogProfile.hpp"

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
		FileLogger::FileLogger( const std::filesystem::path& path )
			: path( path )
		{
			FormattedLogDestinationBase::SetFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc] [&n]: &t"
			);
			FormattedLogDestinationBase::SetRawFormat(
				"[&dd/&mm/&yyyy] [&hh:&zz:&ss:&ccc]: &t"
			);
		}

		FileLogger::~FileLogger() noexcept
		= default;

		std::string_view FileLogger::GetName() const
		{
			return "File Logger";
		}

		std::string FileLogger::GetFileName() const
		{
			return path.stem().string();
		}

		void FileLogger::SetFileName( const std::string_view& newFilename )
		{
			Close();
			path.replace_filename( GetFileNameWithoutExtension( newFilename ) );
			Open();
		}

		std::string FileLogger::GetExtension() const
		{
			return path.extension().string();
		}

		void FileLogger::SetExtension( const std::string_view& newExtension )
		{
			path.replace_extension( newExtension );
		}

		std::string FileLogger::GetDirectory() const
		{
			return path.parent_path().string();
		}

		void FileLogger::SetDirectory( const std::filesystem::path& newDir )
		{
			Close();
			const auto filename = path.filename().string();
			path.clear();
			path = ( newDir / filename );
			Open();
		}

		std::filesystem::path FileLogger::GetPath() const
		{
			return path.string();
		}

		void FileLogger::SetPath( const std::filesystem::path& path )
		{
			this->path = path;
		}

		void FileLogger::Open()
		{
			std::scoped_lock<decltype(lock)> scoped_lock( lock );

			if ( !IsOpen() )
			{
				CreateNewDirectories( path.parent_path() );
				fileStream.open( path, std::ios::out | std::ios::in | std::ios::app );
			}
		}

		bool FileLogger::IsOpen() const
		{
			return fileStream.is_open() && fileStream.good();
		}

		void FileLogger::AddEntry( const LogEntry& entry )
		{
			const auto logLine = CreateLogText( entry );

			Flush( logLine );
		}

		std::string FileLogger::CreateLogText( const LogEntry& entry ) const
		{
			const auto& profile = entry.GetProfile();
			const auto& msg = entry.GetMsg();

			// Level
			const auto lvl = entry.GetLevel();

			// Profile
			const auto profileName = profile.GetName();

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


			std::string logLine = ToString( messageFormat,
				day,
				month,
				year,
				hour,
				minute,
				second,
				milli,
				profileName,
				lvl.ToString(),
				lvl.ToUnderlying(),
				text
			);

			logLine.push_back( '\n' );

			return logLine;
		}

		void FileLogger::AddRaw(const LogMessage& message)
		{
			const auto rawLogLine = CreateRawLogText(message);
			Flush(rawLogLine);
		}

		std::string FileLogger::CreateRawLogText( const LogMessage& msg ) const
		{
			// Message details
			const auto& time = msg.time;
			const auto& hour = time.GetHour();
			const auto& minute = time.GetMinute();
			const auto& second = time.GetSecond();
			const auto& milli = time.GetMillisecond();

			const auto& date = msg.date;
			const auto& day = date.GetDay();
			const auto& month = date.GetMonth();
			const auto& year = date.GetYear();

			const auto& text = msg.text;

			// Profile name

			std::string logLine = ToString( rawMessageFormat,
				day,
				month,
				year,
				hour,
				minute,
				second,
				milli,
				text
			);

			logLine.push_back( '\n' );

			return logLine;
		}


		void FileLogger::Close()
		{
			if ( !IsOpen() )
				return;

			fileStream.close();
		}

		bool FileLogger::Move(const std::filesystem::path& path)
		{
			Close();

			fileStream.seekg(std::ios::beg);
			const std::string contents{ std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>() };

			const auto oldPath = GetPath();

			SetPath(path);

			Open();

			if (IsOpen())
			{
				Remove(oldPath);
				fileStream << contents;
				return true;
			}

			SetPath(oldPath);

			// Failed to open new file. Using the original log file again
			Open();

			return false;
		}

		void FileLogger::Flush( const std::string_view& msg )
		{
			std::scoped_lock scoped_lock( lock );

			fileStream << msg;
			fileStream.flush();
		}
	}
}
