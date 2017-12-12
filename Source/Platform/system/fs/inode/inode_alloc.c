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
rt_inode_t *inode_alloc(const char *name)
{
	int namelen = inode_namelen(name);
	rt_inode_t *node = (rt_inode_t *) os_heap_malloc(FSNODE_SIZE(namelen));
	if (node)
	{
		inode_namecpy(node->i_name, name);
	}

	return node;
}
