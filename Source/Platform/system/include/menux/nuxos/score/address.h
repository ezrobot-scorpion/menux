/*!
********************************************************************************
* @file     : address.h
*
* @brief    : This include file contains the bodies of the routines about
* 			: addresses which are inline.
*
* @version  : V0.0.1
*
* @copyright: All Rights Reserved.
********************************************************************************
* @changelog: L.H.T 2017-10-01 first version
*******************************************************************************/
#ifndef INCLUDE_MENUX_NUXOS_SCORE_ADDRESS_H
#define INCLUDE_MENUX_NUXOS_SCORE_ADDRESS_H

#include <config.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

/*!
********************************************************************************
* @brief : Add offset to address.
*
* @param : base is the base address.
* @param : offset is the offset to add to base.
*
* @retval: This method returns the resulting address.
*******************************************************************************/
static inline void *address_add_offset(const void *base, uintptr_t offset)
{
	return (void *) ((uintptr_t) base + offset);
}

/*!
********************************************************************************
* @brief : Subtract offset from base.
*
* @param : base is the base address.
* @param : offset is the offset to subtract from base.
*
* @retval: This method returns the resulting address.
*******************************************************************************/
static inline void *address_sub_offset(const void *base, uintptr_t offset)
{
	return (void *) ((uintptr_t) base - offset);
}

/*!
********************************************************************************
* @brief : Subtract two address.
*
* @param : left is the address on the left hand side of the subtration.
* @param : right is the address on the right hand side of the subtration.
*
* @retval: This method returns the resulting address.
*******************************************************************************/
static inline void *address_subtract(const void *left, const void *right)
{
	return (int32_t) ((const char *) left - (const char *) right);
}

/*!
********************************************************************************
* @brief : Is address aligned.
*
* @param : address is the address being checked for alignment.
*
* @retval: This method returns true if the address is aligned, false otherwise.
*******************************************************************************/
static inline void *address_is_aligned(const void *address)
{
#if (CONFIG_CPU_ALIGNMENT == 0)
	return true;
#else
	return (((uintptr_t)address % CONFIG_CPU_ALIGNMNET) == 0);
#endif
}

/*!
********************************************************************************
* @brief : Is address in range.
*
* @param : address is the address to check.
* @param : base is the lowest address of the range to check against.
* @param : limit is the highest address of the range to check against.
*
* @retval: This method returns true if the given address is within the memory
* 		   range specified and false otherwise.
*******************************************************************************/
static inline bool address_is_in_range(const void *address, const void *base,
		const void *limit)
{
	return ((address >= base) && (address <= limit));
}

#ifdef __cplusplus
}
#endif
#endif /* INCLUDE_MENUX_NUXOS_SCORE_ADDRESS_H */
