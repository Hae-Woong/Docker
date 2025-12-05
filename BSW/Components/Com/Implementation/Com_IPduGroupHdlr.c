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
/*!        \file  Com_IPduGroupHdlr.c
 *         \unit  IPduGroupHdlr
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_IPDUGROUPHDLR_SOURCE

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
#include "Com_IPduGroupHdlr.h"
#include "Com_Cfg_IPduGroupHdlr.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_RxPduBuffer.h"
#include "Com_Transmission.h"
#include "Com_LLRxTp.h"
#include "Com_Notifications.h"
#include "Com_Reporting.h"
#include "Com_TxDlMon.h"
#include "Com_GwTout.h"
#include "Com_ISRThreshold.h"
#include "Com_RxPduProcessing.h"
#include "Com_RxDlMon.h"
#include "Com_SigGw.h"
#include "Com_Transmission.h"

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
#include "Com_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateRxComIPdu
**********************************************************************************************************************/
/*! \brief        This function activates a Rx ComIPdu and optionally initializes the ComIPdu.
    \details      -
    \pre          -
    \param[in]    rxPduId    - ID of Rx ComIPdu
    \param[in]    initialize - True, if belonging buffer shall be initialized
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateRxComIPdu(Com_RxPduInfoIterType rxPduId, boolean initialize);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateTxComIPdu
**********************************************************************************************************************/
/*! \brief        This function activates a Tx ComIPdu and optionally initializes the ComIPdu.
    \details      -
    \pre          -
    \param[in]    ComTxPduId   - ID of Tx ComIPdu
    \param[in]    initialize - True, if belonging buffer shall be initialized
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec    
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateTxComIPdu(Com_TxPduInfoIterType ComTxPduId, boolean initialize);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_SetRxPduGrpStateToStop
**********************************************************************************************************************/
/*! \brief        This function sets the Rx-PDU-Group-State to FALSE, if the state of all referenced IPDU groups is
 *                equal to COM_INACTIVE_STATEOFIPDUGROUPINFO
    \details      -
    \pre          -
    \param[in]    ipduGroupId  ComIPduGroup handle id
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_SetRxPduGrpStateToStop(Com_IPduGroupInfoIterType ipduGroupId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_SetTxPduGrpStateToStop
**********************************************************************************************************************/
/*! \brief        This function sets the Tx-PDU-Group-State to FALSE, if the state of all referenced IPDU groups is
 *                equal to COM_INACTIVE_STATEOFIPDUGROUPINFO
    \details      -
    \pre          -
    \param[in]    ipduGroupId  ComIPduGroup handle id
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_SetTxPduGrpStateToStop(Com_IPduGroupInfoIterType ipduGroupId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateRxComIPdu
**********************************************************************************************************************/
/*! \brief        This function deactivates the given Rx ComIPdu
    \details      -
    \pre          -
    \param[in]    rxPduId  ComIPdu  handle id
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateRxComIPdu(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateTxComIPdu
**********************************************************************************************************************/
/*! \brief        This function deactivates the given Tx ComIPdu
    \details      -
    \pre          -
    \param[in]    txPduId  ComIPdu  handle id
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateTxComIPdu(Com_TxPduInfoIterType txPduId);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! \brief        This function deactivates Rx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! \brief        This function deactivates Tx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_StopAllIpduGroups
**********************************************************************************************************************/
/*! \brief        Iterates over all ComIPduGroups and stops them by a call of Com_IPduGroupHdlr_IpduGroupStop
    \details      -
    \pre          -
    \param[in]    -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_StopAllIpduGroups(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStartRx
**********************************************************************************************************************/
/*! \brief        Starts a preconfigured I-PDU Rx-IpduGroup.
    \details      -
    \pre          -
    \param[in]    ipduGroupId ID of I-PDU Group to be started
    \param[in]    Initialize  Flag to request initialization of the data in the I-PDUs of this I-PDU group
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStartRx(Com_IPduGroupInfoIterType ipduGroupId, boolean Initialize);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStartTx
**********************************************************************************************************************/
/*! \brief        Starts a preconfigured I-PDU Tx-IpduGroup.
    \details      -
    \pre          -
    \param[in]    ipduGroupId ID of I-PDU Group to be started
    \param[in]    Initialize  Flag to request initialization of the data in the I-PDUs of this I-PDU group
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ipduGroupId < Com_GetSizeOfIPduGroupInfo();
    \endspec    
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStartTx(Com_IPduGroupInfoIterType ipduGroupId, boolean Initialize);



/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateRxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateRxComIPdu(Com_RxPduInfoIterType rxPduId, boolean initialize)
{
  /* If the ComIPdu should be initialized, initialize the Rx ComIPdu parameters */
  if (initialize)
  {
    /* COM614 If a previously stopped I-PDU gets started by the operation specified in COM613 and the parameter initialize is set to true, then the function Com_IpduGroupControl shall (re-)initialize this I-PDU before it is started. */
    Com_IPduGroupHdlr_InitRxIpdu(rxPduId);
  }

  /* If the ComIPdu is a TP Pdu initialize the TP Pdu parameters */
  if (Com_GetTypeOfRxPduInfo(rxPduId) == COM_TP_TYPEOFRXPDUINFO)
  {
    Com_LLRxTp_ResetRxTpConnection(rxPduId);
  }

  /* If the Rx ComIPdu is handled deferred, reset the "Handle Deferred" flag */
  Com_RxPduBuffer_ResetLengthOfDeferredRxPdu(rxPduId);

  /* Set the state of the Rx ComIPdu to active */
  Com_SetActiveOfRxPduInfo(rxPduId, TRUE);

  /* Iterate over all related Rx timeout state machines and set them to active */
  /* \trace SPEC-2736840 */
  for (Com_RxTOutInfoIndIterType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
       idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++) /* FETA_COM_01 */
  {
    /*@ assert idxRxTOutInfoInd < Com_GetSizeOfRxTOutInfoInd(); */ /* VCA_COM_CSL03 */
    Com_RxDlMon_ActivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateTxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateTxComIPdu(Com_TxPduInfoIterType ComTxPduId, boolean initialize)
{
  /* If the ComIPdu should be initialized, initialize the Tx ComIPdu parameters */
  if (initialize)
  {
    /* COM614 If a previously stopped I-PDU gets started by the operation specified in COM613 and the parameter initialize is set to true, then the function Com_IpduGroupControl shall (re-)initialize this I-PDU before it is started. */
    Com_Transmission_InitTxIpdu(ComTxPduId);
  }

  /* If the ComIPdu is a TP Pdu initialize the TP Pdu parameters */
  Com_Transmission_ResetTxTpConnection(ComTxPduId);

  /* Start the Tx deadline monitoring */
  /* \trace SPEC-2736840 */
  Com_TxDlMon_IPduGroupStart(ComTxPduId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  /* Initialize gateway counter */
  Com_GwTout_InitCounter(ComTxPduId);
#endif

  /* Clear all Update Bits */
  Com_Transmission_ClearUpdateBits(ComTxPduId);

  /* Set the Tx ComIPdu Group state to active */
  Com_SetActiveOfTxPduInfo(ComTxPduId, TRUE);
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_SetRxPduGrpStateToStop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_SetRxPduGrpStateToStop(Com_IPduGroupInfoIterType ipduGroupId)
{
  if (Com_IsPartitionsRxStructUsedOfIPduGroupInfo(ipduGroupId) && Com_HasRxPduInfoIndUsedOfIPduGroupInfo())
  {
    /* Iterate over all related Rx ComIPdu of the passed ComIPduGroup */
    const Com_PartitionsRxStructIterType partitionsIdx = Com_GetPartitionsRxStructIdxOfIPduGroupInfo(ipduGroupId);
    /*@ assert partitionsIdx < Com_GetSizeOfPartitionsRxStruct(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    const Com_ISRThresholdInfoIterType rxIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsRxStruct(partitionsIdx);

    Com_ISRThreshold_EnterExclusiveArea(rxIpduGroupISRLockCounterIdx);

    for (Com_RxPduInfoIndIterType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(ipduGroupId);
         rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(ipduGroupId); rxPduIndId++) /* FETA_COM_01 */
    {
      boolean canBeStopped = TRUE;
      /*@ assert rxPduIndId < Com_GetSizeOfRxPduInfoInd(); */ /* VCA_COM_CSL03 */
      const Com_RxPduInfoIterType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);

      /* Iterate over all related ComIPdu Groups */
      for (Com_IPduGroupInfoOfRxPduInfoIndIterType referencedIPduGroupInd = Com_GetIPduGroupInfoOfRxPduInfoIndStartIdxOfRxPduInfo(rxPduId);
           referencedIPduGroupInd < Com_GetIPduGroupInfoOfRxPduInfoIndEndIdxOfRxPduInfo(rxPduId); referencedIPduGroupInd++) /* FETA_COM_01 */
      {
        const Com_IPduGroupInfoIterType referencedIPduGroup = Com_GetIPduGroupInfoOfRxPduInfoInd(referencedIPduGroupInd);
        /* If at least one related ComIPdu Group is active, the ComIPdu remains active */
        if (Com_GetStateOfIPduGroupInfo(referencedIPduGroup) != COM_INACTIVE_STATEOFIPDUGROUPINFO)
        {
          canBeStopped = FALSE;
          break;
        }
      }
      /* If all related ComIPdu Groups are stopped the ComIPdu Group state is set to inactive */
      if (canBeStopped)
      {
        Com_IPduGroupHdlr_DeactivateRxComIPdu(rxPduId);
      }

      Com_ISRThreshold_ThresholdCheck(rxIpduGroupISRLockCounterIdx);
    }

    Com_ISRThreshold_ExitExclusiveArea(rxIpduGroupISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_SetTxPduGrpStateToStop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_SetTxPduGrpStateToStop(Com_IPduGroupInfoIterType ipduGroupId)
{
  if (Com_IsPartitionsTxStructUsedOfIPduGroupInfo(ipduGroupId) && Com_HasTxPduInfoIndUsedOfIPduGroupInfo())
  {
    const Com_PartitionsTxStructIterType partitionsIdx = Com_GetPartitionsTxStructIdxOfIPduGroupInfo(ipduGroupId);
    /*@ assert partitionsIdx < Com_GetSizeOfPartitionsTxStruct(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    const Com_ISRThresholdInfoIterType txIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsTxStruct(partitionsIdx);

    Com_ISRThreshold_EnterExclusiveArea(txIpduGroupISRLockCounterIdx);

    /* Iterate over all related Tx ComIPdu of the passed ComIPduGroup */
    for (Com_TxPduInfoIndIterType txPduIndId = Com_GetTxPduInfoIndStartIdxOfIPduGroupInfo(ipduGroupId);
         txPduIndId < Com_GetTxPduInfoIndEndIdxOfIPduGroupInfo(ipduGroupId); txPduIndId++) /* FETA_COM_01 */
    {
      boolean canBeStopped = TRUE;
      /*@ assert txPduIndId < Com_GetSizeOfTxPduInfoInd(); */ /* VCA_COM_CSL03 */
      const Com_TxPduInfoIterType txPduId = Com_GetTxPduInfoInd(txPduIndId);

      /* Iterate over all related ComIPdu Groups */
      for (Com_IPduGroupInfoOfTxPduInfoIndIterType referencedIPduGroupInd = Com_GetIPduGroupInfoOfTxPduInfoIndStartIdxOfTxPduInfo(txPduId);
           referencedIPduGroupInd < Com_GetIPduGroupInfoOfTxPduInfoIndEndIdxOfTxPduInfo(txPduId); referencedIPduGroupInd++) /* FETA_COM_01 */
      {
        const Com_IPduGroupInfoIterType referencedIPduGroup = Com_GetIPduGroupInfoOfTxPduInfoInd(referencedIPduGroupInd);
        /* If at least one related ComIPdu Group is active, the ComIPdu remains active */
        if (Com_GetStateOfIPduGroupInfo(referencedIPduGroup) != COM_INACTIVE_STATEOFIPDUGROUPINFO)
        {
          canBeStopped = FALSE;
          break;
        }
      }
      /* If all related ComIPdu Groups are stopped, deactivate TxComIPdu */
      if (canBeStopped)
      {
        Com_IPduGroupHdlr_DeactivateTxComIPdu(txPduId);
      }
      Com_ISRThreshold_ThresholdCheck(txIpduGroupISRLockCounterIdx);
    }

    Com_ISRThreshold_ExitExclusiveArea(txIpduGroupISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateRxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateRxComIPdu(Com_RxPduInfoIterType rxPduId)
{
  /* Set RxPduGrpActivate State to FALSE */
  Com_SetActiveOfRxPduInfo(rxPduId, FALSE);

  /* Deactivate Rx Deadline Monitoring */
  for (Com_RxTOutInfoIndIterType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
       idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++) /* FETA_COM_01 */
  {
    /*@ assert idxRxTOutInfoInd < Com_GetSizeOfRxTOutInfoInd(); */ /* VCA_COM_CSL03 */
    Com_RxDlMon_DeactivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateTxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateTxComIPdu(Com_TxPduInfoIterType txPduId)
{
  /* Set the Tx Mode related parameters to the initial value */
  Com_Transmission_StopTxIpdus(txPduId);

  /* If the transmission of a ComIPdu was not confirmed */
  if (Com_HasReportingErrorNotificationIndUsedOfTxPduInfo() && Com_IsWaitingForConfirmation(txPduId))
  {
    /* set WaitForConfirmation flags to the initial value */
    Com_SetWaitingForConfirmation(txPduId, FALSE);

    /* call any configured error notification */
    const Com_PartitionsTxStructIterType partitionsIdx = Com_GetPartitionsTxStructIdxOfTxPduInfo(txPduId);
    const Com_ISRThresholdInfoIterType txIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsTxStruct(partitionsIdx);
    Com_ISRThreshold_ExitExclusiveArea(txIpduGroupISRLockCounterIdx);
    {
      for (Com_ReportingErrorNotificationIndIterType reportingIndIdx = Com_GetReportingErrorNotificationIndStartIdxOfTxPduInfo(txPduId);
           reportingIndIdx < Com_GetReportingErrorNotificationIndEndIdxOfTxPduInfo(txPduId); reportingIndIdx++) /* FETA_COM_01 */
      {
        /*@ assert reportingIndIdx < Com_GetSizeOfReportingErrorNotificationInd(); */ /* VCA_COM_CSL03 */
        const Com_ReportingIterType reportingIdx = Com_GetReportingErrorNotificationInd(reportingIndIdx);
        Com_Reporting_CallNotification(reportingIdx);
      }
    }
    Com_ISRThreshold_EnterExclusiveArea(txIpduGroupISRLockCounterIdx);
  }

  /* Clear HandleTxPduDeferred flag */
  Com_SetCallDeferredConfirmationNotification(txPduId, FALSE);

  /* If the ComIPdu is a TP Pdu, initialize the TP Pdu parameters */
  Com_Transmission_ResetTxTpConnection(txPduId);

  /* Set the Tx ComIPdu Group state to inactive */
  Com_SetActiveOfTxPduInfo(txPduId, FALSE);
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus(void)
{
  /* Iterate over all always active partitions of Rx ComIPdus */
  for (Com_PartitionsAlwaysActiveRxComIPdusStructIterType alwaysActivePartitionsIdx = 0;
       alwaysActivePartitionsIdx < Com_GetSizeOfPartitionsAlwaysActiveRxComIPdusStruct(); alwaysActivePartitionsIdx++) /* FETA_COM_01 */
  {
    const Com_ISRThresholdInfoIterType rxIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsAlwaysActiveRxComIPdusStruct(alwaysActivePartitionsIdx);

    /* Iterate over all always active Rx pdus of the partition */
    Com_ISRThreshold_EnterExclusiveArea(rxIpduGroupISRLockCounterIdx);
    for (Com_AlwaysActiveRxComIPdusIterType alwaysActiveRxPduIdx = Com_GetAlwaysActiveRxComIPdusStartIdxOfPartitionsAlwaysActiveRxComIPdusStruct(alwaysActivePartitionsIdx);
         alwaysActiveRxPduIdx < Com_GetAlwaysActiveRxComIPdusEndIdxOfPartitionsAlwaysActiveRxComIPdusStruct(alwaysActivePartitionsIdx); alwaysActiveRxPduIdx++) /* FETA_COM_01 */
    {
      /* Deactivate the RxComIPdu */
      Com_IPduGroupHdlr_DeactivateRxComIPdu(Com_GetRxPduInfoIdxOfAlwaysActiveRxComIPdus(alwaysActiveRxPduIdx));
      Com_ISRThreshold_ThresholdCheck(rxIpduGroupISRLockCounterIdx);
    }
    Com_ISRThreshold_ExitExclusiveArea(rxIpduGroupISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus(void)
{
  /* Iterate over all always active partitions of Tx ComIPdus */
  for (Com_PartitionsAlwaysActiveTxComIPdusStructIterType alwaysActivePartitionsIdx = 0;
       alwaysActivePartitionsIdx < Com_GetSizeOfPartitionsAlwaysActiveTxComIPdusStruct(); alwaysActivePartitionsIdx++) /* FETA_COM_01 */
  {
    const Com_ISRThresholdInfoIterType txIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsAlwaysActiveTxComIPdusStruct(alwaysActivePartitionsIdx);

    /* Iterate over all always active tx pdus of the partition */
    Com_ISRThreshold_EnterExclusiveArea(txIpduGroupISRLockCounterIdx);
    for (Com_AlwaysActiveTxComIPdusIterType alwaysActiveTxPduIdx = Com_GetAlwaysActiveTxComIPdusStartIdxOfPartitionsAlwaysActiveTxComIPdusStruct(alwaysActivePartitionsIdx);
         alwaysActiveTxPduIdx < Com_GetAlwaysActiveTxComIPdusEndIdxOfPartitionsAlwaysActiveTxComIPdusStruct(alwaysActivePartitionsIdx); alwaysActiveTxPduIdx++) /* FETA_COM_01 */
    {
      /* Deactivate the TxComIPdu */
      Com_IPduGroupHdlr_DeactivateTxComIPdu(Com_GetTxPduInfoIdxOfAlwaysActiveTxComIPdus(alwaysActiveTxPduIdx));
      Com_ISRThreshold_ThresholdCheck(txIpduGroupISRLockCounterIdx);
    }

    Com_ISRThreshold_ExitExclusiveArea(txIpduGroupISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_StopAllIpduGroups
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_StopAllIpduGroups(void)
{
  for (Com_IPduGroupInfoIterType comIPduGroupId = 0; comIPduGroupId < Com_GetSizeOfIPduGroupInfo(); comIPduGroupId++) /* FETA_COM_01 */
  {
    if (!Com_IsInvalidHndOfIPduGroupInfo(comIPduGroupId))
    {
      Com_IPduGroupHdlr_IpduGroupStop(comIPduGroupId);
    }
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStartRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStartRx(Com_IPduGroupInfoIterType ipduGroupId, boolean Initialize)
{
  if (Com_IsPartitionsRxStructUsedOfIPduGroupInfo(ipduGroupId) && Com_HasRxPduInfoIndUsedOfIPduGroupInfo())
  {
    /* Iterate over all related Rx ComIPdus */
    const Com_PartitionsRxStructIterType partitionsIdx = Com_GetPartitionsRxStructIdxOfIPduGroupInfo(ipduGroupId);
    /*@ assert partitionsIdx < Com_GetSizeOfPartitionsRxStruct(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    const Com_ISRThresholdInfoIterType rxIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsRxStruct(partitionsIdx);

    Com_ISRThreshold_EnterExclusiveArea(rxIpduGroupISRLockCounterIdx);

    for (Com_RxPduInfoIndIterType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(ipduGroupId);
         rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(ipduGroupId); rxPduIndId++) /* FETA_COM_01 */
    {
      /*@ assert rxPduIndId < Com_GetSizeOfRxPduInfoInd(); */ /* VCA_COM_CSL03 */
      const Com_RxPduInfoIterType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);
      /* If the Rx ComIPdu is inactive, activate and optionally initialize the Rx ComIPdu */

      if (!Com_IsActiveOfRxPduInfo(rxPduId))
      {
        /* Start Rx I-PDU */
        Com_IPduGroupHdlr_ActivateRxComIPdu(rxPduId, Initialize);
      }
      Com_ISRThreshold_ThresholdCheck(rxIpduGroupISRLockCounterIdx);
    }
    Com_ISRThreshold_ExitExclusiveArea(rxIpduGroupISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStartTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStartTx(Com_IPduGroupInfoIterType ipduGroupId, boolean Initialize)
{
  if (Com_IsPartitionsTxStructUsedOfIPduGroupInfo(ipduGroupId) && Com_HasTxPduInfoIndUsedOfIPduGroupInfo())
  {
    /* Iterate over all related Tx ComIPdus */
    const Com_PartitionsTxStructIterType partitionsIdx = Com_GetPartitionsTxStructIdxOfIPduGroupInfo(ipduGroupId);
    /*@ assert partitionsIdx < Com_GetSizeOfPartitionsTxStruct(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    const Com_ISRThresholdInfoIterType txIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsTxStruct(partitionsIdx);

    Com_ISRThreshold_EnterExclusiveArea(txIpduGroupISRLockCounterIdx);

    for (Com_TxPduInfoIndIterType txPduIndId = Com_GetTxPduInfoIndStartIdxOfIPduGroupInfo(ipduGroupId);
         txPduIndId < Com_GetTxPduInfoIndEndIdxOfIPduGroupInfo(ipduGroupId); txPduIndId++) /* FETA_COM_01 */
    {
      /*@ assert txPduIndId < Com_GetSizeOfTxPduInfoInd(); */ /* VCA_COM_CSL03 */
      const Com_TxPduInfoIterType txPduId = Com_GetTxPduInfoInd(txPduIndId);
      /* If the ComIPdu state is inactive */
      if (!Com_IsActiveOfTxPduInfo(txPduId))
      {
        /* Activate and optionally initialize the Tx ComIPdu */
        Com_IPduGroupHdlr_ActivateTxComIPdu(txPduId, Initialize);
      }

      Com_ISRThreshold_ThresholdCheck(txIpduGroupISRLockCounterIdx);
    }

    Com_ISRThreshold_ExitExclusiveArea(txIpduGroupISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DisableReceptionDM
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DisableReceptionDM(Com_IPduGroupInfoIterType ipduGroupId)
{
  /* If the current deadline monitoring state is DMACTIVE set the state to DMINACTIVE */
  if (Com_GetStateOfIPduGroupInfo(ipduGroupId) == COM_ACTIVE_DMACTIVE_STATEOFIPDUGROUPINFO)
  {
    Com_SetStateOfIPduGroupInfo(ipduGroupId, COM_ACTIVE_DMINACTIVE_STATEOFIPDUGROUPINFO);
    {
      /* Iterate over all related Rx ComIPdus */
      for (Com_RxPduInfoIndIterType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(ipduGroupId);
           rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(ipduGroupId); rxPduIndId++) /* FETA_COM_01 */
      {
        boolean canBeStopped = TRUE;
        /*@ assert rxPduIndId < Com_GetSizeOfRxPduInfoInd(); */ /* VCA_COM_CSL03 */
        const Com_RxPduInfoIterType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);

        /* Iterate over all related ComIPduGroups */
        for (Com_IPduGroupInfoOfRxPduInfoIndIterType referencedIPduGroupInd = Com_GetIPduGroupInfoOfRxPduInfoIndStartIdxOfRxPduInfo(rxPduId);
             referencedIPduGroupInd < Com_GetIPduGroupInfoOfRxPduInfoIndEndIdxOfRxPduInfo(rxPduId); referencedIPduGroupInd++) /* FETA_COM_01 */
        {
          /* If at least one ComIPduGroup state is DMACTIVE the deadline monitoring states remain active */
          const Com_IPduGroupInfoIterType referencedIPduGroup = Com_GetIPduGroupInfoOfRxPduInfoInd(referencedIPduGroupInd);
          if (Com_GetStateOfIPduGroupInfo(referencedIPduGroup) == COM_ACTIVE_DMACTIVE_STATEOFIPDUGROUPINFO)
          {
            canBeStopped = FALSE;
            break;
          }
        }
        /* If all ComIPduGroups are in state DMINACTIVE */
        if (canBeStopped)
        {
          /* Iterate all Rx deadline monitoring states and deactivate them */
          for (Com_RxTOutInfoIndIterType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
               idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++) /* FETA_COM_01 */
          {
            /*@ assert idxRxTOutInfoInd < Com_GetSizeOfRxTOutInfoInd(); */ /* VCA_COM_CSL03 */
            Com_RxDlMon_DeactivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));
          }
        }
      }
    }
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_EnableReceptionDM
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_EnableReceptionDM(Com_IPduGroupInfoIterType ipduGroupId)
{
  /* If the current deadline monitoring state of the IpduGroup is DMINACTIVE set the state to DMACTIVE */
  if (Com_GetStateOfIPduGroupInfo(ipduGroupId) == COM_ACTIVE_DMINACTIVE_STATEOFIPDUGROUPINFO)
  {
    if(Com_IsPartitionsRxStructUsedOfIPduGroupInfo(ipduGroupId)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
    {
      /* Iterate over all related Rx ComIPdus */
      const Com_PartitionsRxStructIterType partitionsIdx = Com_GetPartitionsRxStructIdxOfIPduGroupInfo(ipduGroupId);
      /*@ assert partitionsIdx < Com_GetSizeOfPartitionsRxStruct(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      const Com_ISRThresholdInfoIterType rxIpduGroupISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfPartitionsRxStruct(partitionsIdx);

      Com_ISRThreshold_EnterExclusiveArea(rxIpduGroupISRLockCounterIdx);
      Com_SetStateOfIPduGroupInfo(ipduGroupId, COM_ACTIVE_DMACTIVE_STATEOFIPDUGROUPINFO);

      for (Com_RxPduInfoIndIterType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(ipduGroupId);
          rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(ipduGroupId); rxPduIndId++) /* FETA_COM_01 */
      {
        /*@ assert rxPduIndId < Com_GetSizeOfRxPduInfoInd(); */ /* VCA_COM_CSL03 */
        const Com_RxPduInfoIterType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);

        /* Iterate over all related Rx deadline monitoring state machines */
        for (Com_RxTOutInfoIndIterType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
            idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++) /* FETA_COM_01 */
        {
          /* If the Rx deadline monitoring is INACTIVE, activate the Rx deadline monitoring */
          /*@ assert idxRxTOutInfoInd < Com_GetSizeOfRxTOutInfoInd(); */ /* VCA_COM_CSL03 */
          const Com_RxTOutInfoIterType idxRxTOutInfo = Com_GetRxTOutInfoInd(idxRxTOutInfoInd);
          Com_RxDlMon_ActivateRxDlMon(idxRxTOutInfo);

          Com_ISRThreshold_ThresholdCheck(rxIpduGroupISRLockCounterIdx);
        }
      }
      Com_ISRThreshold_ExitExclusiveArea(rxIpduGroupISRLockCounterIdx);
    }
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStop(Com_IPduGroupInfoIterType ipduGroupId)
{
  if (Com_GetStateOfIPduGroupInfo(ipduGroupId) != COM_INACTIVE_STATEOFIPDUGROUPINFO)
  {
    Com_SetStateOfIPduGroupInfo(ipduGroupId, COM_INACTIVE_STATEOFIPDUGROUPINFO);
    Com_IPduGroupHdlr_SetRxPduGrpStateToStop(ipduGroupId);
    Com_IPduGroupHdlr_SetTxPduGrpStateToStop(ipduGroupId);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupStart
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupStart(Com_IPduGroupInfoIterType ipduGroupId, boolean initialize)
{
  if (Com_GetStateOfIPduGroupInfo(ipduGroupId) == COM_INACTIVE_STATEOFIPDUGROUPINFO)
  {
    Com_SetStateOfIPduGroupInfo(ipduGroupId, COM_ACTIVE_DMACTIVE_STATEOFIPDUGROUPINFO);
    Com_IPduGroupHdlr_IpduGroupStartRx(ipduGroupId, initialize);
    Com_IPduGroupHdlr_IpduGroupStartTx(ipduGroupId, initialize);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IPduGroupsInit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *-
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IPduGroupsInit(void)
{
  for (Com_IPduGroupInfoIterType comIPduGroupId = 0; comIPduGroupId < Com_GetSizeOfIPduGroupInfo(); comIPduGroupId++) /* FETA_COM_01 */
  {
    Com_SetStateOfIPduGroupInfo(comIPduGroupId, COM_INACTIVE_STATEOFIPDUGROUPINFO);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IpduGroupsDeInit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IpduGroupsDeInit(void)
{
  Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus();
  Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus();
  Com_IPduGroupHdlr_StopAllIpduGroups();
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *-
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateAlwaysOnRxComIPdus(void)
{
  for (Com_AlwaysActiveRxComIPdusIterType idx = 0; idx < Com_GetSizeOfAlwaysActiveRxComIPdus(); idx++) /* FETA_COM_01 */
  {
    Com_IPduGroupHdlr_ActivateRxComIPdu(Com_GetRxPduInfoIdxOfAlwaysActiveRxComIPdus(idx), TRUE);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ActivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ActivateAlwaysOnTxComIPdus(void)
{
  for (Com_AlwaysActiveTxComIPdusIterType idx = 0; idx < Com_GetSizeOfAlwaysActiveTxComIPdus(); idx++) /* FETA_COM_01 */
  {
    Com_IPduGroupHdlr_ActivateTxComIPdu(Com_GetTxPduInfoIdxOfAlwaysActiveTxComIPdus(idx), TRUE);
  }
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IsRxPduActive
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_IPduGroupHdlr_IsRxPduActive(Com_RxPduInfoIterType idxRxPduInfo)
{
  return Com_IsActiveOfRxPduInfo(idxRxPduInfo);
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_IsTxPduActive
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_IPduGroupHdlr_IsTxPduActive(Com_TxPduInfoIterType idxTxPduInfo)
{
  return Com_IsActiveOfTxPduInfo(idxTxPduInfo);
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateRxPduActive
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateRxPduActive(Com_RxPduInfoIterType idxRxPduInfo)
{
  Com_SetActiveOfRxPduInfo(idxRxPduInfo, FALSE);
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateTxPduActive
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateTxPduActive(Com_TxPduInfoIterType idxTxPduInfo)
{
  Com_SetActiveOfTxPduInfo(idxTxPduInfo, FALSE);
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_InitRxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_InitRxIpdu(Com_RxPduInfoIterType rxPduId)
{
  /* Initialize the signal buffers */
  Com_RxPduProcessing_InitRxIpdu(rxPduId);

  /* Initialize the Rx Timeout handling parameters */
  Com_RxDlMon_InitRxIpdu(rxPduId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  /* Initialize the Signal Gateway parameters */
  Com_SigGw_InitRxIpdu(rxPduId);
#endif
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

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

/**********************************************************************************************************************
 *  END OF FILE: Com_IPduGroupHdlr.c
 *********************************************************************************************************************/
