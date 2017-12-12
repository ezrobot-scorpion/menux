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
rt_inode_t *inode_unlink(const char *path)
{
	rt_inode_search_t desc;
	rt_inode_t *node = NULL;
	int ret;

	if ((path == NULL) || (path[0] != '/'))
	{
		return NULL;
	}

	desc.path = path;
	desc.node = NULL;
	desc.peer = NULL;
	desc.parent = NULL;
	desc.relpath = NULL;

	ret = inode_search(&desc);
	if (ret >= 0)
	{
		node = desc.node;
		if (desc.peer != NULL)
		{
			desc.peer->i_peer = node->i_peer;
		}
		else if (desc.parent)
		{
			desc.parent->i_child = node->i_peer;
		}
		else
		{
			g_root_inode = node->i_peer;
		}

		node->i_peer = NULL;
	}

	return node;
}
