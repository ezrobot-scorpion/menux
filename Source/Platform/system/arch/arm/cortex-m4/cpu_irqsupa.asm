/*!
********************************************************************************
* @file     : cpu_irqsupa.asm
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-12-13 first version
*******************************************************************************/
	.text
	.align 2
	.syntax unified

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/*!
********************************************************************************
* @brief : This functions used to
*
* @param :
*
* @retval:
*******************************************************************************/
	.thumb_func
	.global cpu_irq_enter_criticalsection
cpu_irq_enter_criticalsection:
	MRS     R0, PRIMASK
	CPSID   I
	BX      LR

/*!
********************************************************************************
* @brief : This functions used to
*
* @param :
*
* @retval:
*******************************************************************************/
	.thumb_func
	.global cpu_irq_leave_criticalsection
cpu_irq_leave_criticalsection:
	MSR     PRIMASK, R0
	BX      LR

	.end
