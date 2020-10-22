#include "core.h"
#include "graphics.h"
#include "mesh.h"

extern "C" {


EXPORT IMesh* CALL CreateCone(int segments) {
    return _Device()->getSceneManager()->getGeometryCreator()->createConeMesh(0.5f, 1, segments);
}


EXPORT IMesh* CALL CreateCube() {
    return _Device()->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(1, 1, 1));
}


EXPORT IMesh* CALL CreateCylinder(int segments) {
    return _Device()->getSceneManager()->getGeometryCreator()->createCylinderMesh(0.5f, 1, segments);
}


EXPORT IMesh* CALL CreateMesh() {
    return new SMesh();
}


EXPORT IMesh* CALL CreateQuad() {
    IMesh* quad = _Device()->getSceneManager()->getGeometryCreator()->createPlaneMesh(dimension2df(1, 1));
    RotateMesh(quad, -90, 0, 0);
    return quad;
}


EXPORT IMesh* CALL CreateSphere(int segments) {
    return _Device()->getSceneManager()->getGeometryCreator()->createSphereMesh(0.5f, segments, segments);
}


EXPORT IMesh* CALL LoadMesh(const char* filename) {
    return _Device()->getSceneManager()->getMesh(filename);
}


EXPORT void CALL FreeMesh(IMesh* mesh) {
    if (_MeshAnimated(mesh)) {
        // Animated meshes are the only cached ones
        _Device()->getSceneManager()->getMeshCache()->removeMesh(mesh);
    } else {
        mesh->drop();
    }
}


EXPORT void CALL SetMeshFPS(IMesh* mesh, float fps) {
    if (_MeshAnimated(mesh)) {
        ((IAnimatedMesh*)mesh)->setAnimationSpeed(fps);
    }
}


EXPORT float CALL MeshFPS(IMesh* mesh) {
    return (_MeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getAnimationSpeed()
        : 0;
}


EXPORT int CALL MeshFrames(IMesh* mesh) {
    return (_MeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getFrameCount()
        : 1;
}


EXPORT IMesh* CALL MeshForFrame(IMesh* mesh, int frame) {
    return (_MeshAnimated(mesh))
        ? ((IAnimatedMesh*)mesh)->getMesh(frame)
        : mesh;
}


EXPORT int CALL NumSurfaces(IMesh* mesh) {
    return mesh->getMeshBufferCount();
}


EXPORT IMeshBuffer* CALL MeshSurface(IMesh* mesh, int index) {
    return mesh->getMeshBuffer(index);
}


EXPORT void CALL UpdateMesh(IMesh* mesh) {
    for (int i = 0; i < NumSurfaces(mesh); ++i) {
        MeshSurface(mesh, i)->recalculateBoundingBox();
    }
    ((SMesh*)mesh)->recalculateBoundingBox();
    mesh->setDirty();
}


EXPORT void CALL TranslateMesh(IMesh* mesh, float x, float y, float z) {
    matrix4 m;
    m.setTranslation(vector3df(x, y, z));
    _Device()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}


EXPORT void CALL RotateMesh(IMesh* mesh, float pitch, float yaw, float roll) {
    matrix4 m;
    m.setRotationDegrees(vector3df(pitch, yaw, roll));
    _Device()->getSceneManager()->getMeshManipulator()->transform(mesh, m);
}


EXPORT void CALL ScaleMesh(IMesh* mesh, float x, float y, float z) {
    _Device()->getSceneManager()->getMeshManipulator()->scale(mesh, vector3df(x, y, z));
}


EXPORT void CALL FlipMesh(IMesh* mesh) {
    _Device()->getSceneManager()->getMeshManipulator()->flipSurfaces(mesh);
}


EXPORT void CALL SetMeshColor(IMesh* mesh, int color) {
    _Device()->getSceneManager()->getMeshManipulator()->setVertexColors(mesh, _Color(color));
}


EXPORT void CALL UpdateMeshNormals(IMesh* mesh) {
    _Device()->getSceneManager()->getMeshManipulator()->recalculateNormals(mesh);
}


EXPORT float CALL MeshWidth(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().X;
}


EXPORT float CALL MeshHeight(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().Y;
}


EXPORT float CALL MeshDepth(IMesh* mesh) {
    return mesh->getBoundingBox().getExtent().Z;
}


bool_t _MeshAnimated(IMesh* mesh) {
    return _Device()->getSceneManager()->getMeshCache()->getMeshIndex(mesh) != -1;
}


} // extern "C"
