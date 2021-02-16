#pragma once

// Maths Library Includes
#include "Maths/kMaths.hpp"

// Simple Type Traits Include
#include "TypeTraits/BooleanTraits.hpp"
#include "TypeTraits/CharacterTraits.hpp"
#include "TypeTraits/ConditionalReference.hpp"
#include "TypeTraits/Constexpr.hpp"
#include "TypeTraits/CustomTraits.hpp"
#include "TypeTraits/IntegerTraits.hpp"
#include "TypeTraits/FloatTraits.hpp"
#include "TypeTraits/SmartPointerTraits.hpp"
#include "TypeTraits/StringTraits.hpp"
#include "TypeTraits/TemplateTraits.hpp"


// Templates
#include "Template/kByteStorager.hpp"
#include "Template/kSimpleOperators.hpp"
#include "Template/kToImpl.hpp"
#include "Template/kTypeInfo.hpp"

// Testing
#include "Testing/kAddTest.hpp"
#include "Testing/TesterBase.hpp"
#include "Testing/TesterManager.hpp"

// Util Includes

// Platform OS
#include "Utility/Platform/kPlatform.hpp"

// Bits
#include "Utility/Bits/kBitTricks.hpp"

// Stopwatch
#include "Utility/Stopwatch/Clock/kClock.hpp"
#include "Utility/Stopwatch/kTimeSpan.hpp"
#include "Utility/Stopwatch/kStopwatch.hpp"

// Calendar
#include "Utility/Calendar/Time/kTime.hpp"
#include "Utility/Calendar/Date/kDate.hpp"
#include "Utility/Calendar/kCalendar.hpp"
#include "Utility/Calendar/kUseCalendarSourceInfo.hpp"
#include "Utility/Calendar/kCalendarToString.hpp"

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
#include "Utility/Enum/kEnumSuper.hpp"

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

// Macros
#include "HelperMacros.hpp"