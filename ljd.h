#include <windows.h>

#if !defined(EXPORT)
#define EXPORT __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
    EXPORT void ljd_file_to_file(const wchar_t *input, const wchar_t *output);
    EXPORT void ljd_bytes_to_file(const wchar_t *name, const char *array, size_t inSize, const wchar_t *outPath);
    EXPORT void ljd_bytes_to_file_append(HANDLE file, const wchar_t *name, const char *array, size_t inSize);
#ifdef __cplusplus
}
#endif
