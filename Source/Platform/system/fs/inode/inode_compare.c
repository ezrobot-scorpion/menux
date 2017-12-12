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
int inode_compare(const char *fname, rt_inode_t *node)
{
	char *nname = node->i_name;

	if (!nname)
	{
		return 1;
	}

	if (!fname)
	{
		return -1;
	}

	for (;;)
	{
		if (!*nname)
		{
			if ((!*fname) || (*fname == '/'))
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else if ((!*fname) || (*fname == '/'))
		{
			return -1;
		}
		else if (*fname > *nname)
		{
			return 1;
		}
		else if (*fname < *nname)
		{
			return -1;
		}
		else
		{
			fname++;
			nname++;
		}
	}
}
