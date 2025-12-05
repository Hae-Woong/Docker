/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_Util.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_Util.
 *      \details  Implementation for sub-module DoIPInt_Util of component DoIPInt.
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

#define DOIPINT_UTIL_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Util.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if !defined (DOIPINT_LOCAL)
# define DOIPINT_LOCAL static
#endif /* !defined (DOIPINT_LOCAL) */

#if !defined (DOIPINT_LOCAL_INLINE)
# define DOIPINT_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (DOIPINT_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DOIPINT_START_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_Util_GetSupportedPduLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(PduLengthType, DOIPINT_CODE) DoIPInt_Util_GetSupportedPduLength(
  uint32 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType supportedPduLength;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return maximum supported PDU length if requested length exceeds maximum supported PDU length. */
  if ( Length > DoIPInt_GetMaxPduLength() )
  {
    supportedPduLength = (PduLengthType)DoIPInt_GetMaxPduLength();
  }
  else
  /* #11 Return requested PDU length otherwise. */
  {
    supportedPduLength = (PduLengthType)Length;
  }

  return supportedPduLength;
} /* DoIPInt_Util_GetSupportedPduLength() */

/**********************************************************************************************************************
 *  DoIPInt_Util_IsLengthInRange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Util_IsLengthInRange(
  PduLengthType Length,
  uint32 Range,
  uint32 MaxRange,
  uint8 Offset)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the range exceeds the maximum range. */
  if ( Range > MaxRange )
  {
    retVal = E_NOT_OK;
  }
  /* #20 Check if the sum of the given offset and length exceeds the desired range otherwise. */
  else if ( ((uint32)Offset + (uint32)Length) > Range )
  {
    retVal = E_NOT_OK;
  }
  /* #30 Return that length is in range otherwise. */
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* DoIPInt_Util_IsLengthInRange() */

/**********************************************************************************************************************
 *  DoIPInt_Util_CopyUpToLevel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Util_CopyUpToLevel(
  uint32 Level,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) OffsetPtr,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) SrcBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy the requested data to the given buffer if data is available. */
  if ( SrcBufPtr->SduDataPtr != NULL_PTR )
  {
    /* PRQA S 314 4 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(                                                                                                    /* VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED */
      (void*)DestBufPtr,
      (void*)SrcBufPtr->SduDataPtr,
      SrcBufPtr->SduLength);

    /* #100 Increment the offset by the copied data size. */
    *OffsetPtr += (uint8)SrcBufPtr->SduLength;
  }

  /* #20 Set available buffer size to zero if parsing up to level has been finished. */
  if ( *OffsetPtr >= Level )
  {
    *AvailableDataPtr = 0u;
    retVal = E_OK;
  }
  /* #21 Set the available buffer size to the remaining length up to level otherwise. */
  else
  {
    *AvailableDataPtr = (uint8)(Level - *OffsetPtr);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* DoIPInt_Util_CopyUpToLevel() */

#define DOIPINT_STOP_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Util.c
 *********************************************************************************************************************/
