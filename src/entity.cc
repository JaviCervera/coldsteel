#include "core.h"
#include "entity.h"
#include "math.h"


static map<ITriangleSelector*, int> _colliders;


extern "C" {


EXPORT ISceneNode* CALL asCreateEntity() {
    return _asDevice()->getSceneManager()->addEmptySceneNode();
}


EXPORT void CALL asFreeEntity(ISceneNode* entity) {
    asSetEntityCollision(entity, ASCOLLISION_NONE, 0);
    entity->remove();
}


EXPORT int CALL asEntityType(ISceneNode* entity) {
    map<ESCENE_NODE_TYPE, int> types;
    types.set(ESNT_EMPTY, ASENTITY_EMPTY);
    types.set(ESNT_BILLBOARD, ASENTITY_SPRITE);
    types.set(ESNT_CAMERA, ASENTITY_CAMERA);
    types.set(ESNT_PARTICLE_SYSTEM, ASENTITY_EMITTER);
    types.set(ESNT_LIGHT, ASENTITY_LIGHT);
    types.set(ESNT_MESH, ASENTITY_MESH);
    types.set(ESNT_ANIMATED_MESH, ASENTITY_MESH);
    types.set(ESNT_OCTREE, ASENTITY_MESH_OCTREE);
    types.set(ESNT_TERRAIN, ASENTITY_TERRAIN);
    types.set(ESNT_WATER_SURFACE, ASENTITY_WATER);
    return types[entity->getType()];
}


EXPORT void CALL asSetEntityName(ISceneNode* entity, const char* name) {
    entity->setName(name);
}


EXPORT const char* CALL asEntityName(ISceneNode* entity) {
    return entity->getName();
}


EXPORT void CALL asSetEntityVisible(ISceneNode* entity, bool_t visible) {
    entity->setVisible(visible);
}


EXPORT bool_t CALL asEntityVisible(ISceneNode* entity) {
    return entity->isVisible();
}


EXPORT void CALL asSetEntityParent(ISceneNode* entity, ISceneNode* parent) {
    entity->setParent(parent);
}


EXPORT ISceneNode* CALL asEntityParent(ISceneNode* entity) {
    return entity->getParent();
}


EXPORT int CALL asEntityNumChildren(ISceneNode* entity) {
    return entity->getChildren().size();
}


EXPORT ISceneNode* CALL asEntityChild(ISceneNode* entity, int index) {
    return *(entity->getChildren().begin() + index);
}


EXPORT void CALL asSetEntityPosition(ISceneNode* entity, float x, float y, float z) {
    entity->setPosition(vector3df(x, y, z));
    entity->updateAbsolutePosition();
}


EXPORT void CALL asMoveEntity(ISceneNode* entity, float x, float y, float z) {
    vector3df dest(x, y, z);
    entity->getRelativeTransformation().transformVect(dest);
    entity->setPosition(dest);
    entity->updateAbsolutePosition();
}


EXPORT ISceneNode* CALL asSlideEntity(ISceneNode* entity, float x, float y, float z, float radiusX, float radiusY, float radiusZ, int group) {
    ISceneCollisionManager* manager = _asDevice()->getSceneManager()->getSceneCollisionManager();
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
                asSetEntityPosition(entity, result.X, result.Y, result.Z);
                _asSetPoint(collisionPoint.X, collisionPoint.Y, collisionPoint.Z);
                _asSetNormal(normal.X, normal.Y, normal.Z);
                break;
            }
        }
    }
    if (collidedEntity == NULL) asMoveEntity(entity, x, y, z);
    return collidedEntity;
}


EXPORT float CALL asEntityX(ISceneNode* entity) {
    return entity->getAbsolutePosition().X;
}


EXPORT float CALL asEntityY(ISceneNode* entity) {
    return entity->getAbsolutePosition().Y;
}


EXPORT float CALL asEntityZ(ISceneNode* entity) {
    return entity->getAbsolutePosition().Z;
}


EXPORT float CALL asEntityLocalX(ISceneNode* entity) {
    return entity->getPosition().X;
}


EXPORT float CALL asEntityLocalY(ISceneNode* entity) {
    return entity->getPosition().Y;
}


EXPORT float CALL asEntityLocalZ(ISceneNode* entity) {
    return entity->getPosition().Z;
}


EXPORT void CALL asSetEntityRotation(ISceneNode* entity, float pitch, float yaw, float roll) {
    entity->setRotation(vector3df(pitch, yaw, roll));
}


EXPORT void CALL asTurnEntity(ISceneNode* entity, float pitch, float yaw, float roll) {
    entity->setRotation(entity->getRotation() + vector3df(pitch, yaw, roll));
}


EXPORT void CALL asPointEntity(ISceneNode* entity, float x, float y, float z) {
    const float xdiff = asEntityX(entity) - x;
    const float ydiff = asEntityY(entity) - y;
    const float zdiff = asEntityZ(entity) - z;
    const float sqdist = asSqr(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff);
    const float pitch = asATan2(ydiff, sqdist);
    const float yaw = asATan2(xdiff, -zdiff);
    asSetEntityRotation(entity, pitch, yaw, asEntityRoll(entity));
}


EXPORT float CALL asEntityPitch(ISceneNode* entity) {
    return entity->getRotation().X;
}


EXPORT float CALL asEntityYaw(ISceneNode* entity) {
    return entity->getRotation().Y;
}


EXPORT float CALL asEntityRoll(ISceneNode* entity) {
    return entity->getRotation().Z;
}


EXPORT void CALL asSetEntityScale(ISceneNode* entity, float x, float y, float z) {
    entity->setScale(vector3df(x, y, z));
}


EXPORT float CALL asEntityScaleX(ISceneNode* entity) {
    return entity->getScale().X;
}


EXPORT float CALL asEntityScaleY(ISceneNode* entity) {
    return entity->getScale().Y;
}


EXPORT float CALL asEntityScaleZ(ISceneNode* entity) {
    return entity->getScale().Z;
}


EXPORT float CALL asEntityWidth(ISceneNode* entity) {
    return entity->getBoundingBox().getExtent().X;
}


EXPORT float CALL asEntityHeight(ISceneNode* entity) {
    return entity->getBoundingBox().getExtent().Y;
}


EXPORT float CALL asEntityDepth(ISceneNode* entity) {
    return entity->getBoundingBox().getExtent().Z;
}


EXPORT int CALL asEntityNumMaterials(ISceneNode* entity) {
    return entity->getMaterialCount();
}


EXPORT SMaterial* CALL asEntityMaterial(ISceneNode* entity, int index) {
    return &entity->getMaterial(index);
}


EXPORT void CALL asSetEntityCollision(ISceneNode* entity, int type, int group) {
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
        case ASCOLLISION_BOX:
            selector = _asDevice()->getSceneManager()->createTriangleSelectorFromBoundingBox(entity);
            break;
        case ASCOLLISION_MESH:
            selector =
                animNode ? _asDevice()->getSceneManager()->createTriangleSelector(animNode) : 
                meshNode ? _asDevice()->getSceneManager()->createTriangleSelector(meshNode->getMesh(), meshNode) :
                NULL;
            break;
        case ASCOLLISION_OCTREE:
            selector =
                animNode ? _asDevice()->getSceneManager()->createOctreeTriangleSelector(animNode->getMesh()->getMesh(0), animNode) :
                meshNode ? _asDevice()->getSceneManager()->createOctreeTriangleSelector(meshNode->getMesh(), meshNode) :
                NULL;
    }
    if (selector != NULL) {
        _colliders.set(selector, group);
        entity->setID(group);
        entity->setTriangleSelector(selector);
        selector->drop();
    }
}


EXPORT bool_t CALL asEntityCollision(ISceneNode* entity) {
    return entity->getTriangleSelector() != NULL;
}


EXPORT int CALL asEntityGroup(ISceneNode* entity) {
    return entity->getID();
}


} // extern "C"
