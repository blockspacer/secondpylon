// Copyright 2009 Brian Legge

#include "secondpylon/diag/diag_assert.h"
#include <cstdio>
#include <cstdarg>

namespace  {
    class DefaultAssertHandler : public secondpylon::diag::IAssertHandler {
    public:
        secondpylon::diag::AssertSystem::EAssertAction OnAssert(
            const char* error
            , const char* message) {
            SPDIAG_UNREFERENCED(error);
            SPDIAG_UNREFERENCED(message);
            return secondpylon::diag::AssertSystem::kBreak;
        }
    };

}  // namespace unnamed


namespace secondpylon {
namespace diag {

static DefaultAssertHandler s_DefaultHandler;
secondpylon::diag::IAssertHandler* AssertSystem::spHandler_ =
    &s_DefaultHandler;

AssertSystem::EAssertAction AssertSystem::HandleAssert(
    const char* pszAssert
    , const char* format, ...) {
    va_list args;
    va_start(args, format);
    char message[1024];
    _vsnprintf_s(message, ArraySize(message), ArraySize(message), format, args);
    va_end(args);

    return spHandler_->OnAssert(pszAssert, message);
}

secondpylon::diag::IAssertHandler* AssertSystem::SetAssertHandler(
    secondpylon::diag::IAssertHandler* pNewHandler) {
    IAssertHandler* pPreviousHandler = spHandler_;
    spHandler_ = pNewHandler;
    return pPreviousHandler;
}

}  // namespace diag
}  // namespace secondpylon


