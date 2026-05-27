/**
 * @file
 * A Model is a scene entity that displays a Mesh. Models can be animated.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a cone model.
   *
   * @param segments The number of radial segments.
   * @return A new cone model.
   */
  EXPORT IMeshSceneNode *CALL CreateConeModel(int segments);

  /**
   * Creates a cube model.
   *
   * @return A new cube model.
   */
  EXPORT IMeshSceneNode *CALL CreateCubeModel();

  /**
   * Creates a cylinder model.
   *
   * @param segments The number of radial segments.
   * @return A new cylinder model.
   */
  EXPORT IMeshSceneNode *CALL CreateCylinderModel(int segments);

  /**
   * Creates a model from an existing mesh.
   *
   * @param mesh The mesh to display.
   * @return A new model using the mesh.
   */
  EXPORT IMeshSceneNode *CALL CreateModel(IMesh *mesh);

  /**
   * Creates a model with octree optimisation for static geometry.
   *
   * @param mesh The mesh to display.
   * @return A new octree model.
   */
  EXPORT IMeshSceneNode *CALL CreateOctreeModel(IMesh *mesh);

  /**
   * Creates a quad model.
   *
   * @return A new quad model.
   */
  EXPORT IMeshSceneNode *CALL CreateQuadModel();

  /**
   * Creates a sphere model.
   *
   * @param segments The number of segments.
   * @return A new sphere model.
   */
  EXPORT IMeshSceneNode *CALL CreateSphereModel(int segments);

  /**
   * Loads a model from a file.
   *
   * @param filename The model file to load.
   * @return The loaded model.
   */
  EXPORT IMeshSceneNode *CALL LoadModel(const char *filename);

  /**
   * Loads a model with octree optimisation.
   *
   * @param filename The model file to load.
   * @return The loaded octree model.
   */
  EXPORT IMeshSceneNode *CALL LoadOctreeModel(const char *filename);

  /**
   * Returns the mesh used by a model.
   *
   * @param entity The model to query.
   * @return The mesh used by the model.
   */
  EXPORT IMesh *CALL ModelMesh(IMeshSceneNode *entity);

  /**
   * Enables or disables shadow casting for a model.
   *
   * @param entity The model to modify.
   * @param enable The new shadow casting state.
   */
  EXPORT void CALL SetModelCastShadows(IMeshSceneNode *entity, bool_t enable);

  /**
   * Returns whether a model casts shadows.
   *
   * @param entity The model to query.
   * @return True if shadow casting is enabled; otherwise false.
   */
  EXPORT bool_t CALL ModelCastShadows(IMeshSceneNode *entity);

  /**
   * Enables or disables looping animation.
   *
   * @param entity The model to modify.
   * @param loop The new looping state.
   */
  EXPORT void CALL SetModelLoop(IMeshSceneNode *entity, bool_t loop);

  /**
   * Returns whether animation looping is enabled.
   *
   * @param entity The model to query.
   * @return True if animation looping is enabled; otherwise false.
   */
  EXPORT bool_t CALL ModelLoop(IMeshSceneNode *entity);

  /**
   * Sets the animation playback speed.
   *
   * @param entity The model to modify.
   * @param fps The playback speed in frames per second.
   */
  EXPORT void CALL SetModelFPS(IMeshSceneNode *entity, float fps);

  /**
   * Returns the animation playback speed.
   *
   * @param entity The model to query.
   * @return The playback speed in frames per second.
   */
  EXPORT float CALL ModelFPS(IMeshSceneNode *entity);

  /**
   * Sets the current animation frame.
   *
   * @param entity The model to modify.
   * @param frame The animation frame to display.
   */
  EXPORT void CALL SetModelFrame(IMeshSceneNode *entity, float frame);

  /**
   * Returns the current animation frame.
   *
   * @param entity The model to query.
   * @return The current animation frame.
   */
  EXPORT float CALL ModelFrame(IMeshSceneNode *entity);

  /**
   * Sets the animation frame range to play.
   *
   * @param entity The model to modify.
   * @param first The first frame in the range.
   * @param last The last frame in the range.
   */
  EXPORT void CALL SetModelFrames(IMeshSceneNode *entity, int first, int last);

  /**
   * Returns the first animation frame.
   *
   * @param entity The model to query.
   * @return The first animation frame.
   */
  EXPORT int CALL ModelFirstFrame(IMeshSceneNode *entity);

  /**
   * Returns the last animation frame.
   *
   * @param entity The model to query.
   * @return The last animation frame.
   */
  EXPORT int CALL ModelLastFrame(IMeshSceneNode *entity);

#ifdef __cplusplus
} /* extern "C" */
#endif
