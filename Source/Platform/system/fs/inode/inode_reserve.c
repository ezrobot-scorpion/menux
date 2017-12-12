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
int inode_reserve(const char *path, rt_inode_t **inode)
{
	rt_inode_search_t desc;
	rt_inode_t *left;
	rt_inode_t *parent;
	const char *name;
	int ret;

	*inode = NULL;

	if (path[0] == '\0' || path[0] != '/')
	{
		return -E_EINVAL;
	}

	desc.path = path;
	desc.node = NULL;
	desc.peer = NULL;
	desc.parent = NULL;
	desc.relpath = NULL;

	ret = inode_search(&desc);
	if (ret >= 0)
	{
		return -E_EEXIST;
	}

	name = desc.path;
	left = desc.peer;
	parent = desc.parent;

	for (;;)
	{
		rt_inode_t *node;
		const char *nextname = inode_nextname(name);
		if (*nextname != '\0')
		{
			node = inode_alloc(name);
			if (node != NULL)
			{
				inode_insert(node, left, parent);
				name = nextname;
				left = NULL;
				parent = node;
				continue;
			}
		}
		else
		{
			node = inode_alloc(name);
			if (node != NULL)
			{
				inode_insert(node, left, parent);
				*inode = node;
				ret = E_OK;
				break;
			}
		}

		ret = -E_ENOMEM;
		break;
	}

	return ret;
}
