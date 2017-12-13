/*!
********************************************************************************
* @file     : dev_serialint.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-12-13 first version
*******************************************************************************/
#include <config.h>
#include <menux/error.h>
#include <menux/nuxos/io/devserial.h>
#include <menux/nuxos/io/devserialimpl.h>

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
void devserial_xmitchars(io_serial_dev_t *dev)
{
	uint16_t nbytes = 0;

	while (dev->xmitbuf.head != dev->xmitbuf.tail && dev->ops->txready(dev))
	{
		dev->ops->send(dev, dev->xmitbuf.databuf[dev->xmitbuf.tail]);
		nbytes++;
		if (++(dev->xmitbuf.tail) >= dev->xmitbuf.size)
		{
			dev->xmitbuf.tail = 0;
		}
	}
	if (dev->xmitbuf.head == dev->xmitbuf.tail)
	{
		dev->ops->txintsetup(dev, false);
	}
	if (nbytes)
	{
		devserial_datasentdone(dev);
	}
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void devserial_recvchars(io_serial_dev_t *dev)
{
	uint16_t nbytes = 0;
	unsigned int status;
	struct io_serial_buf_s *rxbuf = &dev->recvbuf;
	int nexthead = rxbuf->head + 1;

	if (nexthead >= rxbuf->size)
	{
		nexthead = 0;
	}

	while (dev->ops->rxavailable(dev))
	{
		bool is_full = (nexthead == rxbuf->tail);
		char ch;

		ch = dev->ops->recv(dev, &status);
		if (!is_full)
		{
			rxbuf->databuf[rxbuf->head] = ch;
			nbytes++;
			rxbuf->head = nexthead;
			if (++nexthead >= rxbuf->size)
			{
				nexthead = 0;
			}
		}
	}

	if (nbytes)
	{
		devserial_datarecedone(dev);
	}
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void devserial_datarecedone(io_serial_dev_t *dev)
{
	OS_ERR error;

	if (dev->recvwaiting)
	{
		dev->recvwaiting = false;
		OSSemPost(&dev->xmitsemaphore, OS_OPT_POST_NONE, &error);
	}
}

/*!
********************************************************************************
* @brief : This functions used to called from devserial_xmitchars after serial
* 		   data has been sent, freeing up some space in the driver's circular
* 		   buffer.
*
* @param :
*
* @retval:
*******************************************************************************/
void devserial_datasentdone(io_serial_dev_t *dev)
{
	OS_ERR error;

	if (dev->xmitwaiting)
	{
		dev->xmitwaiting = false;
		OSSemPost(&dev->xmitsemaphore, OS_OPT_POST_NONE, &error);
	}
}
