/*!
********************************************************************************
* @file     : vfs_open.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-12-14 first version
*******************************************************************************/
#include <config.h>
#include <stdarg.h>
#include <menux/error.h>
#include <menux/fcntl.h>
#include <menux/nuxos/fs/inode.h>

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int vfs_checkinodeflags(struct rt_inode_s *inode, int oflags)
{
	if (((oflags & O_RDOK) != 0 && !inode->i_ops.i_fops->read) ||
		((oflags & O_WROK) != 0 && !inode->i_ops.i_fops->write))
	{
		return -E_EACCES;
	}

	return E_OK;
}

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
int vfs_open(const char *path, int oflags, ...)
{
	struct rt_inode_search_s desc;
	struct rt_file_s *file;
	struct rt_inode_s *inode;
	mode_t mode = 0666;
	int ret;
	int fd;

	if ((oflags & (O_WRONLY | O_CREAT)) != 0)
	{
		va_list ap;
		va_start(ap, oflags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}

	desc.path = path;
	desc.node = NULL;
	desc.peer = NULL;
	desc.parent = NULL;
	desc.relpath = NULL;
	ret = inode_find(&desc);
	if (ret < 0)
	{
		ret = -ret;
		goto errout_with_search;
	}

	inode = desc.node;

	ret = vfs_checkinodeflags(inode, oflags);
	if (ret < 0)
	{
		ret = -ret;
		goto errout_with_inode;
	}

	ret = E_OK;
	if (inode->i_ops.i_fops->open)
	{
		ret = inode->i_ops.i_fops->open(file);
	}

	if (ret < 0)
	{
		ret = -ret;
		goto errout_with_fd;
	}

errout_with_fd:

errout_with_inode:
	inode_release(inode);

errout_with_search:


	return E_ERROR;
}
