/*!
********************************************************************************
* @file     : devzeroimpl.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_IO_DEVZERO_IMPL_H
#define INCLUDE_MENUX_NUXOS_IO_DEVZERO_IMPL_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <string.h>
#include <menux/nuxos/fs/vfs.h>
#include <menux/nuxos/fs/devfs.h>
#include <menux/nuxos/io/devzero.h>

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
ssize_t devzero_read (rt_file_t *file,       char *buffer, os_size_t len);
ssize_t devzero_write(rt_file_t *file, const char *buffer, os_size_t len);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_IO_DEVZERO_IMPL_H */
