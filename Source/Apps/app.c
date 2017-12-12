/*!
********************************************************************************
* @file     : app.c
*
* @brief    :
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#include <menux/nuxos.h>

static int gx_counter = 0;

static OS_TCB InitTCB;
static CPU_STK InitSTK[128];

void xInitTask_Routine(void *argument);

/*!
********************************************************************************
* @brief :
*
* @param :
*
* @retval:
*******************************************************************************/
int main(int argc, char* argv[])
{
	OS_ERR Error;

	OSTaskCreate((OS_TCB       *)&InitTCB,
                 (CPU_CHAR     *)"App Task Start",
                 (OS_TASK_PTR   )xInitTask_Routine,
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
}

void xInitTask_Routine(void *argument)
{
	OS_ERR Error;

	while (1)
	{
		gx_counter++;
		OSTimeDly(100, OS_OPT_TIME_DLY, &Error);
	}
}

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{

}
#endif

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return 0;
}
#endif


#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    return 0;
}
#endif
