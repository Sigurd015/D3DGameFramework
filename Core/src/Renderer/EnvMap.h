#pragma once
#include "Texture.h"

struct EnvMap
{
	TextureCube RadianceMap;
	TextureCube IrradianceMap;
};