#include <string.h>
#include "color.h"
#include "core.h"
#include "material.h"
#include "math.h"
#include "shader.h"

struct ShaderUniform
{
  stringc Name;
  array<f32> Floats;
  bool IsInt;
  s32 IntValue;
};

enum BuiltinIndex
{
  BUILTIN_WORLDVIEWPROJ = 0,
  BUILTIN_WORLD,
  BUILTIN_VIEW,
  BUILTIN_PROJ,
  BUILTIN_TIME,
  BUILTIN_DIFFUSE,
  BUILTIN_EMISSIVE,
  BUILTIN_AMBIENT,
  BUILTIN_SPECULAR,
  BUILTIN_SHININESS,
  BUILTIN_PARAM1,
  BUILTIN_PARAM2,
  NUM_BUILTINS
};

struct Shader
{
  s32 Type;
  array<ShaderUniform> Uniforms;
  s32 BuiltinIds[NUM_BUILTINS];
  bool Freed;
};

static const char *_BuiltinNames[NUM_BUILTINS] = {
    "mWorldViewProj", "mWorld", "mView", "mProj", "fTime",
    "mDiffuse", "mEmissive", "mAmbient", "mSpecular", "mShininess",
    "mParam1", "mParam2"};

static array<Shader *> _shaders;

class ShaderCallback : public IShaderConstantSetCallBack
{
public:
  ShaderCallback(Shader *shader) : _shader(shader), _material(NULL)
  {
  }

  virtual void OnCreate(IMaterialRendererServices *services, s32 userData)
  {
    for (int i = 0; i < NUM_BUILTINS; ++i)
      _shader->BuiltinIds[i] = services->getVertexShaderConstantID(_BuiltinNames[i]);
  }

  virtual void OnSetMaterial(const SMaterial &material)
  {
    _material = &material;
  }

  virtual void OnSetConstants(IMaterialRendererServices *services, s32 userData)
  {
    if (_shader->Freed)
      return;
    for (u32 i = 0; i < _shader->Uniforms.size(); ++i)
    {
      ShaderUniform &uniform = _shader->Uniforms[i];
      const s32 id = services->getVertexShaderConstantID(uniform.Name.c_str());
      if (id < 0)
        continue;
      if (uniform.IsInt)
        services->setVertexShaderConstant(id, &uniform.IntValue, 1);
      else
        services->setVertexShaderConstant(id, uniform.Floats.const_pointer(), uniform.Floats.size());
    }
    _WriteBuiltins(services);
  }

private:
  Shader *_shader;
  const SMaterial *_material;

  void _WriteBuiltin(IMaterialRendererServices *services, s32 id, const f32 *floats, int count)
  {
    if (id >= 0)
      services->setVertexShaderConstant(id, floats, count);
  }

  void _WriteColor(IMaterialRendererServices *services, s32 id, const SColor &color)
  {
    const f32 rgba[4] = {
        color.getRed() / 255.0f,
        color.getGreen() / 255.0f,
        color.getBlue() / 255.0f,
        color.getAlpha() / 255.0f};
    _WriteBuiltin(services, id, rgba, 4);
  }

  void _WriteBuiltins(IMaterialRendererServices *services)
  {
    IVideoDriver *driver = services->getVideoDriver();
    matrix4 worldViewProj(driver->getTransform(video::ETS_PROJECTION));
    worldViewProj *= driver->getTransform(video::ETS_VIEW);
    worldViewProj *= driver->getTransform(video::ETS_WORLD);
    _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_WORLDVIEWPROJ], worldViewProj.pointer(), 16);
    _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_WORLD], driver->getTransform(video::ETS_WORLD).pointer(), 16);
    _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_VIEW], driver->getTransform(video::ETS_VIEW).pointer(), 16);
    _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_PROJ], driver->getTransform(video::ETS_PROJECTION).pointer(), 16);

    const f32 time = Millisecs() / 1000.0f;
    _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_TIME], &time, 1);

    if (_material)
    {
      _WriteColor(services, _shader->BuiltinIds[BUILTIN_DIFFUSE], _material->DiffuseColor);
      _WriteColor(services, _shader->BuiltinIds[BUILTIN_EMISSIVE], _material->EmissiveColor);
      _WriteColor(services, _shader->BuiltinIds[BUILTIN_AMBIENT], _material->AmbientColor);
      _WriteColor(services, _shader->BuiltinIds[BUILTIN_SPECULAR], _material->SpecularColor);
      const f32 shininess = Clamp(_material->Shininess / 128, 0, 1);
      _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_SHININESS], &shininess, 1);
      _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_PARAM1], &_material->MaterialTypeParam, 1);
      _WriteBuiltin(services, _shader->BuiltinIds[BUILTIN_PARAM2], &_material->MaterialTypeParam2, 1);
    }
  }
};

static stringc _LoadShaderFile(const char *filename)
{
  stringc source;
  IReadFile *file = _Device()->getFileSystem()->createAndOpenFile(filename);
  if (file)
  {
    const s32 size = file->getSize();
    if (size > 0)
    {
      char *buffer = new char[size + 1];
      file->read(buffer, size);
      buffer[size] = 0;
      source = buffer;
      delete[] buffer;
    }
    file->drop();
  }
  return source;
}

extern "C"
{

  EXPORT Shader *CALL CreateShader(const char *vertexSource, const char *fragmentSource, int baseType)
  {
    IVideoDriver *driver = _Device()->getVideoDriver();
    if (!driver)
      return NULL;
    IGPUProgrammingServices *services = driver->getGPUProgrammingServices();
    if (!services)
      return NULL;

    Shader *shader = new Shader();
    shader->Type = -1;
    shader->Freed = false;
    for (int i = 0; i < NUM_BUILTINS; ++i)
      shader->BuiltinIds[i] = -1;
    ShaderCallback *callback = new ShaderCallback(shader);
    shader->Type = services->addHighLevelShaderMaterial(
        vertexSource, "main", EVST_VS_1_1,
        fragmentSource, "main", EPST_PS_1_1,
        callback, _IrrlichtMaterialType(baseType), 0);
    if (shader->Type < 0)
    {
      delete shader;
      return NULL;
    }
    _shaders.push_back(shader);
    return shader;
  }

  EXPORT Shader *CALL LoadShader(const char *vertexFile, const char *fragmentFile, int baseType)
  {
    const stringc vertexSource = _LoadShaderFile(vertexFile);
    const stringc fragmentSource = _LoadShaderFile(fragmentFile);
    return CreateShader(vertexSource.c_str(), fragmentSource.c_str(), baseType);
  }

  EXPORT void CALL FreeShader(Shader *shader)
  {
    if (shader)
      shader->Freed = true;
  }

  EXPORT void CALL SetMaterialShader(SMaterial *material, Shader *shader)
  {
    if (shader)
      material->MaterialType = (E_MATERIAL_TYPE)shader->Type;
    else
      material->MaterialType = _IrrlichtMaterialType(MATERIAL_SOLID);
  }

  EXPORT Shader *CALL ShaderMaterial(SMaterial *material)
  {
    for (u32 i = 0; i < _shaders.size(); ++i)
    {
      Shader *shader = _shaders[i];
      if (shader && !shader->Freed && shader->Type == (s32)material->MaterialType)
        return shader;
    }
    return NULL;
  }

  static ShaderUniform *_FindUniform(Shader *shader, const char *name)
  {
    for (u32 i = 0; i < shader->Uniforms.size(); ++i)
    {
      if (shader->Uniforms[i].Name == name)
        return &shader->Uniforms[i];
    }
    return NULL;
  }

  static ShaderUniform *_GetUniform(Shader *shader, const char *name)
  {
    ShaderUniform *uniform = _FindUniform(shader, name);
    if (!uniform)
    {
      ShaderUniform u;
      u.Name = name;
      u.IsInt = false;
      u.IntValue = 0;
      shader->Uniforms.push_back(u);
      uniform = &shader->Uniforms[shader->Uniforms.size() - 1];
    }
    return uniform;
  }

  EXPORT void CALL SetShaderFloat(Shader *shader, const char *name, float value)
  {
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = false;
    uniform->Floats.set_used(1);
    uniform->Floats[0] = value;
  }

  EXPORT void CALL SetShaderVector3(Shader *shader, const char *name, float x, float y, float z)
  {
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = false;
    uniform->Floats.set_used(3);
    uniform->Floats[0] = x;
    uniform->Floats[1] = y;
    uniform->Floats[2] = z;
  }

  EXPORT void CALL SetShaderVector4(Shader *shader, const char *name, float x, float y, float z, float w)
  {
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = false;
    uniform->Floats.set_used(4);
    uniform->Floats[0] = x;
    uniform->Floats[1] = y;
    uniform->Floats[2] = z;
    uniform->Floats[3] = w;
  }

  EXPORT void CALL SetShaderColor(Shader *shader, const char *name, int color)
  {
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = false;
    uniform->Floats.set_used(4);
    uniform->Floats[0] = RedValF(color);
    uniform->Floats[1] = GreenValF(color);
    uniform->Floats[2] = BlueValF(color);
    uniform->Floats[3] = AlphaValF(color);
  }

  EXPORT void CALL SetShaderInt(Shader *shader, const char *name, int value)
  {
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = true;
    uniform->IntValue = value;
    uniform->Floats.set_used(0);
  }

  EXPORT void CALL SetShaderMatrix(Shader *shader, const char *name, Memblock *memblock)
  {
    if (MemblockSize(memblock) < 16 * 4)
      return;
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = false;
    uniform->Floats.set_used(16);
    memcpy(uniform->Floats.pointer(), (const f32 *)memblock, 16 * 4);
  }

  EXPORT void CALL SetShaderFloats(Shader *shader, const char *name, Memblock *memblock)
  {
    const int count = MemblockSize(memblock) / 4;
    ShaderUniform *uniform = _GetUniform(shader, name);
    uniform->IsInt = false;
    uniform->Floats.set_used(count);
    memcpy(uniform->Floats.pointer(), (const f32 *)memblock, count * 4);
  }

  EXPORT void CALL ClearShaderUniforms(Shader *shader)
  {
    shader->Uniforms.clear();
  }

} // extern "C"
