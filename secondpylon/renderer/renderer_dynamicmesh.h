// Copyright 2009 Brian Legge

#ifndef SECONDPYLON_RENDERER_RENDERER_DYNAMICMESH_H_
#define SECONDPYLON_RENDERER_RENDERER_DYNAMICMESH_H_

#include "secondpylon/plat/plat_types.h"
#include "secondpylon/plat/plat_crt.h"
#include "secondpylon/math/math_vec3.h"

struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct IDirect3DVertexDeclaration9;
struct IDirect3DDevice9;

namespace secondpylon {
namespace renderer {

class DynamicMesh {
public:
    // @todo Long term, the Vertex implementation will probably be
    // externalized (template parameter or dynamic binding?) so that we can
    // have different mesh formats.
    struct SVertex {
        // Vertex implementations provide a 'Write' function to insure writes
        // are ordered. This avoids penalties on platforms where out of order
        // writes to video memory cause penalties.
        void Write(const math::vec3<float>& position) {
            Position_ = position;
        }

        math::vec3<float> Position_;
    };

    DynamicMesh();
    ~DynamicMesh();

    bool Create(
        IDirect3DDevice9* device
        , plat::uint32 nVertexCount
        , plat::uint32 nIndexCount);

    plat::uint32* LockIndices(plat::uint32 nIndices);
    void UnlockIndices();
    IDirect3DVertexBuffer9* GetVertices() { return VertexBuffer_; }

    plat::uint32* LockVertices(plat::uint32 nVertexCount);
    void UnlockVertices();
    IDirect3DIndexBuffer9* GetIndices() { return IndexBuffer_; }

    IDirect3DVertexDeclaration9* GetVertexDecl() const;

    plat::uint32 GetVertexStride() const;
    plat::uint32 GetVertexCount() const;
    plat::uint32 GetIndexCount() const;

    void Destroy() { delete this; }

private:
    SPUNCOPYABLE(DynamicMesh);

    IDirect3DVertexBuffer9* VertexBuffer_;
    IDirect3DIndexBuffer9* IndexBuffer_;
    IDirect3DVertexDeclaration9* pVertexDeclaration_;
    plat::uint32 nVertexCount_;
    plat::uint32 nIndexCount_;
};

}  // namespace renderer
}  // namespace secondpylon

#endif  // SECONDPYLON_RENDERER_RENDERER_DYNAMICMESH_H_
