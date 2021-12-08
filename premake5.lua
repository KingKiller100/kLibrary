project "kLibrary"
    location "."
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
	staticruntime "on"
    

    targetdir (BinDir)
	objdir (ObjDir)

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
        "Bcrypt.lib"
    }

    defines 
    {
        "KLIB_LIB"
    }

    filter "system:Windows"
        systemversion "latest"

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "KLIB_WINDOWS_OS",
            "MSVC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)"
        }

    filter "configurations:Debug"
        defines "KLIB_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "KLIB_RELEASE"
        optimize "Full"
        runtime "Release"

    filter "configurations:Profile"
        defines "KLIB_PROFILE"
        optimize "Speed"
        runtime "Release"


        