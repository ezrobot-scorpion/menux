/*!
********************************************************************************
* @file     : nuxos_boot.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_MEM_HEAP_H
#define INCLUDE_MENUX_NUXOS_MEM_HEAP_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <menux/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void  os_heap_free 				(void *mem);
void *os_heap_malloc 			(os_size_t size);
void *os_heap_zalloc 			(os_size_t size);
void *os_heap_mallocaligned 	(os_size_t size, os_size_t alignment);
void *os_heap_realloc 			(void *oldmem, os_size_t size);
void  os_heap_extend 			(void *oldmem, os_size_t size);


#ifdef __cplusplus
}
#endif
#endif
