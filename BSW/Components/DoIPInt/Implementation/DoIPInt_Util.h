/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_Util.h
 *        \brief  Declarations for sub-module DoIP_Util of component DoIPInt.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (DOIPINT_UTIL_H)
# define DOIPINT_UTIL_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define DOIPINT_MIN(a, b)                     (((a) < (b)) ? (a) : (b))

/* PRQA S 3453 1 */ /* MD_MSR_FctLikeMacro */
#define DOIPINT_MAX(a, b)                     (((a) > (b)) ? (a) : (b))

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_Util_GetSupportedPduLength()
 *********************************************************************************************************************/
/*! \brief        Returns a requested value if it fits into the PDU length type or returns the maximum PDU length.
 *  \details      -
 *  \param[in]    Length                  Length to be checked.
 *  \return       SupportedPduLength      Returns value of parameter Length or maximum PDU length.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(PduLengthType, DOIPINT_CODE) DoIPInt_Util_GetSupportedPduLength(
  uint32 Length);

/**********************************************************************************************************************
 *  DoIPInt_Util_IsLengthInRange()
 *********************************************************************************************************************/
/*! \brief        Checks if length is in valid range considering the current offset and the maximum range.
 *  \details      -
 *  \param[in]    Length            Length to be checked.
 *  \param[in]    Range             Desired range.
 *  \param[in]    MaxRange          Maximum range.
 *  \param[in]    Offset            Current offset.
 *  \return       E_OK              Length is valid.
 *  \return       E_NOT_OK          Length is invalid.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Util_IsLengthInRange(
  PduLengthType Length,
  uint32 Range,
  uint32 MaxRange,
  uint8 Offset);

/**********************************************************************************************************************
 *  DoIPInt_Util_CopyUpToLevel()
 *********************************************************************************************************************/
/*! \brief          Copies data from one buffer to another up to a specific level considering an offset.
 *  \details        -
 *  \param[in]      Level             Level up to which data shall be copied.
 *  \param[in,out]  OffsetPtr         Pointer to the current offset value.
 *                                    [range: IN:   OffsetPtr != NULL_PTR,
 *                                            OUT:  *OffsetPtr <= Level]
 *  \param[out]     DestBufPtr        Pointer to the buffer that shall be written.
 *                                    [range: DestBufPtr != NULL_PTR]
 *  \param[in]      SrcBufPtr         Provides the source buffer and the number of bytes to be copied.
 *                                    [range: SrcBufPtr != NULL_PTR,
 *                                            SrcBufPtr->SduLength == lengthOf(SrcBufPtr->SduDataPtr) ||
 *                                              SrcBufPtr->SduLength == 0 => SrcBufPtr->SduDataPtr == NULL_PTR,
 *                                              (*OffsetPtr + SrcBufPtr->SduLength) < Level]
 *  \param[out]     AvailableDataPtr  Indicates the remaining number of bytes that are available to be copied.
 *                                    [range: AvailableDataPtr != NULL_PTR]
 *  \return         E_OK              Data copied successfully.
 *  \return         E_NOT_OK          Data could not be copied completely.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Util_CopyUpToLevel(
  uint32 Level,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) OffsetPtr,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) SrcBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_UTIL_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Util.h
 *********************************************************************************************************************/
