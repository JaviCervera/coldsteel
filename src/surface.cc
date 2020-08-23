#include "graphics.h"
#include "surface.h"

extern "C" {


EXPORT IMeshBuffer* CALL csAddSurface(IMesh* mesh) {
    IMeshBuffer* buffer = new SMeshBuffer();
    ((SMesh*)mesh)->addMeshBuffer(buffer);
    buffer->drop();
    return buffer;
}


EXPORT int CALL csAddIndex(IMeshBuffer* surface, int index) {
    int lastIndex = csNumIndices(surface);
    surface->append(NULL, 0, (const u16*)&index, 1);
    surface->getIndices()[lastIndex] = index;
    return lastIndex;
}


EXPORT int CALL csNumIndices(IMeshBuffer* surface) {
    return surface->getIndexCount();
}


EXPORT int CALL csSurfaceIndex(IMeshBuffer* surface, int number) {
    return (surface->getIndexType() == EIT_16BIT)
        ? surface->getIndices()[number]
        : ((u32*)surface->getIndices())[number];
}


EXPORT int CALL csAddVertex(IMeshBuffer* surface, float x, float y, float z, float nx, float ny, float nz, int color, float u, float v) {
    switch (surface->getVertexType()) {
        case EVT_STANDARD: {
            S3DVertex vertex(x, y, z, nx, ny, nz, _csColor(color), u, v);
            surface->append(&vertex, 1, NULL, 0);
            break;
        } case EVT_2TCOORDS: {
            S3DVertex2TCoords vertex(x, y, z, nx, ny, nz, _csColor(color), u, v);
            surface->append(&vertex, 1, NULL, 0);
            break;
        } case EVT_TANGENTS: {
            S3DVertexTangents vertex(x, y, z, nx, ny, nz, _csColor(color), u, v);
            surface->append(&vertex, 1, NULL, 0);
            break;
        }
    }
    return csNumVertices(surface) - 1;
}


EXPORT int CALL csNumVertices(IMeshBuffer* surface) {
    return surface->getVertexCount();
}


EXPORT float CALL csVertexX(IMeshBuffer* surface, int index) {
    return surface->getPosition(index).X;
}


EXPORT float CALL csVertexY(IMeshBuffer* surface, int index) {
    return surface->getPosition(index).Y;
}


EXPORT float CALL csVertexZ(IMeshBuffer* surface, int index) {
    return surface->getPosition(index).Z;
}


EXPORT float CALL csVertexNX(IMeshBuffer* surface, int index) {
    return surface->getNormal(index).X;
}


EXPORT float CALL csVertexNY(IMeshBuffer* surface, int index) {
    return surface->getNormal(index).Y;
}


EXPORT float CALL csVertexNZ(IMeshBuffer* surface, int index) {
    return surface->getNormal(index).Z;
}


EXPORT int CALL csVertexColor(IMeshBuffer* surface, int index) {
    switch (surface->getVertexType()) {
        case EVT_STANDARD:
            return _csIntColor(((S3DVertex*)surface->getVertices())[index].Color);
        case EVT_2TCOORDS:
            return _csIntColor(((S3DVertex2TCoords*)surface->getVertices())[index].Color);
        case EVT_TANGENTS:
            return _csIntColor(((S3DVertexTangents*)surface->getVertices())[index].Color);
    }
    return 0;
}


EXPORT float CALL csVertexU(IMeshBuffer* surface, int index) {
    return surface->getTCoords(index).X;
}


EXPORT float CALL csVertexV(IMeshBuffer* surface, int index) {
    return surface->getTCoords(index).Y;
}


} // extern "C"
