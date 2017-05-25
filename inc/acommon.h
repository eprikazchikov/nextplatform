#ifndef ACOMMON_H
#define ACOMMON_H

#ifdef _WIN32
    #ifdef NEXT_LIBRARY
        #define NEXT_LIBRARY_EXPORT __declspec(dllexport)
    #else
        #define NEXT_LIBRARY_EXPORT __declspec(dllimport)
    #endif
#endif

#ifdef BUILD_WITH_EASY_PROFILER
    #include <easy/profiler.h>

    #define PROFILE_BLOCK(name, ...) EASY_BLOCK(name, __VA_ARGS__)
    #define PROFILE_FUNCTION(...) EASY_FUNCTION(__VA_ARGS__)
    #define PROFILE_START EASY_PROFILER_ENABLE
    #define PROFILE_STOP profiler::dumpBlocksToFile("profile.prof");
#else
    #define PROFILE_BLOCK(name, ...)
    #define PROFILE_FUNCTION(...)
    #define PROFILE_START
    #define PROFILE_STOP
#endif

#endif // ACOMMON_H