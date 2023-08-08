workspace "Gunslayer"
	architecture "x86_64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{wks.location}/build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
intdir = "%{wks.location}/build/int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["DirectXTK"] = "%{wks.location}/Core/vendor/DirectXTK_Desktop_2022_Win10/include"
LibraryDir = {}
LibraryDir["DirectXTK"] = "%{wks.location}/Core/vendor/DirectXTK_Desktop_2022_Win10/lib"
Library = {}
Library["DirectXTK_Debug"] = "%{LibraryDir.DirectXTK}/Debug/DirectXTK.lib"
Library["DirectXTK_Release"] = "%{LibraryDir.DirectXTK}/Release/DirectXTK.lib"
IncludeDir["stb_image"] = "%{wks.location}/Core/vendor/stb_image"

include "Core"
include "Game"