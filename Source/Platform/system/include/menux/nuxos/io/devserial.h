/*!
********************************************************************************
* @file     : devserial.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_IO_DEVSERIAL_H
#define INCLUDE_MENUX_NUXOS_IO_DEVSERIAL_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <menux/types.h>
#include <menux/nuxos.h>
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
struct io_serial_buf_s
{
	int16_t head;
	int16_t tail;
	int32_t size;
	char   *databuf;
	OS_MUTEX mutex;
};

struct io_serial_dev_s;
struct io_serial_ops_s
{
	int  (*setup)(struct io_serial_dev_s *dev);
	void (*shutdown)(struct io_serial_dev_s *dev);
	int	 (*attach)(struct io_serial_dev_s *dev);
	void (*detach)(struct io_serial_dev_s *dev);
	int	 (*ioctl)(struct rt_file_s *file, int cmd, unsigned long arg);
	int  (*recv)(struct io_serial_dev_s *dev, unsigned int *status);
	void (*rxintsetup)(struct io_serial_dev_s *dev, bool enable);
	bool (*rxavailable)(struct io_serial_dev_s *dev);
	void (*send)(struct io_serial_dev_s *dev, int ch);
	void (*txintsetup)(struct io_serial_dev_s *dev, bool enable);
	bool (*txready)(struct io_serial_dev_s *dev);
	bool (*txempty)(struct io_serial_dev_s *dev);
};

struct io_serial_dev_s
{
	uint8_t 	  			  		open_count;
	volatile bool 			  		xmitwaiting;
	volatile bool 			  		recvwaiting;
	volatile bool 			  		disconnected;
	volatile bool 			  		isconsole;
	OS_SEM							closesemaphore;
	OS_SEM							xmitsemaphore;
	OS_SEM							recvsemaphore;
	struct io_serial_buf_s 			xmitbuf;
	struct io_serial_buf_s 			recvbuf;
	const struct io_serial_ops_s 	*ops;
	void							*driverdata;
};
typedef struct io_serial_dev_s io_serial_dev_t;

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
int devserial_register(const char *path, io_serial_dev_t *dev);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_IO_DEVSERIAL_H */
