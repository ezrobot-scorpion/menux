/*!
********************************************************************************
* @file     : vfs_read.c
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
ssize_t vfs_write(struct rt_file_s *file, const void *buf, os_size_t nbytes)
{
	struct rt_inode_s *inode;
	ssize_t ret;
	int errcode;

	if ((file->f_oflags & O_WROK) == 0)
	{
		errcode = E_EBADF;
		goto errout;
	}

	inode = file->f_inode;
	if (!inode || !inode->i_ops.i_fops || !inode->i_ops.i_fops->write)
	{
		errcode = E_EBADF;
		goto errout;
	}

	ret = inode->i_ops.i_fops->write(file, buf, nbytes);
	if (ret < 0)
	{
		errcode = -ret;
		goto errout;
	}

	return ret;

errout:
	return E_ERROR;
}
