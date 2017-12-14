/*!
********************************************************************************
* @file     : list.h
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-12-13 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_SCORE_IRQ_H
#define INCLUDE_MENUX_NUXOS_SCORE_IRQ_H

/*
********************************************************************************
* INCLUDES
*******************************************************************************/
#include <config.h>
#include <menux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
********************************************************************************
* CONTANTS
*******************************************************************************/

/*!
********************************************************************************
* TYPE DEFINES
*******************************************************************************/

/*
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
irqstate_t rtx_irq_enter_criticalsection(void);
void 	   rtx_irq_leave_criticalsection(irqstate_t irqsave);

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_SCORE_IRQ_H */