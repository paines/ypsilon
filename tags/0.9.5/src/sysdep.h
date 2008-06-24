/*
    Ypsilon Scheme System
    Copyright (c) 2004-2008 Y.FUJITA / LittleWing Company Limited.
    See license.txt for terms and conditions of use
*/

#ifndef	SYSDEP_H_INCLUDED
#define	SYSDEP_H_INCLUDED

#if _MSC_VER
  #define ARCH_BIG_ENDIAN           0
  #define DECLSPEC(x)               __declspec(x)
  #define ATTRIBUTE(x)
  #define ALIGNOF(x)                (sizeof(x) > __alignof(x) ? __alignof(x) : sizeof(x))    
  #define ARCH_LITTLE_ENDIAN        1
#else
  #define DECLSPEC(x)
  #define ATTRIBUTE(x)              __attribute__ ((x))
  #define ALIGNOF(x)                __alignof__(x)
  #if defined(__LITTLE_ENDIAN__)
    #define ARCH_LITTLE_ENDIAN      1
    #define ARCH_BIG_ENDIAN         0
  #elif defined(__BIG_ENDIAN__)
    #define ARCH_LITTLE_ENDIAN      0
    #define ARCH_BIG_ENDIAN         1
  #elif defined(__BYTE_ORDER)
    #if __BYTE_ORDER == __LITTLE_ENDIAN
      #define ARCH_LITTLE_ENDIAN    1
	  #define ARCH_BIG_ENDIAN       0
    #elif __BYTE_ORDER == __BIG_ENDIAN
      #define ARCH_LITTLE_ENDIAN    0
      #define ARCH_BIG_ENDIAN       1
    #else
      #error unknown __BYTE_ORDER
    #endif
  #else
    #error unknown __BYTE_ORDER
  #endif
#endif

#if _MSC_VER

    #pragma warning(disable:4996)
    #pragma warning(disable:4146)
    #pragma warning(disable:4244)
    #pragma warning(disable:4715)
    #pragma warning(disable:4101)
    #pragma warning(disable:4018)

    #define     WIN32_LEAN_AND_MEAN
    #include    <windows.h>
    #include    <malloc.h>
    #include    <float.h>
    #include    <errno.h>
    #include    <io.h>
    #include    <fcntl.h>
    #include    <winsock2.h>
    #include    <process.h>
    #include    <xmmintrin.h>
    #include    <sys/stat.h>
    #include    <limits>

	extern "C" void __cdecl     _dosmaperr(unsigned long);
    #define snprintf            _snprintf
    
    #define VALUE_NAN           std::numeric_limits<double>::quiet_NaN()
    #define VALUE_INF           std::numeric_limits<double>::infinity()

    #define INT8_MIN            _I8_MIN
    #define INT8_MAX            _I8_MAX
    #define INT16_MIN           _I16_MIN
    #define INT16_MAX           _I16_MAX
    #define INT32_MIN           _I32_MIN
    #define INT32_MAX           _I32_MAX
    #define INT64_MIN           _I64_MIN
    #define INT64_MAX           _I64_MAX
    #define INTPTR_MIN          _I32_MIN
    #define INTPTR_MAX          _I32_MAX
    #define UINT8_MIN           _UI8_MIN
    #define UINT8_MAX           _UI8_MAX
    #define UINT16_MIN          _UI16_MIN
    #define UINT16_MAX          _UI16_MAX
    #define UINT32_MIN          _UI32_MIN
    #define UINT32_MAX          _UI32_MAX
    #define UINT64_MIN          _UI64_MIN
    #define UINT64_MAX          _UI64_MAX
    #define UINTPTR_MIN         _UI32_MIN
    #define UINTPTR_MAX         _UI32_MAX
            
    typedef signed char         int8_t;
    typedef short               int16_t;
    typedef int                 int32_t;
    typedef long long           int64_t;
    typedef unsigned char       uint8_t;
    typedef unsigned short      uint16_t;
    typedef unsigned int        uint32_t;
    typedef unsigned long long  uint64_t;
    typedef int                 ssize_t;
    typedef int64_t             off64_t;
    typedef HANDLE              fd_t;

    #define MEM_STORE_FENCE     _mm_sfence()
    #define INVALID_FD          INVALID_HANDLE_VALUE
    #define PORT_STDIN_FD       GetStdHandle(STD_INPUT_HANDLE)
    #define PORT_STDOUT_FD      GetStdHandle(STD_OUTPUT_HANDLE)
    #define PORT_STDERR_FD      GetStdHandle(STD_ERROR_HANDLE)
    
    inline int      isnan(double x) { return _isnan(x); }
    inline int      isinf(double x) { return !_finite(x); }
    inline double   round(double x) { return (x >= 0.0) ? floor(x + 0.5) : ceil(x - 0.5); }
    inline double   trunc(double x) { return (x >= 0.0) ? floor(x) : ceil(x); }

    inline double msec()
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        return ((double)ft.dwLowDateTime + (double)ft.dwHighDateTime * (double)UINT32_MAX) / 10000.0;
    } 

    inline int gettimeofday(struct timeval *tv, struct timezone *tz)
    {
        FILETIME ft;
        GetSystemTimeAsFileTime(&ft);
        uint64_t ft64 = ((uint64_t)ft.dwLowDateTime + (((uint64_t)ft.dwHighDateTime) << 32)) / 10 - 11644473600000000LL;
        tv->tv_usec = ft64 % 1000000; 
        tv->tv_sec = ft64 / 1000000;
        return 0;
    }
    
    inline int usleep(int usec)
    {
        SleepEx(usec / 1000, FALSE);
        return 0;
    }

    inline int getpagesize()
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        return ((int)si.dwPageSize);
    }
            
    #define HEAP_MAP_FAILED     0
    #define HEAP_UNMAP_FAILED   0
    
    inline void* heap_map(void* adrs, size_t size)
    {
        return VirtualAlloc(adrs, size, MEM_COMMIT, PAGE_READWRITE);
    }
    
    inline int heap_unmap(void* adrs, size_t size)
    {
        return VirtualFree(adrs, size, MEM_DECOMMIT);
    }
        
    inline VM* current_vm()
    {
        extern VM* s_current_vm;
        return s_current_vm;
    }

  #if MTDEBUG
    #define MTVERIFY(expr)                                                                                              \
        do {                                                                                                            \
            intptr_t __RETVAL__ = (intptr_t)(expr);                                                                     \
            if (__RETVAL__ == 0) fatal("error:%s:%u " #expr " %d %d", __FILE__, __LINE__, __RETVAL__, GetLastError());  \
        } while(0)
  #else
    #define MTVERIFY(expr)                              \
        do {                                            \
            intptr_t __RETVAL__ = (intptr_t)(expr);     \
            if (__RETVAL__ == 0) throw GetLastError();  \
        } while(0)
  #endif

    inline void thread_start(unsigned int (__stdcall *func)(void*), void* param)
    {
        MTVERIFY(_beginthreadex(NULL, 0, func, param, 0, NULL));
    }

#else

    #include <pthread.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/mman.h>
    #include <sys/errno.h>
    #include <sys/poll.h>
    #include <sys/socket.h>
    #include <sys/param.h>
    #include <sys/times.h>
    #include <sys/resource.h>
    #include <sys/utsname.h>
    #include <stdint.h>
    #include <unistd.h>
    #include <regex.h>
    #include <dlfcn.h>
    #include <netdb.h>
    #include <dirent.h>
        
    typedef int     fd_t;
    
    #ifndef __off64_t_defined
    typedef off_t   off64_t;
    #endif

    #define VALUE_NAN           __builtin_nan("")   /* strtod("NAN", NULL) */
    #define VALUE_INF           __builtin_inf()     /* strtod("INF", NULL) */
    #define MEM_STORE_FENCE     __asm__ __volatile__ ("sfence" ::: "memory")

    #define INVALID_FD          (-1)
    #define PORT_STDIN_FD       0
    #define PORT_STDOUT_FD      1
    #define PORT_STDERR_FD      2
    
    #define HEAP_MAP_FAILED     MAP_FAILED
    #define HEAP_UNMAP_FAILED   (-1)
        
    inline void* heap_map(void* adrs, size_t size)
    {
        return (uint8_t*)mmap(adrs, size, (PROT_READ | PROT_WRITE), (MAP_ANON | MAP_PRIVATE), -1, 0);
    }
    
    inline int heap_unmap(void* adrs, size_t size)
    {
		return munmap(adrs, size);
    }

    inline double msec()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return (tv.tv_sec * 1000000.0 + tv.tv_usec) / 1000.0;
    }
    
    inline VM* current_vm()
    {
        extern VM* s_current_vm;
        return s_current_vm;
    }

  #if MTDEBUG
    #define MTVERIFY(expr)                                                                                              \
        do {                                                                                                            \
            int __RETVAL__ = (expr);                                                                                    \
             if (__RETVAL__) fatal("error:%s:%u " #expr " %d %s", __FILE__, __LINE__, __RETVAL__, strerror(__RETVAL__));\
        } while(0)
  #else
    #define MTVERIFY(expr)                      \
        do {                                    \
            int __RETVAL__ = (expr);            \
            if (__RETVAL__) throw __RETVAL__;   \
        } while(0)
  #endif
    
    inline void thread_start(void* (*func)(void*), void* param)
    {
        pthread_t tid;
        MTVERIFY(pthread_create(&tid, NULL, func, param));
        MTVERIFY(pthread_detach(tid));
    }
        
#endif

#endif  // SYSDEP_H_INCLUDED

