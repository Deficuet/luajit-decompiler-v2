#pragma once

#if !defined(LJD_API)
#if defined(BUILDING_LJD)
#define LJD_API __declspec(dllexport)
#else
#define LJD_API __declspec(dllimport)
#endif
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef const wchar_t *(*NameBuilderCallback)(void *userData, size_t *lengthPtr);

    typedef struct {
        NameBuilderCallback callback;
        void *userData;
    } NameBuilder;

    typedef void (*CodeCallback)(const char *str, size_t len);

    LJD_API void ljd_file_to_file(const wchar_t *input, const wchar_t *output);
    LJD_API void ljd_bytes_to_file(const char *array, size_t inSize, const NameBuilder *nameBuilder, const wchar_t *outPath);
    LJD_API void ljd_bytes_to_file_append(HANDLE file, const char *array, size_t inSize, const NameBuilder *nameBuilder);
    LJD_API void ljd_bytes_to_buffer(const char *array, size_t inSize, const NameBuilder *nameBuilder, CodeCallback callback);

#ifdef __cplusplus
}
#endif
