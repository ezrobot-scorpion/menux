/*!
********************************************************************************
* @file     : dev_zero.c
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
#include <menux/nuxos/io/devzero.h>
#include <menux/nuxos/io/devzeroimpl.h>

/*!
********************************************************************************
* LOCAL VARIABLES
*******************************************************************************/
static const rt_file_ops_t gx_devzero_fops =
{
	NULL,
	NULL,
	devzero_read,
	devzero_write,
	NULL,
	NULL,
	NULL,
	NULL
};

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
int devzero_register(void)
{
	return devfs_chrdriver_register("/dev/zero", &gx_devzero_fops, 0666, NULL);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
ssize_t devzero_read(rt_file_t *file, char *buffer, os_size_t len)
{
	memset(buffer, 0, len);
	return len;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
ssize_t devzero_write(rt_file_t *file, const char *buffer, os_size_t len)
{
	return len;
}
