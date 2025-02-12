#include "core.h"
#include "entity.h"
#include "math.h"

static map<ITriangleSelector *, int> _colliders;

ISceneNode *CreateEntity()
{
  return _Device()->getSceneManager()->addEmptySceneNode();
}

void FreeEntity(ISceneNode *entity)
{
  SetEntityCollision(entity, COLLISION_NONE, 0);
  entity->remove();
}

int EntityType(ISceneNode *entity)
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

void SetEntityName(ISceneNode *entity, const char *name)
{
  entity->setName(name);
}

const char *EntityName(ISceneNode *entity)
{
  return entity->getName();
}

void SetEntityVisible(ISceneNode *entity, bool_t visible)
{
  entity->setVisible(visible);
}

bool_t EntityVisible(ISceneNode *entity)
{
  return entity->isVisible();
}

void SetEntityParent(ISceneNode *entity, ISceneNode *parent)
{
  entity->setParent(parent);
}

ISceneNode *EntityParent(ISceneNode *entity)
{
  return entity->getParent();
}

int EntityNumChildren(ISceneNode *entity)
{
  return entity->getChildren().size();
}

ISceneNode *EntityChild(ISceneNode *entity, int index)
{
  return *(entity->getChildren().begin() + index);
}

void SetEntityPosition(ISceneNode *entity, float x, float y, float z)
{
  entity->setPosition(vector3df(x, y, z));
  entity->updateAbsolutePosition();
}

void MoveEntity(ISceneNode *entity, float x, float y, float z)
{
  vector3df dest(x, y, z);
  entity->getRelativeTransformation().transformVect(dest);
  entity->setPosition(dest);
  entity->updateAbsolutePosition();
}

ISceneNode *SlideEntity(ISceneNode *entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group)
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

float EntityX(ISceneNode *entity)
{
  return entity->getAbsolutePosition().X;
}

float EntityY(ISceneNode *entity)
{
  return entity->getAbsolutePosition().Y;
}

float EntityZ(ISceneNode *entity)
{
  return entity->getAbsolutePosition().Z;
}

float EntityLocalX(ISceneNode *entity)
{
  return entity->getPosition().X;
}

float EntityLocalY(ISceneNode *entity)
{
  return entity->getPosition().Y;
}

float EntityLocalZ(ISceneNode *entity)
{
  return entity->getPosition().Z;
}

void SetEntityRotation(ISceneNode *entity, float pitch, float yaw, float roll)
{
  entity->setRotation(vector3df(pitch, yaw, roll));
}

void TurnEntity(ISceneNode *entity, float pitch, float yaw, float roll)
{
  entity->setRotation(entity->getRotation() + vector3df(pitch, yaw, roll));
}

void PointEntity(ISceneNode *entity, float x, float y, float z)
{
  const float xdiff = EntityX(entity) - x;
  const float ydiff = EntityY(entity) - y;
  const float zdiff = EntityZ(entity) - z;
  const float sqdist = Sqr(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
  const float pitch = ATan2(ydiff, sqdist);
  const float yaw = ATan2(xdiff, -zdiff);
  SetEntityRotation(entity, pitch, yaw, EntityRoll(entity));
}

float EntityPitch(ISceneNode *entity)
{
  return entity->getRotation().X;
}

float EntityYaw(ISceneNode *entity)
{
  return entity->getRotation().Y;
}

float EntityRoll(ISceneNode *entity)
{
  return entity->getRotation().Z;
}

void SetEntityScale(ISceneNode *entity, float x, float y, float z)
{
  entity->setScale(vector3df(x, y, z));
}

float EntityScaleX(ISceneNode *entity)
{
  return entity->getScale().X;
}

float EntityScaleY(ISceneNode *entity)
{
  return entity->getScale().Y;
}

float EntityScaleZ(ISceneNode *entity)
{
  return entity->getScale().Z;
}

float EntityWidth(ISceneNode *entity)
{
  return entity->getBoundingBox().getExtent().X;
}

float EntityHeight(ISceneNode *entity)
{
  return entity->getBoundingBox().getExtent().Y;
}

float EntityDepth(ISceneNode *entity)
{
  return entity->getBoundingBox().getExtent().Z;
}

int EntityNumMaterials(ISceneNode *entity)
{
  return entity->getMaterialCount();
}

SMaterial *EntityMaterial(ISceneNode *entity, int index)
{
  return &entity->getMaterial(index);
}

void SetEntityCollision(ISceneNode *entity, int type, int group)
{
  ITriangleSelector *selector = NULL;
  IAnimatedMeshSceneNode *animNode = (entity->getType() == ESNT_ANIMATED_MESH)
                                         ? (IAnimatedMeshSceneNode *)entity
                                         : NULL;
  IMeshSceneNode *meshNode = (entity->getType() == ESNT_MESH || entity->getType() == ESNT_OCTREE)
                                 ? (IMeshSceneNode *)entity
                                 : NULL;
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
    selector =
        animNode ? _Device()->getSceneManager()->createTriangleSelector(animNode) : meshNode ? _Device()->getSceneManager()->createTriangleSelector(meshNode->getMesh(), meshNode)
                                                                                             : NULL;
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

bool_t EntityCollision(ISceneNode *entity)
{
  return entity->getTriangleSelector() != NULL;
}

int EntityGroup(ISceneNode *entity)
{
  return entity->getID();
}

float EntityDistance(ISceneNode *entity, float x, float y, float z)
{
  return entity->getAbsolutePosition().getDistanceFrom(vector3df(x, y, z));
}

float EntityDistanceSquare(ISceneNode *entity, float x, float y, float z)
{
  return entity->getAbsolutePosition().getDistanceFromSQ(vector3df(x, y, z));
}
