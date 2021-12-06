project "kTests"
    location "."
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
    
	staticruntime "on"

    targetdir ("bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDir .. "/%{prj.name}")

    files
    {
        ".\\**.hpp",
        ".\\**.cpp",
        "..\\cpp.hint"
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


        