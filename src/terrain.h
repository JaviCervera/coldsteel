#pragma once

#include "common.h"

ITerrainSceneNode *LoadTerrain(const char *heightmap, ISceneNode *parent, float width, float height, float depth);
void ScaleTerrainTexture(ITerrainSceneNode *terrain, float scale1, float scale2);
