project "kLibrary"
    location "."
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
    -- characterset ("MBCS")
	staticruntime "on"

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "pch.hpp"
    pchsource "Source/pch.cpp"

    files
    {
        "Source/**.hpp",
        "Source/**.cpp",
        "cpp.hint"
    }

    links
    {
        "Bcrypt.lib",
    }

    filter "system:Windows"
        systemversion "latest"

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "KLIB_LIB",
            "KLIB_WINDOWS_OS",
            "MSVC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)"
        }

    filter "configurations:Debug"
        defines "KLIB_DEBUG"
        removefiles
        {
            "Source/**/*Test*",
        }
        symbols "On"
        runtime "Debug"

    filter "configurations:Test"
        defines "KLIB_TEST"
        symbols "On"
        runtime "Debug"
        
        files 
        {    
            "Tests/**.hpp",
            "Tests/**.cpp",
        }

    filter "configurations:Release"
        defines "KLIB_RELEASE"
        removefiles
        {
            "Source/**/*Test*",
        }
        optimize "Full"
        runtime "Release"

    filter "configurations:Profile"
        defines "KLIB_PROFILE"
        removefiles
        {
            "Source/**/*Test*",
        }
        optimize "Speed"
        runtime "Release"
        