/**
 * @file
 * A camera is a special type of Entity which is not visible in the world, but defines how the world
 * will be rendered. It can be positioned and rotated in the world using the Entity functions.
 */
#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Creates a new camera.
   *
   * @return A new camera entity.
   */
  EXPORT ICameraSceneNode *CALL CreateCamera();

  /**
   * Set the camera "active" state. Only active cameras are drawn. This is true by default.
   *
   * @param cam The camera whose "active" state will be set.
   * @param active The new "active" state.
   */
  EXPORT void CALL SetCameraActive(ICameraSceneNode *cam, bool_t active);

  /**
   * Returns whether a camera is active.
   *
   * @param cam The camera to query.
   * @return TRUE if the camera is active, otherwise FALSE.
   */
  EXPORT bool_t CALL CameraActive(ICameraSceneNode *cam);

  /**
   * Sets the viewport rectangle of a camera.
   *
   * @param cam The camera whose viewport will be set.
   * @param x The viewport X position.
   * @param y The viewport Y position.
   * @param width The viewport width.
   * @param height The viewport height.
   */
  EXPORT void CALL SetCameraViewport(ICameraSceneNode *cam, int x, int y, int width, int height);

  /**
   * Returns the X position of a camera viewport.
   *
   * @param cam The camera to query.
   * @return The X position of the camera viewport.
   */
  EXPORT int CALL CameraViewportX(ICameraSceneNode *cam);

  /**
   * Returns the Y position of a camera viewport.
   *
   * @param cam The camera to query.
   * @return The Y position of the camera viewport.
   */
  EXPORT int CALL CameraViewportY(ICameraSceneNode *cam);

  /**
   * Returns the width of a camera viewport.
   *
   * @param cam The camera to query.
   * @return The width of the camera viewport.
   */
  EXPORT int CALL CameraViewportWidth(ICameraSceneNode *cam);

  /**
   * Returns the height of a camera viewport.
   *
   * @param cam The camera to query.
   * @return The height of the camera viewport.
   */
  EXPORT int CALL CameraViewportHeight(ICameraSceneNode *cam);

  /**
   * Sets whether a camera clears the color and depth buffers before rendering.
   *
   * @param cam The camera whose clear mode will be set.
   * @param clear_color TRUE to clear the color buffer, otherwise FALSE.
   * @param clear_depth TRUE to clear the depth buffer, otherwise FALSE.
   */
  EXPORT void CALL SetCameraClearMode(ICameraSceneNode *cam, bool_t clear_color, bool_t clear_depth);

  /**
   * Returns whether a camera clears the color buffer before rendering.
   *
   * @param cam The camera to query.
   * @return TRUE if color clearing is enabled, otherwise FALSE.
   */
  EXPORT bool_t CALL CameraClearColorEnabled(ICameraSceneNode *cam);

  /**
   * Returns whether a camera clears the depth buffer before rendering.
   *
   * @param cam The camera to query.
   * @return TRUE if depth clearing is enabled, otherwise FALSE.
   */
  EXPORT bool_t CALL CameraClearDepthEnabled(ICameraSceneNode *cam);

  /**
   * Sets the clear color used before rendering.
   *
   * @param cam The camera whose clear color will be set.
   * @param color The new clear color.
   */
  EXPORT void CALL SetCameraClearColor(ICameraSceneNode *cam, int color);

  /**
   * Returns the clear color used before rendering.
   *
   * @param cam The camera to query.
   * @return The camera clear color.
   */
  EXPORT int CALL CameraClearColor(ICameraSceneNode *cam);

  /**
   * Sets the render target texture for a camera.
   *
   * @param cam The camera whose render target will be set.
   * @param target The texture to render to, or NULL to render to the screen.
   */
  EXPORT void CALL SetCameraRenderTarget(ICameraSceneNode *cam, ITexture *target);

  /**
   * Returns the render target texture of a camera.
   *
   * @param cam The camera to query.
   * @return The render target texture, or NULL if the camera renders to the screen.
   */
  EXPORT ITexture *CALL CameraRenderTarget(ICameraSceneNode *cam);

  /**
   * Sets the near and far clipping plane distances of a camera.
   *
   * @param cam The camera whose clipping range will be set.
   * @param near_ The new near clipping plane distance.
   * @param far_ The new far clipping plane distance.
   */
  EXPORT void CALL SetCameraRange(ICameraSceneNode *cam, float near_, float far_);

  /**
   * Returns the near clipping plane distance of a camera.
   *
   * @param cam The camera to query.
   * @return The near clipping plane distance.
   */
  EXPORT float CALL CameraNearRange(ICameraSceneNode *cam);

  /**
   * Returns the far clipping plane distance of a camera.
   *
   * @param cam The camera to query.
   * @return The far clipping plane distance.
   */
  EXPORT float CALL CameraFarRange(ICameraSceneNode *cam);

  /**
   * Sets the aspect ratio of a camera.
   *
   * @param cam The camera whose aspect ratio will be set.
   * @param ratio The new aspect ratio, or 0 to use the viewport ratio.
   */
  EXPORT void CALL SetCameraAspectRatio(ICameraSceneNode *cam, float ratio);

  /**
   * Returns the aspect ratio of a camera.
   *
   * @param cam The camera to query.
   * @return The camera aspect ratio, or 0 if it uses the viewport ratio.
   */
  EXPORT float CALL CameraAspectRatio(ICameraSceneNode *cam);

  /**
   * Sets the field of view of a camera in degrees.
   *
   * @param cam The camera whose field of view will be set.
   * @param fov The new field of view in degrees.
   */
  EXPORT void CALL SetCameraFOV(ICameraSceneNode *cam, float fov);

  /**
   * Returns the field of view of a camera in degrees.
   *
   * @param cam The camera to query.
   * @return The field of view in degrees.
   */
  EXPORT float CALL CameraFOV(ICameraSceneNode *cam);

  /**
   * Enables or disables orthographic projection for a camera.
   *
   * @param cam The camera whose projection mode will be set.
   * @param enable TRUE to enable orthographic projection, otherwise FALSE.
   */
  EXPORT void CALL SetCameraOrtho(ICameraSceneNode *cam, bool_t enable);

  /**
   * Returns whether orthographic projection is enabled for a camera.
   *
   * @param cam The camera to query.
   * @return TRUE if orthographic projection is enabled, otherwise FALSE.
   */
  EXPORT bool_t CALL CameraOrtho(ICameraSceneNode *cam);

  /**
   * Returns the entity under screen coordinates as seen by a camera.
   *
   * @param camera The camera used for picking.
   * @param x The screen-space X coordinate.
   * @param y The screen-space Y coordinate.
   * @param group The collision group filter, or 0 to match all groups.
   * @return The entity under the given coordinates, or NULL if nothing is picked.
   */
  EXPORT ISceneNode *CALL PickEntity(ICameraSceneNode *camera, int x, int y, int group);

#ifdef __cplusplus
} /* extern "C" */
#endif
