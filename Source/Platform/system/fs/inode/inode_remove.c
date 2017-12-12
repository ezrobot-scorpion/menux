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
int inode_remove(const char *path)
{
	rt_inode_t *node;

	node = inode_unlink(path);
	if (node)
	{
		if (node->i_crefs)
		{
			node->i_flags |= FSNODEFLAG_DELETED;
			return -E_EBUSY;
		}
		else
		{
			inode_free(node);
			return E_OK;
		}
	}

	return -E_ENOENT;
}
