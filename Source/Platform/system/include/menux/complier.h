/*!
********************************************************************************
* @file     : complier.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_COMPLIER_H
#define INCLUDE_MENUX_COMPLIER_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/
#ifdef __CC_ARM                         /* ARM Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define USED                        __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define WEAK						__weak
    #define rt_inline                   static __inline
    /* module compiling */
        #define RTT_API                 __declspec(dllexport)

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  @ x
    #define UNUSED
    #define USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define WEAK                        __weak
    #define rt_inline                   static inline
    #define RTT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #ifdef RT_USING_NEWLIB
        #include <stdarg.h>
    #else
		/* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list   __gnuc_va_list;
        typedef __gnuc_va_list      va_list;
        #define va_start(v,l)       __builtin_va_start(v,l)
        #define va_end(v)           __builtin_va_end(v)
        #define va_arg(v,l)         __builtin_va_arg(v,l)
    #endif

    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define USED                        __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define WEAK						__attribute__((weak))
    #define rt_inline                   static __inline
    #define RTT_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define UNUSED                      __attribute__((unused))
    #define USED                        __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
	#define WEAK                        __attribute__((weak))
    #define rt_inline                   static inline
    #define RTT_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define SECTION(x)
    #define UNUSED
    #define USED
    #define ALIGN(n)                    __declspec(align(n))
	#define WEAK
    #define rt_inline                   static __inline
    #define RTT_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define SECTION(x)
    #define UNUSED
    #define USED
	#define PRAGMA(x)					_Pragma(#x)
    #define ALIGN(n)
	#define WEAK
    #define rt_inline                   static inline
    #define RTT_API
#else
    #error not supported tool chain
#endif

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_COMPLIER_H */
