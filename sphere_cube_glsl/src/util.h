#ifndef __UTIL_H
#define __UTIL_H

#define SNPRINTF snprintf
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define INVALID_OGL_VALUE 0xFFFFFFFF
#define SAFE_DELETE(pointer) if (pointer) { delete pointer; pointer = nullptr; }

class Util
{
    public:
        static char* File2String(const char* path);
};

#endif
