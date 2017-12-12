/*!
********************************************************************************
* @file     :
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#include <menux/nuxos.h>
#include <menux/nuxos/fs/vfs.h>
#include <menux/nuxos/fs/inode.h>

/*!
********************************************************************************
* PUBLIC VARIABLES
*******************************************************************************/
rt_inode_t *g_root_inode = NULL;

/*!
********************************************************************************
* PUBLIC VARIABLES
*******************************************************************************/
static OS_MUTEX gx_inodemutex;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void inode_initialize(void)
{
	OS_ERR error;

	OSMutexCreate(&gx_inodemutex, "inode mutex", &error);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void inode_wait_mutex(void)
{
	OS_ERR error;

	OSMutexPend(&gx_inodemutex, 0, OS_OPT_PEND_BLOCKING, DEF_NULL, &error);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void inode_release_mutex(void)
{
	OS_ERR error;

	OSMutexPost(&gx_inodemutex, OS_OPT_POST_NONE, &error);
}
