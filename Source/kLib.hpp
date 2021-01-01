#pragma once

// Maths Library Includes
#include "Maths/kMaths.hpp"

// Simple Type Traits Include
#include "TypeTraits/BooleanTraits.hpp"
#include "TypeTraits/CharacterTraits.hpp"
#include "TypeTraits/ConditionalReference.hpp"
#include "TypeTraits/Constexpr.hpp"
#include "TypeTraits/CustomTraits.hpp"
#include "TypeTraits/FloatTraits.hpp"
#include "TypeTraits/SmartPointerTraits.hpp"
#include "TypeTraits/TemplateTraits.hpp"
#include "TypeTraits/StringTraits.hpp"
#include "TypeTraits/ToImpl.hpp"

// Util Includes

// Stopwatch
#include "Utility/Stopwatch/Clock/kClock.hpp"
#include "Utility/Stopwatch/kTimeSpan.hpp"
#include "Utility/Stopwatch/kStopwatch.hpp"

// Calendar
#include "Utility/Calendar/Time/kTime.hpp"
#include "Utility/Calendar/Date/kDate.hpp"
#include "Utility/Calendar/kCalendar.hpp"
#include "Utility/Calendar/kUseCalendarSourceInfo.hpp"

// Debug
#include "Utility/Debug/kAssert.hpp"
#include "Utility/Debug/kDebugger.hpp"

// Source code information
#include "Utility/Debug/Source/kSourceInfo.hpp"
#include "Utility/Debug/Source/kSourceInfoToString.hpp"
#include "Utility/Debug/Source/kMutableSourceInfo.hpp"
#include "Utility/Debug/Source/kMutableSourceInfoToString.hpp"

// File System
#include "Utility/FileSystem/kFileSystem.hpp"
#include "Utility/FileSystem/kFileSystemToString.hpp"

// Enum
#include "Utility/Enum/kEnum.hpp"

// Logging
#include "Utility/Logging/kLogLevel.hpp"
#include "Utility/Logging/kLogEntry.hpp"
#include "Utility/Logging/kLogging.hpp"

// Localization
#include "Utility/Localization/kLocale.hpp"

// Miscellaneous
#include "Utility/Misc/kConsoleColour.hpp"
#include "Utility/Misc/kNumberSuffix.hpp"

// Thread
#include "Utility/Thread/kThreadID.hpp"
#include "Utility/Thread/kThreadPool.hpp"

// Profiler
#include "Utility/Profiler/kProfiler.hpp"

// String
#include "Utility/String/kStringConverter.hpp"
#include "Utility/String/kStringTricks.hpp"
#include "Utility/String/kStringTypes.hpp"
#include "Utility/String/kToString.hpp"
#include "Utility/String/kStringView.hpp"
#include "Utility/String/kStringLite.hpp"
#include "Utility/String/kSprintf.hpp"

// Platform OS
#include "Platform/kPlatform.hpp"

// Templates
#include "Template/ByteStorager.hpp"

// Testing
#include "Testing/kAddTest.hpp"
#include "Testing/TesterBase.hpp"
#include "Testing/TesterManager.hpp"

// Macros
#include "HelperMacros.hpp"