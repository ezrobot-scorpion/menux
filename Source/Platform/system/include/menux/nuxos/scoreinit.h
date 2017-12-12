/*!
********************************************************************************
* @file     : scoreinit.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_SCORE_INIT_H
#define INCLUDE_MENUX_NUXOS_SCORE_INIT_H

/*!
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <menux/types.h>

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
rt_err_t rtx_system_init(void);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_SCORE_INIT_H */
