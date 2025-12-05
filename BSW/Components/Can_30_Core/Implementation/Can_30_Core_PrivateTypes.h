/***********************************************************************************************************************
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
/*!       \file  Can_30_Core_PrivateTypes.h
 *        \brief  Can_30_Core private types header file
 *        \details  Contains private type definitions, constants and macros used throughout the component, that cannot be
 *                  associated with a single unit.
 *        \unit *
 **********************************************************************************************************************/
#ifndef CAN_30_CORE_PRIVATE_TYPES_H
#define CAN_30_CORE_PRIVATE_TYPES_H

#include "Std_Types.h"

#if !defined (CAN_30_CORE_LOCAL)
# define CAN_30_CORE_LOCAL static
#endif

#if !defined (CAN_30_CORE_LOCAL_INLINE)
# define CAN_30_CORE_LOCAL_INLINE LOCAL_INLINE
#endif

#if !defined (CAN_30_CORE_LOCAL_API) /* COV_CAN_30_CORE_DEFINE_PROTECTION */
# define CAN_30_CORE_LOCAL_API
#endif

/*! RAM check defines */
#define CAN_30_CORE_PATTERN_RAMCHECK_MAX  3u
#define CAN_30_CORE_PATTERN_RAMCHECK_NONE 3u

/*! Initialization status of component */
typedef enum Can_30_Core_InitStateEnum {
    CAN_30_CORE_UNINITIALIZED = 0,
    CAN_30_CORE_INITIALIZED
}Can_30_Core_InitState;

#endif /* CAN_30_CORE_PRIVATE_TYPES_H */
