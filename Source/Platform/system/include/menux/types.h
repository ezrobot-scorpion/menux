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
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_TYPES_H
#define INCLUDE_MENUX_TYPES_H

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
#ifndef RT_NULL
#define RT_NULL		((void *)0)
#endif

#ifndef RT_TRUE
#define RT_TRUE		1
#endif
#ifndef RT_FALSE
#define RT_FALSE	0
#endif

#ifndef RT_ALIGN_UP
#define RT_ALIGN_UP(size, align)	(((size) + (align) - 1) & ~((align) - 1))
#endif
#ifndef RT_ALIGN_DN
#define RT_ALIGN_DN(size, align)	((size) & ~((align) - 1))
#endif

#ifndef RT_CONTAINER_OF
#define RT_CONTAINER_OF(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))
#endif

/* maximum value of base type */
#define RT_UINT8_MAX                    0xff            /**< Maxium number of UINT8 */
#define RT_UINT16_MAX                   0xffff          /**< Maxium number of UINT16 */
#define RT_UINT32_MAX                   0xffffffff      /**< Maxium number of UINT32 */
#define RT_TICK_MAX                     RT_UINT32_MAX   /**< Maxium number of tick */

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/
typedef signed   char                   os_sbyte_t;		/**<  8bit integer type */
typedef signed   short                  os_sshort_t;	/**< 16bit integer type */
typedef signed   long                   os_sint_t;     	/**< 32bit integer type */
typedef unsigned char                   os_ubyte_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  os_ushort_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   os_uint_t;    	/**< 32bit unsigned integer type */
typedef int                             rt_bool_t;      /**< boolean type */
typedef	int								rt_mode_t;

/* 32bit CPU */
typedef long                            rt_base_t;      /**< Nbit CPU related date type */
typedef unsigned long                   rt_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef rt_base_t                       os_err_t;       /**< Type for error number */
typedef	int								rt_err_t;

typedef os_uint_t                     rt_time_t;      	/**< Type for time stamp */
typedef os_uint_t                     rt_tick_t;      	/**< Type for tick count */
typedef rt_base_t                       rt_flag_t;      /**< Type for flags */
typedef rt_ubase_t                      os_size_t;      /**< Type for size number */
typedef rt_ubase_t                      rt_dev_t;       /**< Type for device */
typedef rt_base_t                       rt_off_t;       /**< Type for offset */

typedef	unsigned	char				irqstate_t;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif
