project "kTests"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
	staticruntime "on"
    

    targetdir (BinDir)
	objdir (ObjDir)

    files
    {
        ".\\**.hpp",
        ".\\**.cpp",
    }

    defines 
    {
        "KLIB_TEST"
    }
    
    links "kLibrary"

    filter "system:Windows"
        systemversion "latest"

        defines
        {
            "KLIB_LIB",
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

        