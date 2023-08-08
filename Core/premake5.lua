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
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
	}

	includedirs
	{
		"src",
		"%{IncludeDir.DirectXTK}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"d3d11.lib",
		"D3DCompiler.lib",
		"d2d1.lib",
		"dwrite.lib",
	}

	filter "files:vendor/stb_image/**.cpp or vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "CORE_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"%{Library.DirectXTK_Debug}"
		}

	filter "configurations:Release"
		defines "CORE_RELEASE"	
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.DirectXTK_Release}"
		}


	filter "configurations:Dist"
		defines "CORE_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.DirectXTK_Release}"
		}