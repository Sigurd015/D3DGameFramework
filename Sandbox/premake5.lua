project "Sandbox"
	language "C"
	staticruntime "on"

	targetdir (outputdir .. "/%{prj.name}")
	objdir (intdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hpp",
		"src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Core/src",
		"src",
	}

	links
	{
		"Core",
	}

	debugdir (workingdir)

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		kind "ConsoleApp"
		defines "CORE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		kind "ConsoleApp"
		defines "CORE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		kind "WindowedApp"
		defines "CORE_DIST"
		runtime "Release"
		optimize "on"