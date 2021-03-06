// Copyright 2009 Brian Legge

#include "secondpylon/renderer/renderer_texture.h"
#include <d3d9.h>
#include "secondpylon/plat/plat_crt.h"
#include "./renderer_utils.h"

namespace secondpylon {
namespace renderer {

Texture::Texture(IDirect3DTexture9* texture
         , const math::vec2<plat::uint32>& size)
         : texture_(texture)
         , size_(size)
         , locked_(NULL) {
}

Texture::~Texture() {
  SafeRelease(texture_);
}

plat::uint32* Texture::Lock() {
  SPDIAG_ASSERT(NULL == locked_, "Only unlocked texture may be locked.");
  D3DLOCKED_RECT rect;
  texture_->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);

  // This assumes the texture block is 4 byte aligned.
  SPDIAG_ASSERT((uintptr_t)rect.pBits % sizeof(plat::uint32) == 0
    , "Texture block must be 4 byte aligned (%d)"
    , (uintptr_t)rect.pBits % sizeof(plat::uint32));

  locked_ = StrictCast<plat::uint32*>(rect.pBits);
  return locked_;
}

void Texture::Unlock(plat::uint32* lock_pointer) {
  SPDIAG_ASSERT(NULL != locked_, "Texture must be locked to call Unlock");

  SPDIAG_ASSERT(lock_pointer == locked_
    , "Passing an invalid pointer to pLockPointer");

  texture_->UnlockRect(0);
  locked_ = NULL;
}

}  // namespace renderer
}  // namespace secondpylon
