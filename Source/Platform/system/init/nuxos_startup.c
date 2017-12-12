/*!
********************************************************************************
* @file     : nuxos_startup.c
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
#include <arch/cpuinit.h>
#include <menux/nuxos.h>
#include <menux/nuxos/ioinit.h>
#include <menux/nuxos/fsinit.h>
#include <menux/nuxos/meminit.h>
#include <menux/nuxos/sapiinit.h>
#include <menux/nuxos/scoreinit.h>
#include <menux/nuxos/sobjectinit.h>

static OS_TCB InitTCB;
static CPU_STK InitSTK[128];

extern void nuxos_premain(void *argument);

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
void nuxos_startup(void)
{
	OS_ERR Error;

	CPU_Init();

	// TODO: Disable the system globle interrupt.

	// TODO: Initialize the hal system.

	// Initialize the cpu system.
	cpu_system_init();

	// Initialize the rtx system.
	if (rtx_system_init() != OS_ERR_NONE)
	{
		for (;;);
	}

	// Initialize the mem system.
	if (mem_system_init() < 0)
	{
		for (;;);
	}

	// Initialize the obj system.
	obj_system_init();

	// Initialize the api system.
	api_system_init();

	// Initialize the vfs system.
	if (vfs_system_init() < 0)
	{
		for (;;);
	}

	// Initialize the dev system.
	if (dev_system_init() < 0)
	{
		for (;;);
	}

	// TODO: Initialize the lib system.

	// TODO: Initialize the net system.
	OS_CPU_SysTickInit(100000);
	OSTaskCreate((OS_TCB       *)&InitTCB,
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )nuxos_premain,
                 (void         *)0,
                 (OS_PRIO       )12,
                 (CPU_STK      *)&InitSTK[0],
                 (CPU_STK_SIZE  )128 / 10,
                 (CPU_STK_SIZE  )128,
                 (OS_MSG_QTY    )0,
                 (OS_TICK       )0,
                 (void         *)0,
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR       *)&Error);

    OSStart(&Error);
}

