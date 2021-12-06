#pragma once

#include "../../../HelperMacros.hpp"
#include "kiLoggerDestination.hpp"
#include <filesystem>
#include <fstream>
#include <mutex>


namespace klib
{
	namespace kLogs
	{
		class LogProfile;
		struct LogMessage;

		class FileLogger final : public FormattedLogDestinationBase
		{
		public:
			FileLogger( const std::filesystem::path& path );
			~FileLogger() noexcept;

			USE_RESULT std::string_view GetName() const override;

			USE_RESULT std::string GetFileName() const;
			void SetFileName( const std::string_view& newFilename );

			USE_RESULT std::string GetExtension() const;
			void SetExtension( const std::string_view& newExtension );

			USE_RESULT std::string GetDirectory() const;
			void SetDirectory( const std::filesystem::path& newDir );

			USE_RESULT std::filesystem::path GetPath() const;
			void SetPath( const std::filesystem::path& path );

			void AddEntry( const LogEntry& entry ) override;

			void AddRaw( const LogMessage& text ) override;

			bool Move( const std::filesystem::path& path );

			void Open() override;

			bool IsOpen() const override;

			void Close() override;

		private:
			void Flush( const std::string_view& msg );
			std::string CreateLogText( const LogEntry& entry ) const;
			std::string CreateRawLogText( const LogMessage& msg ) const;

		private:
			std::filesystem::path path;
			std::fstream fileStream;
			std::mutex lock;
		};
	}
}
