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
#include <stdio.h>
#include <config.h>
#include <menux/error.h>
#include <menux/types.h>
#include <menux/nuxos/fs/vfs.h>
#include <menux/nuxos/fs/inode.h>

#define CONFIG_PATH_MAX 128

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
int inode_foreachlevel(rt_inode_t *node, rt_inode_path_t *info)
{
	int ret = E_OK;

	for (; node; node = node->i_peer)
	{
		ret = info->handler(node, info->path, info->arg);
		if (ret != 0)
		{
			break;
		}

		if (node->i_child)
		{
			int pathlen = strlen(info->path);
			int namlen = strlen(node->i_name) + 1;
			if (pathlen + namlen > CONFIG_PATH_MAX)
			{
				ret = -E_ENAMETOOLONG;
				break;
			}

			sprintf(&info->path[pathlen], "/%s", node->i_name);
			ret = inode_foreachlevel(node->i_child, info);
			info->path[pathlen] = '\0';
			if (ret != 0)
			{
				break;
			}
		}
	}

	return ret;
}
