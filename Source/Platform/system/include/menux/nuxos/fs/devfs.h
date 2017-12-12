/*!
********************************************************************************
* @file     : devfs.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_FS_DEVFS_H
#define INCLUDE_MENUX_NUXOS_FS_DEVFS_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <menux/nuxos/fs/inode.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int devfs_chrdriver_register   (const char *path, const rt_file_ops_t *fops, mode_t mode, void *priv);
int devfs_chrdriver_unregister (const char *path);

int devfs_blkdriver_register   (const char *path, const rt_block_ops_t *bops, mode_t mode, void *priv);
int devfs_blkdriver_unregister (const char *path);
int devfs_blkdriver_find 	   (const char *path, int mountflags, rt_inode_t **ppinode);
int devfs_blkdriver_open       (const char *path, int mountflags, rt_inode_t **ppinode);

#ifdef __cplusplus
}
#endif
#endif
