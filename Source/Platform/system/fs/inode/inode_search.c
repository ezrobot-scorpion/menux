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
int inode_search(rt_inode_search_t *desc)
{
	const char *name;
	rt_inode_t *node = g_root_inode;
	rt_inode_t *left = NULL;
	rt_inode_t *above = NULL;
	const char *relpath = NULL;
	int ret = -E_ENOENT;

	name = desc->path;

	if (*name != '/')
	{
		return -E_EINVAL;
	}

	while (*name == '/')
	{
		name++;
	}

	if (*name == '\0')
	{
		return -E_ENOSYS;
	}

	while (node != NULL)
	{
		int result = inode_compare(name, node);

		if (result < 0)
		{
			node = NULL;
			break;
		}
		else if (result > 0)
		{
			left = node;
			node = node->i_peer;
		}
		else
		{
			name = inode_nextname(name);
			if ((*name == '\0') || (INODE_IS_MOUNTPT(node)))
			{
				relpath = name;
				ret = E_OK;
				break;
			}
			else
			{
				above = node;
				left = NULL;
				node = node->i_child;
			}
		}
	}

	desc->path = name;
	desc->node = node;
	desc->peer = left;
	desc->parent = above;
	desc->relpath = relpath;

	return ret;
}
