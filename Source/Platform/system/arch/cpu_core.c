/*
*********************************************************************************************************
*                                                uC/CPU
*                                    CPU CONFIGURATION & PORT LAYER
*
*                          (c) Copyright 2004-2011; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/CPU is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           CORE CPU MODULE
*
* Filename      : cpu_core.c
* Version       : V1.29.01
* Programmer(s) : SR
*                 ITJ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    MICRIUM_SOURCE
#define    CPU_CORE_MODULE
#include  <cpu_core.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*$PAGE*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                    /* ----------------- CPU TS FNCTS ----------------- */
#if   ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
       (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
static  void        CPU_TS_Init          (void);
#endif


#ifdef  CPU_CFG_INT_DIS_MEAS_EN                                     /* ---------- CPU INT DIS TIME MEAS FNCTS --------- */
static  void        CPU_IntDisMeasInit   (void);

static  CPU_TS_TMR  CPU_IntDisMeasMaxCalc(CPU_TS_TMR  time_tot_cnts);
#endif


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*                                             CPU_Init()
*
* Description : (1) Initialize CPU module :
*
*                   (a) Initialize CPU timestamps
*                   (b) Initialize CPU interrupts disabled time measurements
*                   (c) Initialize CPU host name
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Your Product's Application.
*
*               This function is a CPU initialization function & MAY be called by application/
*               initialization function(s).
*
* Note(s)     : (2) CPU_Init() MUST be called ... :
*
*                   (a) ONLY ONCE from a product's application; ...
*                   (b) BEFORE product's application calls any core CPU module function(s)
*
*               (3) The following initialization functions MUST be sequenced as follows :
*
*                   (a) CPU_TS_Init()           SHOULD precede ALL calls to other CPU timestamp functions
*
*                   (b) CPU_IntDisMeasInit()    SHOULD precede ALL calls to CPU_CRITICAL_ENTER()/CPU_CRITICAL_EXIT()
*                                                   & other CPU interrupts disabled time measurement functions
*********************************************************************************************************
*/

void  CPU_Init (void)
{
                                                                /* --------------------- INIT TS ---------------------- */
#if ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
     (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
    CPU_TS_Init();                                              /* See Note #3a.                                        */
#endif
                                                                /* -------------- INIT INT DIS TIME MEAS -------------- */
#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasInit();                                       /* See Note #3b.                                        */
#endif
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_SW_Exception()
*
* Description : Trap unrecoverable software exception.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : various.
*
* Note(s)     : (1) CPU_SW_Exception() deadlocks the current code execution -- whether multi-tasked/
*                   -processed/-threaded or single-threaded -- when the current code execution cannot 
*                   gracefully recover or report a fault or exception condition.
*
*                   See also 'cpu_core.h  CPU_SW_EXCEPTION()  Note #1'.
*********************************************************************************************************
*/

void  CPU_SW_Exception (void)
{
    while (DEF_ON) {
        ;
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_Get32()
*
* Description : Get current 32-bit CPU timestamp.
*
* Argument(s) : none.
*
* Return(s)   : Current 32-bit CPU timestamp (in timestamp timer counts).
*
* Caller(s)   : Application.
*
*               This function is a CPU module application programming interface (API) function & MAY 
*               be called by application function(s).
*
* Note(s)     : (1) When applicable, the amount of time measured by CPU timestamps is calculated by
*                   either of the following equations :
*
*                       (a) Time measured  =  Number timer counts  *  Timer period
*
*                               where
*
*                                   Number timer counts     Number of timer counts measured
*                                   Timer period            Timer's period in some units of
*                                                               (fractional) seconds
*                                   Time measured           Amount of time measured, in same
*                                                               units of (fractional) seconds
*                                                               as the Timer period
*
*                                              Number timer counts
*                       (b) Time measured  =  ---------------------
*                                                Timer frequency
*
*                               where
*
*                                   Number timer counts     Number of timer counts measured
*                                   Timer frequency         Timer's frequency in some units
*                                                               of counts per second
*                                   Time measured           Amount of time measured, in seconds
*
*                   See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2c1'.
*
*               (2) In case the CPU timestamp timer has lower precision than the 32-bit CPU timestamp;
*                   its precision is extended via periodic updates by accumulating the deltas of the
*                   timestamp timer count values into the higher-precision 32-bit CPU timestamp.
*
*               (3) After initialization, 'CPU_TS_32_Accum' & 'CPU_TS_32_TmrPrev' MUST ALWAYS
*                   be accessed AND updated exclusively with interrupts disabled -- but NOT
*                   with critical sections.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_TS32  CPU_TS_Get32 (void)
{
    CPU_TS32    ts;
#if (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_32)
    CPU_TS_TMR  tmr_cur;
    CPU_TS_TMR  tmr_delta;
    CPU_SR_ALLOC();
#endif


#if (CPU_CFG_TS_TMR_SIZE >= CPU_WORD_SIZE_32)
    ts = (CPU_TS32)CPU_TS_TmrRd();                                  /* Get cur ts tmr val (in 32-bit ts cnts).          */

#else
    CPU_INT_DIS();
    tmr_cur            = (CPU_TS_TMR) CPU_TS_TmrRd();               /* Get cur ts tmr val (in ts tmr cnts).             */
    tmr_delta          = (CPU_TS_TMR)(tmr_cur - CPU_TS_32_TmrPrev); /* Calc      delta ts tmr cnts.                     */
    CPU_TS_32_Accum   += (CPU_TS32  ) tmr_delta;                    /* Inc ts by delta ts tmr cnts (see Note #2).       */
    CPU_TS_32_TmrPrev  = (CPU_TS_TMR) tmr_cur;                      /* Save cur ts tmr cnts for next update.            */
    ts                 = (CPU_TS32  ) CPU_TS_32_Accum;
    CPU_INT_EN();
#endif

    return (ts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_Get64()
*
* Description : Get current 64-bit CPU timestamp.
*
* Argument(s) : none.
*
* Return(s)   : Current 64-bit CPU timestamp (in timestamp timer counts).
*
* Caller(s)   : Application.
*
*               This function is a CPU module application programming interface (API) function & MAY 
*               be called by application function(s).
*
* Note(s)     : (1) When applicable, the amount of time measured by CPU timestamps is calculated by
*                   either of the following equations :
*
*                       (a) Time measured  =  Number timer counts  *  Timer period
*
*                               where
*
*                                   Number timer counts     Number of timer counts measured
*                                   Timer period            Timer's period in some units of
*                                                               (fractional) seconds
*                                   Time measured           Amount of time measured, in same
*                                                               units of (fractional) seconds
*                                                               as the Timer period
*
*                                              Number timer counts
*                       (b) Time measured  =  ---------------------
*                                                Timer frequency
*
*                               where
*
*                                   Number timer counts     Number of timer counts measured
*                                   Timer frequency         Timer's frequency in some units
*                                                               of counts per second
*                                   Time measured           Amount of time measured, in seconds
*
*                   See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2c1'.
*
*               (2) In case the CPU timestamp timer has lower precision than the 64-bit CPU timestamp;
*                   its precision is extended via periodic updates by accumulating the deltas of the
*                   timestamp timer count values into the higher-precision 64-bit CPU timestamp.
*
*               (3) After initialization, 'CPU_TS_64_Accum' & 'CPU_TS_64_TmrPrev' MUST ALWAYS
*                   be accessed AND updated exclusively with interrupts disabled -- but NOT
*                   with critical sections.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_TS64  CPU_TS_Get64 (void)
{
    CPU_TS64    ts;
#if (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_64)
    CPU_TS_TMR  tmr_cur;
    CPU_TS_TMR  tmr_delta;
    CPU_SR_ALLOC();
#endif


#if (CPU_CFG_TS_TMR_SIZE >= CPU_WORD_SIZE_64)
    ts = (CPU_TS64)CPU_TS_TmrRd();                                  /* Get cur ts tmr val (in 64-bit ts cnts).          */

#else
    CPU_INT_DIS();
    tmr_cur            = (CPU_TS_TMR) CPU_TS_TmrRd();               /* Get cur ts tmr val (in ts tmr cnts).             */
    tmr_delta          = (CPU_TS_TMR)(tmr_cur - CPU_TS_64_TmrPrev); /* Calc      delta ts tmr cnts.                     */
    CPU_TS_64_Accum   += (CPU_TS64  ) tmr_delta;                    /* Inc ts by delta ts tmr cnts (see Note #2).       */
    CPU_TS_64_TmrPrev  = (CPU_TS_TMR) tmr_cur;                      /* Save cur ts tmr cnts for next update.            */
    ts                 = (CPU_TS64  ) CPU_TS_64_Accum;
    CPU_INT_EN();
#endif

    return (ts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_Update()
*
* Description : Update current CPU timestamp(s).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application/BSP periodic time handler (see Note #1).
*
*               This function is a CPU timestamp BSP function & SHOULD be called only by appropriate
*               application/BSP function(s).
*
* Note(s)     : (1) (a) CPU timestamp(s) MUST be updated periodically by some application (or BSP) time
*                       handler in order to (adequately) maintain CPU timestamp(s)' time.
*
*                   (b) CPU timestamp(s) MUST be updated more frequently than the CPU timestamp timer
*                       overflows; otherwise, CPU timestamp(s) will lose time.
*
*                       See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2c2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_EN == DEF_ENABLED)
void  CPU_TS_Update (void)
{
#if ((CPU_CFG_TS_32_EN    == DEF_ENABLED)  && \
     (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_32))
   (void)CPU_TS_Get32();
#endif

#if ((CPU_CFG_TS_64_EN    == DEF_ENABLED)  && \
     (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_64))
   (void)CPU_TS_Get64();
#endif
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TS_TmrFreqGet()
*
* Description : Get CPU timestamp's timer frequency.
*
* Argument(s) : p_err       Pointer to variable that will receive the return error code from this function :
*
*                               CPU_ERR_NONE                    CPU timestamp's timer frequency successfully
*                                                                   returned.
*                               CPU_ERR_TS_FREQ_INVALID         CPU timestamp's timer frequency invalid &/or
*                                                                   NOT yet configured.
*
* Return(s)   : CPU timestamp's timer frequency (in Hertz), if NO error(s).
*
*               0,                                          otherwise.
*
* Caller(s)   : Application.
*
*               This function is a CPU module application programming interface (API) function & MAY be 
*               called by application function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR_FREQ  CPU_TS_TmrFreqGet (CPU_ERR  *p_err)
{
    CPU_TS_TMR_FREQ  freq_hz;


    if (p_err == (CPU_ERR *)0) {
        CPU_SW_EXCEPTION(;);
    }

    freq_hz =  CPU_TS_TmrFreq_Hz;
   *p_err   = (freq_hz != 0u) ? CPU_ERR_NONE : CPU_ERR_TS_FREQ_INVALID;

    return (freq_hz);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TS_TmrFreqSet()
*
* Description : Set CPU timestamp's timer frequency.
*
* Argument(s) : freq_hz     Frequency (in Hertz) to set for CPU timestamp's timer.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_TmrInit(),
*               Application/BSP initialization function(s).
*
*               This function is a CPU module BSP function & SHOULD be called only by appropriate
*               application/BSP function(s) [see Note #1].
*
* Note(s)     : (1) (a) (1) CPU timestamp timer frequency is NOT required for internal CPU timestamp
*                           operations but may OPTIONALLY be configured by CPU_TS_TmrInit() or other
*                           application/BSP initialization functions.
*
*                       (2) CPU timestamp timer frequency MAY be used with optional CPU_TSxx_to_uSec()
*                           to convert CPU timestamps from timer counts into microseconds.
*
*                           See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2a'.
*
*                   (b) CPU timestamp timer period SHOULD be less than the typical measured time but MUST
*                       be less than the maximum measured time; otherwise, timer resolution inadequate to
*                       measure desired times.
*
*                       See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2b'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrFreqSet (CPU_TS_TMR_FREQ  freq_hz)
{
    CPU_TS_TmrFreq_Hz = freq_hz;
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                     CPU_IntDisMeasMaxCurReset()
*
* Description : Reset current maximum interrupts disabled time.
*
* Argument(s) : none.
*
* Return(s)   : Maximum interrupts disabled time (in CPU timestamp timer counts) before resetting.
*
*               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                      & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
* Caller(s)   : Application.
*
*               This function is a CPU module application programming interface (API) function 
*               & MAY be called by application function(s).
*
* Note(s)     : (1) After initialization, 'CPU_IntDisMeasMaxCur_cnts' MUST ALWAYS be accessed
*                   exclusively with interrupts disabled -- but NOT with critical sections.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
CPU_TS_TMR  CPU_IntDisMeasMaxCurReset (void)
{
    CPU_TS_TMR  time_max_cnts;
    CPU_SR_ALLOC();


    time_max_cnts             = CPU_IntDisMeasMaxCurGet();
    CPU_INT_DIS();
    CPU_IntDisMeasMaxCur_cnts = 0u;
    CPU_INT_EN();

    return (time_max_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                      CPU_IntDisMeasMaxCurGet()
*
* Description : Get current maximum interrupts disabled time.
*
* Argument(s) : none.
*
* Return(s)   : Current maximum interrupts disabled time (in CPU timestamp timer counts).
*
*               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                      & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
* Caller(s)   : CPU_IntDisMeasMaxCurReset(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function 
*               & MAY be called by application function(s).
*
* Note(s)     : (1) After initialization, 'CPU_IntDisMeasMaxCur_cnts' MUST ALWAYS be accessed
*                   exclusively with interrupts disabled -- but NOT with critical sections.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
CPU_TS_TMR  CPU_IntDisMeasMaxCurGet (void)
{
    CPU_TS_TMR  time_tot_cnts;
    CPU_TS_TMR  time_max_cnts;
    CPU_SR_ALLOC();


    CPU_INT_DIS();
    time_tot_cnts = CPU_IntDisMeasMaxCur_cnts;
    CPU_INT_EN();
    time_max_cnts = CPU_IntDisMeasMaxCalc(time_tot_cnts);

    return (time_max_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                       CPU_IntDisMeasMaxGet()
*
* Description : Get (non-resetable) maximum interrupts disabled time.
*
* Argument(s) : none.
*
* Return(s)   : (Non-resetable) maximum interrupts disabled time (in CPU timestamp timer counts).
*
*               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                      & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
* Caller(s)   : CPU_IntDisMeasInit(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function 
*               & MAY be called by application function(s).
*
* Note(s)     : (1) After initialization, 'CPU_IntDisMeasMax_cnts' MUST ALWAYS be accessed
*                   exclusively with interrupts disabled -- but NOT with critical sections.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
CPU_TS_TMR  CPU_IntDisMeasMaxGet (void)
{
    CPU_TS_TMR  time_tot_cnts;
    CPU_TS_TMR  time_max_cnts;
    CPU_SR_ALLOC();


    CPU_INT_DIS();
    time_tot_cnts = CPU_IntDisMeasMax_cnts;
    CPU_INT_EN();
    time_max_cnts = CPU_IntDisMeasMaxCalc(time_tot_cnts);

    return (time_max_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                        CPU_IntDisMeasStart()
*
* Description : Start interrupts disabled time measurement.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_CRITICAL_ENTER().
*
*               This function is an INTERNAL CPU module function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
void  CPU_IntDisMeasStart (void)
{
    CPU_IntDisMeasCtr++;
    if (CPU_IntDisNestCtr == 0u) {                                  /* If ints NOT yet dis'd, ...                       */
        CPU_IntDisMeasStart_cnts = CPU_TS_TmrRd();                  /* ... get ints dis'd start time.                   */
    }
    CPU_IntDisNestCtr++;
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                        CPU_IntDisMeasStop()
*
* Description : Stop interrupts disabled time measurement.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_CRITICAL_EXIT().
*
*               This function is an INTERNAL CPU module function & MUST NOT be called by application
*               function(s).
*
* Note(s)     : (1) (a) The total amount of time interrupts are disabled by system &/or application code
*                       during critical sections is calculated by the following equations :
*
*                       (1) When interrupts disabled time measurements are disabled :
*
*
*                               |   CRITICAL  |                           |   CRITICAL  |
*                               |<- SECTION ->|                           |<- SECTION ->|
*                               |    ENTER    |                           |    EXIT     |
*
*                            Disable                                    Enable
*                           Interrupts                                Interrupts
*
*                               ||           ||                           ||           ||
*                               ||           ||                           ||           ||
*                               ||       |   ||<------------------------->||       |   ||
*                               ||       |<->||             |             ||<----->|   ||
*                               ||       | | ||             |             ||   |   |   ||
*                                        | |                |                  |   |
*                                   interrupts            time                 interrupts
*                                    disabled                 interrupts       |enabled
*                                          |                   disabled        |
*                                          |              (via application)    |
*                                       time                                 time
*                                           interrupts                           interrupts
*                                         disabled ovrhd                        enabled ovrhd
*
*
*                           (A) time            =  [ time            -  time           ]  -  time
*                                   interrupts     [     interrupts         interrupts ]         total
*                                    disabled      [      enabled            disabled  ]         ovrhd
*                               (via application)
*
*
*                           (B) time       =  time              +  time
*                                   total         interrupts           interrupts
*                                   ovrhd        enabled ovrhd       disabled ovrhd
*
*
*                                   where
*
*                                           time                    time interrupts are disabled between
*                                               interrupts              first critical section enter &
*                                                disabled               last  critical section exit (i.e.
*                                           (via application)           minus total overhead time)
*
*                                           time                    time when interrupts are disabled
*                                               interrupts
*                                                disabled
*
*                                           time                    time when interrupts are  enabled
*                                               interrupts
*                                                enabled
*
*
*                                           time                    total overhead time to disable/enable
*                                               total                   interrupts during critical section
*                                               ovrhd                   enter & exit
*
*                                           time                    total overhead time to disable interrupts
*                                               interrupts              during critical section enter
*                                             disabled ovrhd
*
*                                           time                    total overhead time to enable  interrupts
*                                               interrupts              during critical section exit
*                                              enabled ovrhd
*
*$PAGE*
*
*                       (2) When interrupts disabled time measurements are enabled :
*
*
*        |                                    |                           |                                       |
*        |<----- CRITICAL SECTION ENTER ----->|                           |<------- CRITICAL SECTION EXIT ------->|
*        |                                    |                           |                                       |
*
*                   Time                                                 Time
*     Disable    Measurement                                          Measurement                  Enable
*    Interrupts     Start                                                Stop                    Interrupts
*
*        ||           |                      ||                           ||                         |           ||
*        ||           |                      ||                           ||                         |           ||
*        ||           |        |             ||<------------------------->||               |         |           ||
*        ||       |   |        |<----------->||             |             ||<------------->|         |       |   ||
*        ||       |   |        |      |      ||             |             ||       |       |         |       |   ||
*                 |            |      |                     |                      |       |                 |
*            interrupts       get     |                   time                     |      get            interrupts
*             disabled    start time  |                       interrupts           |   stop time          enabled
*                            meas     |                        disabled            |     meas
*                                   time                  (via application)      time
*                                       start meas                                   stop meas
*                                         ovrhd                                        ovrhd
*
*
*                           (A) time            =  [ time       -  time      ]  -  time
*                                   interrupts     [      stop         start ]         total meas
*                                    disabled      [      meas         meas  ]           ovrhd
*                               (via application)
*
*
*                           (B) time            =  time            +  time
*                                   total meas         start meas         stop meas
*                                     ovrhd              ovrhd              ovrhd
*
*
*                                   where
*
*                                           time                    time interrupts are disabled between first
*                                               interrupts              critical section enter & last critical
*                                                disabled               section exit (i.e. minus measurement
*                                           (via application)           overhead time; however, this does NOT
*                                                                       include any overhead time to disable
*                                                                       or enable interrupts during critical
*                                                                       section enter & exit)
*
*                                           time                    time of disable interrupts start time
*                                               start                   measurement (in timer counts)
*                                               meas
*
*                                           time                    time of disable interrupts stop  time
*                                               stop                    measurement (in timer counts)
*                                               meas
*
*
*                                           time                    total overhead time to start/stop disabled
*                                               total meas              interrupts time measurements (in timer
*                                                 ovrhd                 counts)
*
*                                           time                    total overhead time after getting start
*                                               start meas              time until end of start measurement
*                                                 ovrhd                 function  (in timer counts)
*
*                                           time                    total overhead time from beginning of stop
*                                               stop meas               measurement function until after getting
*                                                 ovrhd                 stop time (in timer counts)
*
*
*$PAGE*
*                   (b) (1) (A) In order to correctly handle unsigned subtraction overflows of start times 
*                               from stop times, CPU timestamp timer count values MUST be returned via 
*                               word-size-configurable 'CPU_TS_TMR' data type.
*
*                               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2a'.
*
*                           (B) Since unsigned subtraction of start times from stop times assumes increasing
*                               values, timestamp timer count values MUST increase with each time count.
*
*                               See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()  Note #2b'.
*
*                       (2) (A) To expedite & reduce interrupts disabled time measurement overhead; only the
*                               subtraction of start times from stop times is performed.
*
*                           (B) The final calculations to subtract the interrupts disabled time measurement
*                               overhead is performed asynchronously in appropriate API functions.
*
*                               See also 'CPU_IntDisMeasMaxCalc()  Note #1b'.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
void  CPU_IntDisMeasStop (void)
{
    CPU_TS_TMR  time_ints_disd_cnts;


    CPU_IntDisNestCtr--;
    if (CPU_IntDisNestCtr == 0u) {                                  /* If ints NO longer dis'd,        ...              */
        CPU_IntDisMeasStop_cnts = CPU_TS_TmrRd();                   /* ... get  ints dis'd stop time & ...              */
                                                                    /* ... calc ints dis'd tot  time (see Note #1b2A).  */
        time_ints_disd_cnts     = CPU_IntDisMeasStop_cnts -
                                  CPU_IntDisMeasStart_cnts;
                                                                    /* Calc max ints dis'd times.                       */
        if (CPU_IntDisMeasMaxCur_cnts < time_ints_disd_cnts) {
            CPU_IntDisMeasMaxCur_cnts = time_ints_disd_cnts;
        }
        if (CPU_IntDisMeasMax_cnts    < time_ints_disd_cnts) {
            CPU_IntDisMeasMax_cnts    = time_ints_disd_cnts;
        }
    }
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           CPU_NameInit()
*
* Description : Initialize CPU Name.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (CPU_CFG_NAME_EN == DEF_ENABLED)
static  void  CPU_NameInit (void)
{
    CPU_NameClr();
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                            CPU_TS_Init()
*
* Description : (1) Initialize CPU timestamp :
*
*                   (a) Initialize/start CPU timestamp timer                            See Note #1
*                   (b) Initialize       CPU timestamp controls
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_Init().
*
* Note(s)     : (1) The following initialization MUST be sequenced as follows :
*
*                   (a) CPU_TS_TmrFreq_Hz     MUST be initialized prior to CPU_TS_TmrInit()
*                   (b) CPU_TS_TmrInit()      SHOULD precede calls to all other CPU timestamp functions;
*                                                 otherwise, invalid time measurements may be calculated/
*                                                 returned.
*
*                   See also 'CPU_Init()  Note #3a'.
*********************************************************************************************************
*/

#if ((CPU_CFG_TS_EN     == DEF_ENABLED) || \
     (CPU_CFG_TS_TMR_EN == DEF_ENABLED))
static  void  CPU_TS_Init (void)
{
#if (((CPU_CFG_TS_32_EN    == DEF_ENABLED     )  && \
      (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_32)) || \
     ((CPU_CFG_TS_64_EN    == DEF_ENABLED     )  && \
      (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_64)))
    CPU_TS_TMR  ts_tmr_cnts;
#endif


                                                                /* ----------------- INIT CPU TS TMR ------------------ */
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
    CPU_TS_TmrFreq_Hz   = 0u;                                   /* Init/clr     ts tmr freq (see Note #1a).             */
    CPU_TS_TmrInit();                                           /* Init & start ts tmr      (see Note #1b).             */
#endif


                                                                /* ------------------- INIT CPU TS -------------------- */
#if (((CPU_CFG_TS_32_EN    == DEF_ENABLED     )  && \
      (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_32)) || \
     ((CPU_CFG_TS_64_EN    == DEF_ENABLED     )  && \
      (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_64)))
    ts_tmr_cnts = CPU_TS_TmrRd();                               /* Get init ts tmr val (in ts tmr cnts).                */
#endif

#if  ((CPU_CFG_TS_32_EN    == DEF_ENABLED)  && \
      (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_32))
    CPU_TS_32_Accum   = 0u;                                     /* Init 32-bit accum'd ts.                              */
    CPU_TS_32_TmrPrev = ts_tmr_cnts;                            /* Init 32-bit ts prev tmr val.                         */
#endif

#if  ((CPU_CFG_TS_64_EN    == DEF_ENABLED)  && \
      (CPU_CFG_TS_TMR_SIZE <  CPU_WORD_SIZE_64))
    CPU_TS_64_Accum   = 0u;                                     /* Init 64-bit accum'd ts.                              */
    CPU_TS_64_TmrPrev = ts_tmr_cnts;                            /* Init 64-bit ts prev tmr val.                         */
#endif
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                        CPU_IntDisMeasInit()
*
* Description : (1) Initialize interrupts disabled time measurements feature :
*
*                   (a) Initialize interrupts disabled time measurement controls
*                   (b) Calculate  interrupts disabled time measurement overhead
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_Init().
*
* Note(s)     : (2) CPU_IntDisMeasInit() SHOULD precede ALL calls to CPU_CRITICAL_ENTER()/CPU_CRITICAL_EXIT()
*                   & other CPU interrupts disabled time measurement functions; otherwise, invalid interrupts
*                   disabled time measurements may be calculated/returned.
*
*                   See also 'CPU_Init()  Note #3b'.
*
*               (3) (a) (1) Interrupts disabled time measurement overhead performed multiple times to calculate
*                           a rounded average with better accuracy, hopefully of +/- one timer count.
*
*                       (2) However, a single overhead time measurement is recommended, even for instruction-
*                           cache-enabled CPUs, since critical sections are NOT typically called within
*                           instruction-cached loops.  Thus a single non-cached/non-averaged time measurement
*                           is a more realistic overhead for the majority of non-cached interrupts disabled
*                           time measurements.
*
*                   (b) Interrupts MUST be disabled while measuring the interrupts disabled time measurement
*                       overhead; otherwise, overhead measurements could be interrupted which would incorrectly
*                       calculate an inflated overhead time which would then incorrectly calculate deflated
*                       interrupts disabled times.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
static  void  CPU_IntDisMeasInit (void)
{
    CPU_TS_TMR  time_meas_tot_cnts;
    CPU_INT16U  i;
    CPU_SR_ALLOC();

                                                                /* ----------- INIT INT DIS TIME MEAS CTRLS ----------- */
    CPU_IntDisMeasCtr         = 0u;
    CPU_IntDisNestCtr         = 0u;
    CPU_IntDisMeasStart_cnts  = 0u;
    CPU_IntDisMeasStop_cnts   = 0u;
    CPU_IntDisMeasMaxCur_cnts = 0u;
    CPU_IntDisMeasMax_cnts    = 0u;
    CPU_IntDisMeasOvrhd_cnts  = 0u;

                                                                /* ----------- CALC INT DIS TIME MEAS OVRHD ----------- */
    time_meas_tot_cnts = 0u;
    CPU_INT_DIS();                                              /* Ints MUST be dis'd for ovrhd calc (see Note #3b).    */
    for (i = 0u; i < CPU_CFG_INT_DIS_MEAS_OVRHD_NBR; i++) {
        CPU_IntDisMeasMaxCur_cnts = 0u;
        CPU_IntDisMeasStart();                                  /* Perform multiple consecutive start/stop time meas's  */
        CPU_IntDisMeasStop();
        time_meas_tot_cnts += CPU_IntDisMeasMaxCur_cnts;        /* ...       & sum time meas max's                  ... */
    }
                                                                /* ... to calc avg time meas ovrhd (see Note #3a).      */
    CPU_IntDisMeasOvrhd_cnts  = (time_meas_tot_cnts + (CPU_CFG_INT_DIS_MEAS_OVRHD_NBR / 2u))
                                                    /  CPU_CFG_INT_DIS_MEAS_OVRHD_NBR;
    CPU_IntDisMeasMaxCur_cnts =  0u;                            /* Reset max ints dis'd times.                          */
    CPU_IntDisMeasMax_cnts    =  0u;
    CPU_INT_EN();
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                       CPU_IntDisMeasMaxCalc()
*
* Description : Calculate maximum interrupts disabled time.
*
* Argument(s) : time_tot_cnts   Total interrupt disabled time, in timer counts.
*
* Return(s)   : Maximum interrupts disabled time (in CPU timestamp timer counts).
*
* Caller(s)   : CPU_IntDisMeasMaxCurGet(),
*               CPU_IntDisMeasMaxGet().
*
* Note(s)     : (1) (a) The total amount of time interrupts are disabled by system &/or application code
*                       during critical sections is calculated by the following equations :
*
*                       (1) time            =   [ time      -  time      ]  -  time
*                               interrupts      [     stop         start ]         total meas
*                                disabled       [     meas         meas  ]           ovrhd
*                           (via application)
*
*
*                       (2) time            =  time            +  time
*                               total meas         start meas         stop meas
*                                 ovrhd              ovrhd              ovrhd
*
*
*                               where
*
*                                       time                    time interrupts are disabled between
*                                           interrupts              first critical section enter &
*                                            disabled               last  critical section exit minus
*                                       (via application)           time measurement overhead
*
*                                       time                    time of disable interrupts start time
*                                           start                   measurement (in timer counts)
*                                           meas
*
*                                       time                    time of disable interrupts stop  time
*                                           stop                    measurement (in timer counts)
*                                           meas
*
*                                       time                    total overhead time to start/stop disabled
*                                           total meas              interrupts time measurements (in timer
*                                             ovrhd                 counts)
*
*                                       time                    total overhead time after getting start
*                                           start meas              time until end of start measurement
*                                             ovrhd                 function  (in timer counts)
*
*                                       time                    total overhead time from beginning of stop
*                                           stop meas               measurement function until after getting
*                                             ovrhd                 stop time (in timer counts)
*
*
*                   (b) To expedite & reduce interrupts disabled time measurement overhead, the final 
*                       calculations to subtract the interrupts disabled time measurement overhead is 
*                       performed asynchronously in API functions.
*
*                       See also 'CPU_IntDisMeasStop()  Note #1b2'.
*$PAGE*
*                   (c) The amount of time interrupts are disabled is calculated by either of the
*                       following equations :
*
*                       (1) Interrupts disabled time  =  Number timer counts  *  Timer period
*
*                               where
*
*                                   Number timer counts             Number of timer counts measured
*                                   Timer period                    Timer's period in some units of
*                                                                       (fractional) seconds
*                                   Interrupts disabled time        Amount of time interrupts are
*                                                                       disabled, in same units of
*                                                                       (fractional) seconds as the
*                                                                       Timer period
*
*                                                         Number timer counts
*                       (2) Interrupts disabled time  =  ---------------------
*                                                           Timer frequency
*
*                               where
*
*                                   Number timer counts             Number of timer counts measured
*                                   Timer frequency                 Timer's frequency in some units
*                                                                       of counts per second
*                                   Interrupts disabled time        Amount of time interrupts are
*                                                                       disabled, in seconds
*
*                       See also 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TS_TmrRd()      Note #2c'
*                              & 'cpu_core.h  FUNCTION PROTOTYPES  CPU_TSxx_to_uSec()  Note #2'.
*
*               (2) Although it is not typical, it is possible for an interrupts disabled time
*                   measurement to be less than the interrupts disabled time measurement overhead;
*                   especially if the overhead was calculated with a single, non-cached measurement
*                   & critical sections are called within instruction-cached loops.
*********************************************************************************************************
*/

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
static  CPU_TS_TMR  CPU_IntDisMeasMaxCalc (CPU_TS_TMR  time_tot_cnts)
{
    CPU_TS_TMR  time_max_cnts;


    time_max_cnts = time_tot_cnts;
    if (time_max_cnts >  CPU_IntDisMeasOvrhd_cnts) {            /* If       max ints dis'd time >  ovrhd time, ...      */
        time_max_cnts -= CPU_IntDisMeasOvrhd_cnts;              /* ... adj  max ints dis'd time by ovrhd time; ...      */
    } else {                                                    /* ... else max ints dis'd time <  ovrhd time, ...      */
        time_max_cnts  = 0u;                                    /* ... clr  max ints dis'd time (see Note #2).          */
    }

    return (time_max_cnts);
}
#endif

