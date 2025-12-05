/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 * \addtogroup  Os_Hal_Derivative
 * \{
 *
 * \file
 * \brief  This file contains derivative specific information.
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 *********************************************************************************************************************/

#ifndef OS_HAL_DERIVATIVE_TC49XNINT_H
# define OS_HAL_DERIVATIVE_TC49XNINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/** TC49xN **/

/* STM module */
# define OS_HAL_TIMER_STM                            STD_OFF
# define OS_HAL_TIMER_CORESTM                        STD_ON

# define OS_HAL_TIMER_STM0_BASE                      (0xF8800000uL)
# define OS_HAL_INT_SRC_STM0_CH0_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0x28uL)
# define OS_HAL_INT_SRC_STM0_CH1_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0x2CuL)
# define OS_HAL_TIMER_STM1_BASE                      (0xF8840000uL)
# define OS_HAL_INT_SRC_STM1_CH0_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0x68uL)
# define OS_HAL_INT_SRC_STM1_CH1_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0x6CuL)
# define OS_HAL_TIMER_STM2_BASE                      (0xF8880000uL)
# define OS_HAL_INT_SRC_STM2_CH0_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0xA8uL)
# define OS_HAL_INT_SRC_STM2_CH1_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0xACuL)
# define OS_HAL_TIMER_STM3_BASE                      (0xF88C0000uL)
# define OS_HAL_INT_SRC_STM3_CH0_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0xE8uL)
# define OS_HAL_INT_SRC_STM3_CH1_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0xECuL)
# define OS_HAL_TIMER_STM4_BASE                      (0xF8900000uL)
# define OS_HAL_INT_SRC_STM4_CH0_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0x128uL)
# define OS_HAL_INT_SRC_STM4_CH1_OFFSET              OS_HAL_COMPUTE_TIMER_INT_SOURCE(0x12CuL)

/* GPT module */
# define OS_HAL_TIMER_GPT                            STD_OFF


/* TriCore core module */
# define OS_HAL_CORE_18                              STD_ON

/* Interrupt router (IR) module */
# define OS_HAL_INTERRUPT_AURIXTC4XX_IR              STD_ON
# define OS_HAL_INTERRUPT_IR_BASE                    (0xF0030000uL)

/*! Defines the cyber security bitmask for the service request control register. */
# define OS_HAL_SRC_CS_MASK                          (0x0uL)



#endif /* OS_HAL_DERIVATIVE_TC49XNINT_H */

/*!
 * \}
 */
/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Derivative_TC49xNInt.h
 *********************************************************************************************************************/
