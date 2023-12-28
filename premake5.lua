workspace "D3DGames"
	architecture "x86_64"
	startproject "dxCraft"

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

outputdir = "%{wks.location}/Binaries/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
intdir = "%{wks.location}/Binaries/Intermediates/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	
workingdir = "%{wks.location}"

IncludeDir = {}
IncludeDir["DirectXTK"] = "%{wks.location}/Core/vendor/DirectXTK_Desktop_2022_Win10/include"
LibraryDir = {}
LibraryDir["DirectXTK"] = "%{wks.location}/Core/vendor/DirectXTK_Desktop_2022_Win10/lib"
Library = {}
Library["DirectXTK_Debug"] = "%{LibraryDir.DirectXTK}/Debug/DirectXTK.lib"
Library["DirectXTK_Release"] = "%{LibraryDir.DirectXTK}/Release/DirectXTK.lib"

include "Core"
include "Gunslayer"
include "dxCraft"