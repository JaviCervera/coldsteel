#include "core.h"
#include "entity.h"
#include "math.h"

static map<ITriangleSelector *, int> _colliders;

extern "C"
{

  EXPORT ISceneNode *CALL CreateEntity()
  {
    return _Device()->getSceneManager()->addEmptySceneNode();
  }

  EXPORT void CALL FreeEntity(ISceneNode *entity)
  {
    SetEntityCollision(entity, COLLISION_NONE, 0);
    entity->remove();
  }

  EXPORT int CALL EntityType(ISceneNode *entity)
  {
    map<ESCENE_NODE_TYPE, int> types;
    types.set(ESNT_EMPTY, ENTITY_EMPTY);
    types.set(ESNT_BILLBOARD, ENTITY_SPRITE);
    types.set(ESNT_CAMERA, ENTITY_CAMERA);
    types.set(ESNT_PARTICLE_SYSTEM, ENTITY_EMITTER);
    types.set(ESNT_LIGHT, ENTITY_LIGHT);
    types.set(ESNT_MESH, ENTITY_MESH);
    types.set(ESNT_ANIMATED_MESH, ENTITY_MESH);
    types.set(ESNT_OCTREE, ENTITY_MESH_OCTREE);
    types.set(ESNT_TERRAIN, ENTITY_TERRAIN);
    types.set(ESNT_WATER_SURFACE, ENTITY_WATER);
    return types[entity->getType()];
  }

  EXPORT void CALL SetEntityName(ISceneNode *entity, const char *name)
  {
    entity->setName(name);
  }

  EXPORT const char *CALL EntityName(ISceneNode *entity)
  {
    return entity->getName();
  }

  EXPORT void CALL SetEntityVisible(ISceneNode *entity, bool_t visible)
  {
    entity->setVisible(visible);
  }

  EXPORT bool_t CALL EntityVisible(ISceneNode *entity)
  {
    return entity->isVisible();
  }

  EXPORT void CALL SetEntityParent(ISceneNode *entity, ISceneNode *parent)
  {
    entity->setParent(parent);
  }

  EXPORT ISceneNode *CALL EntityParent(ISceneNode *entity)
  {
    return entity->getParent();
  }

  EXPORT int CALL EntityNumChildren(ISceneNode *entity)
  {
    return entity->getChildren().size();
  }

  EXPORT ISceneNode *CALL EntityChild(ISceneNode *entity, int index)
  {
    return *(entity->getChildren().begin() + index);
  }

  EXPORT void CALL SetEntityPosition(ISceneNode *entity, float x, float y, float z)
  {
    entity->setPosition(vector3df(x, y, z));
    entity->updateAbsolutePosition();
  }

  EXPORT void CALL MoveEntity(ISceneNode *entity, float x, float y, float z)
  {
    vector3df dest(x, y, z);
    entity->getRelativeTransformation().transformVect(dest);
    entity->setPosition(dest);
    entity->updateAbsolutePosition();
  }

  EXPORT ISceneNode *CALL SlideEntity(ISceneNode *entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group)
  {
    ISceneCollisionManager *manager = _Device()->getSceneManager()->getSceneCollisionManager();
    vector3df attempted(x, y, z);
    entity->getAbsoluteTransformation().transformVect(attempted);
    ISceneNode *collidedEntity = NULL;
    for (map<ITriangleSelector *, int>::Iterator it = _colliders.getIterator(); !it.atEnd(); it++)
    {
      map<ITriangleSelector *, int>::Node *node = it.getNode();
      ITriangleSelector *selector = node->getKey();
      int selectorGroup = node->getValue();
      if ((group & selectorGroup) != 0 && selector != entity->getTriangleSelector())
      {
        const vector3df radius(radiusX, radiusY, radiusZ);
        vector3df direction(x, y, z);
        matrix4()
            .setRotationDegrees(entity->getAbsoluteTransformation().getRotationDegrees())
            .transformVect(direction);
        triangle3df collisionTriangle;
        vector3df collisionPoint;
        bool collisionFalling = false;
        const vector3df result = manager->getCollisionResultPosition(
            selector, entity->getAbsolutePosition(),
            radius,
            direction,
            collisionTriangle,
            collisionPoint,
            collisionFalling,
            collidedEntity);
        if (collidedEntity != NULL)
        {
          const vector3df normal = collisionTriangle.getNormal().normalize();
          SetEntityPosition(entity, result.X, result.Y, result.Z);
          _SetPoint(collisionPoint.X, collisionPoint.Y, collisionPoint.Z);
          _SetNormal(normal.X, normal.Y, normal.Z);
          break;
        }
      }
    }
    if (collidedEntity == NULL)
      MoveEntity(entity, x, y, z);
    return collidedEntity;
  }

  EXPORT float CALL EntityX(ISceneNode *entity)
  {
    return entity->getAbsolutePosition().X;
  }

  EXPORT float CALL EntityY(ISceneNode *entity)
  {
    return entity->getAbsolutePosition().Y;
  }

  EXPORT float CALL EntityZ(ISceneNode *entity)
  {
    return entity->getAbsolutePosition().Z;
  }

  EXPORT float CALL EntityLocalX(ISceneNode *entity)
  {
    return entity->getPosition().X;
  }

  EXPORT float CALL EntityLocalY(ISceneNode *entity)
  {
    return entity->getPosition().Y;
  }

  EXPORT float CALL EntityLocalZ(ISceneNode *entity)
  {
    return entity->getPosition().Z;
  }

  EXPORT void CALL SetEntityRotation(ISceneNode *entity, float pitch, float yaw, float roll)
  {
    entity->setRotation(vector3df(pitch, yaw, roll));
  }

  EXPORT void CALL TurnEntity(ISceneNode *entity, float pitch, float yaw, float roll)
  {
    entity->setRotation(entity->getRotation() + vector3df(pitch, yaw, roll));
  }

  EXPORT void CALL PointEntity(ISceneNode *entity, float x, float y, float z)
  {
    const float xdiff = EntityX(entity) - x;
    const float ydiff = EntityY(entity) - y;
    const float zdiff = EntityZ(entity) - z;
    const float sqdist = Sqr(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
    const float pitch = ATan2(ydiff, sqdist);
    const float yaw = ATan2(xdiff, -zdiff);
    SetEntityRotation(entity, pitch, yaw, EntityRoll(entity));
  }

  EXPORT float CALL EntityPitch(ISceneNode *entity)
  {
    return entity->getRotation().X;
  }

  EXPORT float CALL EntityYaw(ISceneNode *entity)
  {
    return entity->getRotation().Y;
  }

  EXPORT float CALL EntityRoll(ISceneNode *entity)
  {
    return entity->getRotation().Z;
  }

  EXPORT void CALL SetEntityScale(ISceneNode *entity, float x, float y, float z)
  {
    entity->setScale(vector3df(x, y, z));
  }

  EXPORT float CALL EntityScaleX(ISceneNode *entity)
  {
    return entity->getScale().X;
  }

  EXPORT float CALL EntityScaleY(ISceneNode *entity)
  {
    return entity->getScale().Y;
  }

  EXPORT float CALL EntityScaleZ(ISceneNode *entity)
  {
    return entity->getScale().Z;
  }

  EXPORT float CALL EntityWidth(ISceneNode *entity)
  {
    return entity->getBoundingBox().getExtent().X;
  }

  EXPORT float CALL EntityHeight(ISceneNode *entity)
  {
    return entity->getBoundingBox().getExtent().Y;
  }

  EXPORT float CALL EntityDepth(ISceneNode *entity)
  {
    return entity->getBoundingBox().getExtent().Z;
  }

  EXPORT int CALL EntityNumMaterials(ISceneNode *entity)
  {
    return entity->getMaterialCount();
  }

  EXPORT SMaterial *CALL EntityMaterial(ISceneNode *entity, int index)
  {
    return &entity->getMaterial(index);
  }

  EXPORT void CALL SetEntityCollision(ISceneNode *entity, int type, int group)
  {
    ITriangleSelector *selector = NULL;
    IAnimatedMeshSceneNode *animNode = (entity->getType() == ESNT_ANIMATED_MESH) ? (IAnimatedMeshSceneNode *)entity : NULL;
    IMeshSceneNode *meshNode = (entity->getType() == ESNT_MESH || entity->getType() == ESNT_OCTREE) ? (IMeshSceneNode *)entity : NULL;
    ITerrainSceneNode *terrainNode = (entity->getType() == ESNT_TERRAIN) ? (ITerrainSceneNode *)entity : NULL;
    if (entity->getTriangleSelector())
    {
      _colliders.remove(entity->getTriangleSelector());
      entity->setTriangleSelector(NULL);
    }
    switch (type)
    {
    case COLLISION_BOX:
      selector = _Device()->getSceneManager()->createTriangleSelectorFromBoundingBox(entity);
      break;
    case COLLISION_MESH:
      if (animNode)
        selector = _Device()->getSceneManager()->createTriangleSelector(animNode);
      else if (meshNode)
        selector = _Device()->getSceneManager()->createTriangleSelector(meshNode->getMesh(), meshNode);
      else if (terrainNode)
        selector = _Device()->getSceneManager()->createTerrainTriangleSelector(terrainNode);
      break;
    case COLLISION_OCTREE:
      selector =
          animNode ? _Device()->getSceneManager()->createOctreeTriangleSelector(animNode->getMesh()->getMesh(0), animNode) : meshNode ? _Device()->getSceneManager()->createOctreeTriangleSelector(meshNode->getMesh(), meshNode)
                                                                                                                                      : NULL;
    }
    if (selector != NULL)
    {
      _colliders.set(selector, group);
      entity->setID(group);
      entity->setTriangleSelector(selector);
      selector->drop();
    }
  }

  EXPORT bool_t CALL EntityCollision(ISceneNode *entity)
  {
    return entity->getTriangleSelector() != NULL;
  }

  EXPORT int CALL EntityGroup(ISceneNode *entity)
  {
    return entity->getID();
  }

  EXPORT float CALL EntityDistance(ISceneNode *entity, float x, float y, float z)
  {
    return entity->getAbsolutePosition().getDistanceFrom(vector3df(x, y, z));
  }

  EXPORT float CALL EntityDistanceSquare(ISceneNode *entity, float x, float y, float z)
  {
    return entity->getAbsolutePosition().getDistanceFromSQ(vector3df(x, y, z));
  }

} // extern "C"
