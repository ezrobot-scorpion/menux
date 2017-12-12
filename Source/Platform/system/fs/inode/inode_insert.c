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
void inode_insert(rt_inode_t *node, rt_inode_t *peer, rt_inode_t *parent)
{
	if (peer)
	{
		node->i_peer = peer->i_peer;
		peer->i_peer = node;
	}
	else if (parent)
	{
		node->i_peer = parent->i_child;
		parent->i_child = node;
	}
	else
	{
		node->i_peer = g_root_inode;
		g_root_inode = node;
	}
}
