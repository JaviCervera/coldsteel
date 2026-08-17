/**
 * @file
 * Custom shaders let you replace the default rendering pipeline with your own GLSL programs.
 * Shaders are attached to materials and expose uniform variables driven from Lua.
 */
#pragma once

#include "common.h"
#include "memblock.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /** Custom shader handle. */
  typedef struct Shader Shader;

  /**
   * Creates a shader from GLSL source strings.
   *
   * @param vertexSource The vertex shader source code.
   * @param fragmentSource The fragment shader source code.
   * @param baseType The base material type whose render states the shader inherits. See the MATERIAL_* constants.
   * @return The new shader, or NULL if the driver has no GPU programming support or the shader failed to compile.
   */
  EXPORT Shader *CALL CreateShader(const char *vertexSource, const char *fragmentSource, int baseType);

  /**
   * Loads a shader from GLSL source files.
   *
   * @param vertexFile The path to the vertex shader source file.
   * @param fragmentFile The path to the fragment shader source file.
   * @param baseType The base material type whose render states the shader inherits. See the MATERIAL_* constants.
   * @return The new shader, or NULL if the driver has no GPU programming support or the shader failed to compile.
   */
  EXPORT Shader *CALL LoadShader(const char *vertexFile, const char *fragmentFile, int baseType);

  /**
   * Frees a shader. Materials using it fall back to their base material type.
   *
   * @param shader The shader to free.
   */
  EXPORT void CALL FreeShader(Shader *shader);

  /**
   * Attaches a shader to a material. Pass NULL to detach the current shader.
   *
   * @param material The material to modify.
   * @param shader The shader to attach, or NULL to detach.
   */
  EXPORT void CALL SetMaterialShader(SMaterial *material, Shader *shader);

  /**
   * Returns the shader attached to a material.
   *
   * @param material The material to query.
   * @return The attached shader, or NULL if none.
   */
  EXPORT Shader *CALL ShaderMaterial(SMaterial *material);

  /**
   * Sets a single-float uniform on a shader.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param value The float value to write.
   */
  EXPORT void CALL SetShaderFloat(Shader *shader, const char *name, float value);

  /**
   * Sets a vec3 uniform on a shader.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param x The X component.
   * @param y The Y component.
   * @param z The Z component.
   */
  EXPORT void CALL SetShaderVector3(Shader *shader, const char *name, float x, float y, float z);

  /**
   * Sets a vec4 uniform on a shader.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param x The X component.
   * @param y The Y component.
   * @param z The Z component.
   * @param w The W component.
   */
  EXPORT void CALL SetShaderVector4(Shader *shader, const char *name, float x, float y, float z, float w);

  /**
   * Sets a vec4 uniform on a shader from a packed ARGB color.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param color The packed ARGB color to write.
   */
  EXPORT void CALL SetShaderColor(Shader *shader, const char *name, int color);

  /**
   * Sets an integer uniform on a shader.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param value The integer value to write.
   */
  EXPORT void CALL SetShaderInt(Shader *shader, const char *name, int value);

  /**
   * Sets a mat4 uniform on a shader from a memblock containing 16 floats.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param memblock The memblock holding the 16 floats. Must be at least 64 bytes.
   */
  EXPORT void CALL SetShaderMatrix(Shader *shader, const char *name, Memblock *memblock);

  /**
   * Sets a float array uniform on a shader from a memblock.
   *
   * @param shader The shader to modify.
   * @param name The uniform variable name.
   * @param memblock The memblock holding the floats. One float per 4 bytes.
   */
  EXPORT void CALL SetShaderFloats(Shader *shader, const char *name, Memblock *memblock);

  /**
   * Removes all user uniforms from a shader.
   *
   * @param shader The shader to clear.
   */
  EXPORT void CALL ClearShaderUniforms(Shader *shader);

#ifdef __cplusplus
} /* extern "C" */
#endif
