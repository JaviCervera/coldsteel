#include "core.h"
#include "graphics.h"
#include "mesh.h"

extern "C" {


EXPORT IMesh* CALL asCreateCone(int segments) {
    return _asDevice()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments);
}


EXPORT IMesh* CALL asCreateCube() {
    return _asDevice()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1));
}


EXPORT IMesh* CALL asCreateCylinder(int segments) {
    return _asDevice()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments);
}


EXPORT IMesh* CALL asCreateMesh() {
    return new SMesh();
}


EXPORT IMesh* CALL asCreateQuad() {
    IMesh* quad = _asDevice()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1));
    asRotateMesh(quad, -90, 0, 0);
    return quad;
}


EXPORT IMesh* CALL asCreateSphere(int segments) {
    return _asDevice()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments);
}


EXPORT IMesh* CALL asLoadMesh(const char* filename) {
    return _asDevice()->getSceneManager()->getMesh(filename);
}


EXPORT void CALL asFreeMesh(IMesh* mesh) {
    if (_asMeshAnimated(mesh)) {
        // Animated meshes are the only cached ones
        _asDevice()->getSceneManager()->getMeshCache()->removeMesh(mesh);
    } else {
        mesh->drop();
    }
}


EXPORT void CALL asSetMeshFPS(IMesh* mesh, float fps) {
    if (_asMeshAnimated(mesh)) {
        ((IAnimatedMesh*)mesh)->setAnimationSpeed(fps);
    }
}


EXPORT float CALL asMeshFPS(IMesh* mesh) {
    return (_asMeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getAnimationSpeed()
        : 0;
}


EXPORT int CALL asMeshFrames(IMesh* mesh) {
    return (_asMeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getFrameCount()
        : 1;
}


EXPORT IMesh* CALL asMeshForFrame(IMesh* mesh, int frame) {
    return (_asMeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getMesh(frame)
        : mesh;
}


EXPORT int CALL asNumMeshSurfaces(IMesh* mesh) {
    return mesh->getMeshBufferCount();
}


EXPORT IMeshBuffer* CALL asMeshSurface(IMesh* mesh, int index) {
    return mesh->getMeshBuffer(index);
}


EXPORT void CALL asUpdateMesh(IMesh* mesh) {
    mesh->setDirty();
    ((SMesh*)mesh)->recalculateBoundingBox();
}


EXPORT void CALL asTranslateMesh(IMesh* mesh, float x, float y, float z) {
    matrix4 m;
    m.setTranslation(vector3df(x, y, z));
    _asDevice()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}


EXPORT void CALL asRotateMesh(IMesh* mesh, float pitch, float yaw, float roll) {
    matrix4 m;
    m.setRotationDegrees(vector3df(pitch, yaw, roll));
    _asDevice()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}


EXPORT void CALL asScaleMesh(IMesh* mesh, float x, float y, float z) {
    _asDevice()->getSceneManager()->getMeshManipulator()->scale(mesh, vector3df(x, y, z));
}


EXPORT void CALL asFlipMesh(IMesh* mesh) {
    _asDevice()->getSceneManager()->getMeshManipulator()->flipSurfaces(mesh);
}


EXPORT void CALL asSetMeshColor(IMesh* mesh, int color) {
    _asDevice()->getSceneManager()->getMeshManipulator()->setVertexColors(mesh, _asColor(color));
}


EXPORT void CALL asUpdateMeshNormals(IMesh* mesh) {
    _asDevice()->getSceneManager()->getMeshManipulator()->recalculateNormals(mesh);
}


EXPORT float CALL asMeshWidth(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().X;
}


EXPORT float CALL asMeshHeight(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().Y;
}


EXPORT float CALL asMeshDepth(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().Z;
}


bool_t _asMeshAnimated(IMesh* mesh) {
    return _asDevice()->getSceneManager()->getMeshCache()->getMeshIndex(mesh) != -1;
}


} // extern "C"
