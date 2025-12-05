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
 *         \file  PduR.c
 *         \unit  PduR_General
 *        \brief  Pdu Router General source file
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
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Bm.h"
#include "PduR_Fm.h"
#include "PduR_IFQ.h"
#include "PduR_RmIf.h"
#include "PduR_RmIf_DeferredProcessing.h"
#include "PduR_RmTp.h"
#include "PduR_RmTp_RxRouting.h"
#include "PduR_RmTp_RxInst.h"
#include "PduR_RmTp_TxInst.h"
#include "PduR_McQ.h"
#include "PduR_Mm.h"
#include "PduR_Lock.h"
#include "PduR_EvC.h"
#include "PduR_RPG.h"
#include "PduR_SingleBuffer.h"
#include "PduR_MultipleSourceHandler.h"
#include "PduR_Reporting.h"

#include "SchM_PduR.h"

#if(PDUR_MULTICORE == STD_ON)
# include "Os.h"
#endif

#if(PDUR_USE_ECUM_BSW_ERROR_HOOK == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of PduR header file */
#if (  (PDUR_SW_MAJOR_VERSION != (18u)) \
    || (PDUR_SW_MINOR_VERSION != (3u)) \
    || (PDUR_SW_PATCH_VERSION != (2u)) )
# error "Vendor specific version numbers of PduR.c and PduR.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  PduR_InitMemorySection
**********************************************************************************************************************/
/*! \brief       This function initializes the PDUR
 *  \details     Initializes all component variables of a specific memory section and
 *               sets the component state in this memory section to initialized.
 *  \param[in]   memIdx   memory section index
 *  \pre         Module is uninitialized.
 *  \pre         PduR_PreInit has been called.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 ********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_InitMemorySection(PduR_MemIdxType memIdx);

/**********************************************************************************************************************
 * PduR_Rm_CancelTransmit_MultiDest
 *********************************************************************************************************************/
/*!
 * \brief       This function calls the CancelTransmit function for every destination
 * \details     -
 * \param[in]   rmSrcRomIdx          Valid RmSrcRom table index. Internal handle of a PduRSrcPdu.
 * \return      Std_ReturnType
 *              E_OK              Cancellation was executed successfully by at least on of the destination modules.
 *              E_NOT_OK          Cancellation was rejected by all destination modules.
 * \pre         -
 * \context     TASK|ISR1|ISR2
 * \reentrant   TRUE
 * \synchronous TRUE
**********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_Rm_CancelTransmit_MultiDest(PduR_RmSrcRomIterType rmSrcRomIdx);

/**********************************************************************************************************************
 * LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * GLOBAL AND LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if(PDUR_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 * PduR_GetVersionInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, PDUR_APPL_DATA) versionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(versionInfo == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    versionInfo->vendorID = (uint16) PDUR_VENDOR_ID;
    versionInfo->moduleID = (uint8) PDUR_MODULE_ID;
    versionInfo->sw_major_version = (uint8) PDUR_SW_MAJOR_VERSION;
    versionInfo->sw_minor_version = (uint8) PDUR_SW_MINOR_VERSION;
    versionInfo->sw_patch_version = (uint8) PDUR_SW_PATCH_VERSION;
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_VERSIONINFO, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}       /* End of PduR_GetVersionInfo() */
#endif

/**********************************************************************************************************************
 * PduR_GetConfigurationId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(PduR_PBConfigIdType, PDUR_CODE) PduR_GetConfigurationId(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduR_PBConfigIdType configID = 0u;    /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(!PduR_IsInitializedRam(0u, PduR_PartitionIndexOfCSLForCommonSharedMemory))  /* COV_PDUR_MULTICORE_INITIALIZATION */
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else
# endif
  {
    configID = PduR_GetConfigId(PduR_PartitionIndexOfCSLForCommonSharedMemory);
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_CONFIGURATIONID, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
  return configID;
}

/* Upper Layer Interface APIs */
/**********************************************************************************************************************
 * PduR_UpTransmit
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
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, PDUR_CODE) PduR_UpTransmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfTx2Lo())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_Tx2LoIterType tx2LoIdx = (PduR_Tx2LoIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfTx2Lo(tx2LoIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfTx2Lo(tx2LoIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfTx2Lo(tx2LoIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      PduR_Reporting_CallTransmitCallback(tx2LoIdx, info);
      {
        PduR_RmSrcRomIdxOfTx2LoType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfTx2Lo(tx2LoIdx);

        if(PduR_IsIfOfRmSrcRom(rmSrcRomIdx))
        {
          /* Queued and Unqueued If Routing */
          retVal = PduR_RmIf_RoutePdu(rmSrcRomIdx, info);
        }
# if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
        else if(PduR_IsRmBufferedTpPropertiesRomUsedOfRmSrcRom(rmSrcRomIdx))
        {
          /* Queued Tp Routing */
          retVal = PduR_RmTp_RxInst_UpStartOfReception(rmSrcRomIdx, info);
        }
# endif
        else
        {
          /* Unqueued Tp Routing */
          retVal = PduR_RmTp_Transmit_MultiDest(rmSrcRomIdx, info);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_TX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/* Communication Interface APIs */
/**********************************************************************************************************************
 * PduR_LoIfRxIndication
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
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_LoIfRxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(RxPduId >= PduR_GetSizeOfRxIf2Dest())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_RxIf2DestIterType rxIf2DestIdx = (PduR_RxIf2DestIterType)RxPduId;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRxIf2Dest(rxIf2DestIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxIf2Dest(rxIf2DestIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxIf2Dest(rxIf2DestIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      PduR_Reporting_CallRxIndicationCallback(rxIf2DestIdx, info);

      (void) PduR_RmIf_RoutePdu(PduR_GetRmSrcRomIdxOfRxIf2Dest(rxIf2DestIdx), info);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_IFRXIND, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_LoIfTxConfirmation
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, PDUR_CODE) PduR_LoIfTxConfirmation(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(TxPduId >= PduR_GetSizeOfTxIf2Up())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_TxIf2UpIterType txIf2UpIdx = (PduR_TxIf2UpIterType)TxPduId;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfTxIf2Up(txIf2UpIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxIf2Up(txIf2UpIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxIf2Up(txIf2UpIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      PduR_Reporting_CallTxConfirmationCallback(txIf2UpIdx);

      if(PduR_IsTxConfirmationUsedOfTxIf2Up(txIf2UpIdx))
      {
        PduR_MultipleSourceHandler_IfTxConfirmation(PduR_GetRmGDestRomIdxOfTxIf2Up(txIf2UpIdx));
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_IFTXCFM, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_LoIfTriggerTransmit
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
FUNC(Std_ReturnType, PDUR_CODE) PduR_LoIfTriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(TxPduId >= PduR_GetSizeOfTxIf2Up())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_TxIf2UpIterType txIf2Up = (PduR_TxIf2UpIterType)TxPduId;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfTxIf2Up(txIf2Up))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxIf2Up(txIf2Up))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxIf2Up(txIf2Up)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      retVal = PduR_MultipleSourceHandler_TriggerTransmit(PduR_GetRmGDestRomIdxOfTxIf2Up(txIf2Up), info);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_TT, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/* Transport Protocol APIs */
/**********************************************************************************************************************
 * PduR_LoTpStartOfReception
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
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_LoTpStartOfReception(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, PduLengthType TpSduLength,
                                                             P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfRxTp2Dest())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_RxTp2DestIterType rxTp2DestIdx = (PduR_RxTp2DestIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRxTp2Dest(rxTp2DestIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else if(bufferSizePtr == NULL_PTR)
    {
      errorId = PDUR_E_PARAM_POINTER;
    }
    else
# endif
    {
      PduR_Reporting_CallTpStartOfReceptionCallback(rxTp2DestIdx, info, TpSduLength, bufferSizePtr);
      retVal = PduR_RmTp_RxRouting_DispatchStartOfReception(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx), info, TpSduLength, bufferSizePtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_SOR, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_LoTpCopyRxData
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_LoTpCopyRxData(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfRxTp2Dest())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_RxTp2DestIterType rxTp2DestIdx = (PduR_RxTp2DestIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRxTp2Dest(rxTp2DestIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else if(bufferSizePtr == NULL_PTR)
    {
      errorId = PDUR_E_PARAM_POINTER;
    }
    else
# endif
    {
      retVal = PduR_RmTp_RxRouting_DispatchCopyRxData(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx), info, bufferSizePtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_CPYRX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_LoTpCopyTxData
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
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_LoTpCopyTxData(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
                                                       P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfTxTp2Src())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_TxTp2SrcIterType txTp2SrcIdx = (PduR_TxTp2SrcIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfTxTp2Src(txTp2SrcIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxTp2Src(txTp2SrcIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxTp2Src(txTp2SrcIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      retVal = PduR_MultipleSourceHandler_CopyTxData(PduR_GetRmGDestRomIdxOfTxTp2Src(txTp2SrcIdx), info, retry, availableDataPtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_CPYTX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_LoTpRxIndication
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
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_LoTpRxIndication(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfRxTp2Dest())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_RxTp2DestIterType rxTp2DestIdx = (PduR_RxTp2DestIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRxTp2Dest(rxTp2DestIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      PduR_Reporting_CallTpRxIndicationCallback(rxTp2DestIdx, result);
      PduR_RmTp_RxRouting_DispatchTpRxIndication(PduR_GetRmSrcRomIdxOfRxTp2Dest(rxTp2DestIdx), result);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_TPRXIND, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_LoTpTxConfirmation
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
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_LoTpTxConfirmation(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfTxTp2Src())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_TxTp2SrcIterType txTp2SrcIdx = (PduR_TxTp2SrcIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfTxTp2Src(txTp2SrcIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxTp2Src(txTp2SrcIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfTxTp2Src(txTp2SrcIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      PduR_Reporting_CallTpTxConfirmationCallback(txTp2SrcIdx, result);
      PduR_MultipleSourceHandler_TpTxConfirmation(PduR_GetRmGDestRomIdxOfTxTp2Src(txTp2SrcIdx), result);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_TPTXCFM, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_CancelReceive
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, PDUR_CODE) PduR_CancelReceive(PduIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfRxTp2Src())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_RxTp2SrcIterType rxTp2Src = (PduR_RxTp2SrcIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRxTp2Src(rxTp2Src))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(PduR_GetRmDestRomIdxOfRxTp2Src(rxTp2Src)))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(PduR_GetRmDestRomIdxOfRxTp2Src(rxTp2Src))))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      PduR_RmDestRomIdxOfRxTp2SrcType rmDestRomIdx = PduR_GetRmDestRomIdxOfRxTp2Src(rxTp2Src);
      PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
      PduR_MmRomIterType mmRomIdx = PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx);

      if(PduR_IsCancelReceiveSupportedOfMmRom(mmRomIdx))
      {
        if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
        {
          PduR_SrcHndOfRmSrcRomType srcHnd = PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx);
          retVal = PduR_Mm_CallTpCancelReceive(mmRomIdx, srcHnd);
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_CRX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_ChangeParameter
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
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, PDUR_CODE) PduR_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfRxTp2Src())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_RxTp2SrcIterType rxTp2SrcIdx = (PduR_RxTp2SrcIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRxTp2Src(rxTp2SrcIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(PduR_GetRmDestRomIdxOfRxTp2Src(rxTp2SrcIdx)))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(PduR_GetRmDestRomIdxOfRxTp2Src(rxTp2SrcIdx))))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      PduR_RmDestRomIdxOfRxTp2SrcType rmDestRomIdx = PduR_GetRmDestRomIdxOfRxTp2Src(rxTp2SrcIdx);
      PduR_RmSrcRomIdxOfRmDestRomType rmSrcRomIdx = PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx);
      PduR_MmRomIterType mmRomIdx = PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx);

      if(PduR_IsChangeParameterSupportedOfMmRom(mmRomIdx))
      {
        retVal = PduR_Mm_CallTpChangeParameter(mmRomIdx, PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), parameter, value);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_CHPARA, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_CancelTransmit
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, PDUR_CODE) PduR_CancelTransmit(PduIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
  else if(id >= PduR_GetSizeOfTx2Lo())
  {
    errorId = PDUR_E_PDU_ID_INVALID;
  }
  else
# endif
  {
    PduR_Tx2LoIterType tx2LoIdx = (PduR_Tx2LoIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfTx2Lo(tx2LoIdx))
    {
      errorId = PDUR_E_PDU_ID_INVALID;
    }
    else if(!PduR_IsInitializedRam(PduR_GetInitializedIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfTx2Lo(tx2LoIdx))), PduR_GetInitializedPartitionIdxOfDestApplicationManagerRom(PduR_GetDestApplicationManagerRomIdxOfRmSrcRom(PduR_GetRmSrcRomIdxOfTx2Lo(tx2LoIdx)))))
    {
      errorId = PDUR_E_INVALID_REQUEST;
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      if(PduR_IsCancelTransmitUsedOfTx2Lo(tx2LoIdx))
      {
        retVal = PduR_Rm_CancelTransmit_MultiDest(PduR_GetRmSrcRomIdxOfTx2Lo(tx2LoIdx));
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_CTX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}

/**********************************************************************************************************************
 * PduR_Rm_CancelTransmit_MultiDest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
PDUR_LOCAL_INLINE FUNC(Std_ReturnType, PDUR_CODE) PduR_Rm_CancelTransmit_MultiDest(PduR_RmSrcRomIterType rmSrcRomIdx)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  PduR_RmDestRomIterType rmDestRomIdx;

  for(rmDestRomIdx = PduR_GetRmDestRomStartIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx < PduR_GetRmDestRomEndIdxOfRmSrcRom(rmSrcRomIdx); rmDestRomIdx++)  /* FETA_PDUR_01 */
  {
    if(PduR_RPG_IsDestPduEnabled(rmDestRomIdx) == TRUE)
    {
      if(!PduR_IsQueuedOfRmDestRom(rmDestRomIdx))  /* COV_PDUR_UNQUEUED_ROUTING_ONLY */
      {
        if(PduR_IsIfOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)))
        {
          if(PduR_IsIfCancelTransmitSupportedOfMmRom(PduR_GetMmRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx))))
          {
            retVal &= PduR_MultipleSourceHandler_IfCancelTransmit(rmDestRomIdx);
          }
        }
        if(PduR_IsTpOfRmSrcRom(PduR_GetRmSrcRomIdxOfRmDestRom(rmDestRomIdx)))
        {
          if(PduR_IsTpCancelTransmitSupportedOfMmRom(PduR_GetMmRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx))))
          {
            retVal &= PduR_MultipleSourceHandler_TpCancelTransmit(rmDestRomIdx);
          }
        }
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  PduR_PreInit
 **********************************************************************************************************************/
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
 *
 *
 *
 *
 */
FUNC(void, PDUR_CODE) PduR_PreInit(P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_INIT_DATA) ConfigPtr)
{
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduR_PartitionIdentifiersIterType partitionIdentifiersIdx;

#if(PDUR_USE_INIT_POINTER == STD_ON)
  PduR_ConfigDataPtr = ConfigPtr;
  if(PduR_ConfigDataPtr == NULL_PTR)
  {
# if(PDUR_USE_ECUM_BSW_ERROR_HOOK == STD_ON)    /* COV_PDUR_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(PDUR_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = PDUR_E_PARAM_POINTER;
  }
# if (PDUR_FINALMAGICNUMBER == STD_ON)  /* COV_PDUR_VAR_DEPENDENT_SWITCH */
  else if(PduR_GetFinalMagicNumber() != PDUR_FINAL_MAGIC_NUMBER)
  {
#  if(PDUR_USE_ECUM_BSW_ERROR_HOOK == STD_ON)   /* COV_PDUR_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(PDUR_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = PDUR_E_INIT_FAILED;
    PduR_ConfigDataPtr = (const PduR_PBConfigType *) NULL_PTR;
  }
  else
# endif
#else
  if(ConfigPtr != NULL_PTR)
  {
    errorId = PDUR_E_INIT_FAILED;
  }
  else
#endif
  {
    for (partitionIdentifiersIdx = 0u; partitionIdentifiersIdx < PduR_GetSizeOfPartitionIdentifiers(); partitionIdentifiersIdx++)  /* FETA_PDUR_01 */
    {
      PduR_SetInitializedRam(0u, FALSE, PduR_GetPCPartitionConfigIdxOfPartitionIdentifiers(partitionIdentifiersIdx));
    }

#if(PDUR_MULTICORE == STD_ON)
    /* Initialize shared memory section. */
    PduR_InitMemorySection(PduR_PartitionIndexOfCSLForCommonSharedMemory);
#endif

    PduR_PreInitialized = TRUE;
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_PREINIT, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_Init
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
FUNC(void, PDUR_CODE) PduR_Init(P2CONST(PduR_PBConfigType, AUTOMATIC, PDUR_INIT_DATA) ConfigPtr)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if(PDUR_MULTICORE == STD_ON)
  PduR_ApplicationId2DestApplicationManagerRomIterType applicationId = (PduR_ApplicationId2DestApplicationManagerRomIterType) GetApplicationID();
# endif

  /* ----- Development Error Checks ------------------------------------- */
  if (!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_NO_PREINIT;
  }
# if(PDUR_MULTICORE == STD_ON)
  else if(applicationId >= PduR_GetSizeOfApplicationId2DestApplicationManagerRom())
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(PduR_IsInvalidHndOfApplicationId2DestApplicationManagerRom(applicationId))
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(PduR_IsInitializedRam(0u, PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId)))
  {
    errorId = PDUR_E_ALREADY_INITIALIZED;
  }
# else
  else if(PduR_IsInitializedRam(0u, PduR_PartitionIndexOfCSLForCommonSharedMemory))
  {
    errorId = PDUR_E_ALREADY_INITIALIZED;
  }
# endif
  else
  {
# if(PDUR_MULTICORE == STD_ON)
    PduR_InitMemorySection(PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId));
# else
    PduR_InitMemorySection(PduR_PartitionIndexOfCSLForCommonSharedMemory);
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_INIT, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
  PDUR_DUMMY_STATEMENT(ConfigPtr);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_InitMemorySection
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_InitMemorySection(PduR_MemIdxType memIdx)
{
  PduR_RPG_Init(memIdx);

  PduR_MultipleSourceHandler_Init(memIdx);

#if(PDUR_BMTXBUFFERRAM == STD_ON)
  PduR_Bm_Init(memIdx);
#endif

#if(PDUR_FMFIFORAM == STD_ON)
  PduR_Fm_Init(memIdx);
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  PduR_RmIf_DeferredProcessing_Init(memIdx);
#endif

#if(PDUR_SINGLEBUFFERROM == STD_ON)
  PduR_SingleBuffer_Init(memIdx);
#endif

#if (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  PduR_RmTp_RxInst_Init(memIdx);
  PduR_RmTp_TxInst_Init(memIdx);
#endif

#if(PDUR_MCQBUFFERROM == STD_ON)
  PduR_McQ_Init(memIdx);
#endif

#if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)
  PduR_EvC_Init(memIdx);
#endif

#if (PDUR_SPINLOCKRAM == STD_ON)
  PduR_Lock_Init(memIdx);
#endif

#if (PDUR_INTERFACEFIFOQUEUEROM == STD_ON)
  PduR_IFQ_Init(memIdx);
#endif

  PduR_SetInitializedRam(0u, TRUE, memIdx);
}

/**********************************************************************************************************************
  PduR_MainFunction
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, PDUR_CODE) PduR_MainFunction(void)
{
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if(PDUR_MULTICORE == STD_ON)
  PduR_ApplicationId2DestApplicationManagerRomIterType applicationId = (PduR_ApplicationId2DestApplicationManagerRomIterType) GetApplicationID();
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# if(PDUR_MULTICORE == STD_ON)
  else if(applicationId >= PduR_GetSizeOfApplicationId2DestApplicationManagerRom())
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(PduR_IsInvalidHndOfApplicationId2DestApplicationManagerRom(applicationId))
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(!PduR_IsInitializedRam(0u, PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId)))
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# else
  else if(!PduR_IsInitializedRam(0u, PduR_PartitionIndexOfCSLForCommonSharedMemory))
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# endif
  else
# endif
  {
    {
      PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx = 0u; /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if(PDUR_MULTICORE == STD_ON)
      destApplicationManagerRomIdx = PduR_GetDestApplicationManagerRomIdxOfApplicationId2DestApplicationManagerRom(applicationId);
# endif

      {
# if ((PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON) || (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON))
#  if (PDUR_DEFERREDEVENTCACHEROM == STD_ON)
        PduR_EvC_MainFunction(destApplicationManagerRomIdx);
#  else
        PduR_ProcessAllDestinationsOfDestApplication(destApplicationManagerRomIdx);
#  endif
# endif

# if (PDUR_MCQBUFFERROM == STD_ON)
        /* Process deferred If Pdus which are handled by the multicore queue in context of cyclic MainFunction */
        PduR_McQ_MainFunction(destApplicationManagerRomIdx);
# endif
      }

      PDUR_DUMMY_STATEMENT(destApplicationManagerRomIdx);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_MAINFUNCTION_TX, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_InitMemory
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_InitMemory(void)
{
  PduR_PreInitialized = FALSE;
#if(PDUR_USE_INIT_POINTER == STD_ON)
  PduR_ConfigDataPtr = NULL_PTR;
#endif
}

#if ((PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON) || (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON))
/**********************************************************************************************************************
 * PduR_DispatchMainfuntion
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_DispatchMainfuntion(PduR_RmGDestRomIterType rmGDestRomIdx)
{
# if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_QUEUED_IF_ROUTING_ONLY */
  {
    PduR_RmIf_DeferredProcessing_MainFunction(rmGDestRomIdx);
  }
# endif
# if(PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)
  if(PduR_IsRmGDestQueuedTpRomUsedOfRmGDestRom(rmGDestRomIdx))  /* COV_PDUR_QUEUED_TP_ROUTING_ONLY */
  {
    PduR_RmTp_TxInst_MainFunction(rmGDestRomIdx);
  }
# endif
}
#endif

#if ((PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON) || (PDUR_EXISTS_TP_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON))
/**********************************************************************************************************************
 * PduR_ProcessAllDestinationsOfDestApplication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_ProcessAllDestinationsOfDestApplication(PduR_DestApplicationManagerRomIterType destApplicationManagerRomIdx)
{
  PduR_RmGDestRomIterType rmGDestRomIdx;

  for(rmGDestRomIdx = PduR_GetRmGDestRomStartIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); rmGDestRomIdx < PduR_GetRmGDestRomEndIdxOfDestApplicationManagerRom(destApplicationManagerRomIdx); rmGDestRomIdx++)  /* FETA_PDUR_01 */
  {
    PduR_DispatchMainfuntion(rmGDestRomIdx);
  }
}
#endif

/**********************************************************************************************************************
 * PduR_EnableRouting
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, PDUR_CODE) PduR_EnableRouting(PduR_RoutingPathGroupIdType id)
{
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
# if(PDUR_MULTICORE == STD_ON)
  PduR_ApplicationId2DestApplicationManagerRomIterType applicationId = (PduR_ApplicationId2DestApplicationManagerRomIterType) GetApplicationID();
# endif

  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# if(PDUR_MULTICORE == STD_ON)
  else if(applicationId >= PduR_GetSizeOfApplicationId2DestApplicationManagerRom())
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(PduR_IsInvalidHndOfApplicationId2DestApplicationManagerRom(applicationId))
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(!PduR_IsInitializedRam(0u, PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId)))
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# else
  else if(!PduR_IsInitializedRam(0u, PduR_PartitionIndexOfCSLForCommonSharedMemory))
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# endif
  else if(id >= PduR_GetSizeOfRpgExtIdRom())
  {
    errorId = PDUR_E_ROUTING_PATH_GROUP_ID_INVALID;
  }
  else
# endif
  {
    PduR_RpgExtIdRomIterType rpgExtIdRomIdx = (PduR_RpgExtIdRomIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRpgExtIdRom(rpgExtIdRomIdx))
    {
      errorId = PDUR_E_ROUTING_PATH_GROUP_ID_INVALID;
    }
    else
# endif
    {
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
#  if(PDUR_MULTICORE == STD_ON)
      if(PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId) != PduR_GetRpgRomPartitionIdxOfRpgExtIdRom(rpgExtIdRomIdx))
      {
        errorId = PDUR_E_INVALID_PARTITION_CONTEXT;
      }
      else
#  endif
# endif
      {
        PduR_RPG_EnableRouting(PduR_GetRpgRomIdxOfRpgExtIdRom(rpgExtIdRomIdx), PduR_GetRpgRomPartitionIdxOfRpgExtIdRom(rpgExtIdRomIdx));
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_ENABLEROUTING, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

/**********************************************************************************************************************
 * PduR_DisableRouting
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, PDUR_CODE) PduR_DisableRouting(PduR_RoutingPathGroupIdType id)
{
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
# if(PDUR_MULTICORE == STD_ON)
  PduR_ApplicationId2DestApplicationManagerRomIterType applicationId = (PduR_ApplicationId2DestApplicationManagerRomIterType) GetApplicationID();
# endif

  if(!PduR_IsPreInitialized())
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# if(PDUR_MULTICORE == STD_ON)
  else if(applicationId >= PduR_GetSizeOfApplicationId2DestApplicationManagerRom())
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(PduR_IsInvalidHndOfApplicationId2DestApplicationManagerRom(applicationId))
  {
    errorId = PDUR_E_INVALID_APPLICATION_ID;
  }
  else if(!PduR_IsInitializedRam(0u, PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId)))
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# else
  else if(!PduR_IsInitializedRam(0u, PduR_PartitionIndexOfCSLForCommonSharedMemory))
  {
    errorId = PDUR_E_INVALID_REQUEST;
  }
# endif
  else if(id >= PduR_GetSizeOfRpgExtIdRom())
  {
    errorId = PDUR_E_ROUTING_PATH_GROUP_ID_INVALID;
  }
  else
# endif
  {
    PduR_RpgExtIdRomIterType rpgExtIdRomIdx = (PduR_RpgExtIdRomIterType)id;

# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
    if(PduR_IsInvalidHndOfRpgExtIdRom(rpgExtIdRomIdx))
    {
      errorId = PDUR_E_ROUTING_PATH_GROUP_ID_INVALID;
    }
    else
# endif
    {
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
#  if(PDUR_MULTICORE == STD_ON)
      if(PduR_GetPartitionConfigIdxOfApplicationId2DestApplicationManagerRom(applicationId) != PduR_GetRpgRomPartitionIdxOfRpgExtIdRom(rpgExtIdRomIdx))
      {
        errorId = PDUR_E_INVALID_PARTITION_CONTEXT;
      }
      else
#  endif
# endif
      {
        PduR_RPG_DisableRouting(PduR_GetRpgRomIdxOfRpgExtIdRom(rpgExtIdRomIdx), PduR_GetRpgRomPartitionIdxOfRpgExtIdRom(rpgExtIdRomIdx));
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_DISABLEROUTING, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* module specific MISRA deviations:
  MD_PduR_1505:  Misra Rule 8.7
     Reason:      It depends on the configuration if the function is only referenced in the translation unit is is defined or not.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_PduR_1336: Misra Rule 8.2
     Reason:      No identifier name needed in a function pointer typedef.
     Risk:        No risk, because only warning that with a identifier name the readability is better.
     Prevention:  No prevention necessary.

  MD_PduR_0771: Misra Rule 15.4
     Reason:      There should be no more than one break or goto statement used to terminate any iteration statement.
     Risk:        No risk. The code is most easily readable with more than one break statement.
     Prevention:  No prevention necessary.

  MD_PduR_2880: Misra Rule 2.1
     Reason:      If the development error checks are switched off, no error will be reported ever.
     Risk:        No risk, as the code will be removed by the compiler.
     Prevention:  No prevention necessary.

  MD_PduR_2982_2983: Misra Rule 2.2
     Reason:      It depends on the configuration if the value is modified.
     Risk:        No risk, because only warning that assignment is redundant.
     Prevention:  No prevention necessary.

  MD_PduR_2985_2986: Misra Rule 2.2
     Reason:      The value of the operands depends on the configuration.
     Risk:        No risk, because only warning that the operation is redundant.
     Prevention:  No prevention necessary.

  MD_PduR_3415: Misra Rule 13.5
     Reason:      The right hand operand is known to have no side effects.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_PduR_3451_3449: Misra Rule 8.8
     Reason:      The global identifier has been declared in more than one file. The function declaration has to be in
                  the PduR_<Module>.h. Affected APIs are CancelReceive, CancelTransmit and ChangeParameter.
     Risk:        No risk.
     Prevention:  To avoid a duplicate declaration use the BSW-Module "use-Tag" flag.
*/

/* FETA_JUSTIFICATION_BEGIN

    \ID FETA_PDUR_01
      \DESCRIPTION Loop over static number of elements.
      \COUNTERMEASURE \N The loop has a static upper limit determined by a ROM value. It will always terminate in finite time.
                         The memory section index is either read from a CSL indirection or calculated with the OsApplicationId, which is considered valid.

    \ID FETA_PDUR_02
      \DESCRIPTION Loop with external termination condition.
      \COUNTERMEASURE \N The loop uses a local loop counter with static upper limit to break the loop in case the external termination condition never breaks the loop.

    \ID FETA_PDUR_03
      \DESCRIPTION Loop with external termination condition.
      \COUNTERMEASURE \N The termination condition is a queue get method. By design, nothing can be put into the queue while reading elements from it. This is ensured by exclusive areas or spinlocks.

    \ID FETA_PDUR_04
      \DESCRIPTION Loop over sizeof(datatype).
      \COUNTERMEASURE \N The loop has a static upper limit determined by a compile time value. It will always terminate in finite time.

   FETA_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN
  \ID COV_PDUR_FEATURE_ALWAYS_TRUE
    \ACCEPT TX
    \REASON Depending on the configuration the feature check can always be true. In this case the compiler removes the check.

  \ID COV_PDUR_LAST_COND_ALWAYS_TRUE
    \ACCEPT TF tf tx
    \ACCEPT TF tf tf tx
    \REASON The second condition is always true if the first one is true. The second condition is only added for silent bsw argumentation.

  \ID COV_PDUR_RUNS_ON_ALL_APPLICATIONS
    \APPLYOVERRIDE
    \REASON The PduR is executed on all configured Os Applications. Hence this loop always finds a valid Partition SNV and breaks the loop.

  \ID COV_PDUR_MISRA
    \ACCEPT X
    \ACCEPT TX
    \REASON [MSR_COV_MISRA]

  \ID COV_PDUR_IF_ONLY
   \ACCEPT X
   \ACCEPT XX
   \ACCEPT XF
   \ACCEPT TX
   \REASON This is only called if buffered transport protocol routing paths exist. This is not the case for communication interface routing path only configurations.

  \ID COV_PDUR_VCA_CHECK
    \ACCEPT TX
    \ACCEPT TX tx tx
    \REASON This check was introduced so that VCA can do the silent analysis correctly.

  \ID COV_PDUR_ROBUSTNESS_UNCOVERED
    \ACCEPT XX
    \REASON This function can never be reached in a working ECU where no arbitrary software overwrites PduR RAM.

  \ID COV_PDUR_UNQUEUED_ROUTING_ONLY
    \APPLYOVERRIDE
    \REASON This condition can not be covered completely in configurations with only unqueue routing paths.

  \ID COV_PDUR_UNQUEUED_ROUTING_ONLY_STATIC
    \ACCEPT TF xf tf
    \ACCEPT TF xf tf tf
    \ACCEPT TF tf xf tf
    \REASON This condition can not be covered completely in configurations with only unqueue routing paths.

  \ID COV_PDUR_MULTICORE_INITIALIZATION
    \APPLYOVERRIDE
    \REASON In case of Multicore configuration, the commonSharedMemory will always be initialized in PreInit function. This way the IsInitialized flag in commonSharedMemory can never be false in the API pattern.

  \ID COV_PDUR_QUEUED_IF_ROUTING_ONLY
    \APPLYOVERRIDE
    \REASON This condition can not be covered completely in configurations with only queued IF routing paths.

  \ID COV_PDUR_QUEUED_TP_ROUTING_ONLY
    \APPLYOVERRIDE
    \REASON This condition can not be covered completely in configurations with only queued TP routing paths.

  \ID COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING
    \APPLYOVERRIDE
    \REASON This condition can not be covered completely in configurations without queued TP routing paths. A N:1 TP routing with only unqueued paths is not possible.

  \ID COV_PDUR_MULTIPLESOURCEHANDLER_NO_QUEUED_TP_ROUTING_STATIC
    \ACCEPT XX xx xx xx
    \REASON This condition can not be covered completely in configurations without queued TP routing paths. A N:1 TP routing with only unqueued paths is not possible.

  \ID COV_PDUR_UNCOVERED_FUNCTION_LOCKBYSRCPDU
    \ACCEPT X
    \REASON This function may not be covered on component level depending on the actual configuration.

  \ID COV_PDUR_ROBUSTNESS
    \ACCEPT TF tf tx
    \ACCEPT TX
    \REASON This check increases robustness. Under normal circumstances it is never false/true.

  \ID COV_PDUR_MULTIPARTITION_ONLY
    \APPLYOVERRIDE
    \REASON This code is only used if PduR is used on multi partition systems. Otherwise the code is not used.

  \ID COV_PDUR_MULTICORE_ONLY
    \ACCEPT XF
    \REASON This code is only used if PduR is used on multicore systems. Otherwise the code is not used.

  \ID COV_PDUR_VAR_QAL_QUEUE_TYPES
    \ACCEPT TX
    \REASON QAL unit is only tested with all queue types on. Rationale: Only compile errors will be detected if some queue types are not supported.

  \ID COV_PDUR_VAR_DEV_ERROR_DETECT
    \ACCEPT TX
    \REASON Det error detection must always be enabled for SafeBsw.

  \ID COV_PDUR_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \ACCEPT TX tx tx
    \REASON The nested switch depends on a previous switch.

  \ID COV_PDUR_VAR_QM_FEATURE
    \ACCEPT XF
    \REASON Error notification is a QM feature and hence not enabled in safeBsw test configurations.

  \ID COV_PDUR_WRAPPER_FUNC
    \ACCEPT X
    \REASON The wrapper function may not be needed in some configurations.

  \ID COV_PDUR_VAR_UT_OPTIONAL_UNIT
    \ACCEPT TX
    \REASON The unit is disabled completely and therefore must not be tested in this case.

  \ID COV_PDUR_VAR_EXTENDED_ERROR_CHECKS
    \ACCEPT TX
    \REASON The extended error checks always have to be switched on.

COV_JUSTIFICATION_END */

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_PDUR_CALLING_UNDEFINED_FUNCTION
    \DESCRIPTION    VCA warns that PduR is calling undefined function.
    \COUNTERMEASURE \N No countermeasure is necessary. VCA is only analyzing PduR itself. The correctness of the functions in the function pointer table is tested in the integration test.

  \ID VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY
    \DESCRIPTION    VCA warns that this function is not used in the module and is no analysis entry.
    \COUNTERMEASURE \N No countermeasure is necessary.
                    Module entry functions are declared in Gw_AsrPduRCfg5.vcaplugin.
                    Unused functions shall be removed by the linker.

  \ID VCA_PDUR_GLUE_FUNCTION
    \DESCRIPTION    VCA warns that this function is not used in the module and is no analysis entry.
    \COUNTERMEASURE \S The module can be connected to components using generated glue functions. Those function directly call the generic public APIs of PduR. Not available APIs are detected during integration testing (SMI-4).

  \ID VCA_PDUR_SINGLEBUFFER_MEMCPY_S_1
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is read using a CSL03 qualified use-case.
                       The source pointer and length is handed over by the caller and has to be valid.
                       The input parameter to this API are checked using VCA specifications.

  \ID VCA_PDUR_SINGLEBUFFER_MEMCPY_S_2
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is read using a CSL03 qualified use-case.
                       The source pointer and length is read using a CSL03 qualified use-case.
                       The input parameter to this API are checked using VCA specifications.

  \ID VCA_PDUR_IFQ_MEMCPY_S
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \R The destination pointer and length is checked with a runtime check so that it is always correct.
                       The source pointer and length is handed over by the caller and has to be valid.
                       The input parameter to this API are checked using VCA specifications.

  \ID VCA_PDUR_SPINLOCK_COUNTER_VARIABLES
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \R The spinlockCounterRamIdx is calculated using the core_id. The result is runtime checked to be inbound.

  \ID VCA_PDUR_RMIF_QUEUEPROCESSING_MEMCPY_S
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \R The destination pointer and length is checked with a runtime check so that it is always correct.
                       The source pointer points to PduR internal RAM and is read via the Get API. It is not used if the Get API returns with negative result.

  \ID VCA_PDUR_BM_MEMCPY_S_1
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer is calculated using the writeIdx. The writeIdx is sanitized in the corresponding getter API.
                       The destination pointer length is the linear buffer size, which is the free buffer length until either the readIdx or the end of the ring buffer is reached. This value is directly calculated using the read writeIdx and readIdx,
                       both of which are sanitized in the corresponding getter APIs.
                       The source pointer is handed over by the caller and has to be valid. It is checked against NULL_PTR.
                       The source pointer length is either the handed over PduLength by the caller (which is considered correct) or a smaller value if a write with wrap-around has to be done. These values are calculated above.

  \ID VCA_PDUR_BM_MEMCPY_S_2
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer is statically calculated using the startIdx of the buffer and is hence valid.
                       The destination pointer length is calculated using the different buffer sizes above.
                       The source pointer is calculated using the handed over pointer. It is checked against NULL_PTR. The offset inside the pointer is calculated using the different buffer sizes.
                       The source pointer length is calculated using the different buffer sizes.

  \ID VCA_PDUR_BM_MEMCPY_S_3
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer is handed over by the caller and has to be valid. It is checked against NULL_PTR.
                       The destination pointer length is handed over by the caller and has to be valid.
                       The source pointer is calculated using the readIdx. The readIdx is sanitized in the corresponding getter API.
                       The source pointer length is either the linearBufferSize (if no wrap around is required) or the the requested PduLength.

  \ID VCA_PDUR_BM_MEMCPY_S_4
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer is handed over by the caller and has to be valid. It is checked against NULL_PTR. The offset is calculated using the different buffer sizes.
                       The destination pointer length is calculated using the different buffer sizes.
                       The source pointer is statically calculated using the startIdx of the buffer and is hence valid.
                       The source pointer length is calculated using the different buffer sizes.

  \ID VCA_PDUR_EXTENDED_ERROR_CHECK
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N The extended error check makes sure that the condition is true. The read value is sanitized against static Rom values of the CSL indirection.
                       The inputs of the API are checked with VCA specifications.

  \ID VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N The pointer is handed over by the caller of PduR and has to point to valid memory. The corresponding length has to match the pointers length. This has to be ensured by the caller.
                       VCA shall assume that this pointer points to external memory and the length is correct.

  \ID VCA_PDUR_IS_QUEUED_TP_ROUTING
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N Restricting the range in PduR_RmTp_GetValidTpSrcPduFromQueue works, but VCA adds the rmSrcRomIdx initial value of 0 to range.

  \ID VCA_PDUR_VCA_MEMCPY_S
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \N The VCA spec ensures a potential incorrect call to this API is shown as an error in the calling code. These errors will be justified in the calling code.

VCA_JUSTIFICATION_END */

/* *INDENT-ON* */

/**********************************************************************************************************************
 * END OF FILE: PduR.c
 *********************************************************************************************************************/
