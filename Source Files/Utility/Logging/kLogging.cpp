#include "pch.hpp"
#include "kLogging.hpp"

#include "kLogMessage.hpp"
#include "Destinations/kFileLogger.hpp"
#include "Destinations/kConsoleLogger.hpp"
#include "kLogDescriptor.hpp"

#include "../../TypeTraits/ToImpl.hpp"
#include "../String/kToString.hpp"
#include "../FileSystem/kFileSystem.hpp"


namespace klib::kLogs
{
	using namespace kString;
	using namespace kCalendar;
	using namespace type_trait;

	Logging::Logging(const std::string_view& directory
		, const std::string_view& filename
		, const std::string_view& extension
		, const std::string_view& name)
		: Logging(std::filesystem::path(
			ToString("{0}{1}", directory, klib::kFileSystem::AppendFileExtension(filename, extension)))
			, name)
	{
	}

	Logging::Logging(const std::filesystem::path& path, const std::string_view& name)
		: minimumLoggingLevel(LogLevel::DBG),
		name(name),
		isEnabled(false),
		cacheMode(false),
		constantFlushing(false)
	{
		Initialize(path);
	}

	Logging::~Logging()
	{
		if (!entriesQ.empty())
			FinalOutput();
	}

	void Logging::Initialize(const std::filesystem::path& path)
	{
		destinations[LogDestination::FILE].reset(new FileLogger(&name, path));
		destinations[LogDestination::CONSOLE].reset(new ConsoleLogger(&name));

		ToggleLoggingEnabled();
		Open();
	}

	void Logging::Open()
	{
		for (auto& dest : destinations)
		{
			if (!dest.second->Open())
			{
				const auto destPrettyValue = LogDestination(dest.first).ToString();
				const auto errMsg = ToString("{0} - Unable to open log destination: {1}", name, destPrettyValue);
				throw std::runtime_error(errMsg);
			}
		}
	}

	constexpr void Logging::ToggleLoggingEnabled() noexcept
	{
		isEnabled = !isEnabled;
	}

	void Logging::SetName(const std::string_view& newName)
	{
		name = newName;
	}

	void Logging::SetMinimumLoggingLevel(const LogLevel newMin) noexcept
	{
		minimumLoggingLevel = newMin;
	}

	void Logging::ToggleConsoleEnabled() noexcept
	{
		auto& consoleLogger = destinations.at(LogDestination::CONSOLE);
		if (consoleLogger->IsOpen())
		{
			consoleLogger->Close(false);
		}
		else
		{
			consoleLogger->Open();
		}
	}

	void Logging::SetFileFormat(const std::string_view& format)
	{
		SetFormat(format, LogDestination::FILE);
	}

	void Logging::SetConsoleFormat(const std::string_view& format)
	{
		SetFormat(format, LogDestination::CONSOLE);
	}

	void Logging::SetFormat(const std::string_view& format, LogDestination logDest)
	{
		const auto& dest = destinations.at(logDest);
		dest->SetFormat(format);
	}

	constexpr void Logging::SetCacheMode(const bool enable) noexcept
	{
		if (cacheMode == enable)
			return;

		cacheMode = enable;

		if (!enable)
			Flush();
	}

	void Logging::ChangeOutputPath(const std::string_view& dir, const std::string_view& filename)
	{
		ChangeOutputDirectory(dir);
		ChangeFilename(filename);
	}

	void Logging::ChangeOutputDirectory(const std::string_view& directory)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(LogDestination::FILE));
		fLogger.SetDirectory(directory);
	}

	void Logging::ChangeFilename(const std::string_view& filename)
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(LogDestination::FILE));
		fLogger.SetFileName(filename);
	}

	std::string Logging::GetOutputPath() const
	{
		auto& fLogger = type_trait::ToImpl<FileLogger>(destinations.at(LogDestination::FILE));
		const auto path = std::string(fLogger.GetPath());
		return path;
	}

	void Logging::SuspendFileLogging()
	{
		Close();
		SetCacheMode(true);
	}

	void Logging::ResumeFileLogging()
	{
		SetCacheMode(false);
		Flush();
	}

	void Logging::AddFatal(const LogMessage& msg)
	{
		Open();
		AddEntry(LogLevel::FTL, msg);
	}

	void Logging::AddRaw(const std::string_view& text)
	{
		AddLog(LogEntry(text.data(), LogDescriptor(LogLevel::RAW)));
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

	void Logging::AddBanner(const std::string_view& descriptor, const LogMessage& message, const std::string_view& frontPadding
		, const std::string_view& backPadding, const std::uint16_t paddingCount)
	{
		if (!isEnabled)
			return;

		std::string front, back;
		for (auto i = 0; i < paddingCount; ++i)
		{
			front.append(frontPadding);
			back.append(backPadding);
		}

		const auto text = ToString<char>(NoFormatTag{} 
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
			const auto& entry = entriesQ.front();
			for (auto& dest : destinations)
			{
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

	bool Logging::HasCache() const noexcept
	{
		return !entriesQ.empty();
	}

	const LogEntry& Logging::GetLastCachedEntry() const
	{
		if (entriesQ.empty() || !cacheMode)
			throw std::runtime_error(name + " log cache is empty");

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
