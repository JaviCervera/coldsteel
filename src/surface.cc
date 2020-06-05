#include "graphics.h"
#include "surface.h"

extern "C" {


EXPORT IMeshBuffer* CALL asAddSurface(IMesh* mesh) {
    IMeshBuffer* buffer = new SMeshBuffer();
    ((SMesh*)mesh)->addMeshBuffer(buffer);
    buffer->drop();
    return buffer;
}


EXPORT int CALL asAddIndex(IMeshBuffer* surface, int index) {
    u16 sindex = index;
    surface->append(NULL, 0, (surface->getIndexType() == EIT_16BIT) ? &sindex : (const u16*)&index, 1);
    return asNumIndices(surface) - 1;
}


EXPORT int CALL asNumIndices(IMeshBuffer* surface) {
    return surface->getIndexCount();
}


EXPORT int CALL asSurfaceIndex(IMeshBuffer* surface, int number) {
    return (surface->getIndexType() == EIT_16BIT)
        ? surface->getIndices()[number]
        : ((u32*)surface->getIndices())[number];
}


EXPORT int CALL asAddVertex(IMeshBuffer* surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v) {
    switch (surface->getVertexType()) {
        case EVT_STANDARD: {
            S3DVertex vertex(x, y, z, nx, ny, nz, _asColor(color), u, v);
            surface->append(&vertex, 1, NULL, 0);
            break;
        } case EVT_2TCOORDS: {
            S3DVertex2TCoords vertex(x, y, z, nx, ny, nz, _asColor(color), u, v);
            surface->append(&vertex, 1, NULL, 0);
            break;
        } case EVT_TANGENTS: {
            S3DVertexTangents vertex(x, y, z, nx, ny, nz, _asColor(color), u, v);
            surface->append(&vertex, 1, NULL, 0);
            break;
        }
    }
    return asNumVertices(surface) - 1;
}


EXPORT int CALL asNumVertices(IMeshBuffer* surface) {
    return surface->getVertexCount();
}


EXPORT float CALL asVertexX(IMeshBuffer* surface, int index) {
    return surface->getPosition(index).X;
}


EXPORT float CALL asVertexY(IMeshBuffer* surface, int index) {
    return surface->getPosition(index).Y;
}


EXPORT float CALL asVertexZ(IMeshBuffer* surface, int index) {
    return surface->getPosition(index).Z;
}


EXPORT float CALL asVertexNX(IMeshBuffer* surface, int index) {
    return surface->getNormal(index).X;
}


EXPORT float CALL asVertexNY(IMeshBuffer* surface, int index) {
    return surface->getNormal(index).Y;
}


EXPORT float CALL asVertexNZ(IMeshBuffer* surface, int index) {
    return surface->getNormal(index).Z;
}


EXPORT int CALL asVertexColor(IMeshBuffer* surface, int index) {
    switch (surface->getVertexType()) {
        case EVT_STANDARD:
            return _asIntColor(((S3DVertex*)surface->getVertices())[index].Color);
        case EVT_2TCOORDS:
            return _asIntColor(((S3DVertex2TCoords*)surface->getVertices())[index].Color);
        case EVT_TANGENTS:
            return _asIntColor(((S3DVertexTangents*)surface->getVertices())[index].Color);
    }
    return 0;
}


EXPORT float CALL asVertexU(IMeshBuffer* surface, int index) {
    return surface->getTCoords(index).X;
}


EXPORT float CALL asVertexV(IMeshBuffer* surface, int index) {
    return surface->getTCoords(index).Y;
}


} // extern "C"
