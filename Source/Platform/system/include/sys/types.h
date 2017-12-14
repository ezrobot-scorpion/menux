/*!
********************************************************************************
* @file     : types.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-12-13 first version
*******************************************************************************/
#ifndef INCLUDE_SYS_TYPES_H
#define INCLUDE_SYS_TYPES_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <menux/complier.h>

#ifndef __ASSEMBLY__
#  include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/
#ifndef TRUE
#	define TRUE  1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

/* NULL is usually defined in stddef.h (which includes this file) */
#ifndef NULL
#	if defined(SDCC) || defined(__cplusplus)
#		define NULL (0)
#  	else
#    	define NULL ((void*)0)
#  	endif
#endif

/* POSIX-like OS return values: */

#if !defined(__cplusplus)
#	undef  ERROR
#  	define ERROR -1
#endif

#undef	OK
#define OK 0

/* HPUX-like MIN/MAX value */
#define PRIOR_RR_MIN      0
#define PRIOR_RR_MAX    255
#define PRIOR_FIFO_MIN    0
#define PRIOR_FIFO_MAX  255
#define PRIOR_OTHER_MIN   0
#define PRIOR_OTHER_MAX 255

/* Scheduling Priorities.  NOTE:  Only the idle task can take the true
 * minimum priority.
 */
#define SCHED_PRIORITY_MAX     255
#define SCHED_PRIORITY_DEFAULT 100
#define SCHED_PRIORITY_MIN       1
#define SCHED_PRIORITY_IDLE      0

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/
#ifndef __ASSEMBLY__
typedef float  float32;
#ifndef CONFIG_HAVE_DOUBLE
typedef float  double_t;
typedef float  float64;
#else
typedef double double_t;
typedef double float64;
#endif

typedef unsigned int mode_t;

#ifdef CONFIG_SMALL_MEMORY

typedef uint16_t     size_t;
typedef int16_t      ssize_t;
typedef uint16_t     rsize_t;

#else /* CONFIG_SMALL_MEMORY */
typedef uintptr_t    size_t;
typedef intptr_t     ssize_t;
typedef uintptr_t    rsize_t;
#endif /* CONFIG_SMALL_MEMORY */

typedef int16_t      uid_t;
typedef int16_t      gid_t;

typedef uint16_t     dev_t;
typedef uint16_t     ino_t;
typedef int16_t      pid_t;

/* id_t is a general identifier that can be used to contain at least a pid_t,
 * uid_t, or gid_t.
 */

typedef int16_t      id_t;

/* Unix requires a key of type key_t defined in file sys/types.h for requesting
 * resources such as shared memory segments, message queues and semaphores. A key
 * is simply an integer of type key_t
 */

typedef int16_t      key_t;

/* Signed integral type of the result of subtracting two pointers */

typedef intptr_t     ptrdiff_t;

#ifndef CONFIG_WCHAR_BUILTIN
/* Wide, 16-bit character types.  wchar_t is a built-in type in C++ and
 * its declaration here may cause compilation errors on some compilers
 * if -DCONFIG_WCHAR_BUILTIN is not included in the CXXFLAGS.
 *
 * REVISIT: wchar_t belongs in stddef.h
 */

typedef uint16_t     wchar_t;
#endif

/* blkcnt_t and off_t are signed integer types.
 *
 *   blkcnt_t is used for file block counts.
 *   off_t is used for file offsets and sizes.
 *   fpos_t is used for file positions.
 *
 * Hence, all should be independent of processor architecture.
 */

typedef uint32_t     blkcnt_t;
typedef int32_t      off_t;
typedef off_t        fpos_t;

#ifdef CONFIG_HAVE_LONG_LONG
typedef int64_t      off64_t;
typedef int64_t      fpos64_t;
#endif

/* blksize_t is a signed integer value used for file block sizes */

typedef int16_t      blksize_t;

/* Network related */

typedef unsigned int socklen_t;
typedef uint16_t     sa_family_t;

/* Used for system times in clock ticks */

typedef uint32_t     clock_t;

/* The type useconds_t shall be an unsigned integer type capable of storing
 * values at least in the range [0, 1000000]. The type suseconds_t shall be
 * a signed integer type capable of storing values at least in the range
 * [-1, 1000000].
 */

typedef uint32_t     useconds_t;
typedef int32_t      suseconds_t;

#ifdef CONFIG_SMP
/* This is the smallest integer type that will hold a bitset of all CPUs */

#if (CONFIG_SMP_NCPUS <= 8)
typedef volatile uint8_t cpu_set_t;
#elif (CONFIG_SMP_NCPUS <= 16)
typedef volatile uint16_t cpu_set_t;
#elif (CONFIG_SMP_NCPUS <= 32)
typedef volatile uint32_t cpu_set_t;
#else
#  error SMP: Extensions needed to support this number of CPUs
#endif
#endif /* CONFIG_SMP */

/* BSD types provided only to support porting to NuttX. */
typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int   u_int;
typedef unsigned long  u_long;

/* SYSV types provided only to support porting to NuttX.  */
typedef unsigned char  unchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;
typedef signed 	char   s_char;
typedef 		char  *caddr_t;

typedef int (*main_t)(int argc, char *argv[]);

#endif /* __ASSEMBLY__ */

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_SYS_TYPES_H */
