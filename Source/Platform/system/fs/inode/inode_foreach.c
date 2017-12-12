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
#include <config.h>
#include <menux/error.h>
#include <menux/types.h>
#include <menux/nuxos/fs/vfs.h>
#include <menux/nuxos/fs/inode.h>
#include <menux/nuxos/mem/heap.h>

/*!
********************************************************************************
* EXTERN VARIABLES
*******************************************************************************/
extern rt_inode_t *g_root_inode;

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
int inode_foreach(rt_foreach_inode_t handler, void *arg)
{
	rt_inode_path_t *info;
	int ret;

	info = (rt_inode_path_t *) os_heap_malloc(sizeof(rt_inode_path_t));
	if (!info)
	{
		return -E_ENOMEM;
	}

	info->handler = handler;
	info->arg = arg;
	info->path[0] = '\0';

	inode_wait_mutex();
	ret = inode_foreachlevel(g_root_inode, info);
	inode_release_mutex();

	os_heap_free(info);

	return ret;
}
