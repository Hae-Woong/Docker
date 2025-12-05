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
/*!        \file  LdCom.c
 *        \unit   LdCom_Generic
 *        \brief  MICROSAR Efficient COM for Large Data
 *
 *      \details  Source
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

#define LDCOM_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

/**********************************************************************************************************************
*  INCLUDES
**********************************************************************************************************************/
#include "LdCom.h"
#include "LdCom_Cbk.h"

#include "LdCom_Cfg_Generic.h"

#include "LdCom_Reporting.h"
#if(LDCOM_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of LdCom header file */
#if ((LDCOM_SW_MAJOR_VERSION != 5u) || (LDCOM_SW_MINOR_VERSION != 0u) || (LDCOM_SW_PATCH_VERSION != 0u))
# error "Vendor specific version numbers of LdCom.c and LdCom.h are inconsistent"
#endif

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
**********************************************************************************************************************/
#define LDCOM_START_SEC_CODE
#include "LdCom_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LdCom_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_Init(P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_PBCFG) config)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if(LDCOM_USE_INIT_POINTER == STD_ON)
  LdCom_ConfigDataPtr = config;
  /* Check validity of config pointer */
  if(LdCom_ConfigDataPtr == NULL_PTR)
  {
    LdCom_Reporting_CallBswErrorHook(ECUM_BSWERROR_NULLPTR);
    errorId = LDCOM_E_PARAM_POINTER;
  }
  else if(LdCom_HasFinalMagicNumber() && (LdCom_GetFinalMagicNumber() != LDCOM_FINAL_MAGIC_NUMBER))
  {
    LdCom_Reporting_CallBswErrorHook(ECUM_BSWERROR_MAGICNUMBER);
    errorId = LDCOM_E_PARAM_POINTER;
    LdCom_ConfigDataPtr = NULL_PTR;
  }
  else
#else
# if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  if(config != NULL_PTR)
  {
    errorId = LDCOM_E_PARAM_POINTER;
  }
  else
# endif
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    LdCom_SetInitialized(TRUE);
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_INIT, errorId);
  LDCOM_DUMMY_STATEMENT(config);      /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 *  LdCom_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_InitMemory(void)
{
  LdCom_SetInitialized(FALSE);
#if(LDCOM_USE_INIT_POINTER == STD_ON)
  LdCom_ConfigDataPtr = NULL_PTR; /* VCA_LDCOM_CONFIGPOINTER_MODIFIEDAFTERINIT */
#endif
}

/**********************************************************************************************************************
 *  LdCom_DeInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (LDCOM_USE_INIT_POINTER == STD_ON)
    LdCom_ConfigDataPtr = NULL_PTR; /* VCA_LDCOM_CONFIGPOINTER_MODIFIEDAFTERINIT */
#endif
    LdCom_SetInitialized(FALSE);
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_DE_INIT, errorId);
}

/**********************************************************************************************************************
 *  LdCom_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, LDCOM_CODE) LdCom_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  if(versioninfo == NULL_PTR)
  {
    errorId = LDCOM_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = LDCOM_VENDOR_ID;
    versioninfo->moduleID = LDCOM_MODULE_ID;

    versioninfo->sw_major_version = LDCOM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = LDCOM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = LDCOM_SW_PATCH_VERSION;
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_GET_VERSION_INFO, errorId);
}

/**********************************************************************************************************************
 * LdCom_Transmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, LDCOM_CODE) LdCom_Transmit(PduIdType Id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) Id;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'Id' */
  else if(Id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_SIGNAL_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_SIGNAL_ID;
  }
  /* Check if the transmitted Pdu/signal is a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_SIGNAL_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = PduR_LdComTransmit(LdCom_GetPduRTxHandleIdOfRxTxPduInfo(rxTxPduInfoIdx), PduInfoPtr);
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_TRANSMIT, errorId);
  return retVal;
}

/**********************************************************************************************************************
 * LdCom_RxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LDCOM_CODE) LdCom_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) RxPduId;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'RxPduId' */
  else if(RxPduId >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an If Pdu */
  else if(!LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsIfRxIndicationFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))  
    {
      LdCom_GetIfRxIndicationFctPtr(LdCom_GetIfRxIndicationFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (PduInfoPtr);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_RX_INDICATION, errorId);
}

/**********************************************************************************************************************
 * LdCom_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LDCOM_CODE) LdCom_TxConfirmation(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) TxPduId;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'TxPduId' */
  else if(TxPduId >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an If Pdu */
  else if(!LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsIfTxConfirmationFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      LdCom_GetIfTxConfirmationFctPtr(LdCom_GetIfTxConfirmationFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) ();
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_TX_CONFIRMATION, errorId);
}

/**********************************************************************************************************************
 * LdCom_TriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, LDCOM_CODE) LdCom_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) TxPduId;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'TxPduId' */
  else if(TxPduId >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an If Pdu */
  else if(!LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsIfTxTriggerTransmitFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      retVal = LdCom_GetIfTxTriggerTransmitFctPtr(LdCom_GetIfTxTriggerTransmitFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (PduInfoPtr);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_TRIGGER_TRANSMIT, errorId);
  return retVal;
}

/**********************************************************************************************************************
 * LdCom_StartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_StartOfReception(PduIdType id,
                                                           P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
                                                           PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) id;

/* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsTpStartOfReceptionFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      retVal = LdCom_GetTpStartOfReceptionFctPtr(LdCom_GetTpStartOfReceptionFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (info, TpSduLength, bufferSizePtr);
    }
  }
/* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_START_OF_RECEPTION, errorId);
  return retVal;
}

/**********************************************************************************************************************
 * LdCom_CopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyRxData(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) id;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsTpCopyRxDataFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      retVal = LdCom_GetTpCopyRxDataFctPtr(LdCom_GetTpCopyRxDataFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (info, bufferSizePtr);
    }
  }
/* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_COPY_RX_DATA, errorId);
  return retVal;
}

/**********************************************************************************************************************
 * LdCom_TpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LDCOM_CODE) LdCom_TpRxIndication(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) id;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Rx Pdu */
  else if(LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsTpRxIndicationFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      LdCom_GetTpRxIndicationFctPtr(LdCom_GetTpRxIndicationFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (result);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_TP_RX_INDICATION, errorId);
}

/**********************************************************************************************************************
 * CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyTxData(PduIdType id,
                                                     P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,
                                                     P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_VAR) retry, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_VAR) availableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) id;

/* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsTpCopyTxDataFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      retVal = LdCom_GetTpCopyTxDataFctPtr(LdCom_GetTpCopyTxDataFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (info, retry, availableDataPtr);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_COPY_TX_DATA, errorId);
  return retVal;
}  

/**********************************************************************************************************************
 * LdCom_TpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, LDCOM_CODE) LdCom_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = LDCOM_E_NO_ERROR;
  const LdCom_RxTxPduInfoIterType rxTxPduInfoIdx = (LdCom_RxTxPduInfoIterType) id;

  /* ----- Development Error Checks ------------------------------------- */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* Check initialization state of the component */
  if(!LdCom_IsInitialized())
  {
    errorId = LDCOM_E_UNINIT;
  }
  /* Check parameter 'id' */
  else if(id >= LdCom_GetSizeOfRxTxPduInfo())
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else if(LdCom_IsInvalidHndOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to a Tx Pdu */
  else if(!LdCom_IsTxPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  /* Check if the handle Id refers to an Tp Pdu */
  else if(LdCom_IsIfPduOfRxTxPduInfo(rxTxPduInfoIdx))
  {
    errorId = LDCOM_E_INVALID_PDU_SDU_ID;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    if(LdCom_IsTpTxConfirmationFctPtrUsedOfRxTxPduInfo(rxTxPduInfoIdx))
    {
      LdCom_GetTpTxConfirmationFctPtr(LdCom_GetTpTxConfirmationFctPtrIdxOfRxTxPduInfo(rxTxPduInfoIdx)) (result);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  LdCom_Reporting_CallDetReportError(LDCOM_SID_TP_TX_CONFIRMATION, errorId);
}

#define LDCOM_STOP_SEC_CODE
#include "LdCom_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* Justifications for module-specific MISRA deviations:
   MD_LdCom_1336: Misra Rule 8.2
     Reason:      No identifier name needed in a function pointer typedef.
     Risk:        No risk, because only warning that with a identifier name the readability is better.
     Prevention:  No prevention necessary.
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

  \ID COV_LDCOM_ECUM_BSW_ERROR_HOOK
    \ACCEPT TX
    \REASON LDCOM_USE_ECUM_BSW_ERROR_HOOK is always STD_ON if LDCOM_USE_INIT_POINTER is STD_ON, because both are generated as STD_ON if the config is postbuild loadable.

END_COVERAGE_JUSTIFICATION */

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_LDCOM_CALLBSWERRORHOOK_UNREFERENCED
    \DESCRIPTION    VCA warns that LdCom_Reporting_CallBswErrorHook is never referenced [RCH-21].
    \COUNTERMEASURE \R false positive by VCA (see TAR-42423).

  \ID VCA_LDCOM_CONFIGPOINTER_MODIFIEDAFTERINIT
    \DESCRIPTION    VCA warns that the write access to config pointer after initialization is illegal [SLC-60].
    \COUNTERMEASURE \R false positive by VCA (see MOLE-1939). 

VCA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: LdCom.c
 *********************************************************************************************************************/
