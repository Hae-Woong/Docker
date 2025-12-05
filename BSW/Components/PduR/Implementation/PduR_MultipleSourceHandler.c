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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_MultipleSourceHandler.c
 *         \unit  MultipleSourceHandler
 *        \brief  Pdu Router Multiple Source Handler source file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 3673 EOF */ /* MD_MSR_Rule8.13 */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_MULTIPLESOURCEHANDLER_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_MultipleSourceHandler.h"
#include "PduR_Mm.h"
#include "PduR_RmIf.h"
#include "PduR_RmTp_TxRouting.h"
#include "PduR_RmTp_TxInst.h"
#include "PduR_Lock.h"
#include "PduR_Reporting.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

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
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_GetSrcDestPdu
**********************************************************************************************************************/
/*! \brief       This function returns the rmDestRomIdx which last transmitted to the global destination (rmGDestRomIdx).
 *  \details     This value is always the same for 1:1 routings, but may be different and not present for N:1 routings.
 *  \param[in]   rmGDestRomIdx     ID of the globalPdu
 *  \param[out]  rmDestRomIdx      ID of the source DestPdu
 *  \return      Std_ReturnType    E_OK     The rmDestRomIdx was read successfully.
 *                                 E_NOT_OK The rmDestRomIdx was not read successfully.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 ***********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_GetSrcDestPdu(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_Init(PduR_MemIdxType memIdx)
{
  PduR_RmGDestNto1InfoRamIterType rmGDestNto1InfoRamIdx;

  for(rmGDestNto1InfoRamIdx = 0u; rmGDestNto1InfoRamIdx < PduR_GetSizeOfRmGDestNto1InfoRam(memIdx); rmGDestNto1InfoRamIdx++)  /* FETA_PDUR_01 */
  {
    PduR_SetTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoRamIdx, FALSE, memIdx);
    PduR_SetRmDestRomIdxOfRmGDestNto1InfoRam(rmGDestNto1InfoRamIdx, PDUR_NO_RMDESTROMIDXOFRMGDESTNTO1INFORAM, memIdx);
  }
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_GetSrcDestPdu
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_GetSrcDestPdu(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx)
{
  Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))
  {
    PduR_RmDestRomIterType readRmDestRomIdx = PduR_GetRmDestRomIdxOfRmGDestNto1InfoRam(PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx));

    if(readRmDestRomIdx == PDUR_NO_RMDESTROMIDXOFRMGDESTNTO1INFORAM)
    {
      retVal = E_NOT_OK;
    }
    else if(readRmDestRomIdx >= PduR_GetSizeOfRmDestRom())
    {
      /* If rmDestRomIdx is out-of-bounds: use one of the valid source rmDests as fallback. */
      *rmDestRomIdx = PduR_GetRmDestRomIdxOfRmGDestRom(rmGDestRomIdx);
    }
    else if(PduR_GetRmGDestRomIdxOfRmDestRom(readRmDestRomIdx) != rmGDestRomIdx)
    {
      /* The read source rmDestRomIdx is valid but does not belong to this rmGDestRom. It is erroneous. Use one of the valid source rmDests as fallback. */
      *rmDestRomIdx = PduR_GetRmDestRomIdxOfRmGDestRom(rmGDestRomIdx);
    }
    else
    {
      *rmDestRomIdx = readRmDestRomIdx;
    }
  }
  else
  {
    *rmDestRomIdx = PduR_GetRmDestRomIdxOfRmGDestRom(rmGDestRomIdx);
  }
  /*@ assert retVal != E_OK || *rmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
  return retVal;
}


/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_Reset(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))
  {
    PduR_SetTransmissionActiveOfRmGDestNto1InfoRam(PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx), FALSE, PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx));
    PduR_SetRmDestRomIdxOfRmGDestNto1InfoRam(PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx), PDUR_NO_RMDESTROMIDXOFRMGDESTNTO1INFORAM, PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx));
  }
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_IfTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_IfTransmit(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
  {
    PduR_RmGDestRomIterType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
    {
      if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))
      {
        PduR_RmGDestNto1InfoRomIdxOfRmGDestRomType rmGDestNto1InfoIdx = PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx);
        PduR_RmGDestNto1InfoRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

        /* Forward transmit if either no TxConfirmation is ever expected or no transmission is ongoing right now. */
        if(!PduR_IsExpectTxConfirmationOfRmGDestNto1InfoRom(rmGDestNto1InfoIdx, memIdx) || !PduR_IsTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, memIdx))
        {
          PduR_SetRmDestRomIdxOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, (PduR_RmDestRomIdxOfRmGDestNto1InfoRamType) rmDestRomIdx, memIdx);
          PduR_SetTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, PduR_IsExpectTxConfirmationOfRmGDestNto1InfoRom(rmGDestNto1InfoIdx, memIdx), memIdx);

          PduR_Reporting_CallPreTransmitCallback(rmDestRomIdx,  info);

          retVal = PduR_Mm_CallIfTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info);
          if(retVal != E_OK)
          {
            PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);
          }
        }
      }
      else
      {
        PduR_Reporting_CallPreTransmitCallback(rmDestRomIdx, info);
        retVal = PduR_Mm_CallIfTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info);
      }
    }
  }
  PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);

  return retVal;
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TriggerTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TriggerTransmit(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  {
    PduR_RmDestRomIterType rmDestRomIdx = 0u;

    if(PduR_MultipleSourceHandler_GetSrcDestPdu(rmGDestRomIdx, &rmDestRomIdx) == E_OK)
    {
      /* @ assert rmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
      retVal = PduR_RmIf_DispatchTriggerTransmit(rmDestRomIdx, info);
    }
    else
    {
      PduR_Reporting_ReportError(PDUR_RMIF_MULTIPLESOURCEHANDLER_TRIGGERTRANSMIT, PDUR_E_PDU_ID_INVALID);
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_IfCancelTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_IfCancelTransmit(PduR_RmDestRomIterType rmDestRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
  {
    PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);

    if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))
    {
      PduR_RmDestRomIterType lastTxdRmDestRomIdx = 0u;
      Std_ReturnType foundSrcPdu = PduR_MultipleSourceHandler_GetSrcDestPdu(rmGDestRomIdx, &lastTxdRmDestRomIdx);

      /* @ assert lastTxdRmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
      if(PduR_IsTransmissionActiveOfRmGDestNto1InfoRam(PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx)) && (foundSrcPdu == E_OK) && (rmDestRomIdx == lastTxdRmDestRomIdx))
      {
        retVal = PduR_Mm_CallIfCancelTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx));
      }
      else
      {
        PduR_Reporting_ReportError(PDUR_RMIF_MULTIPLESOURCEHANDLER_CANCELTRANSMIT, PDUR_E_UNEXPECTED_CALL);
      }
    }
    else
    {
      retVal = PduR_Mm_CallIfCancelTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx));
    }
  }
  PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);
  return retVal;
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_IfTxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_IfTxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_RmDestRomIterType rmDestRomIdx = 0u;

  if(PduR_MultipleSourceHandler_GetSrcDestPdu(rmGDestRomIdx, &rmDestRomIdx) == E_OK)
  {
    /* @ assert rmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
    PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);
    PduR_RmIf_DispatchTxConfirmation(rmDestRomIdx);
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_RMIF_MULTIPLESOURCEHANDLER_TXCONFIRMATION, PDUR_E_PDU_ID_INVALID);
  }
}

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TpTriggerTransmission
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TpTriggerTransmission(PduR_RmDestRomIterType rmDestRomIdx, PduR_DestApplicationManagerRomIterType applicationContext)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);

  if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))
  {
    PduR_RmGDestNto1InfoRomIdxOfRmGDestRomType rmGDestNto1InfoIdx = PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx);
    PduR_RmGDestNto1InfoRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

    if(!PduR_IsTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, memIdx))
    {
      PduR_SetRmDestRomIdxOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, (PduR_RmDestRomIdxOfRmGDestNto1InfoRamType) rmDestRomIdx, memIdx);
      PduR_SetTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, TRUE, memIdx);

      retVal = PduR_RmTp_TxInst_TriggerTransmit(rmGDestRomIdx, applicationContext);
      if(E_OK != retVal)
      {
        PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);
      }
    }
  }
  else
  {
    retVal = PduR_RmTp_TxInst_TriggerTransmit(rmGDestRomIdx, applicationContext);
  }

  return retVal;
}
#endif

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TpTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TpTransmit(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
  {
    PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);

    if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING */
    {
      PduR_RmGDestNto1InfoRomIdxOfRmGDestRomType rmGDestNto1InfoIdx = PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx);
      PduR_RmGDestNto1InfoRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

      if(!PduR_IsTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, memIdx))  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING */
      {
        PduR_SetRmDestRomIdxOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, (PduR_RmDestRomIdxOfRmGDestNto1InfoRamType) rmDestRomIdx, memIdx);
        PduR_SetTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, TRUE, memIdx);

        retVal = PduR_Mm_CallTpTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info);
        if(retVal != E_OK)  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING */
        {
          PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);
        }
      }
    }
    else
    {
      retVal = PduR_Mm_CallTpTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), info);
    }
  }
  PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);

  return retVal;
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TpCancelTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_TpCancelTransmit(PduR_RmDestRomIterType rmDestRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
  {
    PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);

    if(PduR_IsRmGDestNto1InfoRomUsedOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING */
    {
      PduR_RmGDestNto1InfoRomIdxOfRmGDestRomType rmGDestNto1InfoIdx = PduR_GetRmGDestNto1InfoRomIdxOfRmGDestRom(rmGDestRomIdx);
      PduR_RmDestRomIterType lastTxdRmDestRomIdx = 0u;
      Std_ReturnType foundSrcPdu = PduR_MultipleSourceHandler_GetSrcDestPdu(rmGDestRomIdx, &lastTxdRmDestRomIdx);
      PduR_RmGDestNto1InfoRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmGDestNto1InfoRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

      /* @ assert lastTxdRmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
      if(PduR_IsTransmissionActiveOfRmGDestNto1InfoRam(rmGDestNto1InfoIdx, memIdx) && (foundSrcPdu == E_OK) && (rmDestRomIdx == lastTxdRmDestRomIdx))  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING_STATIC */
      {
        retVal = PduR_Mm_CallTpCancelTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx));
      }
      else
      {
        PduR_Reporting_ReportError(PDUR_RMTP_MULTIPLESOURCEHANDLER_CANCELTRANSMIT, PDUR_E_UNEXPECTED_CALL);
      }
    }
    else
    {
      retVal = PduR_Mm_CallTpCancelTransmit(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx));
    }
  }
  PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);
  return retVal;
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_TpTxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_MultipleSourceHandler_TpTxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx, Std_ReturnType result)
{
  PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
  {
    PduR_RmDestRomIterType rmDestRomIdx = 0u;

    if(PduR_MultipleSourceHandler_GetSrcDestPdu(rmGDestRomIdx, &rmDestRomIdx) == E_OK)  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING */
    {
      /* @ assert rmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
      PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);
      PduR_RmTp_TxRouting_DispatchTpTxConfirmation(rmDestRomIdx, result);
    }
    else
    {
      PduR_Reporting_ReportError(PDUR_RMTP_MULTIPLESOURCEHANDLER_TPTXCONFIRMATION, PDUR_E_UNEXPECTED_CALL);
    }
  }
  PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
}

/**********************************************************************************************************************
 * PduR_MultipleSourceHandler_CopyTxData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_MultipleSourceHandler_CopyTxData(PduR_RmGDestRomIterType rmGDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                              P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
  {
    PduR_RmDestRomIterType rmDestRomIdx = 0u;

    if(PduR_MultipleSourceHandler_GetSrcDestPdu(rmGDestRomIdx, &rmDestRomIdx) == E_OK)  /* COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING */
    {
      /* @ assert rmDestRomIdx < PduR_GetSizeOfRmDestRom(); */
      retVal = PduR_RmTp_TxRouting_DispatchCopyTxData(rmDestRomIdx, info, retry, availableDataPtr);
    }
    else
    {
      PduR_Reporting_ReportError(PDUR_RMTP_MULTIPLESOURCEHANDLER_COPYTXDATA, PDUR_E_UNEXPECTED_CALL);
    }
  }
  PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
  return retVal;
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_MultipleSourceHandler.c
 *********************************************************************************************************************/
