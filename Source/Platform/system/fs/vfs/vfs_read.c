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
ssize_t vfs_read(struct rt_file_s *file, void *buf, os_size_t nbytes)
{
	struct rt_inode_s *inode;
	int ret = -E_EBADF;

	inode = file->f_inode;
	if ((file->f_oflags & O_RDOK) == 0)
	{
		ret = -E_EACCES;
	}
	else if (inode != NULL && inode->i_ops.i_fops->read)
	{
		ret = (int)inode->i_ops.i_fops->read(file, (char *)buf, nbytes);
	}

	if (ret < 0)
	{
		return E_ERROR;
	}

	return ret;
}
