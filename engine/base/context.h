#ifndef CONTEXT_H
#define CONTEXT_H

#if !defined(ENABLE_ASSERT)
    #define ENABLE_ASSERT 0
#endif

#if !defined(ENABLE_ASAN)
    #define ENABLE_ASAN 0
#endif

#if (ENABLE_ASAN)
    #include <sanitizer/asan_interface.h>
#endif

#if ENABLE_ASAN
#define AsanPoison(p,z)   __asan_poison_memory_region((p),(z))
#define AsanUnpoison(p,z) __asan_unpoison_memory_region((p),(z))
#else
    #define AsanPoison(p,z)
    #define AsanUnpoison(p,z)
#endif

#if defined(__clang__)
    #define COMPILER_CLANG 1
    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #elif defined(__gnu_linux__) || defined(__EMSCRIPTEN__)
        #define OS_LINUX 1
    #elif defined(__APPLE__) && defined(__MACH__)
        #define OS_MAC 1
    #else
        #error OS Detection failed!
    #endif
    #if defined(__amd64__)
        #define ARCH_X64 1
    #elif defined(__wasm64__)
        #define ARCH_WASM64 1
    #elif defined(__wasm32__)
        #define ARCH_WASM32 1
        //#error "WASM32 not supported! Please do 64-bits"
    #elif defined(__i386__)
        #define ARCH_X86 1
    #elif defined(__arm__)
        #define ARCH_ARM 1
    #elif defined(__aarch64__)
        #define ARCH_ARM64 1
    #else
        #error ARCH detection failed!
    #endif
#elif defined(_MSC_VER)
    #define COMPILER_CL 1
    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #else
        #error OS detection failed!
    #endif
    #if defined(_M_AMD64)
        #define ARCH_X64 1
    #elif defined(_M_I86)
        #define ARCH_X86 1
    #elif defined(_M_ARM)
        #define ARCH_ARM 1
    #else
        #error ARCH detection failed!
    #endif
#elif defined(__GNUC__)
    #define COMPILER_GCC 1
    #if defined(_WIN32)
        #define OS_WINDOWS 1
    #elif defined(__gnu_linux__)
        #define OS_LINUX 1
    #elif defined(__APPLE__) && defined(__MACH__)
        #define OS_MAC 1
    #else
        #error OS detection failed!
    #endif
    #if defined(__amd64__)
        #define ARCH_X64 1
    #elif defined(__i386__)
        #define ARCH_X86 1
    #elif defined(__arm__)
        #define ARCH_ARM 1
    #elif defined(__aarch64__)
        #define ARCH_ARM64 1
    #else
        #error ARCH detection failed!
    #endif

#else
#error Couldnt find anything about this context
#endif

#if !defined(COMPILER_CL)
    #define COMPILER_CL 0
#endif
#if !defined(COMPILER_CLANG)
    #define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
    #define COMPILER_GCC 0
#endif
#if !defined(OS_WINDOWS)
    #define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
    #define OS_LINUX 0
#endif
#if !defined(OS_MAC)
    #define OS_MAC 0
#endif
#if !defined(ARCH_X64)
    #define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
    #define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
    #define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
    #define ARCH_ARM64 0
#endif
#if !defined(ARCH_WASM64)
    #define ARCH_WASM64 0
#endif
#if !defined(ARCH_WASM32)
    #define ARCH_WASM32 0
#endif


#if defined(__cplusplus)
    #define LANG_CXX 1
#else
    #define LANG_C 1
#endif

#if ARCH_X64 || ARCH_ARM64 || ARCH_WASM64
    #define ARCH_ADDRSIZE 64
#else
    #define ARCH_ADDRSIZE 32
#endif


#if (OS_WINDOWS)
    #include <windows.h>
#endif

#define INLINE static inline

#endif