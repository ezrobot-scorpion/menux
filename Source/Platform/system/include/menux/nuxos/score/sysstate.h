/*!
********************************************************************************
* @file     : sysstate.h
*
* @brief    : This include file contains the system state enumration defines.
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_SCORE_SYSSTATE_H
#define INCLUDE_MENUX_NUXOS_SCORE_SYSSTATE_H

#ifdef __cplusplus
extern "C"{
#endif

enum tx_sysstate_e
{
	SYSTEM_STATE_BEFORE_INITIALIZATION,
	SYSTEM_STATE_BEFORE_MULTITASKING,
	SYSTEM_STATE_BEGIN_MULTITASKING,
	SYSTEM_STATE_UP,
	SYSTEM_STATE_SHUTDOWN,
	SYSTEM_STATE_FAILED
};

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_SCORE_SYSSTATE_H */
