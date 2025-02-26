#include "core.h"
#include "terrain.h"

extern "C"
{

  EXPORT ITerrainSceneNode *CALL LoadTerrain(const char *heightmap, ISceneNode *parent, float width, float height, float depth)
  {
    ITerrainSceneNode *terrain = _Device()->getSceneManager()->addTerrainSceneNode(heightmap, parent);
    terrain->setScale(vector3df(
        width / terrain->getBoundingBox().getExtent().X,
        height / terrain->getBoundingBox().getExtent().Y,
        depth / terrain->getBoundingBox().getExtent().Z));
    return terrain;
  }

  EXPORT void CALL ScaleTerrainTexture(ITerrainSceneNode *terrain, float scale1, float scale2)
  {
    terrain->scaleTexture(scale1, scale2);
  }

} // extern "C"
