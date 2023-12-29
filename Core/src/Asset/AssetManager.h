#pragma once

void AssetManager_Init();
void AssetManager_Shutdown();

// You can pass in a path to a texture, mesh, or sound, 
// alos you can pass in a shader name, and set isMemoryAsset to true
// TODO: Better shader management
void* AssetManager_GetAsset(const char* assetPath, bool isMemoryAsset = false);