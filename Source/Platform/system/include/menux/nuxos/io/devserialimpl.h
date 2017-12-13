/*!
********************************************************************************
* @file     : devserialimpl.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_IO_DEVSERIAL_IMPL_H
#define INCLUDE_MENUX_NUXOS_IO_DEVSERIAL_IMPL_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <menux/types.h>
#include <menux/nuxos/fs/vfs.h>
#include <menux/nuxos/io/devserial.h>

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
int     devserial_open (struct rt_file_s *filep);
int     devserial_close(struct rt_file_s *filep);
int     devserial_ioctl(struct rt_file_s *filep, int cmd, unsigned long arg);
ssize_t devserial_read (struct rt_file_s *filep,       char *buf, os_size_t len);
ssize_t devserial_write(struct rt_file_s *filep, const char *buf, os_size_t len);
int 	devserial_putxmitchar(io_serial_dev_t *dev, int ch, bool oktoblock);


void 	devserial_xmitchars(io_serial_dev_t *dev);
void 	devserial_recvchars(io_serial_dev_t *dev);
void 	devserial_datarecedone(io_serial_dev_t *dev);
void 	devserial_datasentdone(io_serial_dev_t *dev);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_IO_DEVSERIAL_IMPL_H */
