/*!
********************************************************************************
* @file     : devfs_chrdriver.c
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
#include <menux/nuxos/fs/devfs.h>
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
int devfs_chrdriver_register(const char *path, const rt_file_ops_t *fops,
		mode_t mode, void *priv)
{
	rt_inode_t *node;
	int ret;

	inode_wait_mutex();
	ret = inode_reserve(path, &node);
	if (ret >= 0)
	{
		INODE_SET_DRIVER(node);
		node->i_ops.i_fops = fops;
		node->i_private = priv;
		ret = E_OK;
	}

	inode_release_mutex();

	return ret;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int devfs_chrdriver_unregister(const char *path)
{
	int ret;

	inode_wait_mutex();
	ret = inode_remove(path);
	inode_release_mutex();

	return ret;
}


