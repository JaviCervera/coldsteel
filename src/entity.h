/** @file
 * An Entity represents a scene object that can be positioned, rotated, and scaled in the 3D
 * world. Cameras, models, lights, sprites, etc. are all Entities.
 */
#pragma once

#include "common.h"

// Collision types
#define COLLISION_NONE 0
#define COLLISION_BOX 1
#define COLLISION_MESH 2
#define COLLISION_OCTREE 3

// Entity types
#define ENTITY_EMPTY 0
#define ENTITY_SPRITE 1
#define ENTITY_CAMERA 2
#define ENTITY_PARTICLESYSTEM 3
#define ENTITY_LIGHT 4
#define ENTITY_MESH 5
#define ENTITY_MESH_OCTREE 6
#define ENTITY_TERRAIN 7
#define ENTITY_WATER 8

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a new empty entity.
   *
   * @return A new empty entity.
   */
  EXPORT ISceneNode *CALL CreateEntity();

  /**
   * Frees an entity and removes it from the scene.
   *
   * @param entity The entity to free.
   */
  EXPORT void CALL FreeEntity(ISceneNode *entity);

  /**
   * Returns the type of an entity.
   *
   * @param entity The entity to query.
   * @return The entity type as one of the ENTITY_* constants.
   */
  EXPORT int CALL EntityType(ISceneNode *entity);

  /**
   * Sets the name of an entity.
   *
   * @param entity The entity whose name will be set.
   * @param name The new entity name.
   */
  EXPORT void CALL SetEntityName(ISceneNode *entity, const char *name);

  /**
   * Returns the name of an entity.
   *
   * @param entity The entity to query.
   * @return The entity's name.
   */
  EXPORT const char *CALL EntityName(ISceneNode *entity);

  /**
   * Sets whether an entity is visible.
   *
   * @param entity The entity whose visibility will be set.
   * @param visible The new visibility state.
   */
  EXPORT void CALL SetEntityVisible(ISceneNode *entity, bool_t visible);

  /**
   * Returns whether an entity is visible.
   *
   * @param entity The entity to query.
   * @return True if the entity is visible.
   */
  EXPORT bool_t CALL EntityVisible(ISceneNode *entity);

  /**
   * Sets the parent of an entity.
   *
   * @param entity The entity whose parent will be set.
   * @param parent The new parent entity, or NULL for no parent.
   */
  EXPORT void CALL SetEntityParent(ISceneNode *entity, ISceneNode *parent);

  /**
   * Returns the parent of an entity.
   *
   * @param entity The entity to query.
   * @return The parent entity, or NULL if the entity has no parent.
   */
  EXPORT ISceneNode *CALL EntityParent(ISceneNode *entity);

  /**
   * Returns the number of child entities.
   *
   * @param entity The entity to query.
   * @return The number of child entities.
   */
  EXPORT int CALL EntityNumChildren(ISceneNode *entity);

  /**
   * Returns a child entity by index.
   *
   * @param entity The parent entity.
   * @param index The child index.
   * @return The child entity at the given index.
   */
  EXPORT ISceneNode *CALL EntityChild(ISceneNode *entity, int index);

  /**
   * Sets the world position of an entity.
   *
   * @param entity The entity whose position will be set.
   * @param x The world X position.
   * @param y The world Y position.
   * @param z The world Z position.
   */
  EXPORT void CALL SetEntityPosition(ISceneNode *entity, float x, float y, float z);

  /**
   * Moves an entity relative to its local axes.
   *
   * @param entity The entity to move.
   * @param x The local X movement.
   * @param y The local Y movement.
   * @param z The local Z movement.
   */
  EXPORT void CALL MoveEntity(ISceneNode *entity, float x, float y, float z);

  /**
   * Moves an entity with ellipsoid collision response.
   *
   * @param entity The entity to move.
   * @param x The local X movement.
   * @param y The local Y movement.
   * @param z The local Z movement.
   * @param radiusX The collision ellipsoid radius on the X axis.
   * @param radiusY The collision ellipsoid radius on the Y axis.
   * @param radiusZ The collision ellipsoid radius on the Z axis.
   * @param group The collision group to test against.
   * @return The entity collided with, or NULL if no collision occurred.
   */
  EXPORT ISceneNode *CALL SlideEntity(ISceneNode *entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group);

  /**
   * Translates an entity in world space.
   *
   * @param entity The entity to translate.
   * @param x The world X movement.
   * @param y The world Y movement.
   * @param z The world Z movement.
   */
  EXPORT void CALL TranslateEntity(ISceneNode *entity, float x, float y, float z);

  /**
   * Returns the world X position of an entity.
   *
   * @param entity The entity to query.
   * @return The world X position.
   */
  EXPORT float CALL EntityX(ISceneNode *entity);

  /**
   * Returns the world Y position of an entity.
   *
   * @param entity The entity to query.
   * @return The world Y position.
   */
  EXPORT float CALL EntityY(ISceneNode *entity);

  /**
   * Returns the world Z position of an entity.
   *
   * @param entity The entity to query.
   * @return The world Z position.
   */
  EXPORT float CALL EntityZ(ISceneNode *entity);

  /**
   * Returns the local X position of an entity.
   *
   * @param entity The entity to query.
   * @return The local X position relative to the parent.
   */
  EXPORT float CALL EntityLocalX(ISceneNode *entity);

  /**
   * Returns the local Y position of an entity.
   *
   * @param entity The entity to query.
   * @return The local Y position relative to the parent.
   */
  EXPORT float CALL EntityLocalY(ISceneNode *entity);

  /**
   * Returns the local Z position of an entity.
   *
   * @param entity The entity to query.
   * @return The local Z position relative to the parent.
   */
  EXPORT float CALL EntityLocalZ(ISceneNode *entity);

  /**
   * Sets the world rotation of an entity in degrees.
   *
   * @param entity The entity whose rotation will be set.
   * @param pitch The pitch in degrees.
   * @param yaw The yaw in degrees.
   * @param roll The roll in degrees.
   */
  EXPORT void CALL SetEntityRotation(ISceneNode *entity, float pitch, float yaw, float roll);

  /**
   * Rotates an entity relative to its local axes.
   *
   * @param entity The entity to rotate.
   * @param pitch The pitch change in degrees.
   * @param yaw The yaw change in degrees.
   * @param roll The roll change in degrees.
   */
  EXPORT void CALL TurnEntity(ISceneNode *entity, float pitch, float yaw, float roll);

  /**
   * Rotates an entity to point at a world position.
   *
   * @param entity The entity to rotate.
   * @param x The target world X position.
   * @param y The target world Y position.
   * @param z The target world Z position.
   */
  EXPORT void CALL PointEntity(ISceneNode *entity, float x, float y, float z);

  /**
   * Returns the pitch of an entity.
   *
   * @param entity The entity to query.
   * @return The pitch in degrees.
   */
  EXPORT float CALL EntityPitch(ISceneNode *entity);

  /**
   * Returns the yaw of an entity.
   *
   * @param entity The entity to query.
   * @return The yaw in degrees.
   */
  EXPORT float CALL EntityYaw(ISceneNode *entity);

  /**
   * Returns the roll of an entity.
   *
   * @param entity The entity to query.
   * @return The roll in degrees.
   */
  EXPORT float CALL EntityRoll(ISceneNode *entity);

  /**
   * Sets the scale of an entity.
   *
   * @param entity The entity whose scale will be set.
   * @param x The X scale.
   * @param y The Y scale.
   * @param z The Z scale.
   */
  EXPORT void CALL SetEntityScale(ISceneNode *entity, float x, float y, float z);

  /**
   * Returns the X scale of an entity.
   *
   * @param entity The entity to query.
   * @return The X scale.
   */
  EXPORT float CALL EntityScaleX(ISceneNode *entity);

  /**
   * Returns the Y scale of an entity.
   *
   * @param entity The entity to query.
   * @return The Y scale.
   */
  EXPORT float CALL EntityScaleY(ISceneNode *entity);

  /**
   * Returns the Z scale of an entity.
   *
   * @param entity The entity to query.
   * @return The Z scale.
   */
  EXPORT float CALL EntityScaleZ(ISceneNode *entity);

  /**
   * Returns the bounding box width of an entity.
   *
   * @param entity The entity to query.
   * @return The bounding box width in world units.
   */
  EXPORT float CALL EntityWidth(ISceneNode *entity);

  /**
   * Returns the bounding box height of an entity.
   *
   * @param entity The entity to query.
   * @return The bounding box height in world units.
   */
  EXPORT float CALL EntityHeight(ISceneNode *entity);

  /**
   * Returns the bounding box depth of an entity.
   *
   * @param entity The entity to query.
   * @return The bounding box depth in world units.
   */
  EXPORT float CALL EntityDepth(ISceneNode *entity);

  /**
   * Returns the number of materials on an entity.
   *
   * @param entity The entity to query.
   * @return The number of materials on the entity.
   */
  EXPORT int CALL EntityNumMaterials(ISceneNode *entity);

  /**
   * Returns a material on an entity by index.
   *
   * @param entity The entity to query.
   * @param index The material index.
   * @return The material at the given index.
   */
  EXPORT SMaterial *CALL EntityMaterial(ISceneNode *entity, int index);

  /**
   * Sets the collision type and group of an entity.
   *
   * @param entity The entity whose collision settings will be set.
   * @param type The collision type as one of the COLLISION_* constants.
   * @param group The collision group.
   */
  EXPORT void CALL SetEntityCollision(ISceneNode *entity, int type, int group);

  /**
   * Returns whether collision is enabled for an entity.
   *
   * @param entity The entity to query.
   * @return True if collision is enabled.
   */
  EXPORT bool_t CALL EntityCollision(ISceneNode *entity);

  /**
   * Returns the collision group of an entity.
   *
   * @param entity The entity to query.
   * @return The collision group.
   */
  EXPORT int CALL EntityGroup(ISceneNode *entity);

  /**
   * Returns the distance from an entity to a world position.
   *
   * @param entity The entity to query.
   * @param x The target world X position.
   * @param y The target world Y position.
   * @param z The target world Z position.
   * @return The distance to the given world position.
   */
  EXPORT float CALL EntityDistance(ISceneNode *entity, float x, float y, float z);

  /**
   * Returns the squared distance from an entity to a world position.
   *
   * @param entity The entity to query.
   * @param x The target world X position.
   * @param y The target world Y position.
   * @param z The target world Z position.
   * @return The squared distance to the given world position.
   */
  EXPORT float CALL EntitySquareDistance(ISceneNode *entity, float x, float y, float z);

#ifdef __cplusplus
} /* extern "C" */
#endif
