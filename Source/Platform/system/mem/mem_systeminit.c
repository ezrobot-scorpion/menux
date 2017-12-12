/*!
********************************************************************************
* @file     : mem_systeminit.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#include <config.h>
#include <menux/error.h>
#include <menux/nuxos/meminit.h>
#include <menux/nuxos/mem/heapdef.h>

/*!
********************************************************************************
* EXTERN VARIABLSES
*******************************************************************************/
extern char *__HeapBase;
extern char *__HeapLimit;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/*!
********************************************************************************
* @brief : This function used to initialize memory manager module.
*
* @param : None.
*
* @retval: None.
*******************************************************************************/
int mem_system_init(void)
{
	mm_heap_init(&__HeapBase, &__HeapLimit - &__HeapBase);

	return E_OK;
}
