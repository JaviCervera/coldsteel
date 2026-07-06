#include "core.h"
#include "color.h"
#include "particle.h"

extern "C"
{

  EXPORT IParticleSystemSceneNode *CALL CreateParticleSystem()
  {
    IParticleSystemSceneNode *ps = _Device()->getSceneManager()->addParticleSystemSceneNode(false);
    if (ps)
      ps->setParticleBehavior(EPB_CLEAR_ON_INVISIBLE);
    return ps;
  }

  EXPORT void CALL SetParticleSize(IParticleSystemSceneNode *ps, float width, float height)
  {
    ps->setParticleSize(dimension2df(width, height));
  }

  EXPORT void CALL SetParticlesAreGlobal(IParticleSystemSceneNode *ps, bool_t global)
  {
    ps->setParticlesAreGlobal(global != 0);
  }

  EXPORT void CALL ClearParticles(IParticleSystemSceneNode *ps)
  {
    ps->clearParticles();
  }

  // --- Emitter type selection ---

  EXPORT void CALL SetPointEmitter(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = ps->createPointEmitter();
    ps->setEmitter(em);
    em->drop();
  }

  EXPORT void CALL SetBoxEmitter(IParticleSystemSceneNode *ps, float x, float y, float z, float w, float h, float d)
  {
    IParticleBoxEmitter *em = ps->createBoxEmitter(
        aabbox3df(x - w / 2, y - h / 2, z - d / 2, x + w / 2, y + h / 2, z + d / 2));
    ps->setEmitter(em);
    em->drop();
  }

  EXPORT void CALL SetSphereEmitter(IParticleSystemSceneNode *ps, float centerX, float centerY, float centerZ, float radius)
  {
    IParticleSphereEmitter *em = ps->createSphereEmitter(vector3df(centerX, centerY, centerZ), radius);
    ps->setEmitter(em);
    em->drop();
  }

  EXPORT void CALL SetMeshEmitter(IParticleSystemSceneNode *ps, ISceneNode *entity,
      bool_t useNormalDirection, float normalDirectionModifier,
      int mbNumber, bool_t everyMeshVertex)
  {
    if (entity->getType() == ESNT_ANIMATED_MESH)
    {
      IParticleAnimatedMeshSceneNodeEmitter *em = ps->createAnimatedMeshSceneNodeEmitter(
          static_cast<IAnimatedMeshSceneNode *>(entity), useNormalDirection != 0,
          vector3df(0.0f, 0.03f, 0.0f), normalDirectionModifier,
          mbNumber, everyMeshVertex != 0,
          5, 10,
          SColor(255, 0, 0, 0), SColor(255, 255, 255, 255),
          2000, 4000, 0,
          dimension2df(5.0f, 5.0f), dimension2df(5.0f, 5.0f));
      ps->setEmitter(em);
      em->drop();
    }
    else if (entity->getType() == ESNT_MESH || entity->getType() == ESNT_OCTREE)
    {
      IMesh *mesh = static_cast<IMeshSceneNode *>(entity)->getMesh();
      if (!mesh)
        return;
      IParticleMeshEmitter *em = ps->createMeshEmitter(
          mesh, useNormalDirection != 0,
          vector3df(0.0f, 0.03f, 0.0f), normalDirectionModifier,
          mbNumber, everyMeshVertex != 0,
          5, 10,
          SColor(255, 0, 0, 0), SColor(255, 255, 255, 255),
          2000, 4000, 0,
          dimension2df(5.0f, 5.0f), dimension2df(5.0f, 5.0f));
      ps->setEmitter(em);
      em->drop();
    }
  }

  // --- Emitter properties ---

  static IParticleEmitter *_GetEmitter(IParticleSystemSceneNode *ps)
  {
    return ps->getEmitter();
  }

  EXPORT void CALL SetEmitterDirection(IParticleSystemSceneNode *ps, float x, float y, float z)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    if (em)
      em->setDirection(vector3df(x, y, z));
  }

  EXPORT float CALL EmitterDirectionX(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getDirection().X : 0;
  }

  EXPORT float CALL EmitterDirectionY(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getDirection().Y : 0;
  }

  EXPORT float CALL EmitterDirectionZ(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getDirection().Z : 0;
  }

  EXPORT void CALL SetEmitterRate(IParticleSystemSceneNode *ps, int minRate, int maxRate)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    if (em)
    {
      em->setMinParticlesPerSecond(minRate);
      em->setMaxParticlesPerSecond(maxRate);
    }
  }

  EXPORT int CALL EmitterMinRate(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? (int)em->getMinParticlesPerSecond() : 0;
  }

  EXPORT int CALL EmitterMaxRate(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? (int)em->getMaxParticlesPerSecond() : 0;
  }

  EXPORT void CALL SetEmitterColor(IParticleSystemSceneNode *ps, int minColor, int maxColor)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    if (em)
    {
      em->setMinStartColor(_Color(minColor));
      em->setMaxStartColor(_Color(maxColor));
    }
  }

  EXPORT int CALL EmitterMinColor(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? _IntColor(em->getMinStartColor()) : 0;
  }

  EXPORT int CALL EmitterMaxColor(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? _IntColor(em->getMaxStartColor()) : 0;
  }

  EXPORT void CALL SetEmitterLife(IParticleSystemSceneNode *ps, int minLife, int maxLife)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    if (em)
    {
      em->setMinLifeTime(minLife);
      em->setMaxLifeTime(maxLife);
    }
  }

  EXPORT int CALL EmitterMinLife(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? (int)em->getMinLifeTime() : 0;
  }

  EXPORT int CALL EmitterMaxLife(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? (int)em->getMaxLifeTime() : 0;
  }

  EXPORT void CALL SetEmitterSize(IParticleSystemSceneNode *ps, float minWidth, float minHeight, float maxWidth, float maxHeight)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    if (em)
    {
      em->setMinStartSize(dimension2df(minWidth, minHeight));
      em->setMaxStartSize(dimension2df(maxWidth, maxHeight));
    }
  }

  EXPORT float CALL EmitterMinWidth(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getMinStartSize().Width : 0;
  }

  EXPORT float CALL EmitterMinHeight(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getMinStartSize().Height : 0;
  }

  EXPORT float CALL EmitterMaxWidth(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getMaxStartSize().Width : 0;
  }

  EXPORT float CALL EmitterMaxHeight(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? em->getMaxStartSize().Height : 0;
  }

  EXPORT void CALL SetEmitterSpread(IParticleSystemSceneNode *ps, int angleDeg)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    if (em)
      em->setMaxAngleDegrees(angleDeg);
  }

  EXPORT int CALL EmitterSpread(IParticleSystemSceneNode *ps)
  {
    IParticleEmitter *em = _GetEmitter(ps);
    return em ? (int)em->getMaxAngleDegrees() : 0;
  }

  // --- Affectors ---

  EXPORT void CALL AddFadeOutAffector(IParticleSystemSceneNode *ps, int targetColor, int timeNeeded)
  {
    IParticleFadeOutAffector *af = ps->createFadeOutParticleAffector(_Color(targetColor), timeNeeded);
    ps->addAffector(af);
    af->drop();
  }

  EXPORT void CALL AddGravityAffector(IParticleSystemSceneNode *ps, float x, float y, float z, int timeForceLost)
  {
    IParticleGravityAffector *af = ps->createGravityAffector(vector3df(x, y, z), timeForceLost);
    ps->addAffector(af);
    af->drop();
  }

  EXPORT void CALL AddScaleAffector(IParticleSystemSceneNode *ps, float scaleToX, float scaleToY)
  {
    IParticleAffector *af = ps->createScaleParticleAffector(dimension2df(scaleToX, scaleToY));
    ps->addAffector(af);
    af->drop();
  }

  EXPORT void CALL AddAttractionAffector(IParticleSystemSceneNode *ps, float pointX, float pointY, float pointZ, float speed, bool_t attract)
  {
    IParticleAttractionAffector *af = ps->createAttractionAffector(vector3df(pointX, pointY, pointZ), speed, attract != 0);
    ps->addAffector(af);
    af->drop();
  }

  EXPORT void CALL AddRotationAffector(IParticleSystemSceneNode *ps, float speedX, float speedY, float speedZ, float pivotX, float pivotY, float pivotZ)
  {
    IParticleRotationAffector *af = ps->createRotationAffector(
        vector3df(speedX, speedY, speedZ), vector3df(pivotX, pivotY, pivotZ));
    ps->addAffector(af);
    af->drop();
  }

  EXPORT void CALL RemoveAllAffectors(IParticleSystemSceneNode *ps)
  {
    ps->removeAllAffectors();
  }

} // extern "C"
