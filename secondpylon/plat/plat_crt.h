#ifndef SPPLAT_CRT_H
#define SPPLAT_CRT_H

#include <secondpylon/plat/plat_compiler.h> // for SPPLAT_RESTRICT
#include <cstring> // for strlen, strcmp, stricmp, memcpy

// The module contains functions which are low level or are common to most applications. Examples include error 
// reporting, diagnostic messages, string functions and low level utilities among others. Some of this functions are
// designed as direct replacements for the c/c++ runtime libraries due to potential security holes, platform specific
// optimization, provide additional portability, etc. Memory will explicitly not be part of this library to avoid issues
// with compatibility with other frameworks. 
//
// Saftey handling is similar to support provided by ISO/IEC TR 24731, which is not yet widely supported across
// compilers.
//
// As this functionality is primitive, it exists directly within the secondpylon namespace. This functionality overlaps
// with the platform library - it may subsume the platform lib or it may be split into a separate component in the
// future.
//
// These functions are likely to move around a bit as we figure out the best home for each.

namespace secondpylon {

    // Returns the size of an array.
    //
    // This is similar to the more traditional: 
    //
    // #define ARRAYSIZE(array)sizeof(array) / sizeof(array[0])
    //
    // The template version avoids accidental use on a char*, as only arrays can be
    // passed in. THe downsize is that locally defined classes can't be handled, as
    // they can't be used in template parameters. If this causes compiler issues or
    // performance overhead, we can switch back to the macro version.
    //
    // Note that this is not a compile time solution even though it inlines down to nothing in release.
    template <typename T, size_t kSize>
    size_t ArraySize(T (&)[kSize]);

    // Casting between different types is undefined by the C++ Standard in most cases. More specifically, pointers of
    // different types to the same address has undefined behavior.
    //
    // Some compilers, like VC, don't yet apply optimizations which rely heavily on strict aliasing compliance. GCC has
    // such an option. It greatly improves the results generated by the optimizer. Using this class when casting between
    // unrelated types insures we won't get hit by strict aliasing bugs.
    //
    // The union cast works around this because of a compiler provided 'special case'. This isn't standard guarenteed
    // functionality, but it is common enough that we will rely on it.
    //
    // See http://en.wikipedia.org/wiki/Aliasing_(computing) for more details and links to references.
    template <typename TDest, typename TSource>
    TDest StrictCast(TSource source);

    static void StringCopy(char* SPPLAT_RESTRICT dest, const char* SPPLAT_RESTRICT source, size_t destCapacity);
    static bool StringEquals();
    static bool StringIEquals();

    // This is macro serves two purposes:
    //
    // 1)  Describe a class as uncopyable for documentation purposes.
    // 2)  Suppress 'copy constructor could not be generated' and 'assignment operator could not be generated' errors.
    //
    // The initial implementation was a trait base class. This didn't work cleanly when we had multiple classes in a
    // hierarchy that needed to be uncopyable as this generates errors about deriving multiple times from a class.
    #define SPUNCOPYABLE(ClassName) \
    private: \
        ClassName(ClassName&); \
        ClassName& operator=(ClassName&)


    //
    // inline functions
    //
    template <typename T, size_t kSize>
    size_t ArraySize(T (&)[kSize])
    {
        return kSize;
    }

    // @todo Implement the error handling
    SPPLAT_INLINE void StringCopy(char* SPPLAT_RESTRICT dest, const char* SPPLAT_RESTRICT source, size_t destCapacity)
    {
        // Verify the string fits.
        size_t sourceLength = strlen(source);
        if (sourceLength + 1 > destCapacity)
        {
            dest[0] = '\0';
            //RSDBG_ERROR((""));
            return;
        }

        // Verify the ranges don't overlap by checking the boundaries.
        //RSDBG_ASSERT(source > dest+destCapacity || source+sourceLength+1 < dest);

        // Copy and terminate the string.
        memcpy(dest, source, sourceLength*sizeof(char));
        dest[sourceLength] = '\0';
    }

    SPPLAT_INLINE bool StringEquals(const char* string0, const char* string1)
    {
        return (0 == strcmp(string0, string1));
    }

    SPPLAT_INLINE bool StringIEquals(const char* string0, const char* string1)
    {
        return (0 == stricmp(string0, string1));
    }

    // @todo Add a compile time assert to insure the source and dest are of the same size.
    template <typename TDest, typename TSource>
    TDest StrictCast(TSource source)
    {
        union Converter
        {
            TSource source;
            TDest dest;
        };

        Converter converter;
        converter.source = source;
        return converter.dest;
    };

}

#endif // SPPLAT_CRT_H
