/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Com_CheckUpdateBit.c
 *        \unit   CheckUpdateBit
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_CHECKUPDATEBIT_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_CheckUpdateBit.h"
#include "Com_Cfg_DescriptionGatewayData.h"
#include "Com_Cfg_CheckUpdateBit.h"
#include "Com_Cfg_ReceptionProcessingData.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_CheckUpdateBit_CheckUpdateIdxAndMask
**********************************************************************************************************************/
/*! \brief              This function checks if the UpdateBit is received and set.
 *   \details           -
 *   \pre               -
 *   \param[in]         PduInfoPtr         contains the SDU buffer and the SDU length.           -
 *   \param[in]         idxUpdateBitInfo   Index of UpdateBitInfo
 *   \return            boolean
 *                        TRUE  UpdateBit isn't received or is received and set
 *                        FALSE UpdateBit is received and isn't set
 *   \trace             SPEC-2736853
 *   \reentrant         TRUE, for different handles
 *   \synchronous       TRUE
 *   \context           TASK|ISR2
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_CheckUpdateBit_CheckUpdateIdxAndMask(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_UpdateBitInfoIterType idxUpdateBitInfo);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_CheckUpdateBit_CheckUpdateIdxAndMask
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_CheckUpdateBit_CheckUpdateIdxAndMask(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_UpdateBitInfoIterType idxUpdateBitInfo)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the configured UpdateBit was not received mark the ComSignalGroup or ComSignal as updated */
  /* <= because a length is compared with an index */
  /* \trace SPEC-2736853 */
  if(PduInfoPtr->SduLength <= Com_GetUbIdxOfUpdateBitInfo(idxUpdateBitInfo))
  {
    /* Update-bit not received */
    retVal = TRUE;
  }
  /* Else the configured UpdateBit was received and if the UpdateBit is not set, mark the ComSignalGroup or ComSignal as not updated */
  else if(0u == (PduInfoPtr->SduDataPtr[Com_GetUbIdxOfUpdateBitInfo(idxUpdateBitInfo)] & Com_GetUpdateBitMaskOfUpdateBitInfo(idxUpdateBitInfo)))
  {
    /* Update-bit not set */
    retVal = FALSE;
  }
  /*  Else the configured UpdateBit was received and is set, then mark the ComSignalGroup or ComSignal as updated */
  else
  {
    /* Update-bit set */
    retVal = TRUE;
  }

  return retVal;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_CheckUpdateBit_Signal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_CheckUpdateBit_Signal(Com_RxSigInfoIterType idxRxSigInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduInfoPtr->SduDataPtr != NULL_PTR)
  {
    if(Com_IsUpdateBitInfoUsedOfRxSigInfo(idxRxSigInfo))
    {
      const Com_UpdateBitInfoIterType idxUpdateBitInfo = Com_GetUpdateBitInfoIdxOfRxSigInfo(idxRxSigInfo);
      retVal = Com_CheckUpdateBit_CheckUpdateIdxAndMask(PduInfoPtr, idxUpdateBitInfo);
    }
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_CheckUpdateBit_SignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_CheckUpdateBit_SignalGroup(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduInfoPtr->SduDataPtr != NULL_PTR)
  {
    if(Com_IsUpdateBitInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
    {
      const Com_UpdateBitInfoIterType idxUpdateBitInfo = Com_GetUpdateBitInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
      retVal = Com_CheckUpdateBit_CheckUpdateIdxAndMask(PduInfoPtr, idxUpdateBitInfo);
    }
  }
  return retVal;
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_CheckUpdateBit_GwDescription
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_CheckUpdateBit_GwDescription(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_IsUpdateBitInfoUsedOfGwDescriptionAccessInfo(gwDescrAccessIdx))
  {
    const Com_UpdateBitInfoIterType idxUpdateBitInfo = Com_GetUpdateBitInfoIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx); /* PRQA S 2983 */ /* MD_Com_2982_2983 */
    retVal = Com_CheckUpdateBit_CheckUpdateIdxAndMask(PduInfoPtr, idxUpdateBitInfo);
  }
  return retVal;
}
#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_CheckUpdateBit.c
 *********************************************************************************************************************/
