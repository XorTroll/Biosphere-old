
/*

    Biosphere library

    @file Macros.hpp
    @brief Basic macros for the library.
    @author XorTroll

    @copyright Biosphere project - Bringing life to homebrew!

*/

#pragma once
#include <cstdint>

#define SSIZE_MAX (SIZE_MAX >> 1)
#define U64_MAX UINT64_MAX

#define BIO_PACKED __attribute__((packed))
#define BIO_NORETURN __attribute__((noreturn))
#define BIO_WEAK __attribute__((weak))
#define BIO_DEPRECATED __attribute__((deprecated))
#define BIO_UNUSED __attribute__(unused)
#define BIO_NOINLINE __attribute__(noinline)
#define BIO_IGNORE(Var) (void)(Var)
#define BIO_OVERRIDE override
#define BIO_NOEXCEPT noexcept
#define BIO_EXPLICIT_OPERATOR operator
#define BIO_CURRENT_FUNCTION_NAME __FUNCTION__
#define BIO_FUNCTION_LOCAL_STATIC(Type, Name, ...) static Type Name __VA_ARGS__
#define BIO_BITSIZEOF(Type) (static_cast<int>(sizeof(Type)))
#define BIO_BITMASK(Number) (1u << Number)