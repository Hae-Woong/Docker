/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/*!        \file     FiM_LHelper.h
 *         \entity   LHelper
 *         \brief    Internal Header for Autosar 4 module FiM - Logical Unit Helper
 *
 *         \details  Static source file for FiM. May not be changed by customer / generator
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  
 *  see FiM.h
 *  
 **********************************************************************************************************************/

#if !defined(FIM_LHELPER_H)
#define FIM_LHELPER_H

/* *********************************************************************************************************************
 *  GLOBAL MISRA Justifications
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "FiM_Types.h"
#include "FiM_Lcfg.h"
#include "FiM_Int.h"


/***********************************************************************************************************************
 *  GLOBAL (LOGICAL UNIT'S PUBLIC) CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL (LOGICAL UNIT'S PUBLIC) FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL (LOGICAL UNIT'S PUBLIC) DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL (LOGICAL UNIT'S PUBLIC) DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL (LOGICAL UNIT'S PUBLIC) FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define FIM_START_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* \unit UBitfieldHdl */

/***********************************************************************************************************************
 * public FiM_UBitfieldHdl_TestBitInBitfield
 **********************************************************************************************************************/
/*! \brief                      Test if the bit at a given position is set in a bitfield.
 *
 *  \details                    see brief
 *  \param[in]  bitfieldWordPtr Pointer to bitfield array (actually word array containing bitfield).
 *                              Internally only used for read access.
 *  \param[in]  bitPosition     Specified bit position.
 *  \return                     TRUE if bit is set.
 *                              FALSE if bit is not set.
 *  \pre                        -
 *  \context                    TASK|ISR2
 *  \synchronous                TRUE
 *  \reentrant                  TRUE
 **********************************************************************************************************************/
FUNC(boolean, FIM_CODE) FiM_UBitfieldHdl_TestBitInBitfield
                                    (volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                     CONST(uint32, AUTOMATIC) bitPosition);


/***********************************************************************************************************************
 * public FiM_UBitfieldHdl_SetBitInBitfield
 **********************************************************************************************************************/
/*! \brief                          Set the bit at a given position in a bitfield.
 *
 *  \details                        see brief
 *  \param[in]  bitfieldPtr         Pointer to bitfield array (actually word array containing bitfield).
 *  \param[in]  bitfieldWordArrSize Total size of word array containing bitfield.
 *                                  Must not exceed length of word array allocated in valid memory.
 *  \param[in]  bitPosition         Specified bit position.
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \synchronous                    TRUE
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE) FiM_UBitfieldHdl_SetBitInBitfield
                                      (volatile CONSTP2VAR(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                       CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                       CONST(uint32, AUTOMATIC) bitPosition);


/***********************************************************************************************************************
 * public FiM_UBitfieldHdl_ResetBitInBitfield
 **********************************************************************************************************************/
/*! \brief                          Reset the bit at a given position in a bitfield.
 *
 *  \details                        see brief
 *  \param[in]  bitfieldWordPtr     Pointer to bitfield array (actually word array containing bitfield).
 *  \param[in]  bitfieldWordArrSize Total size of word array contianing bitfield.
 *                                  Must not exceed length of word array allocated in valid memory.
 *  \param[in]  bitPosition         Specified bit position.
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \synchronous                    TRUE
 *  \reentrant                      TRUE
 **********************************************************************************************************************/
FUNC(void, FIM_CODE) FiM_UBitfieldHdl_ResetBitInBitfield
                                      (volatile CONSTP2VAR(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                       CONST(uint32, AUTOMATIC) bitfieldWordArrSize,
                                       CONST(uint32, AUTOMATIC) bitPosition);


/***********************************************************************************************************************
 *  public FiM_UBitfieldHdl_GetNextSetBit()
 **********************************************************************************************************************/
/*! \brief                          Scans a bitfield range between for the next bit that is set.
 *
 *  \details                        This function scans from a given start index in the provided bitfield up to a given
 *                                  stop position for the next bit that is set. It also considers wrap around and
 *                                  actual length of bitfield (not necessarily being identical with end of word array).
 *
 *  \param[in]      bitfieldWordPtr Pointer to bitfield array (actually word array containing bitfield).
 *                                  Internally only used for read access.
 *  \param[in]      bitfieldLength  Bitfield length (in bits).
 *                                  Can be smaller than bitfield array length (in words) * FIM_BITS_PER_PROCESSORWORD.
 *  \param[in,out]  bitIndexPtr     Must point to a valid memory location.
 *                                  In:   Start of bitfield range (including) to scan.
 *                                  Out:  Index of bit that was found.
 *                                        If no bit was found, the index where the scan stopped plus 1 (with wrap
 *                                        around if necessary)
 *  \param[in]      stopIndex       Stop of bitfield range (excluding) to scan. Can be smaller than start index due
 *                                  to wrap around at end of bitfield.
 *  \return                         TRUE if bit was found
 *                                  FALSE if bit was not found
 *  \pre                            -
 *  \context                        TASK|ISR2
 *  \synchronous                    TRUE
 *  \reentrant                      TRUE
 *  \spec                           requires (bitIndexPtr != 0) \endspec
 **********************************************************************************************************************/
FUNC(boolean, FIM_CODE)
  FiM_UBitfieldHdl_GetNextSetBit(volatile CONSTP2CONST(FiM_ProcessorWordType, AUTOMATIC, AUTOMATIC) bitfieldWordPtr,
                                 CONST(uint32, AUTOMATIC) bitfieldLength, /* in bits */
                                 P2VAR(uint32, AUTOMATIC, AUTOMATIC) bitIndexPtr,
                                 CONST(uint32, AUTOMATIC) stopBit);

#define FIM_STOP_SEC_CODE
#include "FiM_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



#endif /* FIM_LHELPER_H */

/***********************************************************************************************************************
*  END OF FILE: FiM_LHelper.h
***********************************************************************************************************************/
