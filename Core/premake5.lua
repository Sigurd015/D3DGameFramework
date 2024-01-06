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
		"Resources/Shaders/**.hlsl"
	}

	filter { "files:**.hlsl" }
		flags "ExcludeFromBuild"
		shadermodel "5.0"
 	filter { "files:**_p.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Pixel"
 	filter { "files:**_v.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Vertex"
	filter { "files:**_g.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Geometry"
	filter { "files:**_c.hlsl" }
		removeflags "ExcludeFromBuild"
		shadertype "Compute"
	filter {}

	postbuildcommands 
	{
		'{COPY} "%{cfg.targetdir}"/*.cso %{wks.location}/assets/shaders/cache/'
	}

	includedirs
	{
		"src",
		"%{IncludeDir.DirectXTK}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"d3d11.lib",
		"d3dCompiler.lib",
		"d2d1.lib",
		"dwrite.lib",
		"dxguid.lib",
	}

	filter "files:vendor/stb_image/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "CORE_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"%{Library.DirectXTK_Debug}",
			"%{Library.assimp_Debug}",
			"%{Library.ImGui_Debug}"
		}

		postbuildcommands 
		{
			'{COPY} "%{LibraryDir.assimp_Debug}"/assimp-vc143-mtd.dll %{wks.location}'
		}

	filter "configurations:Release"
		defines "CORE_RELEASE"	
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.DirectXTK_Release}",
			"%{Library.assimp_Release}",
			"%{Library.ImGui_Release}"
		}

		postbuildcommands 
		{
			'{COPY} "%{LibraryDir.assimp_Release}"/assimp-vc143-mt.dll %{wks.location}'
		}

	filter "configurations:Dist"
		defines "CORE_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.DirectXTK_Release}",
			"%{Library.assimp_Release}",
			"%{Library.ImGui_Release}"
		}

		postbuildcommands 
		{
			'{COPY} "%{LibraryDir.assimp_Release}"/assimp-vc143-mt.dll %{wks.location}'
		}