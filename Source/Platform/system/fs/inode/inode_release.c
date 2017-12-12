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
void inode_release(rt_inode_t *node)
{
	if (node)
	{
		inode_wait_mutex();
		if (node->i_crefs)
		{
			node->i_crefs--;
		}

		if ((node->i_crefs <= 0) && ((node->i_flags & FSNODEFLAG_DELETED) != 0))
		{
			inode_release_mutex();
			inode_free(node);
		}
		else
		{
			inode_release_mutex();
		}
	}
}
