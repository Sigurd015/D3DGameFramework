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
IncludeDir["stb_image"] = "%{wks.location}/Core/vendor/stb_image"
IncludeDir["assimp"] = "%{wks.location}/Core/vendor/assimp/include"
LibraryDir = {}
LibraryDir["DirectXTK"] = "%{wks.location}/Core/vendor/DirectXTK_Desktop_2022_Win10/lib"
LibraryDir["assimp_Debug"] = "%{wks.location}/Core/vendor/assimp/bin/Debug"
LibraryDir["assimp_Release"] = "%{wks.location}/Core/vendor/assimp/bin/Release"
Library = {}
Library["DirectXTK_Debug"] = "%{LibraryDir.DirectXTK}/Debug/DirectXTK.lib"
Library["DirectXTK_Release"] = "%{LibraryDir.DirectXTK}/Release/DirectXTK.lib"
Library["assimp_Debug"] = "%{LibraryDir.assimp_Debug}/assimp-vc143-mtd.lib"
Library["assimp_Release"] = "%{LibraryDir.assimp_Release}/assimp-vc143-mt.lib"

include "Core"
include "Gunslayer"
include "dxCraft"