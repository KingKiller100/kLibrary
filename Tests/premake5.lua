project "kTests"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    characterset ("default")
    
	staticruntime "off"

    targetdir ("../bin/" .. OutputDir .. "/kLibrary")
	objdir ("../bin-int/" .. OutputDir .. "/kLibrary")

    files
    {
        ".\\**.hpp",
        ".\\**.cpp",
        "..\\cpp.hint"
    }
    links "kLibrary"

    filter "system:Windows"
        systemversion "latest"

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "KLIB_LIB",
            "KLIB_WINDOWS_OS",
            "MSVC_PLATFORM_TOOLSET=$(PlatformToolsetVersion)"
        }


        