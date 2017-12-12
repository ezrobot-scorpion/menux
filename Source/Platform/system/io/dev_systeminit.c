/*!
********************************************************************************
* @file     : dev_systeminit.c
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
#include <menux/nuxos/ioinit.h>
#include <menux/nuxos/io/devnull.h>
#include <menux/nuxos/io/devzero.h>

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/*!
********************************************************************************
* @brief : This function used to initializ kernel device.
*
* @param : None.
*
* @retval: If success returns E_OK, otherwise returns negative value.
*******************************************************************************/
int dev_system_init(void)
{
	int ret;

	ret = devnull_register();
	if (ret < 0)
	{
		return ret;
	}

	ret = devzero_register();
	if (ret < 0)
	{
		return ret;
	}

	return ret;
}
