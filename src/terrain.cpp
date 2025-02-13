#include "core.h"
#include "terrain.h"

ITerrainSceneNode *LoadTerrain(const char *heightmap, ISceneNode *parent, float width, float height, float depth)
{
  ITerrainSceneNode *terrain = _Device()->getSceneManager()->addTerrainSceneNode(heightmap, parent);
  terrain->setScale(vector3df(
      width / terrain->getBoundingBox().getExtent().X,
      height / terrain->getBoundingBox().getExtent().Y,
      depth / terrain->getBoundingBox().getExtent().Z));
  return terrain;
}

void ScaleTerrainTexture(ITerrainSceneNode *terrain, float scale1, float scale2)
{
  terrain->scaleTexture(scale1, scale2);
}
