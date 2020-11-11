#include "pch.hpp"
#include "kLogging.hpp"

#include "kLogMessage.hpp"
#include "Destinations/kFileLogger.hpp"
#include "Destinations/kConsoleLogger.hpp"
#include "kLogDescriptor.hpp"

#include "../../Type Traits/ToImpl.hpp"
#include "../String/kToString.hpp"


namespace klib::kLogs
{
	using namespace kString;
	using namespace kCalendar;
	using namespace type_trait;

	const LogEntry kLogs_Empty("NO ENTRIES! CACHE IS EMPTY", LogDescriptor("Empty"));

	Logging::Logging(const std::string_view& directory, const std::string_view& filename
		, const std::string_view& extension
		, const std::string_view& name)
		: minimumLoggingLevel(LogLevel::DBUG),
		name(name),
		isEnabled(false),
		cacheMode(false),
		constantFlushing(false)
	{
		Initialize(directory, filename, extension);
	}

	Logging::~Logging()
	{
		if (!entriesQ.empty())
			FinalOutput();
	}

	void Logging::Initialize(const std::string_view& directory, const std::string_view& filename, const std::string_view& extension)
	{
		destinations[DestionationType::FILE].reset(new FileLogger(name, directory, filename, extension));
		destinations[DestionationType::CONSOLE].reset(new ConsoleLogger(name));
		
		ToggleLoggingEnabled();
		Open();
	}

	void Logging::Open()
	{
		for (auto& dest : destinations)
		{
			if (!dest.second->Open())
			{
				const auto destType = DestionationType::PrettyType(dest.first);
				const auto errMsg = ToString("{0} - Unable to open log destination: {1}", name, destType);
				throw std::runtime_error(errMsg);
			}
		}
	}

	void Logging::OutputInitialized(const std::string_view& openingMsg)
	{
		if (!isEnabled) { return; }
		
		for (auto& dest : destinations)
		{
			if (dest.second->IsOpen())
				dest.second->OutputInitialized(openingMsg);
		}
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
		for (const auto& dest : destinations)
		{
			dest.second->SetName(newName);
		}
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleConsoleEnabled() noexcept
	{
		auto& consoleLogger = destinations.at(DestionationType::CONSOLE);
		if (consoleLogger->IsOpen())
		{
			consoleLogger->Close(false);
		}
		else
		{
			consoleLogger->Open();
		}
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		if (cacheMode == enable)
			return;
		
		cacheMode = enable;

		if (!enable)
			Flush();
	}

	void Logging::ChangeOutputPath(const std::string_view & dir, const std::string_view & filename)
	{
		ChangeOutputDirectory(dir);
		ChangeFilename(filename);
	}

	void Logging::ChangeOutputDirectory(const std::string_view& directory)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(DestionationType::FILE));
		fLogger.SetDirectory(directory);
	}

	void Logging::ChangeFilename(const std::string_view& filename)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(DestionationType::FILE));
		fLogger.SetFileName(filename);
	}
	
	std::string Logging::GetOutputPath() const
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(DestionationType::FILE));
		const auto path = fLogger.GetPath();
		return path;
	}

	void Logging::SuspendFileLogging()
	{
		constexpr char pauseLog[] = "************************************************************************";
		AddVerbatim();
		AddVerbatim(pauseLog);
		AddVerbatim();
		
		Close();
		SetCacheMode(true);
	}

	void Logging::ResumeFileLogging()
	{
		SetCacheMode(false);
	}


	void Logging::OutputToFatalFile(const LogMessage& msg)
	{
		AddEntry(LogLevel::FATL, msg);
		FinalOutput();
	}

	void Logging::AddVerbatim(const std::string_view& text)
	{
		AddLog(LogEntry(text.data(), LogDescriptor(LogLevel::VBAT)));
	}

	void Logging::AddEntry(const LogLevel& level, const LogMessage& message)
	{
		if (!isEnabled || !IsLoggable(level)) 
			return;

		AddLog(LogEntry(
				message, 
				LogDescriptor(level)
		));
	}

	void Logging::AddBanner(const std::string_view& descriptor, const LogMessage& message, const std::string_view& frontPadding, const std::
	                        string_view& backPadding, const std::uint16_t paddingCount)
	{
		if (!isEnabled)
			return;

		constexpr auto format = "{0} {1} {2}";
		
		std::string front, back;
		for (auto i = 0; i < paddingCount; ++i)
		{
			front.append(frontPadding);
			back.append(backPadding);
		}
		
		const auto text = ToString(format
			, front
			, message.text
			, back
		);

		const LogMessage banner(text, message);
		
		AddLog(LogEntry(banner, LogDescriptor(descriptor)));
	}

	void Logging::AddLog(const LogEntry& entry)
	{
		entriesQ.push_back(entry);

		if (!cacheMode)
			Flush();
	}
	
	void Logging::FinalOutput()
	{
		Close();
		isEnabled = false;
	}

	void Logging::Flush()
	{
		while (!entriesQ.empty())
		{
			for (auto& dest : destinations)
			{
				const auto& entry = entriesQ.front();
				dest.second->AddEntry(entry);
			}
			entriesQ.pop_front();
		}
	}

	void Logging::Close()
	{
		Flush();
		
		for (auto& dest : destinations)
		{
			dest.second->Close(true);
		}
	}

	bool Logging::IsLoggable(const LogLevel lvl) const
	{
		return (lvl >= minimumLoggingLevel);
	}

	const LogEntry& Logging::GetLastCachedEntry() const
	{
		if (entriesQ.empty() || !cacheMode)
			return kLogs_Empty;

		const auto& lastLog = entriesQ.back();
		
		return lastLog;
	}

	void Logging::ClearCache()
	{
		if (!entriesQ.empty())
			entriesQ.clear();
	}

	void Logging::EnableConstantFlush(bool enable)
	{
		constantFlushing = enable;
	}

	bool Logging::ErasePrevious(size_t count)
	{
		if (entriesQ.empty())
			return false;

		
		while (count-- != 0)
		{
			entriesQ.pop_back();
		}

		return true;
	}
}
