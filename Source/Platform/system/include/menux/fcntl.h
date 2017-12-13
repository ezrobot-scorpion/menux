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
#ifndef INCLUDE_MENUX_FCNTL_H
#define INCLUDE_MENUX_FCNTL_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/
#define O_RDONLY    (1 << 0)        /* Open for read access (only) */
#define O_RDOK      O_RDONLY        /* Read access is permitted (non-standard) */
#define O_WRONLY    (1 << 1)        /* Open for write access (only) */
#define O_WROK      O_WRONLY        /* Write access is permitted (non-standard) */
#define O_RDWR      (O_RDOK|O_WROK) /* Open for both read & write access */
#define O_CREAT     (1 << 2)        /* Create file/sem/mq object */
#define O_EXCL      (1 << 3)        /* Name must not exist when opened  */
#define O_APPEND    (1 << 4)        /* Keep contents, append to end */
#define O_TRUNC     (1 << 5)        /* Delete contents */
#define O_NONBLOCK  (1 << 6)        /* Don't wait for data */
#define O_NDELAY    O_NONBLOCK      /* Synonym for O_NONBLOCK */
#define O_SYNC      (1 << 7)        /* Synchronize output on write */
#define O_DSYNC     O_SYNC          /* Equivalent to OSYNC in NuttX */
#define O_BINARY    (1 << 8)        /* Open the file in binary (untranslated) mode. */

/* Unsupported, but required open flags */
#define O_RSYNC     0               /* Synchronize input on read */
#define O_ACCMODE   O_RDWR          /* Mask for access mode */
#define O_NOCTTY    0               /* Required by POSIX */
#define O_TEXT      0               /* Open the file in text (translated) mode. */

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_FCNTL_H */
