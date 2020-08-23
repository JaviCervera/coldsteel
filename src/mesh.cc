#include "core.h"
#include "graphics.h"
#include "mesh.h"

extern "C" {


EXPORT IMesh* CALL csCreateCone(int segments) {
    return _csDevice()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments);
}


EXPORT IMesh* CALL csCreateCube() {
    return _csDevice()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1));
}


EXPORT IMesh* CALL csCreateCylinder(int segments) {
    return _csDevice()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments);
}


EXPORT IMesh* CALL csCreateMesh() {
    return new SMesh();
}


EXPORT IMesh* CALL csCreateQuad() {
    IMesh* quad = _csDevice()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1));
    csRotateMesh(quad, -90, 0, 0);
    return quad;
}


EXPORT IMesh* CALL csCreateSphere(int segments) {
    return _csDevice()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments);
}


EXPORT IMesh* CALL csLoadMesh(const char* filename) {
    return _csDevice()->getSceneManager()->getMesh(filename);
}


EXPORT void CALL csFreeMesh(IMesh* mesh) {
    if (_csMeshAnimated(mesh)) {
        // Animated meshes are the only cached ones
        _csDevice()->getSceneManager()->getMeshCache()->removeMesh(mesh);
    } else {
        mesh->drop();
    }
}


EXPORT void CALL csSetMeshFPS(IMesh* mesh, float fps) {
    if (_csMeshAnimated(mesh)) {
        ((IAnimatedMesh*)mesh)->setAnimationSpeed(fps);
    }
}


EXPORT float CALL csMeshFPS(IMesh* mesh) {
    return (_csMeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getAnimationSpeed()
        : 0;
}


EXPORT int CALL csMeshFrames(IMesh* mesh) {
    return (_csMeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getFrameCount()
        : 1;
}


EXPORT IMesh* CALL csMeshForFrame(IMesh* mesh, int frame) {
    return (_csMeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getMesh(frame)
        : mesh;
}


EXPORT int CALL csNumSurfaces(IMesh* mesh) {
    return mesh->getMeshBufferCount();
}


EXPORT IMeshBuffer* CALL csMeshSurface(IMesh* mesh, int index) {
    return mesh->getMeshBuffer(index);
}


EXPORT void CALL csUpdateMesh(IMesh* mesh) {
    for (int i = 0; i < csNumSurfaces(mesh); ++i) {
        csMeshSurface(mesh, i)->recalculateBoundingBox();
    }
    ((SMesh*)mesh)->recalculateBoundingBox();
    mesh->setDirty();
}


EXPORT void CALL csTranslateMesh(IMesh* mesh, float x, float y, float z) {
    matrix4 m;
    m.setTranslation(vector3df(x, y, z));
    _csDevice()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}


EXPORT void CALL csRotateMesh(IMesh* mesh, float pitch, float yaw, float roll) {
    matrix4 m;
    m.setRotationDegrees(vector3df(pitch, yaw, roll));
    _csDevice()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}


EXPORT void CALL csScaleMesh(IMesh* mesh, float x, float y, float z) {
    _csDevice()->getSceneManager()->getMeshManipulator()->scale(mesh, vector3df(x, y, z));
}


EXPORT void CALL csFlipMesh(IMesh* mesh) {
    _csDevice()->getSceneManager()->getMeshManipulator()->flipSurfaces(mesh);
}


EXPORT void CALL csSetMeshColor(IMesh* mesh, int color) {
    _csDevice()->getSceneManager()->getMeshManipulator()->setVertexColors(mesh, _csColor(color));
}


EXPORT void CALL csUpdateMeshNormals(IMesh* mesh) {
    _csDevice()->getSceneManager()->getMeshManipulator()->recalculateNormals(mesh);
}


EXPORT float CALL csMeshWidth(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().X;
}


EXPORT float CALL csMeshHeight(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().Y;
}


EXPORT float CALL csMeshDepth(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().Z;
}


bool_t _csMeshAnimated(IMesh* mesh) {
    return _csDevice()->getSceneManager()->getMeshCache()->getMeshIndex(mesh) != -1;
}


} // extern "C"
