#include "core.h"
#include "entity.h"
#include "math.h"


static map<ITriangleSelector*, int> _colliders;


extern "C" {


EXPORT ISceneNode* CALL csCreateEntity() {
    return _csDevice()->getSceneManager()->addEmptySceneNode();
}


EXPORT void CALL csFreeEntity(ISceneNode* entity) {
    csSetEntityCollision(entity, CSCOLLISION_NONE, 0);
    entity->remove();
}


EXPORT int CALL csEntityType(ISceneNode* entity) {
    map<ESCENE_NODE_TYPE, int> types;
    types.set(ESNT_EMPTY, CSENTITY_EMPTY);
    types.set(ESNT_BILLBOARD, CSENTITY_SPRITE);
    types.set(ESNT_CAMERA, CSENTITY_CAMERA);
    types.set(ESNT_PARTICLE_SYSTEM, CSENTITY_EMITTER);
    types.set(ESNT_LIGHT, CSENTITY_LIGHT);
    types.set(ESNT_MESH, CSENTITY_MESH);
    types.set(ESNT_ANIMATED_MESH, CSENTITY_MESH);
    types.set(ESNT_OCTREE, CSENTITY_MESH_OCTREE);
    types.set(ESNT_TERRAIN, CSENTITY_TERRAIN);
    types.set(ESNT_WATER_SURFACE, CSENTITY_WATER);
    return types[entity->getType()];
}


EXPORT void CALL csSetEntityName(ISceneNode* entity, const char* name) {
    entity->setName(name);
}


EXPORT const char* CALL csEntityName(ISceneNode* entity) {
    return entity->getName();
}


EXPORT void CALL csSetEntityVisible(ISceneNode* entity, bool_t visible) {
    entity->setVisible(visible);
}


EXPORT bool_t CALL csEntityVisible(ISceneNode* entity) {
    return entity->isVisible();
}


EXPORT void CALL csSetEntityParent(ISceneNode* entity, ISceneNode* parent) {
    entity->setParent(parent);
}


EXPORT ISceneNode* CALL csEntityParent(ISceneNode* entity) {
    return entity->getParent();
}


EXPORT int CALL csEntityNumChildren(ISceneNode* entity) {
    return entity->getChildren().size();
}


EXPORT ISceneNode* CALL csEntityChild(ISceneNode* entity, int index) {
    return *(entity->getChildren().begin() + index);
}


EXPORT void CALL csSetEntityPosition(ISceneNode* entity, float x, float y, float z) {
    entity->setPosition(vector3df(x, y, z));
    entity->updateAbsolutePosition();
}


EXPORT void CALL csMoveEntity(ISceneNode* entity, float x, float y, float z) {
    vector3df dest(x, y, z);
    entity->getRelativeTransformation().transformVect(dest);
    entity->setPosition(dest);
    entity->updateAbsolutePosition();
}


EXPORT ISceneNode* CALL csSlideEntity(ISceneNode* entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group) {
    ISceneCollisionManager* manager = _csDevice()->getSceneManager()->getSceneCollisionManager();
    vector3df attempted(x, y, z);
    entity->getAbsoluteTransformation().transformVect(attempted);
    ISceneNode* collidedEntity = NULL;
    for (map<ITriangleSelector*, int>::Iterator it = _colliders.getIterator(); !it.atEnd(); it++) {
        map<ITriangleSelector*, int>::Node* node = it.getNode();
        ITriangleSelector* selector = node->getKey();
        int selectorGroup = node->getValue();
        if ((group & selectorGroup) != 0 && selector != entity->getTriangleSelector()) {
            const vector3df radius(radiusX, radiusY, radiusZ);
            vector3df direction(x, y, z);
            matrix4()
                .setRotationDegrees(entity->getAbsoluteTransformation().getRotationDegrees())
                .transformVect(direction);
            triangle3df collisionTriangle;
            vector3df collisionPoint;
            bool collisionFalling = false;
            const vector3df result = manager->getCollisionResultPosition(
                selector, entity->getAbsolutePosition(),
                radius,
                direction,
                collisionTriangle,
                collisionPoint,
                collisionFalling,
                collidedEntity);
            if (collidedEntity != NULL) {
                const vector3df normal = collisionTriangle.getNormal();
                csSetEntityPosition(entity, result.X, result.Y, result.Z);
                _csSetPoint(collisionPoint.X, collisionPoint.Y, collisionPoint.Z);
                _csSetNormal(normal.X, normal.Y, normal.Z);
                break;
            }
        }
    }
    if (collidedEntity == NULL) csMoveEntity(entity, x, y, z);
    return collidedEntity;
}


EXPORT float CALL csEntityX(ISceneNode* entity) {
    return entity->getAbsolutePosition().X;
}


EXPORT float CALL csEntityY(ISceneNode* entity) {
    return entity->getAbsolutePosition().Y;
}


EXPORT float CALL csEntityZ(ISceneNode* entity) {
    return entity->getAbsolutePosition().Z;
}


EXPORT float CALL csEntityLocalX(ISceneNode* entity) {
    return entity->getPosition().X;
}


EXPORT float CALL csEntityLocalY(ISceneNode* entity) {
    return entity->getPosition().Y;
}


EXPORT float CALL csEntityLocalZ(ISceneNode* entity) {
    return entity->getPosition().Z;
}


EXPORT void CALL csSetEntityRotation(ISceneNode* entity, float pitch, float yaw, float roll) {
    entity->setRotation(vector3df(pitch, yaw, roll));
}


EXPORT void CALL csTurnEntity(ISceneNode* entity, float pitch, float yaw, float roll) {
    entity->setRotation(entity->getRotation() + vector3df(pitch, yaw, roll));
}


EXPORT void CALL csPointEntity(ISceneNode* entity, float x, float y, float z) {
    const float xdiff = csEntityX(entity) - x;
    const float ydiff = csEntityY(entity) - y;
    const float zdiff = csEntityZ(entity) - z;
    const float sqdist = csSqr(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
    const float pitch = csATan2(ydiff, sqdist);
    const float yaw = csATan2(xdiff, -zdiff);
    csSetEntityRotation(entity, pitch, yaw, csEntityRoll(entity));
}


EXPORT float CALL csEntityPitch(ISceneNode* entity) {
    return entity->getRotation().X;
}


EXPORT float CALL csEntityYaw(ISceneNode* entity) {
    return entity->getRotation().Y;
}


EXPORT float CALL csEntityRoll(ISceneNode* entity) {
    return entity->getRotation().Z;
}


EXPORT void CALL csSetEntityScale(ISceneNode* entity, float x, float y, float z) {
    entity->setScale(vector3df(x, y, z));
}


EXPORT float CALL csEntityScaleX(ISceneNode* entity) {
    return entity->getScale().X;
}


EXPORT float CALL csEntityScaleY(ISceneNode* entity) {
    return entity->getScale().Y;
}


EXPORT float CALL csEntityScaleZ(ISceneNode* entity) {
    return entity->getScale().Z;
}


EXPORT float CALL csEntityWidth(ISceneNode* entity) {
    return entity->getBoundingBox().getExtent().X;
}


EXPORT float CALL csEntityHeight(ISceneNode* entity) {
    return entity->getBoundingBox().getExtent().Y;
}


EXPORT float CALL csEntityDepth(ISceneNode* entity) {
    return entity->getBoundingBox().getExtent().Z;
}


EXPORT int CALL csEntityNumMaterials(ISceneNode* entity) {
    return entity->getMaterialCount();
}


EXPORT SMaterial* CALL csEntityMaterial(ISceneNode* entity, int index) {
    return &entity->getMaterial(index);
}


EXPORT void CALL csSetEntityCollision(ISceneNode* entity, int type, int group) {
    ITriangleSelector* selector = NULL;
    IAnimatedMeshSceneNode* animNode = (entity->getType() == ESNT_ANIMATED_MESH)
        ? (IAnimatedMeshSceneNode*)entity
        : NULL;
    IMeshSceneNode* meshNode = (entity->getType() == ESNT_MESH || entity->getType() == ESNT_OCTREE)
        ? (IMeshSceneNode*)entity
        : NULL;
    if (entity->getTriangleSelector()) {
        _colliders.remove(entity->getTriangleSelector());
        entity->setTriangleSelector(NULL);
    }
    switch (type) {
        case CSCOLLISION_BOX:
            selector = _csDevice()->getSceneManager()->createTriangleSelectorFromBoundingBox(entity);
            break;
        case CSCOLLISION_MESH:
            selector =
                animNode ? _csDevice()->getSceneManager()->createTriangleSelector(animNode) : 
                meshNode ? _csDevice()->getSceneManager()->createTriangleSelector(meshNode->getMesh(), meshNode) :
                NULL;
            break;
        case CSCOLLISION_OCTREE:
            selector =
                animNode ? _csDevice()->getSceneManager()->createOctreeTriangleSelector(animNode->getMesh()->getMesh(0), animNode) :
                meshNode ? _csDevice()->getSceneManager()->createOctreeTriangleSelector(meshNode->getMesh(), meshNode) :
                NULL;
    }
    if (selector != NULL) {
        _colliders.set(selector, group);
        entity->setID(group);
        entity->setTriangleSelector(selector);
        selector->drop();
    }
}


EXPORT bool_t CALL csEntityCollision(ISceneNode* entity) {
    return entity->getTriangleSelector() != NULL;
}


EXPORT int CALL csEntityGroup(ISceneNode* entity) {
    return entity->getID();
}


} // extern "C"
