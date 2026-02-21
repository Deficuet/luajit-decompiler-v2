#include <windows.h>

#if !defined(LJD_API)
#if defined(BUILDING_LJD)
#define LJD_API __declspec(dllexport)
#else
#define LJD_API __declspec(dllimport)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*CodeCallback)(const char *str, size_t len);

    LJD_API void ljd_file_to_file(const wchar_t *input, const wchar_t *output);
    LJD_API void ljd_bytes_to_file(const wchar_t *name, const char *array, size_t inSize, const wchar_t *outPath);
    LJD_API void ljd_bytes_to_file_append(HANDLE file, const wchar_t *name, const char *array, size_t inSize);
    LJD_API void ljd_bytes_to_buffer(const wchar_t *name, const char *array, size_t inSize, CodeCallback callback);

#ifdef __cplusplus
}
#endif
