/**
 * @file
 * A ParticleSystem is a scene node that emits and animates particles. Use emitters to define
 * the emission volume and affectors to modify particle behavior over time.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a particle system.
   *
   * The particle system is initially configured to clear all particles when hidden.
   *
   * @return A new particle system entity.
   */
  EXPORT IParticleSystemSceneNode *CALL CreateParticleSystem();

  /**
   * Sets the size of all particles in world units.
   *
   * @param ps The particle system.
   * @param width The particle width in world units.
   * @param height The particle height in world units.
   */
  EXPORT void CALL SetParticleSize(IParticleSystemSceneNode *ps, float width, float height);

  /**
   * Sets whether particles are affected by the node's transformation.
   *
   * When global (true), particles move with the node. When false, particles are
   * independent of the node's position/rotation.
   *
   * @param ps The particle system.
   * @param global True for global particles, false for local.
   */
  EXPORT void CALL SetParticlesAreGlobal(IParticleSystemSceneNode *ps, bool_t global);

  /**
   * Removes all currently visible particles.
   *
   * @param ps The particle system.
   */
  EXPORT void CALL ClearParticles(IParticleSystemSceneNode *ps);

  /**
   * Sets a point emitter, which emits particles from a single point.
   *
   * This replaces any existing emitter on the particle system.
   *
   * @param ps The particle system.
   */
  EXPORT void CALL SetPointEmitter(IParticleSystemSceneNode *ps);

  /**
   * Sets a box emitter, which emits particles from within a box volume.
   *
   * This replaces any existing emitter on the particle system.
   *
   * @param ps The particle system.
   * @param x The box center X position.
   * @param y The box center Y position.
   * @param z The box center Z position.
   * @param w The box width.
   * @param h The box height.
   * @param d The box depth.
   */
  EXPORT void CALL SetBoxEmitter(IParticleSystemSceneNode *ps, float x, float y, float z, float w, float h, float d);

  /**
   * Sets a sphere emitter, which emits particles from within a sphere volume.
   *
   * This replaces any existing emitter on the particle system.
   *
   * @param ps The particle system.
   * @param centerX The sphere center X position.
   * @param centerY The sphere center Y position.
   * @param centerZ The sphere center Z position.
   * @param radius The sphere radius.
   */
  EXPORT void CALL SetSphereEmitter(IParticleSystemSceneNode *ps, float centerX, float centerY, float centerZ, float radius);

  /**
   * Sets a mesh emitter, which emits particles from the vertices of a mesh.
   *
   * Supports both static and animated meshes. For animated meshes, particles emit from
   * the current animation frame.
   * This replaces any existing emitter on the particle system.
   *
   * @param ps The particle system.
   * @param entity The entity whose mesh to emit particles from (both static and animated meshes).
   * @param useNormalDirection If true, particles emit along vertex normals.
   * @param normalDirectionModifier Divisor for normal direction strength.
   * @param mbNumber Specific mesh buffer index, or -1 for random.
   * @param everyMeshVertex If true, emit from every vertex; if false, emit from random vertices.
   */
  EXPORT void CALL SetMeshEmitter(IParticleSystemSceneNode *ps, ISceneNode *entity,
      bool_t useNormalDirection, float normalDirectionModifier,
      int mbNumber, bool_t everyMeshVertex);

  /**
   * Sets the emitter direction and speed.
   *
   * @param ps The particle system.
   * @param x The direction X component.
   * @param y The direction Y component.
   * @param z The direction Z component.
   */
  EXPORT void CALL SetEmitterDirection(IParticleSystemSceneNode *ps, float x, float y, float z);

  /**
   * Returns the emitter direction X component.
   *
   * @param ps The particle system.
   * @return The direction X component.
   */
  EXPORT float CALL EmitterDirectionX(IParticleSystemSceneNode *ps);

  /**
   * Returns the emitter direction Y component.
   *
   * @param ps The particle system.
   * @return The direction Y component.
   */
  EXPORT float CALL EmitterDirectionY(IParticleSystemSceneNode *ps);

  /**
   * Returns the emitter direction Z component.
   *
   * @param ps The particle system.
   * @return The direction Z component.
   */
  EXPORT float CALL EmitterDirectionZ(IParticleSystemSceneNode *ps);

  /**
   * Sets the minimum and maximum particle emission rate (particles per second).
   *
   * @param ps The particle system.
   * @param minRate Minimum particles per second.
   * @param maxRate Maximum particles per second.
   */
  EXPORT void CALL SetEmitterRate(IParticleSystemSceneNode *ps, int minRate, int maxRate);

  /**
   * Returns the minimum particle emission rate.
   *
   * @param ps The particle system.
   * @return The minimum particles per second.
   */
  EXPORT int CALL EmitterMinRate(IParticleSystemSceneNode *ps);

  /**
   * Returns the maximum particle emission rate.
   *
   * @param ps The particle system.
   * @return The maximum particles per second.
   */
  EXPORT int CALL EmitterMaxRate(IParticleSystemSceneNode *ps);

  /**
   * Sets the minimum and maximum starting color for particles.
   *
   * Each particle's color is randomly interpolated between minColor and maxColor.
   *
   * @param ps The particle system.
   * @param minColor The minimum start color.
   * @param maxColor The maximum start color.
   */
  EXPORT void CALL SetEmitterColor(IParticleSystemSceneNode *ps, int minColor, int maxColor);

  /**
   * Returns the minimum emitter start color.
   *
   * @param ps The particle system.
   * @return The minimum start color.
   */
  EXPORT int CALL EmitterMinColor(IParticleSystemSceneNode *ps);

  /**
   * Returns the maximum emitter start color.
   *
   * @param ps The particle system.
   * @return The maximum start color.
   */
  EXPORT int CALL EmitterMaxColor(IParticleSystemSceneNode *ps);

  /**
   * Sets the minimum and maximum particle lifetime in milliseconds.
   *
   * @param ps The particle system.
   * @param minLife The minimum lifetime in milliseconds.
   * @param maxLife The maximum lifetime in milliseconds.
   */
  EXPORT void CALL SetEmitterLife(IParticleSystemSceneNode *ps, int minLife, int maxLife);

  /**
   * Returns the minimum particle lifetime.
   *
   * @param ps The particle system.
   * @return The minimum lifetime in milliseconds.
   */
  EXPORT int CALL EmitterMinLife(IParticleSystemSceneNode *ps);

  /**
   * Returns the maximum particle lifetime.
   *
   * @param ps The particle system.
   * @return The maximum lifetime in milliseconds.
   */
  EXPORT int CALL EmitterMaxLife(IParticleSystemSceneNode *ps);

  /**
   * Sets the minimum and maximum starting particle size.
   *
   * Each particle's size is randomly interpolated between min and max.
   *
   * @param ps The particle system.
   * @param minWidth The minimum start width.
   * @param minHeight The minimum start height.
   * @param maxWidth The maximum start width.
   * @param maxHeight The maximum start height.
   */
  EXPORT void CALL SetEmitterSize(IParticleSystemSceneNode *ps, float minWidth, float minHeight, float maxWidth, float maxHeight);

  /**
   * Returns the minimum emitter start width.
   *
   * @param ps The particle system.
   * @return The minimum start width.
   */
  EXPORT float CALL EmitterMinWidth(IParticleSystemSceneNode *ps);

  /**
   * Returns the minimum emitter start height.
   *
   * @param ps The particle system.
   * @return The minimum start height.
   */
  EXPORT float CALL EmitterMinHeight(IParticleSystemSceneNode *ps);

  /**
   * Returns the maximum emitter start width.
   *
   * @param ps The particle system.
   * @return The maximum start width.
   */
  EXPORT float CALL EmitterMaxWidth(IParticleSystemSceneNode *ps);

  /**
   * Returns the maximum emitter start height.
   *
   * @param ps The particle system.
   * @return The maximum start height.
   */
  EXPORT float CALL EmitterMaxHeight(IParticleSystemSceneNode *ps);

  /**
   * Sets the maximum random angle deviation from the emission direction in degrees.
   *
   * @param ps The particle system.
   * @param angleDeg The maximum angle in degrees.
   */
  EXPORT void CALL SetEmitterSpread(IParticleSystemSceneNode *ps, int angleDeg);

  /**
   * Returns the maximum emitter angle deviation.
   *
   * @param ps The particle system.
   * @return The maximum angle in degrees.
   */
  EXPORT int CALL EmitterSpread(IParticleSystemSceneNode *ps);

  /**
   * Adds a fade-out affector that fades particle color over time.
   *
   * @param ps The particle system.
   * @param targetColor The color to fade to.
   * @param timeNeeded The time in milliseconds to complete the fade.
   */
  EXPORT void CALL AddFadeOutAffector(IParticleSystemSceneNode *ps, int targetColor, int timeNeeded);

  /**
   * Adds a gravity affector that applies gravity to particles.
   *
   * @param ps The particle system.
   * @param x The gravity force X component.
   * @param y The gravity force Y component.
   * @param z The gravity force Z component.
   * @param timeForceLost Time in milliseconds when emitter force is fully lost.
   */
  EXPORT void CALL AddGravityAffector(IParticleSystemSceneNode *ps, float x, float y, float z, int timeForceLost);

  /**
   * Adds a scale affector that scales particles over time.
   *
   * @param ps The particle system.
   * @param scaleToX The final width multiplier.
   * @param scaleToY The final height multiplier.
   */
  EXPORT void CALL AddScaleAffector(IParticleSystemSceneNode *ps, float scaleToX, float scaleToY);

  /**
   * Adds an attraction affector that attracts or repels particles to a point.
   *
   * @param ps The particle system.
   * @param pointX The attraction point X.
   * @param pointY The attraction point Y.
   * @param pointZ The attraction point Z.
   * @param speed The attraction speed in units per second.
   * @param attract True to attract, false to repel.
   */
  EXPORT void CALL AddAttractionAffector(IParticleSystemSceneNode *ps, float pointX, float pointY, float pointZ, float speed, bool_t attract);

  /**
   * Adds a rotation affector that rotates particles around a pivot point.
   *
   * @param ps The particle system.
   * @param speedX The rotation speed X component in degrees per second.
   * @param speedY The rotation speed Y component in degrees per second.
   * @param speedZ The rotation speed Z component in degrees per second.
   * @param pivotX The pivot point X.
   * @param pivotY The pivot point Y.
   * @param pivotZ The pivot point Z.
   */
  EXPORT void CALL AddRotationAffector(IParticleSystemSceneNode *ps, float speedX, float speedY, float speedZ, float pivotX, float pivotY, float pivotZ);

  /**
   * Removes all affectors from the particle system.
   *
   * @param ps The particle system.
   */
  EXPORT void CALL RemoveAllAffectors(IParticleSystemSceneNode *ps);

#ifdef __cplusplus
} /* extern "C" */
#endif
