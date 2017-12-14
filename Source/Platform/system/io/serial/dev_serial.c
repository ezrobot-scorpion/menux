/*!
********************************************************************************
* @file     : dev_serial.c
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
#include <menux/fcntl.h>
#include <menux/nuxos/fs/ioctl.h>
#include <menux/nuxos/fs/devfs.h>
#include <menux/nuxos/io/devserial.h>
#include <menux/nuxos/io/devserialimpl.h>
#include <menux/nuxos/score/irq.h>

/*!
********************************************************************************
* LOCAL VARIABLES
*******************************************************************************/
static const rt_file_ops_t gx_devserial_fops =
{
	devserial_open,
	devserial_close,
	devserial_read,
	devserial_write,
	NULL,
	devserial_ioctl,
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
int devserial_register(const char *path, io_serial_dev_t *dev)
{
	OS_ERR error;

	OSMutexCreate(&dev->xmitbuf.mutex, "mutex", &error);
	OSMutexCreate(&dev->recvbuf.mutex, "mutex", &error);
	OSSemCreate(&dev->xmitsemaphore, "semaphore", 0, &error);
	OSSemCreate(&dev->recvsemaphore, "semaphore", 0, &error);
	//sem_init(&dev->closesem, 0, 1);

	return devfs_chrdriver_register(path, &gx_devserial_fops, 0666, dev);
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int devserial_open(struct rt_file_s *filep)
{
	struct rt_inode_s *inode = filep->f_inode;
	io_serial_dev_t *dev = inode->i_private;
	uint8_t tmp;
	int ret;

	/*
	ret = uart_takesem(&dev->closesemaphore, true);
	if (ret < 0)
	{
		return ret;
	}*/

	if (dev->disconnected)
	{
		ret = -E_ENOTCONN;
		goto errout_with_sem;
	}

	tmp = dev->open_count + 1;
	if (tmp == 0)
	{
		ret = -E_EMFILE;
		goto errout_with_sem;
	}

	if (tmp == 1)
	{
		irqstate_t flags = rtx_irq_enter_criticalsection();

		if (!dev->isconsole)
		{
			ret = dev->ops->setup(dev);
			if (ret < 0)
			{
				rtx_irq_leave_criticalsection(flags);
				goto errout_with_sem;
			}
		}

		ret = dev->ops->attach(dev);
		if (ret < 0)
		{
			dev->ops->shutdown(dev);
			rtx_irq_leave_criticalsection(flags);
			goto errout_with_sem;
		}

		dev->xmitbuf.head = 0;
		dev->xmitbuf.tail = 0;
		dev->recvbuf.head = 0;
		dev->recvbuf.tail = 0;

		dev->ops->txintsetup(dev, true);
		rtx_irq_leave_criticalsection(flags);
	}

	dev->open_count = tmp;

errout_with_sem:
  //uart_givesem(&dev->closesem);
  return ret;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int devserial_close(struct rt_file_s *filep)
{
	struct rt_inode_s *inode = filep->f_inode;
	io_serial_dev_t *dev = inode->i_private;
	irqstate_t flags;

	//uart_takesem(&dev->closesem, false);
	if (dev->open_count > 1)
	{
		dev->open_count--;
		//uart_givesem(&dev->closesem);
		return E_OK;
	}

	dev->open_count = 0;
	dev->ops->rxintsetup(dev, false);
	while (dev->xmitbuf.head != dev->xmitbuf.tail)
	{
	}

	while (!(dev->ops->txempty(dev)))
	{
	}

	flags = rtx_irq_enter_criticalsection(); /* Disable interrupts */
	dev->ops->detach(dev);
	if (!dev->isconsole)
	{
		dev->ops->shutdown(dev);
	}

	rtx_irq_leave_criticalsection(flags);

	//sem_reset(&dev->xmitsem, 0);
	//sem_reset(&dev->recvsem, 0);
	//sem_reset(&dev->xmit.sem, 1);
	//sem_reset(&dev->recv.sem, 1);

	//uart_givesem(&dev->closesem);
	return E_OK;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int devserial_ioctl(struct rt_file_s *filep, int cmd, unsigned long arg)
{
	struct rt_inode_s *inode = filep->f_inode;
	io_serial_dev_t *dev = inode->i_private;

	int ret = dev->ops->ioctl(filep, cmd, arg);

	if (ret == -E_ENOTTY)
	{
		switch (cmd)
		{
			case FIONREAD:
			{
				int count;
				irqstate_t flags = rtx_irq_enter_criticalsection();

				if (dev->recvbuf.tail <= dev->recvbuf.head)
				{
					count = dev->recvbuf.head - dev->recvbuf.tail;
				}
				else
				{
					count = dev->recvbuf.size - (dev->recvbuf.tail - dev->recvbuf.head);
				}

				rtx_irq_leave_criticalsection(flags);

				*(int *) ((uintptr_t) arg) = count;
				ret = 0;
				break;
			}

			case FIONWRITE:
			{
				int count;
				irqstate_t flags = rtx_irq_enter_criticalsection();

				if (dev->xmitbuf.tail <= dev->xmitbuf.head)
				{
					count = dev->xmitbuf.head - dev->xmitbuf.tail;
				}
				else
				{
					count = dev->xmitbuf.size - (dev->xmitbuf.tail - dev->xmitbuf.head);
				}

				rtx_irq_leave_criticalsection(flags);

				*(int *) ((uintptr_t) arg) = count;
				ret = 0;
				break;
			}

			case FIONSPACE:
			{
				int count;
				irqstate_t flags = rtx_irq_enter_criticalsection();

				if (dev->xmitbuf.head < dev->xmitbuf.tail)
				{
					count = dev->xmitbuf.tail - dev->xmitbuf.head - 1;
				}
				else
				{
					count = dev->xmitbuf.size - (dev->xmitbuf.head - dev->xmitbuf.tail)
							- 1;
				}

				rtx_irq_leave_criticalsection(flags);

				*(int *) ((uintptr_t) arg) = count;
				ret = 0;
				break;
			}
		}
	}

	return ret;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
ssize_t devserial_read(struct rt_file_s *filep, char *buf, os_size_t len)
{
	OS_ERR error;
	struct rt_inode_s *inode = filep->f_inode;
	io_serial_dev_t *dev = inode->i_private;
	struct io_serial_buf_s *rxbuf = &dev->recvbuf;
	irqstate_t flags;
	ssize_t recvd = 0;
	int16_t tail;
	char ch;
	int ret;

	//ret = uart_takesem(&rxbuf->sem, true);
	OSMutexPend(&rxbuf->mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &error);
	//if (ret < 0)
	//{
	//	return ret;
	//}

	while ((size_t) recvd < len)
	{
		if (dev->disconnected)
		{
			if (recvd == 0)
			{
				recvd = -E_ENOTCONN;
			}
			break;
		}

		tail = rxbuf->tail;
		if (rxbuf->head != tail)
		{
			ch = rxbuf->databuf[tail];
			if (++tail >= rxbuf->size)
			{
				tail = 0;
			}

			rxbuf->tail = tail;
			*buf++ = ch;
			recvd++;
		}
		else if (recvd > 0)
		{
			break;
		}
		else if ((filep->f_oflags & O_NONBLOCK) != 0)
		{
			recvd = -E_EAGAIN;
			break;
		}
		else
        {
			dev->ops->rxintsetup(dev, false);
			if (rxbuf->head == rxbuf->tail)
			{
				flags = rtx_irq_enter_criticalsection();
				dev->ops->rxintsetup(dev, true);
				if (dev->disconnected)
				{
					ret = -E_ENOTCONN;
				}
				else
				{
					dev->recvwaiting = true;
					//ret = uart_takesem(&dev->recvsem, true);
				}

				rtx_irq_leave_criticalsection(flags);

				if (ret < 0 || dev->disconnected)
				{
					if (recvd == 0)
					{
						recvd = dev->disconnected ? -E_ENOTCONN : -E_EINTR;
					}
					break;
				}
			}
			else
			{
				dev->ops->rxintsetup(dev, true);
			}
		}
	}

	//uart_givesem(&dev->recv.sem);

	return recvd;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
ssize_t devserial_write(struct rt_file_s *filep, const char *buf, os_size_t len)
{
	struct rt_inode_s *inode = filep->f_inode;
	io_serial_dev_t *dev = inode->i_private;
	ssize_t nwritten = len;
	bool oktoblock;
	OS_ERR error;
	int ret;
	char ch;

	//if (up_interrupt_context() || getpid() == 0)
	{
		if (dev->disconnected)
		{
			return -E_ENOTCONN;
		}
		if (dev->isconsole)
		{
			irqstate_t flags = rtx_irq_enter_criticalsection();
			//ret = uart_irqwrite(dev, buffer, buflen);
			rtx_irq_leave_criticalsection(flags);
			return ret;
		}
		else
		{
			return -E_EPERM;
		}
	}

	OSMutexPend(&dev->xmitbuf.mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &error);
	if (dev->disconnected)
	{
		OSMutexPost(&dev->xmitbuf.mutex, OS_OPT_POST_NONE, &error);
		return -E_ENOTCONN;
	}

	oktoblock = ((filep->f_oflags & O_NONBLOCK) == 0);
	dev->ops->txintsetup(dev, false);
	for (; len; len--)
	{
		ch = *buf++;
		ret = E_OK;

		if (dev->isconsole && ch == '\n')
		{
			ret = devserial_putxmitchar(dev, '\r', oktoblock);
		}
		if (ret == E_OK)
		{
			ret = devserial_putxmitchar(dev, ch, oktoblock);
		}
		if (ret < 0)
		{
			if (len < (size_t) nwritten)
			{
				nwritten -= len;
			}
			else
			{
				nwritten = ret;
			}

			break;
		}
	}

	if (dev->xmitbuf.head != dev->xmitbuf.tail)
	{
		dev->ops->txintsetup(dev, true);
	}

	OSMutexPost(&dev->xmitbuf.mutex, OS_OPT_POST_NONE, &error);

	return nwritten;
}

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int devserial_putxmitchar(io_serial_dev_t *dev, int ch, bool oktoblock)
{
	irqstate_t flags;
	int nexthead;
	int ret;

	nexthead = dev->xmitbuf.head + 1;
	if (nexthead >= dev->xmitbuf.size)
	{
		nexthead = 0;
	}

	for (;;)
	{
		if (nexthead != dev->xmitbuf.tail)
		{
			dev->xmitbuf.databuf[dev->xmitbuf.head] = ch;
			dev->xmitbuf.head = nexthead;
			return E_OK;
		}
		else if (oktoblock)
		{
			flags = rtx_irq_enter_criticalsection();

			if (nexthead != dev->xmitbuf.tail)
			{
				ret = E_OK;
			}
			else if (dev->disconnected)
			{
				ret = -E_ENOTCONN;
			}
			else
			{
				dev->xmitwaiting = true;

				dev->ops->txintsetup(dev, true);

				//ret = uart_takesem(&dev->xmitsem, true);

				dev->ops->txintsetup(dev, false);
			}

			rtx_irq_leave_criticalsection(flags);
			if (dev->disconnected)
			{
				return -E_ENOTCONN;
			}
			if (ret < 0)
			{
				return -E_EINTR;
			}
		}
		else
		{
			return -E_EAGAIN;
		}
	}

	return E_OK;
}
