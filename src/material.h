/**
 * @file
 * Functions for setting material properties on meshes and other entities. Materials control how
 * surfaces are rendered, including textures, lighting, blending, filtering, and related flags.
 */
#pragma once

#include "common.h"

// Blend modes
#define MATERIAL_SOLID 0 /**< Standard opaque material. */
#define MATERIAL_LIGHTMAP 1 /**< Lightmap material. */
#define MATERIAL_LIGHTMAP2X 2 /**< 2x lightmap material. */
#define MATERIAL_LIGHTMAP4X 3 /**< 4x lightmap material. */
#define MATERIAL_DETAIL 4 /**< Detail texture material. */
#define MATERIAL_ADD 5 /**< Additive blending material. */
#define MATERIAL_ALPHA 6 /**< Alpha blending material. */
#define MATERIAL_VERTEXALPHA 7 /**< Vertex alpha blending material. */
#define MATERIAL_MASKED 8 /**< Masked material. */
#define MATERIAL_SPHERE 9 /**< Sphere mapping material. */
#define MATERIAL_REFLECTION 10 /**< Reflection mapping material. */
#define MATERIAL_NORMAL 11 /**< Normal mapped material. */
#define MATERIAL_NORMALADD 12 /**< Additive normal mapped material. */
#define MATERIAL_PARALLAX 13 /**< Parallax mapped material. */
#define MATERIAL_PARALLAXADD 14 /**< Additive parallax mapped material. */

// Render modes
#define RENDER_FILLED 0 /**< Render filled geometry. */
#define RENDER_WIREFRAME 1 /**< Render wireframe geometry. */
#define RENDER_POINTCLOUD 2 /**< Render geometry as points. */

// Lighting modes
#define LIGHTING_DISABLED 0 /**< Disable lighting. */
#define LIGHTING_GOURAUD 1 /**< Use Gouraud lighting. */
#define LIGHTING_FLAT 2 /**< Use flat lighting. */

// Filtering modes
#define FILTER_DISABLED 0 /**< Disable texture filtering. */
#define FILTER_BILINEAR 1 /**< Use bilinear filtering. */
#define FILTER_TRILINEAR 2 /**< Use trilinear filtering. */
#define FILTER_ANISOTROPIC 3 /**< Use anisotropic filtering. */

// Material flags
#define FLAG_LIGHTING 1 /**< Enable lighting. */
#define FLAG_FOG 2 /**< Enable fog. */
#define FLAG_ZREAD 4 /**< Enable depth reads. */
#define FLAG_ZWRITE 8 /**< Enable depth writes. */
#define FLAG_BACKFACECULLING 16 /**< Enable back-face culling. */
#define FLAG_NORMALIZE 32 /**< Normalize transformed normals. */
#define FLAG_VERTEXCOLORS 64 /**< Use vertex colors. */
#define FLAG_CLAMP 128 /**< Clamp texture coordinates. */

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Sets the texture at the given texture layer.
   *
   * @param material The material whose texture will be set.
   * @param index The 0-based texture layer index.
   * @param texture The texture to assign to the layer.
   */
  EXPORT void CALL SetMaterialTexture(SMaterial *material, int index, ITexture *texture);

  /**
   * Returns the texture at the given texture layer.
   *
   * @param material The material to query.
   * @param index The 0-based texture layer index.
   * @return The texture assigned to the layer.
   */
  EXPORT ITexture *CALL MaterialTexture(SMaterial *material, int index);

  /**
   * Sets a 2D transform on the texture at the given layer.
   *
   * @param material The material whose texture transform will be set.
   * @param index The 0-based texture layer index.
   * @param xoffset The horizontal offset in UV space.
   * @param yoffset The vertical offset in UV space.
   * @param angle The rotation angle in degrees.
   * @param xscale The horizontal UV scale.
   * @param yscale The vertical UV scale.
   */
  EXPORT void CALL SetMaterialTextureTransform(SMaterial *material, int index, float xoffset, float yoffset, float angle, float xscale, float yscale);

  /**
   * Sets the material blend or shader type.
   *
   * @param material The material whose type will be set.
   * @param type The material type to use. See the MATERIAL_* constants.
   */
  EXPORT void CALL SetMaterialType(SMaterial *material, int type);

  /**
   * Returns the material blend or shader type.
   *
   * @param material The material to query.
   * @return The current material type.
   */
  EXPORT int CALL MaterialType(SMaterial *material);

  /**
   * Sets the diffuse color.
   *
   * @param material The material whose diffuse color will be set.
   * @param color The diffuse color.
   */
  EXPORT void CALL SetMaterialDiffuse(SMaterial *material, int color);

  /**
   * Returns the diffuse color.
   *
   * @param material The material to query.
   * @return The diffuse color.
   */
  EXPORT int CALL MaterialDiffuse(SMaterial *material);

  /**
   * Sets the emissive color.
   *
   * @param material The material whose emissive color will be set.
   * @param color The emissive color.
   */
  EXPORT void CALL SetMaterialEmissive(SMaterial *material, int color);

  /**
   * Returns the emissive color.
   *
   * @param material The material to query.
   * @return The emissive color.
   */
  EXPORT int CALL MaterialEmissive(SMaterial *material);

  /**
   * Sets the ambient color.
   *
   * @param material The material whose ambient color will be set.
   * @param color The ambient color.
   */
  EXPORT void CALL SetMaterialAmbient(SMaterial *material, int color);

  /**
   * Returns the ambient color.
   *
   * @param material The material to query.
   * @return The ambient color.
   */
  EXPORT int CALL MaterialAmbient(SMaterial *material);

  /**
   * Sets the specular highlight color.
   *
   * @param material The material whose specular color will be set.
   * @param color The specular color.
   */
  EXPORT void CALL SetMaterialSpecular(SMaterial *material, int color);

  /**
   * Returns the specular highlight color.
   *
   * @param material The material to query.
   * @return The specular color.
   */
  EXPORT int CALL MaterialSpecular(SMaterial *material);

  /**
   * Sets the shininess value.
   *
   * @param material The material whose shininess will be set.
   * @param shininess The specular exponent.
   */
  EXPORT void CALL SetMaterialShininess(SMaterial *material, float shininess);

  /**
   * Returns the shininess value.
   *
   * @param material The material to query.
   * @return The current shininess.
   */
  EXPORT float CALL MaterialShininess(SMaterial *material);

  /**
   * Sets the first custom material parameter.
   *
   * @param material The material whose parameter will be set.
   * @param param The custom parameter value.
   */
  EXPORT void CALL SetMaterialParam1(SMaterial *material, float param);

  /**
   * Returns the first custom material parameter.
   *
   * @param material The material to query.
   * @return The current parameter value.
   */
  EXPORT float CALL MaterialParam1(SMaterial *material);

  /**
   * Sets the second custom material parameter.
   *
   * @param material The material whose parameter will be set.
   * @param param The custom parameter value.
   */
  EXPORT void CALL SetMaterialParam2(SMaterial *material, float param);

  /**
   * Returns the second custom material parameter.
   *
   * @param material The material to query.
   * @return The current parameter value.
   */
  EXPORT float CALL MaterialParam2(SMaterial *material);

  /**
   * Sets the render mode.
   *
   * @param material The material whose render mode will be set.
   * @param mode The render mode to use. See RENDER_* constants.
   */
  EXPORT void CALL SetMaterialRenderMode(SMaterial *material, int mode);

  /**
   * Returns the render mode.
   *
   * @param material The material to query.
   * @return The current render mode.
   */
  EXPORT int CALL MaterialRenderMode(SMaterial *material);

  /**
   * Sets the texture filter mode.
   *
   * @param material The material whose filter mode will be set.
   * @param mode The filter mode to use. See FILTER_* constants.
   */
  EXPORT void CALL SetMaterialFilterMode(SMaterial *material, int mode);

  /**
   * Returns the texture filter mode.
   *
   * @param material The material to query.
   * @return The current filter mode.
   */
  EXPORT int CALL MaterialFilterMode(SMaterial *material);

  /**
   * Sets the lighting mode.
   *
   * @param material The material whose lighting mode will be set.
   * @param mode The lighting mode to use. See LIGHTING_* constants.
   */
  EXPORT void CALL SetMaterialLightingMode(SMaterial *material, int mode);

  /**
   * Returns the lighting mode.
   *
   * @param material The material to query.
   * @return The current lighting mode.
   */
  EXPORT int CALL MaterialLightingMode(SMaterial *material);

  /**
   * Enables or disables a material flag.
   *
   * @param material The material whose flag will be changed.
   * @param flag The material flag to change. See FLAG_* constants.
   * @param enable The new state of the flag.
   */
  EXPORT void CALL SetMaterialFlag(SMaterial *material, int flag, bool_t enable);

  /**
   * Returns whether a material flag is enabled.
   *
   * @param material The material to query.
   * @param flag The material flag to query. See FLAG_* constants.
   * @return True if the flag is enabled; otherwise false.
   */
  EXPORT bool_t CALL MaterialFlag(SMaterial *material, int flag);

#ifndef SWIG
void _FixMaterial(SMaterial *material, bool fix_specular);
E_MATERIAL_TYPE _IrrlichtMaterialType(int type);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
