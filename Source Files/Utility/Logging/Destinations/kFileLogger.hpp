#pragma once

#include "kiLoggerDestination.hpp"
#include <filesystem>
#include <fstream>
#include <mutex>

namespace klib
{
	namespace kLogs
	{
		class FileLogger final : public iLoggerDestination
		{
		public:
			FileLogger(const std::string_view& newName, const std::filesystem::path& path);
			~FileLogger() noexcept;

			USE_RESULT std::string_view GetName() const override;
			void SetName(const std::string_view& newName) override;

			USE_RESULT std::string_view GetFileName() const;
			void SetFileName(const std::string_view& newFilename);

			USE_RESULT std::string_view GetExtension() const;
			void SetExtension(const std::string_view& newExtension);

			USE_RESULT std::string_view GetDirectory() const;
			void SetDirectory(const std::string_view& newDir);

			USE_RESULT std::string_view GetPath() const;
			void SetPath(const std::filesystem::path& path);
			
			void OutputInitialized(const std::string_view& openingMsg) override;
			void AddEntry(const LogEntry& entry) override;

			bool Open() override;

			bool IsOpen() override;

			void Close(const bool outputClosingMsg) override;

		private:
			void Flush(const std::string_view& msg);
			std::string CreateLogText(const LogEntry& entry) const;
			
		private:
			std::string name;

			std::filesystem::path path;
			std::fstream fileStream;
			std::mutex lock;
		};
		
	}
}
