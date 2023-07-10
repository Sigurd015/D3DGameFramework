project "Core"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir (outputdir .. "/%{prj.name}")
	objdir (intdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
	}

	links
	{
		"d3d11.lib",
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BM_RELEASE"	
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BM_DIST"
		runtime "Release"
		optimize "on"