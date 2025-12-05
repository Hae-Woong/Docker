/* ********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*!
 *  \defgroup   Dem_Utility Utility
 *  \{
 *  \file       Dem_Utility_Interface.h
 *  \brief      Subcomponent to provide common utility functions
 *  \details    Provides common utility functions used by other Dem modules
 *  \entity     Utility
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_UTILITY_INTERFACE_H)
#define DEM_UTILITY_INTERFACE_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_Utility_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Utility_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Utility_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Infrastructure_Utility_RoundFloatToUint16
 *****************************************************************************/
/*!
 * \brief         Round a floating point value to the nearest uint16 value.
 *
 * \details       Round a floating point value to the nearest uint16 value.
 *
 * \param[in]     Value
 *                Float32 value
 *
 * \return        value in uint16 format
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Infrastructure_Utility_RoundFloatToUint16(
  float32 Value
);

/* ****************************************************************************
 * Dem_Infrastructure_Utility_ExtractValueUint32
 *****************************************************************************/
/*!
 * \brief         Extract a uint32 from the given array of 4 bytes.
 *
 * \details       Extract a uint32 from the given array of 4 bytes.
 *
 * \param[in]     ValueArray
 *                The array in which value is stored.
 *
 * \return        The value as UInt32 type.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint32, DEM_CODE)
Dem_Infrastructure_Utility_ExtractValueUint32(
  CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ValueArray
);

/* ****************************************************************************
* Dem_Infrastructure_Utility_LatchUint16
*****************************************************************************/
/*!
 * \brief         Latches an uint16 value at the specified latching value.
 *
 * \details       Latches an uint16 value at the specified latching value.
 *
 * \param[in]     Value
 *                The value to be latched.
 *                Should be at most an uint16 value. Uint32 is accepted in
 *                order to latch a value > 0xFFFF at 0xFFFF.
 * \param[in]     LatchingValue
 *                Latching value to which Value is to be restricted.
 *
 * \return        The latched value
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Infrastructure_Utility_LatchUint16(
  uint32 Value,
  uint16 LatchingValue
);

/* ****************************************************************************
* Dem_Infrastructure_Utility_LatchUint8
*****************************************************************************/
/*!
 * \brief         Latches an uint8 value at the specified latching value.
 *
 * \details       Latches an uint8 value at the specified latching value.
 *
 * \param[in]     Value
 *                The value to be latched.
 *                Should be at most an uint8 value. Uint16 is accepted in
 *                order to latch a value > 0xFF at 0xFF.
 * \param[in]     LatchingValue
 *                Latching value to which Value is to be restricted.
 *
 * \return        The latched value
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Infrastructure_Utility_LatchUint8(
  uint16 Value,
  uint8 LatchingValue
);

/* ****************************************************************************
* Dem_Infrastructure_Utility_SetBitAtIndex
*****************************************************************************/
/*!
 * \brief         Sets or resets a bit in a given value.
 *
 * \details       Sets or resets a bit at a bit index in a given value.
 *
 * \param[in]     Value
 *                The value to set or reset a bit in.
 * \param[in]     BitIndex
 *                Index of the bit.
 * \param[in]     BitValue
 *                The value to set the bit at given bit index to.
 *
 * \return        Value with bit at given bit index set or reset.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Infrastructure_Utility_SetBitAtIndex(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint32 Value,
  uint8 BitIndex,
  boolean BitValue
);

/* ****************************************************************************
* Dem_Infrastructure_Utility_TestBitAtIndex
*****************************************************************************/
/*!
 * \brief         Tests whether a bit is set at a given bit index in a value.
 *
 * \details       Tests whether a bit is set at a given bit index in a value.
 *
 * \param[in]     Value
 *                The value to check the bit in.
 * \param[in]     BitIndex
 *                Index of the bit.
 *
 * \return        TRUE
 *                Bit at BitIndex is set.
 * \return        FALSE
 *                Bit at BitIndex is not set.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Infrastructure_Utility_TestBitAtIndex(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint32 Value,
  uint8 BitIndex
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_UTILITY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Utility_Interface.h
 *********************************************************************************************************************/
