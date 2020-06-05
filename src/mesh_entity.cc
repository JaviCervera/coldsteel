#include "core.h"
#include "entity.h"
#include "mesh.h"
#include "mesh_entity.h"

extern "C" {


EXPORT IMeshSceneNode* CALL asCreateMeshEntity(IMesh* mesh) {
    if (_asMeshAnimated(mesh)) {
        return (IMeshSceneNode*)_asDevice()->getSceneManager()->addAnimatedMeshSceneNode((IAnimatedMesh*)mesh);
    } else {
        return _asDevice()->getSceneManager()->addMeshSceneNode(mesh);
    }
}


EXPORT IMeshSceneNode* CALL asCreateOctreeMeshEntity(IMesh* mesh) {
    return _asDevice()->getSceneManager()->addOctreeSceneNode(mesh);
}


EXPORT IMesh* CALL asMeshEntityMesh(IMeshSceneNode* entity) {
    if (entity->getType() == ESNT_ANIMATED_MESH) {
        return ((IAnimatedMeshSceneNode*)entity)->getMesh();
    } else {
        return entity->getMesh();
    }
}


EXPORT void CALL asSetMeshEntityCastShadows(IMeshSceneNode* entity, bool_t enable) {
    int num = asEntityNumChildren(entity);
    ISceneNode* shadowNode = NULL;
    for (int i = 0; i < num; ++i) {
        ISceneNode* child = asEntityChild(entity, i);
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


EXPORT bool_t CALL asMeshEntityCastShadows(IMeshSceneNode* entity) {
    int num = asEntityNumChildren(entity);
    for (int i = 0; i < num; ++i) {
        if (asEntityChild(entity, i)->getType() == ESNT_SHADOW_VOLUME) {
            return true;
        }
    }
    return false;
}


EXPORT void CALL asSetMeshEntityLoop(IMeshSceneNode* entity, bool_t loop) {
    if (entity->getType() == ESNT_ANIMATED_MESH ) ((IAnimatedMeshSceneNode*)entity)->setLoopMode(loop);
}


EXPORT bool_t CALL asMeshEntityLoop(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getLoopMode()
        : false;
}


EXPORT void CALL asSetMeshEntityFPS(IMeshSceneNode* entity, float fps) {
    if (entity->getType() == ESNT_ANIMATED_MESH) ((IAnimatedMeshSceneNode*)entity)->setAnimationSpeed(fps);
}


EXPORT float CALL asMeshEntityFPS(IMeshSceneNode* entity) {
  return (entity->getType() == ESNT_ANIMATED_MESH)
    ? ((IAnimatedMeshSceneNode*)entity)->getAnimationSpeed()
    : 0;
}


EXPORT void CALL asSetMeshEntityFrame(IMeshSceneNode* entity, float frame) {
    if (entity->getType() == ESNT_ANIMATED_MESH) ((IAnimatedMeshSceneNode*)entity)->setCurrentFrame(frame);
}


EXPORT float CALL asMeshEntityFrame(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getFrameNr()
        : -1;
}


EXPORT void CALL asSetMeshEntityFrames(IMeshSceneNode* entity, int first, int last) {
    if (entity->getType() == ESNT_ANIMATED_MESH) ((IAnimatedMeshSceneNode*)entity)->setFrameLoop(first, last);
}


EXPORT int CALL asMeshEntityFirstFrame(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getStartFrame()
        : -1;
}


EXPORT int CALL asMeshEntityLastFrame(IMeshSceneNode* entity) {
    return (entity->getType() == ESNT_ANIMATED_MESH)
        ? ((IAnimatedMeshSceneNode*)entity)->getEndFrame()
        : -1;
}


} // extern "C"
