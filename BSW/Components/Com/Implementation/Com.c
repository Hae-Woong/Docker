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
/*!        \file  Com.c
 *         \unit  Com_General
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
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

#define COM_SOURCE
/*! compatibility define for COM_SOURCE. */
#define V_IL_ASRCOMCFG5_COMPONENT_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_Cfg_IPduGroupHdlr.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_Initialization.h"
#include "Com_Reporting.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_LLRxIf.h"
#include "Com_LLRxTp.h"
#include "Com_RxSignalIf.h"
#include "Com_Transmission.h"

#include "SchM_Com.h"
#if(COM_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
  VERSION CHECK
**********************************************************************************************************************/
#if ((COM_SW_MAJOR_VERSION != 25u) || (COM_SW_MINOR_VERSION != 2u) || (COM_SW_PATCH_VERSION != 0u))
# error "Vendor specific version numbers of Com.c and Com.h are inconsistent"
#endif

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

#if (COM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  IsDynamicLengthSignalValid
**********************************************************************************************************************/
/*! \brief              This function checks the validity of the signal.
 *   \details           -
 *   \pre               -
 *   \param[in]         txAccessInfoIdx    Index of the signal.
 *   \param[in]         Length             Current length of dynamic length signal.
 *   \return            boolean
 *                        TRUE  dynamic length signal is valid
 *                        FALSE dynamic length signal is not valid
 *   \reentrant         TRUE, for different handles
 *   \synchronous       TRUE
 *   \context           ISR2
 *   \spec
 *     requires txAccessInfoIdx < Com_GetSizeOfTxAccessInfo();
 *   \endspec
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) IsDynamicLengthSignalValid(Com_TxAccessInfoIterType txAccessInfoIdx, uint16 Length);
#endif

#if (COM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
  IsDynamicLengthGroupSignalValid
**********************************************************************************************************************/
/*! \brief              This function checks the validity of the groupSignal.
 *   \details           -
 *   \pre               -
 *   \param[in]         txAccessInfoIdx    Index of the groupSignal.
 *   \param[in]         Length             Current length of dynamic length groupSignal.
 *   \return            boolean
 *                        TRUE  dynamic length groupSignal is valid
 *                        FALSE dynamic length groupSignal is not valid
 *   \reentrant         TRUE, for different handles
 *   \synchronous       TRUE
 *   \context           ISR2
 *   \spec
 *     requires txAccessInfoIdx < Com_GetSizeOfTxAccessInfo();
 *   \endspec
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) IsDynamicLengthGroupSignalValid(Com_TxAccessInfoIterType txAccessInfoIdx, uint16 Length);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
IsDynamicLengthSignalValid
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) IsDynamicLengthSignalValid(Com_TxAccessInfoIterType txAccessInfoIdx, uint16 Length)
{
  boolean retVal = TRUE;
  const Com_TxSigInfoIterType signalId = Com_GetTxSigInfoIdxOfTxAccessInfo(txAccessInfoIdx);
  const Com_TxPduInfoIterType pduId = Com_GetTxPduInfoIdxOfTxSigInfo(signalId);

  if(Com_GetTypeOfTxPduInfo(pduId) == COM_NORMAL_TYPEOFTXPDUINFO)
  {
    retVal = FALSE;
  }
  else if(!Com_IsDynamicInitialLengthUsedOfTxAccessInfo(txAccessInfoIdx))
  {
    retVal = FALSE;
  }
  else if(Length > Com_GetByteLengthOfTxSigInfo(signalId))
  {
    retVal = FALSE;
  }
  else
  {
    retVal = TRUE;
  }
  return retVal;
}
#endif

#if (COM_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
IsDynamicLengthGroupSignalValid
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) IsDynamicLengthGroupSignalValid(Com_TxAccessInfoIterType txAccessInfoIdx, uint16 Length)
{
  boolean retVal = TRUE;
  Com_TxGrpSigInfoIterType grpSignalId = Com_GetTxGrpSigInfoIdxOfTxAccessInfo(txAccessInfoIdx);
  const Com_TxPduInfoIterType pduId = Com_GetTxPduInfoIdxOfTxGrpSigInfo(grpSignalId);

  if(Com_GetTypeOfTxPduInfo(pduId) == COM_NORMAL_TYPEOFTXPDUINFO)
  {
    retVal = FALSE;
  }
  else if(!Com_IsDynamicInitialLengthUsedOfTxAccessInfo(txAccessInfoIdx))
  {
    retVal = FALSE;
  }
  else if(Length > Com_GetByteLengthOfTxGrpSigInfo(grpSignalId))
  {
    retVal = FALSE;
  }
  else
  {
    retVal = TRUE;
  }
  return retVal;
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
Com_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Init(P2CONST(Com_ConfigType, AUTOMATIC, COM_INIT_DATA) config)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

#if(COM_USE_INIT_POINTER == STD_ON)
  Com_ConfigDataPtr = config;
#endif
/* ----- Development Error Checks ------------------------------------- */
  /* Check plausibility of all input parameters */
  if(Com_Initialization_GetInitialized())
  {
    errorId = COM_E_INIT_FAILED;
  }
#if(COM_USE_INIT_POINTER == STD_ON)
    /* Check validity of Config pointer */
  else if(Com_ConfigDataPtr == NULL_PTR)
  {
    Com_Reporting_CallBswErrorHook(ECUM_BSWERROR_NULLPTR);
    errorId = COM_E_PARAM_POINTER;
  }
  else if(Com_HasFinalMagicNumber() && (Com_GetFinalMagicNumber() != COM_FINAL_MAGIC_NUMBER))
  {
    Com_Reporting_CallBswErrorHook(ECUM_BSWERROR_MAGICNUMBER);
    errorId = COM_E_PARAM_POINTER;
    Com_ConfigDataPtr = NULL_PTR;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Initialize data of individual units and set component state to initialized */
    Com_Initialization_Init();
  }
  Com_Reporting_CallDetReportError(COMServiceId_Init, errorId);

  COM_DUMMY_STATEMENT(config);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
Com_InitMemory
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_InitMemory(void)
{
  /* Set the ConfigDataPtr as unused */
#if(COM_USE_INIT_POINTER == STD_ON)
  Com_ConfigDataPtr = NULL_PTR;
#endif
  /* Initialize all component _INIT_ variables and set component state to initialized */
  Com_Initialization_InitMemory();
}

/**********************************************************************************************************************
Com_DeInit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DeInit(void)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Deactivate Pdus and set component state to uninitialized */
    Com_Initialization_DeInit();
  }
  Com_Reporting_CallDetReportError(COMServiceId_DeInit, errorId);
}

/**********************************************************************************************************************
Com_GetStatus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_StatusType, COM_CODE) Com_GetStatus(void)
{
  return (Com_Initialization_GetInitialized()? COM_INIT : COM_UNINIT);
}

/**********************************************************************************************************************
Com_IpduGroupStart
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IpduGroupStart(Com_IpduGroupIdType IpduGroupId, boolean Initialize)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(IpduGroupId >= Com_GetSizeOfIPduGroupInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_IPduGroupInfoIterType iPduGroupInfoIdx = (Com_IPduGroupInfoIterType) IpduGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfIPduGroupInfo(iPduGroupInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Start the IPduGroup */
      Com_IPduGroupHdlr_IpduGroupStart(iPduGroupInfoIdx, Initialize);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_IpduGroupStart, errorId);
}

/**********************************************************************************************************************
  Com_IpduGroupStop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IpduGroupStop(Com_IpduGroupIdType IpduGroupId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(IpduGroupId >= Com_GetSizeOfIPduGroupInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_IPduGroupInfoIterType iPduGroupInfoIdx = (Com_IPduGroupInfoIterType) IpduGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfIPduGroupInfo(iPduGroupInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Stop the IPduGroup */
      Com_IPduGroupHdlr_IpduGroupStop(iPduGroupInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_IpduGroupStop, errorId);
}

/**********************************************************************************************************************
Com_EnableReceptionDM
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EnableReceptionDM(Com_IpduGroupIdType IpduGroupId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(IpduGroupId >= Com_GetSizeOfIPduGroupInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_IPduGroupInfoIterType iPduGroupInfoIdx = (Com_IPduGroupInfoIterType) IpduGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfIPduGroupInfo(iPduGroupInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Enable the Rx deadline monitoring */
      Com_IPduGroupHdlr_EnableReceptionDM(iPduGroupInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_EnableReceptionDM, errorId);
}

/**********************************************************************************************************************
Com_DisableReceptionDM
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DisableReceptionDM(Com_IpduGroupIdType IpduGroupId)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(IpduGroupId >= Com_GetSizeOfIPduGroupInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_IPduGroupInfoIterType iPduGroupInfoIdx = (Com_IPduGroupInfoIterType) IpduGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfIPduGroupInfo(iPduGroupInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Disable the Rx deadline monitoring */
      Com_IPduGroupHdlr_DisableReceptionDM(iPduGroupInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_DisableReceptionDM, errorId);
}

/**********************************************************************************************************************
  Com_RxIndication
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, COM_CODE) Com_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(RxPduId >= Com_GetSizeOfRxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxPduInfoIterType rxPduInfoIdx = (Com_RxPduInfoIterType) RxPduId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfRxPduInfo(rxPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(PduInfoPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(Com_GetTypeOfRxPduInfo(rxPduInfoIdx) == COM_TP_TYPEOFRXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else if((Com_GetMetaDataLengthOfRxPduInfo(rxPduInfoIdx) > 0u) && (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if((Com_GetMetaDataLengthOfRxPduInfo(rxPduInfoIdx) > 0u) && (PduInfoPtr->SduLength < Com_GetMetaDataLengthOfRxPduInfo(rxPduInfoIdx)))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(!(Com_GetMetaDataLengthOfRxPduInfo(rxPduInfoIdx) > 0u) && (PduInfoPtr->SduDataPtr == NULL_PTR) && (PduInfoPtr->SduLength > 0u))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Do Rx Indication processing */
      (void) Com_LLRxIf_IfRxIndicationProcessing(rxPduInfoIdx, PduInfoPtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_RxIndication, errorId);
}

/**********************************************************************************************************************
  Com_StartOfReception
**********************************************************************************************************************/
/*!  * Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 3673, 6050, 6080 1 */ /* MD_MSR_Rule8.13, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(BufReq_ReturnType, COM_CODE) Com_StartOfReception(PduIdType ComRxPduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_VAR) TpSduInfoPtr, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, COM_APPL_VAR) RxBufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType bufReq_status = BUFREQ_E_NOT_OK;    /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(RxBufferSizePtr == NULL_PTR)
  {
    errorId = COM_E_PARAM_POINTER;
  }
  else if(ComRxPduId >= Com_GetSizeOfRxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxPduInfoIterType rxPduInfoIdx = (Com_RxPduInfoIterType) ComRxPduId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfRxPduInfo(rxPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfRxPduInfo(rxPduInfoIdx) == COM_NORMAL_TYPEOFRXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_LLRxTp_IsRxTpBufferMetaDataUsed(rxPduInfoIdx) && (TpSduInfoPtr == NULL_PTR))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(Com_LLRxTp_IsRxTpBufferMetaDataUsed(rxPduInfoIdx) && (TpSduInfoPtr->SduDataPtr == NULL_PTR))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(Com_LLRxTp_IsRxTpBufferMetaDataUsed(rxPduInfoIdx) && (TpSduInfoPtr->SduLength < Com_LLRxTp_GetRxTpBufferMetaDataLength(rxPduInfoIdx)))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call StartOfReception processing function */
      bufReq_status = Com_LLRxTp_StartOfReceptionProcessing(rxPduInfoIdx, TpSduInfoPtr, TpSduLength, RxBufferSizePtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_StartOfReception, errorId);
  return bufReq_status;
}

/**********************************************************************************************************************
  Com_TpRxIndication
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TpRxIndication(PduIdType PduId, Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(PduId >= Com_GetSizeOfRxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxPduInfoIterType rxPduInfoIdx = (Com_RxPduInfoIterType) PduId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfRxPduInfo(rxPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfRxPduInfo(rxPduInfoIdx) == COM_NORMAL_TYPEOFRXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call TpRxIndication processing function */
      Com_LLRxTp_TpRxIndicationProcessing(rxPduInfoIdx, Result);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_TpRxIndication, errorId);
}

/**********************************************************************************************************************
  Com_CopyRxData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 3673, 6080 1 */ /* MD_MSR_Rule8.13, MD_MSR_STMIF */
FUNC(BufReq_ReturnType, COM_CODE) Com_CopyRxData(PduIdType PduId, CONSTP2VAR(PduInfoType, AUTOMATIC, COM_APPL_VAR) PduInfoPointer, P2VAR(PduLengthType, AUTOMATIC, COM_APPL_VAR) RxBufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType bufReq_status = BUFREQ_E_NOT_OK;    /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(PduId >= Com_GetSizeOfRxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxPduInfoIterType rxPduInfoIdx = (Com_RxPduInfoIterType) PduId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfRxPduInfo(rxPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfRxPduInfo(rxPduInfoIdx) == COM_NORMAL_TYPEOFRXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else if(PduInfoPointer == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if((PduInfoPointer->SduDataPtr == NULL_PTR) && (PduInfoPointer->SduLength > 0u))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(RxBufferSizePtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call CopyRxData processing function */
      bufReq_status = Com_LLRxTp_CopyRxDataProcessing(rxPduInfoIdx, PduInfoPointer, RxBufferSizePtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_CopyRxData, errorId);
  return bufReq_status;
}

/**********************************************************************************************************************
  Com_TriggerTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, COM_CODE) Com_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(TxPduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) TxPduId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(PduInfoPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if((PduInfoPtr->SduDataPtr == NULL_PTR) && (PduInfoPtr->SduLength > 0u))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(Com_GetTypeOfTxPduInfo(txPduInfoIdx) == COM_TP_TYPEOFTXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call TriggerTransmit processing function */
      retVal = Com_Transmission_TriggerTransmitProcessing(txPduInfoIdx, PduInfoPtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_TriggerTransmit, errorId);
  return retVal;
}

/**********************************************************************************************************************
  Com_TxConfirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, COM_CODE) Com_TxConfirmation(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(TxPduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) TxPduId;
#if (COM_DEV_ERROR_DETECT == STD_ON )
    if(Com_IsInvalidHndOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfTxPduInfo(txPduInfoIdx) == COM_TP_TYPEOFTXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call TxConfirmation processing function */
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      {
        Com_Transmission_TxConfirmationProcessing(txPduInfoIdx);
      }
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_TxConfirmation, errorId);
}

/**********************************************************************************************************************
  Com_TpTxConfirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TpTxConfirmation(PduIdType PduId, Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(PduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) PduId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfTxPduInfo(txPduInfoIdx) == COM_NORMAL_TYPEOFTXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call TpTxConfirmation processing function */
      Com_Transmission_TpTxConfirmationProcessing(txPduInfoIdx, Result);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_TpTxConfirmation, errorId);
}

/**********************************************************************************************************************
  Com_CopyTxData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 3673, 6080 1 */ /* MD_MSR_Rule8.13, MD_MSR_STMIF */
FUNC(BufReq_ReturnType, COM_CODE) Com_CopyTxData(PduIdType PduId, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_VAR) PduInfoPtr, P2VAR(RetryInfoType, AUTOMATIC, COM_APPL_VAR) RetryInfoPtr, P2VAR(PduLengthType, AUTOMATIC, COM_APPL_VAR) TxDataCntPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(PduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) PduId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfTxPduInfo(txPduInfoIdx) == COM_NORMAL_TYPEOFTXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else if(PduInfoPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if((PduInfoPtr->SduDataPtr == NULL_PTR) && (PduInfoPtr->SduLength > 0u))
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(TxDataCntPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call CopyTxData processing function */
      retVal = Com_Transmission_CopyTxDataProcessing(txPduInfoIdx, PduInfoPtr, RetryInfoPtr, TxDataCntPtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_CopyTxData, errorId);
  return retVal;
}

/**********************************************************************************************************************
  Com_SwitchIpduTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SwitchIpduTxMode(PduIdType PduId, boolean Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */

  else if(PduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) PduId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxPduInfo((Com_TxPduInfoIterType) PduId))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call the SwitchIpduTxMode processing function */
      Com_Transmission_SwitchIpduTxMode(txPduInfoIdx, Mode);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_SwitchIpduTxMode, errorId);
}

/**********************************************************************************************************************
  Com_SendDynSignal
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_SendDynSignal(Com_SignalIdType SignalId, P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr, uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(SignalDataPtr == NULL_PTR)
  {
    errorId = COM_E_PARAM_POINTER;
  }
  else if(SignalId >= Com_GetSizeOfTxAccessInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxAccessInfoIterType txAccessInfoIdx = (Com_TxAccessInfoIterType) SignalId;

#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxAccessInfo(txAccessInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if((Com_IsTxSigInfoUsedOfTxAccessInfo(txAccessInfoIdx)) && !IsDynamicLengthSignalValid(txAccessInfoIdx, Length))
    {
      errorId = COM_E_PARAM;
    }
    else if((Com_IsTxGrpSigInfoUsedOfTxAccessInfo(txAccessInfoIdx)) && !IsDynamicLengthGroupSignalValid(txAccessInfoIdx, Length))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      {
        /* Process the passed Tx ComSignal */
        const ComDataTypeConst data = { SignalDataPtr, (PduLengthType)Length };
        /*@ assert $external(&data); */ /* VCA_COM_SIGNAL_DATA_PTR */
        retVal = Com_Transmission_SendSignal_Processing(txAccessInfoIdx, &data);
      }
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_SendDynSignal, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_SendSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_SendSignal(Com_SignalIdType SignalId, P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(SignalId >= Com_GetSizeOfTxAccessInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxAccessInfoIterType txAccessInfoIdx = (Com_TxAccessInfoIterType) SignalId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxAccessInfo((Com_TxAccessInfoIterType) SignalId))
    {
      errorId = COM_E_PARAM;
    }
    else if(SignalDataPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(Com_IsTxSigInfoUsedOfTxAccessInfo(txAccessInfoIdx) && (Com_IsDynamicInitialLengthUsedOfTxAccessInfo(txAccessInfoIdx)))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_IsTxGrpSigInfoUsedOfTxAccessInfo((Com_TxAccessInfoIterType) SignalId) && (Com_IsDynamicInitialLengthUsedOfTxAccessInfo(txAccessInfoIdx)))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      {
        /* Process the passed Tx ComSignal */
        const ComDataTypeConst data = { SignalDataPtr, 0u };
        retVal = Com_Transmission_SendSignal_Processing(txAccessInfoIdx, &data);
      }
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_SendSignal, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_SendSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_SendSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(SignalGroupId >= Com_GetSizeOfTxSigGrpInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxSigGrpInfoIterType txSigGrpInfoIdx  = (Com_TxSigGrpInfoIterType) SignalGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxSigGrpInfo(txSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_IsArrayAccessUsedOfTxSigGrpInfo(txSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      P2CONST(Com_TxBufferType, AUTOMATIC, AUTOMATIC) shdBuffer = NULL_PTR;

      /* If the ComSignalGroup has a shadow buffer, get the address of this shadow buffer */
      if(Com_IsTxBufferInfoUsedOfTxSigGrpInfo(txSigGrpInfoIdx))
      {
        shdBuffer = Com_Transmission_GetAddrTxBufferFirstElement(Com_GetTxBufferInfoIdxOfTxSigGrpInfo(txSigGrpInfoIdx));
      }
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      {
        /* Process the passed ComSignalGroup */
        retVal = Com_Transmission_SendSignalGroup_Processing(txSigGrpInfoIdx, shdBuffer);
      }
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_SendSignalGroup, errorId);
  return retVal;
}

/**********************************************************************************************************************
  Com_SendSignalGroupArray
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_SendSignalGroupArray(Com_SignalGroupIdType SignalGroupId, P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SignalGroupArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(SignalGroupId >= Com_GetSizeOfTxSigGrpInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxSigGrpInfoIterType txSigGrpInfoIdx = (Com_TxSigGrpInfoIterType) SignalGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxSigGrpInfo(txSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(!Com_IsArrayAccessUsedOfTxSigGrpInfo(txSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(SignalGroupArrayPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
# endif
    { /* ----- Implementation ----------------------------------------------- */
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      {
        /* Initiate the transmission of the signal group array */
        /*@ assert $external(SignalGroupArrayPtr); */ /* VCA_COM_SIGNAL_DATA_PTR */
        retVal = Com_Transmission_SendSignalGroupArray_Processing(txSigGrpInfoIdx, SignalGroupArrayPtr);
      }
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_SendSignalGroupArray, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_InvalidateSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_InvalidateSignal(Com_SignalIdType SignalId)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if(COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(SignalId >= Com_GetSizeOfTxAccessInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxAccessInfoIterType txAccessInfoIdx = (Com_TxAccessInfoIterType) SignalId;
#if(COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxAccessInfo(txAccessInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Invalidate the signal */
      retVal = Com_Transmission_InvalidateSignal(txAccessInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_InvalidateSignal, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_InvalidateSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_InvalidateSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(SignalGroupId >= Com_GetSizeOfTxSigGrpInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxSigGrpInfoIterType txSigGrpInfoIdx = (Com_TxSigGrpInfoIterType) SignalGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxSigGrpInfo(txSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* Invalidate the signal group */
      retVal = Com_Transmission_InvalidateSignalGroup(txSigGrpInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_InvalidateSignalGroup, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_ReceiveSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_ReceiveSignal(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_VAR) SignalDataPtr)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(SignalId >= Com_GetSizeOfRxAccessInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxAccessInfoIterType rxAccessInfoIdx = (Com_RxAccessInfoIterType) SignalId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfRxAccessInfo(rxAccessInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(SignalDataPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else if(Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call ReceiveSignal processing function */
      retVal = Com_RxSignalIf_ReceiveSignalProcessing(rxAccessInfoIdx, SignalDataPtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_ReceiveSignal, errorId);
  return retVal;
}

/**********************************************************************************************************************
  Com_ReceiveDynSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_ReceiveDynSignal(Com_SignalIdType SignalId, P2VAR(void, AUTOMATIC, COM_APPL_VAR) SignalDataPtr, P2VAR(uint16, AUTOMATIC, COM_APPL_VAR) Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(SignalDataPtr == NULL_PTR)
  {
    errorId = COM_E_PARAM_POINTER;
  }
  else if(Length == NULL_PTR)
  {
    errorId = COM_E_PARAM_POINTER;
  }
  else if(SignalId >= Com_GetSizeOfRxAccessInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxAccessInfoIterType rxAccessInfoIdx = (Com_RxAccessInfoIterType) SignalId;
# if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfRxAccessInfo(rxAccessInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_GetTypeOfRxPduInfo(Com_GetRxPduInfoIdxOfRxAccessInfo(rxAccessInfoIdx)) == COM_NORMAL_TYPEOFRXPDUINFO)
    {
      errorId = COM_E_PARAM;
    }
    else if(!Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
# endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call Com_RxSignalIf_ReceiveDynSignalProcessing processing function */
      retVal = Com_RxSignalIf_ReceiveDynSignalProcessing(rxAccessInfoIdx, SignalDataPtr, Length);
      if(retVal == E_NOT_OK)
      {
        errorId = COM_E_PARAM;
      }
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_ReceiveDynSignal, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_ReceiveSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_ReceiveSignalGroup(Com_SignalGroupIdType SignalGroupId)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(SignalGroupId >= Com_GetSizeOfRxSigGrpInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxSigGrpInfoIterType rxSigGrpInfoIdx = (Com_RxSigGrpInfoIterType) SignalGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfRxSigGrpInfo(rxSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(Com_IsArrayAccessUsedOfRxSigGrpInfo(rxSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call the ReceiveSignalGroup processing function */
      retVal = Com_RxSignalIf_ReceiveSignalGroupProcessing(rxSigGrpInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_ReceiveSignalGroup, errorId);
  return retVal;
}

/**********************************************************************************************************************
  Com_ReceiveSignalGroupArray
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(uint8, COM_CODE) Com_ReceiveSignalGroupArray(Com_SignalGroupIdType SignalGroupId, P2VAR(uint8, AUTOMATIC, COM_APPL_VAR) SignalGroupArrayPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
  /* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
    /* Check plausibility of input parameters */
  else if(SignalGroupId >= Com_GetSizeOfRxSigGrpInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_RxSigGrpInfoIterType rxSigGrpInfoIdx = (Com_RxSigGrpInfoIterType) SignalGroupId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfRxSigGrpInfo(rxSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(!Com_IsArrayAccessUsedOfRxSigGrpInfo(rxSigGrpInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(SignalGroupArrayPtr == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call the Com_RxSignalIf_ReceiveSignalGroupArrayProcessing processing function*/
      retVal = Com_RxSignalIf_ReceiveSignalGroupArrayProcessing(rxSigGrpInfoIdx, SignalGroupArrayPtr);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_ReceiveSignalGroupArray, errorId);
  return retVal;
}

/**********************************************************************************************************************
  Com_TriggerIPDUSend
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TriggerIPDUSend(PduIdType PduId)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(PduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) PduId;
#if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else
#endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call the TriggerIPDUSend processing function */
      Com_Transmission_TriggerIpduSend(txPduInfoIdx);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_TriggerIPDUSend, errorId);
}

/**********************************************************************************************************************
Com_TriggerIPDUSendWithMetaData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, COM_CODE) Com_TriggerIPDUSendWithMetaData(PduIdType PduId, P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) MetaData)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
# if (COM_DEV_ERROR_DETECT == STD_ON)
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
  }
/* Check plausibility of input parameters */
  else if(PduId >= Com_GetSizeOfTxPduInfo())
  {
    errorId = COM_E_PARAM;
  }
  else
#endif
  {
    const Com_TxPduInfoIterType txPduInfoIdx = (Com_TxPduInfoIterType) PduId;
# if (COM_DEV_ERROR_DETECT == STD_ON)
    if(Com_IsInvalidHndOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(!Com_IsTxBufferInfoMetaDataUsedOfTxPduInfo(txPduInfoIdx))
    {
      errorId = COM_E_PARAM;
    }
    else if(MetaData == NULL_PTR)
    {
      errorId = COM_E_PARAM_POINTER;
    }
    else
# endif
    { /* ----- Implementation ----------------------------------------------- */
      /* call the TriggerIPDUSendWithMetaData processing function */
      Com_Transmission_TriggerIpduSendWithMetaData(txPduInfoIdx, MetaData);
    }
  }
  Com_Reporting_CallDetReportError(COMServiceId_TriggerIPDUSendWithMetaData, errorId);
}

/**********************************************************************************************************************
Com_GetConfigurationId
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint32, COM_CODE) Com_GetConfigurationId(void)
{
/* ----- Local Variables ---------------------------------------------- */
  uint32 retVal;                /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
#if (COM_DEV_ERROR_DETECT == STD_ON )
/* Check if component is initialized */
  if(!Com_Initialization_GetInitialized())
  {
    errorId = COM_E_UNINIT;
    retVal = 0u;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Get current configuration Id */
    retVal = Com_GetConfigId();
  }
  Com_Reporting_CallDetReportError(COMServiceId_GetConfigurationId, errorId);
  return retVal;
}

/**********************************************************************************************************************
Com_GetVersionInfo
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, COM_APPL_VAR) versioninfo)
{
/* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = COM_E_NO_ERROR;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

/* ----- Development Error Checks ------------------------------------- */
/* Check plausibility of all input parameters */
# if (COM_DEV_ERROR_DETECT == STD_ON )
  if(versioninfo == NULL_PTR)
  {
    errorId = COM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* Write component data to versioninfo */
    versioninfo->vendorID = COM_VENDOR_ID;      /* SBSW_COM_PTR_API */
    versioninfo->moduleID = COM_MODULE_ID;      /* SBSW_COM_PTR_API */

    versioninfo->sw_major_version = COM_SW_MAJOR_VERSION;       /* SBSW_COM_PTR_API */
    versioninfo->sw_minor_version = COM_SW_MINOR_VERSION;       /* SBSW_COM_PTR_API */
    versioninfo->sw_patch_version = COM_SW_PATCH_VERSION;       /* SBSW_COM_PTR_API */
  }
  Com_Reporting_CallDetReportError(COMServiceId_GetVersionInfo, errorId);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:
    MD_Com_0314_0315_0316_0317: Misra Rule 11.5
     Reason:     A pointer is cast to a void pointer or a void pointer is cast to a object type pointer, because it depends on the configuration of which type the pointer is. It is easier to use a void pointer instead of different functions for different pointer types. At some point the void pointer may be cast back to the appropriate object type.
     Risk:       No risk, because the underlying pointer type is known via the configuration.
     Prevention: No prevention necessary.

   MD_Com_2982_2983: Misra Rule 2.2
     Reason:     It depends on the configuration if the value is modified.
     Risk:       No risk, because only warning that assignment is redundant.
     Prevention: No prevention necessary.

   MD_Com_2985_2986: Misra Rule 2.2
     Reason:     The value of the operands depends on the configuration.
     Risk:       No risk, because only warning that the operation is redundant.
     Prevention: No prevention necessary.

   MD_Com_2003:  Misra Rule 16.3
     Reason:     A break-statement is not required, the switch case needs to perform the remaining operations of the succeeding case.
     Risk:       No risk
     Prevention: No prevention necessary

  MD_Com_4394:  Misra Rule 10.8
     Reason:     The composite expression has to be cast as a whole because of the shift or bitwise operation. Shift and bitwise operations should only be performed with essentially unsigned types. The cast is necessary, because the result of the shift or bitwise operation is signed.
     Risk:       No risk
     Prevention: No prevention necessary

   MD_Com_UnityBuild:
     Reason:     The code is compiled as unity build where c files are used like h files.
     Risk:       No risk, because duplicate symbols are warned detected by the compiler.
     Prevention: No prevention necessary.
*/

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

  \ID COV_COM_MISRA
    \ACCEPT X
    \ACCEPT TX
    \REASON [COV_MSR_MISRA]
  
  \ID COV_COM_REPORT_ERROR
    \ACCEPT X
    \ACCEPT XX
    \REASON [COV_MSR_INV_STATE]

  \ID COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION
    \ACCEPT TX
    \REASON This is a safety check for optional indirections that checks if the indirection exists. If the call context already implicitly guarantees that the indirection exists
    (and the generated data is correct) this always evaluates true. The safety check is also used in combination with other checks which might lead to coverage gaps. These gaps can not
    be fully covered depending on the configuration. In combined-checks (else-if) some statements are not fully covered if the leading check prohibits the execution of the following if-statements.

  \ID COV_COM_CSL04_CORRECTARGET_CHECK
    \ACCEPT TX
    \ACCEPT TX tx tx
    \REASON This is a safety check for indirections with multiple targets that the correct target is accessed. If the call context already implicitly guarantees that the correct target is accessed,
     this always evaluates to true.

  \ID COV_COM_DEFERRED_MAINFUNCTION_RX
    \ACCEPT TX
    \ACCEPT TF tx tf
    \REASON MainFunction Rx Struct must always be generated for Rx I-Pdus using deferred processing.

  \ID COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI
    \ACCEPT TX
    \REASON The mainfunction route signals api is only available in gateway configurations.

  \ID COV_COM_RX_TP_BUFFER
    \ACCEPT TX
    \REASON Statement is always TRUE if available Rx Tp Buffer is always greater than the copied data.

  \ID COV_COM_TXPROCESS_ALWAYS_TRUE_FOR_CFG_WITH_CYCLIC_TXIPDUS
    \ACCEPT TX
    \REASON Statement will always be TRUE if cfg contains cyclic Tx I-PDUs

  \ID COV_COM_BUFFER_ALWAYS_USED_IF_TRIGGERED_ON_CHANGE_IS_USED
    \ACCEPT TF tf tx
    \REASON Statement will always be TRUE if the transmission property trigger on change is used. This is because for trigger on change to work, buffer needs to exist.

  \ID COV_COM_UBMASK_CHECK
    \ACCEPT TX
    \REASON The ub mask is always the same or smaller than the tx buffer, because the update mask contains the position of the update bits in the tx buffer.

  \ID COV_COM_CANCELLATION_SUPPORT_OFF
    \ACCEPT XF
    \REASON If COM_CANCEL_TRANSMIT_API is STD_OFF then the Cancellation Support is always false as well.

  \ID COV_COM_ECUM_BSW_ERROR_HOOK
    \ACCEPT TX
    \REASON COM_USE_ECUM_BSW_ERROR_HOOK is always STD_ON if COM_USE_INIT_POINTER is STD_ON, because both are generated as STD_ON if the config is postbuild loadable.

  \ID COV_COM_EVENT_CACHE_FLUSH
    \ACCEPT X
    \ACCEPT XX
    \REASON The function is used for safety purposes so that no cached PDUs are lost but the calling condition cannot be realized with the component-level interface.

COV_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_COM_OPTIONAL_INDIRECTION
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N The isUsedOf check ensures that the returned handle ID is correct. This is ensured by CSL03.

  \ID VCA_COM_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ARGUMENT
    \DESCRIPTION A function with pointer parameters is directly called, but the function is not
             defined within the analyzed sources. VCA is unable to determine the
             behavior of the function.
    \COUNTERMEASURE \N Arguments that contain var pointer are checked by review: Pointer type corresponds to function
                   parameter type. If the pointer argument is a struct, this struct does not contain a pointer element.

  \ID VCA_COM_CSL03
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N The combination of startIdx and endIdx from the ComStackLib ensures that the returned handle ID is correct. This is ensured by CSL01.

  \ID VCA_COM_VALID_FUNCTION_POINTER
    \DESCRIPTION    VCA warns that determining points-to set may fail.
    \COUNTERMEASURE \N Function pointer is checked against NULL before calling it.

  \ID VCA_COM_ENUM_INDEX_CSL04
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N The combination of enum check and index access is ensured by CSL04.

  \ID VCA_COM_CSL01
    \DESCRIPTION VCA warns that function is possibly called outside of its specification.
    \COUNTERMEASURE \N The CSL01 boundary check ensures that the ID is correct.

  \ID VCA_COM_MEMCPY_CSL_DEST
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s or VStdLib_MemCpy is called outside of its specification.
    \COUNTERMEASURE \N The pointer and the length of the destination are provided by the CSL. This is CSL03.

  \ID VCA_COM_BITCPY_CSL_DEST
    \DESCRIPTION    VCA warns that VStdLib_BitCpySawBigToBigEndian_s or VStdLib_BitCpySawLittleToLittleEndian_s is called outside of its specification.
    \COUNTERMEASURE \N The pointer and the length of the destination are provided by the CSL. This is CSL03.

  \ID VCA_COM_MEMCPY_BOUNDARY_CHECK
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s or VStdLib_MemCpy is called outside of its specification.
    \COUNTERMEASURE \N Buffer boundary is checked before copy operation.

  \ID VCA_COM_INDIRECTION_CSL02_CSL03
    \DESCRIPTION    VCA warns that array access might be out of bounds.
    \COUNTERMEASURE \N The validity of the index is ensured by CSL02 and CSL03 of ComStackLib.

  \ID VCA_COM_BIT_COPY
    \DESCRIPTION    VCA warns that array access might be out of bounds.
    \COUNTERMEASURE \N The VCA spec ensures a potential incorrect call to this API is shown as an error in the calling code. These errors will be justified in the calling code.

  \ID VCA_COM_CACHE_STRUCT
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \N The pointer and the index are checked before used for access.

  \ID VCA_COM_BOUNDARY_CHECK
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \R The boundary of the index is assessed by comparison with the configured ROM values. 

  \ID VCA_COM_SIGNAL_DATA_PTR
    \DESCRIPTION    VCA warns that function is called outside its specification.
    \COUNTERMEASURE \N The pointer is checked in the API pattern.
                    \S The SMI-1104 ensures that the passed pointer has the correct Signal Type.

  \ID VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY
    \DESCRIPTION    VCA warns that assertion may fail.
    \COUNTERMEASURE \N The pointer is handed over by the caller of Com and has to point to valid memory. The corresponding length has to match the pointers length. This has to be ensured by the caller.
                       VCA shall assume that this pointer points to external memory and the length is correct.

  \ID VCA_COM_VCA_MEMCPY_S
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \N The VCA spec ensures a potential incorrect call to this API is shown as an error in the calling code. These errors will be justified in the calling code.

  \ID VCA_COM_VCA_MEMCPY
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \N The VCA spec ensures a potential incorrect call to this API is shown as an error in the calling code. These errors will be justified in the calling code.

VCA_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  FINITE TIME EXECUTION JUSTIFICATIONS
 **********************************************************************************************************************/

/* FETA_JUSTIFICATION_BEGIN

    \ID FETA_COM_01
      \DESCRIPTION Loop over static number of elements.
      \COUNTERMEASURE \N The loop has a static upper limit determined by a ROM value. It will always terminate in finite time.

    \ID FETA_COM_02
      \DESCRIPTION Loop with external termination condition.
      \COUNTERMEASURE \R The loop uses a local loop counter with static upper limit to break the loop in case the external termination condition never breaks the loop.

    \ID FETA_COM_03
      \DESCRIPTION Loop over sizeof(datatype).
      \COUNTERMEASURE \N The loop has a static upper limit determined by a compile time value. It will always terminate in finite time.

FETA_JUSTIFICATION_END */

/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com.c
 *********************************************************************************************************************/
