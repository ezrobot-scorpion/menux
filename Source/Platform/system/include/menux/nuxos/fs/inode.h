/*!
********************************************************************************
* @file     : inode.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_FS_INODE_H
#define INCLUDE_MENUX_FS_INODE_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <menux/nuxos/fs/vfs.h>

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
typedef struct rt_inode_search_s
{
	const char *path;      	/* Path of inode to find */
	rt_inode_t *node;    	/* Pointer to the inode found */
	rt_inode_t *peer;    	/* Node to the "left" for the found inode */
	rt_inode_t *parent;		/* Node "above" the found inode */
	const char *relpath;   	/* Relative path into the mountpoint */
} rt_inode_search_t;

typedef int (*rt_foreach_inode_t)(rt_inode_t *node, char *dirpath, void *arg);

typedef struct rt_inode_path_s
{
	rt_foreach_inode_t handler;
	void              *arg;
	char               path[12];
} rt_inode_path_t;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
void 		inode_initialize	(void);
rt_inode_t *inode_alloc 		(const char *name);
void 		inode_wait_mutex	(void);
void 		inode_release_mutex	(void);
int			inode_compare 		(const char *fname, rt_inode_t *node);
int  		inode_search		(rt_inode_search_t *desc);
int  		inode_find			(rt_inode_search_t *desc);
int 		inode_foreach 		(rt_foreach_inode_t handler, void *arg);
int 		inode_foreachlevel 	(rt_inode_t *node, rt_inode_path_t *info);
int  		inode_stat			(rt_inode_t *inode, struct stat *buf);
void 		inode_free			(rt_inode_t *node);
void 		inode_insert 		(rt_inode_t *node, rt_inode_t *peer, rt_inode_t *parent);
void 		inode_namecpy 		(char *dest, const char *src);
int 		inode_namelen 		(const char *name);
const char *inode_nextname		(const char *name);
int 		inode_reserve		(const char *path, rt_inode_t **inode);
rt_inode_t *inode_unlink		(const char *path);
int 		inode_remove		(const char *path);
void 		inode_addref		(rt_inode_t *inode);
void 		inode_release		(rt_inode_t *inode);

#ifdef __cplusplus
}
#endif
#endif
