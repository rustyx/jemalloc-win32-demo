
#ifdef WITH_JEMALLOC

#ifndef JEMALLOC_EXPORT
#define JEMALLOC_EXPORT // to tell jemalloc that it is statically linked
#endif
#include "jemalloc/jemalloc.h" // don't forget to add jemalloc/include and msvc_compat to the include path
#include <new>

#ifdef _DEBUG
#pragma comment(lib, "jemalloc-vc140-Debug-static")
#else
#pragma comment(lib, "jemalloc-vc140-Release-static")
#endif

static inline void* my_malloc(size_t size)
{
  if (!size)
    size = 1; // je_malloc(0) doesn't work
  return je_malloc(size);
}

static inline void* my_mallocx(size_t size, int flags)
{
  if (!size)
    size = 1; // je_malloc(0) doesn't work
  return je_mallocx(size, flags);
}

static inline void* my_realloc(void* pointer, size_t size, int flags)
{
  if (!size)
    size = 1; // je_malloc(0) doesn't work
  return pointer ? je_rallocx(pointer, size, flags) : my_mallocx(size, flags);
}

static inline void my_free(void* pointer)
{
  if (pointer) // je_free(NULL) doesn't work
    je_free(pointer);
}

static inline size_t my_msize(void* pointer)
{
  return je_malloc_usable_size(pointer);
}

void* operator new(size_t size)
{
  void* ptr = my_malloc(size);
  if (ptr)
    return ptr;
  else
    throw std::bad_alloc(); //TODO use std::get_new_handler
}

void* operator new[](size_t size)
{
  void* ptr = my_malloc(size);
  if (ptr)
    return ptr;
  else
    throw std::bad_alloc(); //TODO use std::get_new_handler
}

void* operator new(size_t size, const std::nothrow_t&) throw()
{
  return my_malloc(size);
}

void* operator new[](size_t size, const std::nothrow_t&) throw()
{
  return my_malloc(size);
}

void operator delete(void* pointer) throw()
{
  my_free(pointer);
}

void operator delete[](void* pointer) throw()
{
  my_free(pointer);
}

void operator delete(void* pointer, const std::nothrow_t&) throw()
{
  my_free(pointer);
}

void operator delete[](void* pointer, const std::nothrow_t&) throw()
{
  my_free(pointer);
}

// C++14's sized-delete operators

void operator delete(void* pointer, std::size_t size) noexcept
{
  my_free(pointer);
}

void operator delete[](void* pointer, std::size_t size) noexcept
{
  my_free(pointer);
}

//TODO: Add C++17's aligned new operators

#if !defined(_DEBUG) && defined(JEMALLOC_MALLOC)
// Optionally also redirect malloc and free
// * Not possible in Debug mode, or link with /FORCE:MULTIPLE
// * Not possible with dynamic CRT (/MD)

extern "C" {

void* malloc(size_t size)
{
  return my_malloc(size);
}

void free(void* pointer)
{
  my_free(pointer);
}

void* calloc(size_t count, size_t size)
{
  return my_mallocx(count * size, MALLOCX_ZERO);
}
void* _expand(void* pointer, size_t size)
{
  return my_realloc(pointer, size, 0);
}

size_t _msize(void* pointer)
{
  return my_msize(pointer);
}

void* realloc(void* pointer, size_t size)
{
  return my_realloc(pointer, size, 0);
}

void* _recalloc(void* pointer, size_t count, size_t size)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO);
}

void _aligned_free(void* pointer)
{
  my_free(pointer);
}

void* _aligned_malloc(size_t size, size_t align)
{
  return my_mallocx(size, MALLOCX_ALIGN(align));
}

size_t _aligned_msize(void* pointer, size_t align, size_t offset)
{
  return my_msize(pointer);
}

void* _aligned_offset_malloc(size_t size, size_t align, size_t offset)
{
  return my_mallocx(size, MALLOCX_ALIGN(offset)); // align + offset not implemented
}

void* _aligned_offset_realloc(void* pointer, size_t size, size_t align, size_t offset)
{
  return my_realloc(pointer, size, MALLOCX_ALIGN(offset)); // align + offset not implemented
}

void* _aligned_offset_recalloc(void* pointer, size_t count, size_t size, size_t align, size_t offset)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO | MALLOCX_ALIGN(offset)); // align + offset not implemented
}

void* _aligned_realloc(void* pointer, size_t size, size_t align)
{
  return my_realloc(pointer, size, MALLOCX_ALIGN(align));
}

void* _aligned_recalloc(void* pointer, size_t count, size_t size, size_t align)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO | MALLOCX_ALIGN(align));
}

// MSVC CRT internal functions

void* _calloc_base(size_t count, size_t size)
{
  return my_mallocx(count * size, MALLOCX_ZERO);
}

void* _expand_base(void* pointer, size_t size)
{
  return my_realloc(pointer, size, 0);
}

void _free_base(void* pointer)
{
  my_free(pointer);
}

void* _malloc_base(size_t size)
{
  return my_malloc(size);
}

size_t _msize_base(void* pointer)
{
  return my_msize(pointer);
}

void* realloc_base(void* pointer, size_t size)
{
  return my_realloc(pointer, size, 0);
}

void* _recalloc_base(void* pointer, size_t count, size_t size)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO);
}

void _aligned_free_base(void* pointer)
{
  my_free(pointer);
}

void* _aligned_malloc_base(size_t size, size_t align)
{
  return my_mallocx(size, MALLOCX_ALIGN(align));
}

size_t _aligned_msize_base(void* pointer, size_t align, size_t offset)
{
  return my_msize(pointer);
}

void* _aligned_offset_malloc_base(size_t size, size_t align, size_t offset)
{
  return my_mallocx(size, MALLOCX_ALIGN(offset)); // align + offset not implemented
}

void* _aligned_offset_realloc_base(void* pointer, size_t size, size_t align, size_t offset)
{
  return my_realloc(pointer, size, MALLOCX_ALIGN(offset)); // align + offset not implemented
}

void* _aligned_offset_recalloc_base(void* pointer, size_t count, size_t size, size_t align, size_t offset)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO | MALLOCX_ALIGN(offset)); // align + offset not implemented
}

void* _aligned_realloc_base(void* pointer, size_t size, size_t align)
{
  return my_realloc(pointer, size, MALLOCX_ALIGN(align));
}

void* _aligned_recalloc_base(void* pointer, size_t count, size_t size, size_t align)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO | MALLOCX_ALIGN(align));
}

#undef _calloc_dbg
void* _calloc_dbg(size_t count, size_t size, int block, const char* file, int line)
{
  return my_mallocx(count * size, MALLOCX_ZERO);
}

#undef _expand_dbg
void* _expand_dbg(void* pointer, size_t size, int block, const char* file, int line)
{
  return my_realloc(pointer, size, 0);
}

#undef _free_dbg
void _free_dbg(void* pointer, int block)
{
  my_free(pointer);
}

#undef _malloc_dbg
void* _malloc_dbg(size_t size, int block, const char* file, int line)
{
  return my_malloc(size);
}

#undef _msize_dbg
size_t _msize_dbg(void* pointer, int block)
{
  return my_msize(pointer);
}

#undef _realloc_dbg
void* _realloc_dbg(void* pointer, size_t size, int block, const char* file, int line)
{
  return my_realloc(pointer, size, 0);
}

#undef _recalloc_dbg
void* _recalloc_dbg(void* pointer, size_t count, size_t size, int block, const char* file, int line)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO);
}

#undef _aligned_free_dbg
void _aligned_free_dbg(void* pointer)
{
  my_free(pointer);
}

#undef _aligned_malloc_dbg
void* _aligned_malloc_dbg(size_t size, size_t align, int block, const char* file, int line)
{
  return my_mallocx(size, MALLOCX_ALIGN(align));
}

#undef _aligned_msize_dbg
size_t _aligned_msize_dbg(void* pointer, size_t align, size_t offset)
{
  return my_msize(pointer);
}

#undef _aligned_offset_malloc_dbg
void* _aligned_offset_malloc_dbg(size_t size, size_t align, size_t offset, int block, const char* file, int line)
{
  return my_mallocx(size, MALLOCX_ALIGN(offset)); // align + offset not implemented
}

#undef _aligned_offset_realloc_dbg
void* _aligned_offset_realloc_dbg(void* pointer, size_t size, size_t align, size_t offset, int block, const char* file, int line)
{
  return my_realloc(pointer, size, MALLOCX_ALIGN(offset)); // align + offset not implemented
}

#undef _aligned_offset_recalloc_dbg
void* _aligned_offset_recalloc_dbg(void* pointer, size_t count, size_t size, size_t align, size_t offset, int block, const char* file, int line)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO | MALLOCX_ALIGN(offset)); // align + offset not implemented
}

#undef _aligned_realloc_dbg
void* _aligned_realloc_dbg(void* pointer, size_t size, size_t align, int block, const char* file, int line)
{
  return my_realloc(pointer, size, MALLOCX_ALIGN(align));
}

#undef _aligned_recalloc_dbg
void* _aligned_recalloc_dbg(void* pointer, size_t count, size_t size, size_t align, int block, const char* file, int line)
{
  return my_realloc(pointer, count * size, MALLOCX_ZERO | MALLOCX_ALIGN(align));
}

} // extern C

#endif // Override malloc and free (in Release only)

#endif
