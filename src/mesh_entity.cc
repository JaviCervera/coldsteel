#include "core.h"
#include "entity.h"
#include "mesh.h"
#include "mesh_entity.h"

extern "C" {


EXPORT IMeshSceneNode* CALL csCreateMeshEntity(IMesh* mesh) {
    if (_csMeshAnimated(mesh)) {
        return (IMeshSceneNode*)_csDevice()->getSceneManager()->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);
    } else {
        return _csDevice()->getSceneManager()->addMeshSceneNode(mesh);
    }
}


EXPORT IMeshSceneNode* CALL csCreateOctreeMeshEntity(IMesh* mesh) {
    return _csDevice()->getSceneManager()->addOctreeSceneNode(mesh);
}


EXPORT IMesh* CALL csMeshEntityMesh(IMeshSceneNode* entity) {
    if (entity->getType() == ESNT_ANIMATED_MESH) {
        return ((IAnimatedMeshSceneNode*)entity)->getMesh();
    } else {
        return entity->getMesh();
    }
}


EXPORT void CALL csSetMeshEntityCastShadows(IMeshSceneNode* entity, bool_t enable) {
    int num = csEntityNumChildren(entity);
    ISceneNode* shadowNode = NULL;
    for (int i = 0; i < num; ++i) {
        ISceneNode* child = csEntityChild(entity, i);
        if (child->getType() == ESNT_SHADOW_VOLUME) {
            shadowNode = child;
            break;
        }
    }
    if (shadowNode) shadowNode->remove();
    if (enable) {
        if (entity->getType() == ESNT_ANIMATED_MESH) {
            ((IAnimatedMeshSceneNode*)entity)->addShadowVolumeSceneNode();
        } else {
            entity->addShadowVolumeSceneNode();
        }
    }
}


EXPORT bool_t CALL csMeshEntityCastShadows(IMeshSceneNode* entity) {
    int num = csEntityNumChildren(entity);
    for (int i = 0; i < num; ++i) {
        if (csEntityChild(entity, i)->getType() == ESNT_SHADOW_VOLUME) {
            return true;
        }
    }
    return false;
}


EXPORT void CALL csSetMeshEntityLoop(IMeshSceneNode* entity, bool_t loop) {
    if (entity->getType() == ESNT_ANIMATED_MESH ) ((IAnimatedMeshSceneNode*)entity)->setLoopMode(loop);
}


EXPORT bool_t CALL csMeshEntityLoop(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getLoopMode()
        : false;
}


EXPORT void CALL csSetMeshEntityFPS(IMeshSceneNode* entity, float fps) {
    if (entity->getType() == ESNT_ANIMATED_MESH) ((IAnimatedMeshSceneNode*)entity)->setAnimationSpeed(fps);
}


EXPORT float CALL csMeshEntityFPS(IMeshSceneNode* entity) {
  return (entity->getType() == ESNT_ANIMATED_MESH)
    ? ((IAnimatedMeshSceneNode*)entity)->getAnimationSpeed()
    : 0;
}


EXPORT void CALL csSetMeshEntityFrame(IMeshSceneNode* entity, float frame) {
    if (entity->getType() == ESNT_ANIMATED_MESH) ((IAnimatedMeshSceneNode*)entity)->setCurrentFrame(frame);
}


EXPORT float CALL csMeshEntityFrame(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getFrameNr()
        : -1;
}


EXPORT void CALL csSetMeshEntityFrames(IMeshSceneNode* entity, int first, int last) {
    if (entity->getType() == ESNT_ANIMATED_MESH) ((IAnimatedMeshSceneNode*)entity)->setFrameLoop(first, last);
}


EXPORT int CALL csMeshEntityFirstFrame(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getStartFrame()
        : -1;
}


EXPORT int CALL csMeshEntityLastFrame(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getEndFrame()
        : -1;
}


} // extern "C"
