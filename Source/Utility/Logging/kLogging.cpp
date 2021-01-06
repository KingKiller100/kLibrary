#include "pch.hpp"
#include "kLogging.hpp"

#include "kLogMessage.hpp"
#include "kLogDescriptor.hpp"

#include "../../Template/ToImpl.hpp"
#include "../String/kToString.hpp"
#include "../FileSystem/kFileSystem.hpp"


namespace klib::kLogs
{
	using namespace kString;
	using namespace kCalendar;
	using namespace kTemplate;

	Logging::Logging(const std::string_view& directory
		, const std::string_view& filename
		, const std::string_view& extension
		, const std::string_view& name)
		: Logging(std::filesystem::path(
			ToString<char>(NoFormatTag{}, directory, klib::kFileSystem::AppendFileExtension(filename, extension)))
			, name)
	{
	}

	Logging::Logging(const std::filesystem::path& path, const std::string_view& name)
		: minimumLoggingLevel(LogLevel::DBG)
		, name(name)
		, isEnabled(false)
	{
		Initialize(path);
	}

	Logging::~Logging()
	{
		if (!entriesCache.empty())
			FinalOutput(false);
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

	FileLogger& Logging::GetFile()
	{
		return ToImpl<FileLogger>(destinations.at(LogDestination::FILE));
	}

	const FileLogger& Logging::GetFile() const
	{
		return ToImpl<FileLogger>(destinations.at(LogDestination::FILE));
	}

	ConsoleLogger& Logging::GetConsole()
	{
		return ToImpl<ConsoleLogger>(destinations.at(LogDestination::CONSOLE));
	}

	const ConsoleLogger& Logging::GetConsole() const
	{
		return ToImpl<ConsoleLogger>(destinations.at(LogDestination::CONSOLE));
	}

	void Logging::SetCacheMode(const bool enable) noexcept
	{
		if (enable)
			Close(false);
		else
			Open();
	}

	void Logging::AddFatal(const LogMessage& msg)
	{
		auto& file = destinations.at(LogDestination::FILE);
		if (!file->IsOpen())
			file->Open();

		AddEntry(LogLevel::FTL, msg);
		FinalOutput(false);
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
		entriesCache.push_back(entry);
		if (GetFile().IsOpen() || GetConsole().IsOpen())
			Flush();
	}

	void Logging::FinalOutput(bool outputDefaultClosingMsg)
	{
		Close(outputDefaultClosingMsg);
		isEnabled = false;
	}

	void Logging::Flush()
	{
		while (!entriesCache.empty())
		{
			const auto& entry = entriesCache.front();
			for (auto& dest : destinations)
			{
				dest.second->AddEntry(entry);
			}
			entriesCache.pop_front();
		}
	}

	void Logging::Close(bool outputDefaultClosingMsg)
	{
		Flush();
		for (auto& dest : destinations)
		{
			dest.second->Close(outputDefaultClosingMsg);
		}
	}

	bool Logging::IsLoggable(const LogLevel lvl) const
	{
		return (lvl >= minimumLoggingLevel);
	}

	bool Logging::HasCache() const noexcept
	{
		return !entriesCache.empty();
	}

	const LogEntry& Logging::GetLastCachedEntry() const
	{
		if (entriesCache.empty())
			throw std::runtime_error(name + " log cache is empty");

		const auto& lastLog = entriesCache.back();

		return lastLog;
	}

	void Logging::ClearCache()
	{
		if (!entriesCache.empty())
			entriesCache.clear();
	}

	bool Logging::ErasePrevious(size_t count)
	{
		if (entriesCache.empty())
			return false;

		while (count-- != 0)
		{
			entriesCache.pop_back();
		}

		return true;
	}
}
