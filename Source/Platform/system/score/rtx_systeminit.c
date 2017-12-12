/*!
********************************************************************************
* @file     : rtx_systeminit.c
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
#include <menux/nuxos.h>
#include <menux/nuxos/scoreinit.h>

/*!
********************************************************************************
* PUBLIC FUNCTIONS
*******************************************************************************/
/*!
********************************************************************************
* @brief : This function used to initialize kernel manager moudule.
*
* @param : None.
*
* @retval: None.
*******************************************************************************/
rt_err_t rtx_system_init(void)
{
	OS_ERR error;

    CPU_STK      *p_stk;
    CPU_STK_SIZE  size;


    OSInitHook();                                           /* Call port specific initialization code                 */

    OSIntNestingCtr                 = (OS_NESTING_CTR)0;    /* Clear the interrupt nesting counter                    */

    OSRunning                       =  OS_STATE_OS_STOPPED; /* Indicate that multitasking not started                 */

    OSSchedLockNestingCtr           = (OS_NESTING_CTR)0;    /* Clear the scheduling lock counter                      */

    OSTCBCurPtr                     = (OS_TCB *)0;          /* Initialize OS_TCB pointers to a known state            */
    OSTCBHighRdyPtr                 = (OS_TCB *)0;

    OSPrioCur                       = (OS_PRIO)0;           /* Initialize priority variables to a known state         */
    OSPrioHighRdy                   = (OS_PRIO)0;
    OSPrioSaved                     = (OS_PRIO)0;

#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
    OSSchedLockTimeBegin            = (CPU_TS)0;
    OSSchedLockTimeMax              = (CPU_TS)0;
    OSSchedLockTimeMaxCur           = (CPU_TS)0;
#endif

#ifdef OS_SAFETY_CRITICAL_IEC61508
    OSSafetyCriticalStartFlag       =  DEF_FALSE;
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
    OSSchedRoundRobinEn             = DEF_FALSE;
    OSSchedRoundRobinDfltTimeQuanta = OSCfg_TickRate_Hz / 10u;
#endif

    if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) {
        p_stk = OSCfg_ISRStkBasePtr;                        /* Clear exception stack for stack checking.              */
        if (p_stk != (CPU_STK *)0) {
            size  = OSCfg_ISRStkSize;
            while (size > (CPU_STK_SIZE)0) {
                size--;
               *p_stk = (CPU_STK)0;
                p_stk++;
            }
        }
    }

#if OS_CFG_TASK_REG_TBL_SIZE > 0u
    OSTaskRegNextAvailID    = (OS_REG_ID)0;
#endif

    OS_PrioInit();                                          /* Initialize the priority bitmap table                   */

    OS_RdyListInit();                                       /* Initialize the Ready List                              */


#if OS_CFG_FLAG_EN > 0u                                     /* Initialize the Event Flag module                       */
    OS_FlagInit(&error);
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif

#if (OS_MSG_EN) > 0u                                        /* Initialize the free list of OS_MSGs                    */
    OS_MsgPoolInit(&error);
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


#if OS_CFG_MUTEX_EN > 0u                                    /* Initialize the Mutex Manager module                    */
    OS_MutexInit(&error);
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


#if OS_CFG_Q_EN > 0u
    OS_QInit(&error);                                        /* Initialize the Message Queue Manager module            */
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


#if OS_CFG_SEM_EN > 0u                                      /* Initialize the Semaphore Manager module                */
    OS_SemInit(&error);
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_Init(p_err);                                     /* Initialize Task Local Storage, before creating tasks   */
    if (error != OS_ERR_NONE) {
        return;
    }
#endif


    OS_TaskInit(&error);                                     /* Initialize the task manager                            */
    if (error != OS_ERR_NONE) {
        return error;
    }


#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
    OS_IntQTaskInit(&error);                                 /* Initialize the Interrupt Queue Handler Task            */
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


    OS_IdleTaskInit(&error);                                 /* Initialize the Idle Task                               */
    if (error != OS_ERR_NONE) {
        return error;
    }


    OS_TickTaskInit(&error);                                 /* Initialize the Tick Task                               */
    if (error != OS_ERR_NONE) {
        return error;
    }


#if OS_CFG_STAT_TASK_EN > 0u                                /* Initialize the Statistic Task                          */
    OS_StatTaskInit(&error);
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


#if OS_CFG_TMR_EN > 0u                                      /* Initialize the Timer Manager module                    */
    OS_TmrInit(&error);
    if (error != OS_ERR_NONE) {
        return error;
    }
#endif


#if OS_CFG_DBG_EN > 0u
    OS_Dbg_Init();
#endif


    OSCfg_Init();

    return OS_ERR_NONE;
}
