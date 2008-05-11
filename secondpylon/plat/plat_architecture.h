#ifndef SPPLAT_ARCHITECTURE_H
#define SPPLAT_ARCHITECTURE_H

// Define the byte ordering for this platform.
#include <boost/detail/endian.hpp>
#if defined(BOOST_BIG_ENDIAN)

    #define SPPLAT_BIGENDIAN

#elif defined(BOOST_LITTLE_ENDIAN)

    #define SPPLAT_LITTLEENDIAN

#else

    #error Byte ordering is not defined

#endif

#endif // SPPLAT_ARCHITECTURE_H
