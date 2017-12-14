/*!
********************************************************************************
* @file     : vfs_ioctl.c
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
int vfs_ioctl(struct rt_file_s *file, int reqnum, unsigned long argval)
{
	struct rt_inode_s *inode;
	int errcode;
	int ret;

	inode = file->f_inode;
	if (!inode)
	{
		errcode = E_EBADF;
		goto errout;
	}

	if (inode->i_ops.i_fops == NULL || inode->i_ops.i_fops->ioctl == NULL)
	{
		errcode = E_ENOTTY;
		goto errout;
	}

	ret = (int)inode->i_ops.i_fops->ioctl(file, reqnum, argval);
	if (ret < 0)
	{
		errcode = -ret;
		goto errout;
	}

	return ret;

errout:
	return E_ERROR;
}
