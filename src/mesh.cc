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


EXPORT IMesh* CALL asCreateSphere(int segments) {
    return _asDevice()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments);
}


EXPORT IMesh* CALL asLoadMesh(const char* filename) {
    return _asDevice()->getSceneManager()->getMesh(filename);
}


EXPORT void CALL asDeleteMesh(IMesh* mesh) {
    if (_asDevice()->getSceneManager()->getMeshCache()->getMeshIndex(mesh) != -1) {
        _asDevice()->getSceneManager()->getMeshCache()->removeMesh(mesh);
    } else {
        mesh->drop();
    }
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


} // extern "C"
