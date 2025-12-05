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
/*!        \file  CddDrm_LowerLayer.c
 *        \brief  Drm source file
 *      \details  Implementation of Lower Layer CddDrm subcomponent
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

#define CDDDRM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "PduR_CddDrm.h"
#include "CddDrm.h"


/* critical sections handled by SchM */
#include "SchM_CddDrm.h"

#if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- TxPdu States ---------------------------------------------------------------------------------------------- */
#define CDDDRM_TXPDU_STATE_INACTIVE                                   (0U)      /*<! Connection inactive */
#define CDDDRM_TXPDU_STATE_COPYMESSAGE                                (1U)      /*<! Copy and forward message */

/* ----- Transmission States --------------------------------------------------------------------------------------- */
#define CDDDRM_TRANSMISSION_ONGOING                                   (0u)
#define CDDDRM_TRANSMISSION_CANCEL_RECEIVE                            (1U)
#define CDDDRM_TRANSMISSION_CANCEL_TRANSMIT                           (2U)
#define CDDDRM_TRANSMISSION_SUCCESS                                   (3U)
#define CDDDRM_TRANSMISSION_FAIL                                      (4U)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if(CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
typedef uint8 transmissionStateType;
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#if(CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
# define CDDDRM_START_SEC_VAR_ZERO_INIT_8BIT
# include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
CDDDRM_LOCAL VAR(uint8, CDDDRM_VAR_ZERO_INIT) CddDrm_LowerLayerModuleInitialized = CDDDRM_UNINIT;

# define CDDDRM_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "CddDrm_MemMap.h"                                                                                                                                  /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CDDDRM_START_SEC_CODE
#include "CddDrm_MemMap.h"                                                                                                                       /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if(CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
/**********************************************************************************************************************
 * CddDrm_LowerLayer_CopyMessage()
*********************************************************************************************************************/
/*! \brief      Copies and forwards the message to PduR/Dcm
 *  \details     This function is called to forward the Rx or Tx messages to PduR/DCM.
 *  \param[in]   forwardingHandle         [range: forwardingHandle < CDDDRM_CFG_LOWERLAYER_TXPDU_COUNT]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE for the same forwardingHandle, TRUE otherwise
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_LowerLayer_CopyMessage(CddDrm_SizeOfForwardingTableLLType forwardingHandle);

/**********************************************************************************************************************
 * CddDrm_HandleTransmissionResult()
*********************************************************************************************************************/
/*! \brief      Handles the different transmission states.
 *  \details     This function handles different transmission states for lower layer.
 *  \param[in]   forwardingHandle           [range: forwardingHandle < CDDDRM_CFG_LOWERLAYER_TXPDU_COUNT]
 *  \param[in]   TransmissionState The transmission state of the message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_HandleTransmissionResult(CddDrm_SizeOfForwardingTableLLType forwardingHandle, transmissionStateType transmissionState);

/**********************************************************************************************************************
 * CddDrm_DetermineSduLength()
*********************************************************************************************************************/
/*! \brief      Returns the Sdu length based on buffer sizes and message length.
 *  \details     This function fetches the length of message to be forwarded.
 *  \param[in]   forwardingHandle  Handle for the index in Forwarding table
 *  \param[in]   bufferSize        Buffer size of the transmission data
 *  \pre         [range: forwardingHandle < CDDDRM_CFG_LOWERLAYER_TXPDU_COUNT]
 *  \context     TASK
 *  \reentrant   TRUE
*********************************************************************************************************************/
CDDDRM_LOCAL FUNC(PduLengthType, CDDDRM_CODE) CddDrm_DetermineSduLength(CddDrm_SizeOfForwardingTableLLType forwardingHandle, PduLengthType bufferSize);


/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  CddDrm_DetermineSduLength()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CDDDRM_LOCAL FUNC(PduLengthType, CDDDRM_CODE) CddDrm_DetermineSduLength(CddDrm_SizeOfForwardingTableLLType forwardingHandle, PduLengthType bufferSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType retValue;

  /* ----- Implementation ----------------------------------------------- */
  /*#10 Determining the maximum SduLength based on internal buffer size, upper layer buffer size and remaining message length. */
  if ((bufferSize > CDDDRM_CFG_INTERNAL_BUFFER_SIZE) && (CddDrm_GetRemainingMessageLengthOfTransmissionData(forwardingHandle) > CDDDRM_CFG_INTERNAL_BUFFER_SIZE))
  {
    retValue = CDDDRM_CFG_INTERNAL_BUFFER_SIZE;
  }
  else if (CddDrm_GetRemainingMessageLengthOfTransmissionData(forwardingHandle) > bufferSize)
  {
    retValue = bufferSize;
  }
  else
  {
    retValue = CddDrm_GetRemainingMessageLengthOfTransmissionData(forwardingHandle);
  }
  return retValue;
}


/**********************************************************************************************************************
 *  CddDrm_LowerLayer_CopyMessage()
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
 */
CDDDRM_LOCAL_INLINE FUNC(void, CDDDRM_CODE) CddDrm_LowerLayer_CopyMessage(CddDrm_SizeOfForwardingTableLLType forwardingHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType lBufferStatus;
  PduLengthType lBufferSize;
  PduInfoType lPduInfo;
  PduLengthType lAvailableData = 0;
  uint8 lInternalBuffer[CDDDRM_CFG_INTERNAL_BUFFER_SIZE];
  transmissionStateType lTransmissionState = CDDDRM_TRANSMISSION_FAIL;

  /* ----- Implementation ----------------------------------------------- */
  lBufferSize = CddDrm_GetAvilableBufferSizeOfTransmissionData(forwardingHandle);

  /* #110 Determine the SduLength based on buffersizes. */
  lPduInfo.SduLength = CddDrm_DetermineSduLength(forwardingHandle, lBufferSize);

  /* #120 Copy data from PduR into the internal buffer. */
  lPduInfo.SduDataPtr = lInternalBuffer;

  lBufferStatus = PduR_CddDrmCopyTxData(CddDrm_Cfg_GetTxPduRDestIdByTxPdu(forwardingHandle),                                                         /* VCA_CDDDRM_PDURCOPYTXDATA_CALL */
                                        &lPduInfo,
                                        NULL_PTR,
                                        &lAvailableData);

  /* #130 If copying to internal buffer was successful. */
  if (lBufferStatus == BUFREQ_OK)
  {
    /* #131 Copy data from internal buffer to Upper Layer. */
    lBufferStatus = PduR_CddDrmCopyRxData(CddDrm_Cfg_GetRxPduRSrcIdByTxPdu(forwardingHandle),                                                        /* VCA_CDDDRM_PDURCOPYRXDATA_CALL */
                                          &lPduInfo,
                                          &lBufferSize);

    /* #132 If copying from internal buffer to UL is successful then verify if remaining message length is 0, \
            set transmission state to CDDDRM_TRANSMISSION_SUCCESS. */
    if (lBufferStatus == BUFREQ_OK)
    {
      CddDrm_SetAvilableBufferSizeOfTransmissionData(forwardingHandle, lBufferSize);
      CddDrm_SetRemainingMessageLengthOfTransmissionData(forwardingHandle, (CddDrm_GetRemainingMessageLengthOfTransmissionData(forwardingHandle) - lPduInfo.SduLength));

      if (CddDrm_GetRemainingMessageLengthOfTransmissionData(forwardingHandle) == 0u)
      {
        lTransmissionState = CDDDRM_TRANSMISSION_SUCCESS;
      }
      else if (lBufferSize == 0U)
      {
        /* No receiving buffer available, i.e. transmissionStateType lTransmissionState = CDDDRM_TRANSMISSION_FAIL */
      }
      else
      {
        lTransmissionState = CDDDRM_TRANSMISSION_ONGOING;
      }
    }
  }
  /* #140 Otherwise, if BUFREQ_E_BUSY is returned, set transmission state to CDDDRM_TRANSMISSION_ONGOING. */
  else if (lBufferStatus == BUFREQ_E_BUSY)
  {
    lTransmissionState = CDDDRM_TRANSMISSION_ONGOING;
  }
  /* #150 Otherwise, transmission failed. */
  else
  {
    /* Transmission failed.*/
  }

  if (lTransmissionState != CDDDRM_TRANSMISSION_ONGOING)
  {
    CddDrm_SetTxPduStateOfTransmissionData(forwardingHandle, CDDDRM_TXPDU_STATE_INACTIVE);
  }

  /* #200 Handle the transmission result. */
  CddDrm_HandleTransmissionResult(forwardingHandle, lTransmissionState);

} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CddDrm_HandleTransmissionResult()
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
 */
CDDDRM_LOCAL FUNC(void, CDDDRM_CODE) CddDrm_HandleTransmissionResult(CddDrm_SizeOfForwardingTableLLType forwardingHandle, transmissionStateType transmissionState)
{

  /* ----- Implementation ----------------------------------------------- */
  if (transmissionState == CDDDRM_TRANSMISSION_SUCCESS)
  {
    PduR_CddDrmRxIndication(CddDrm_Cfg_GetRxPduRSrcIdByTxPdu(forwardingHandle), E_OK);
    PduR_CddDrmTxConfirmation(CddDrm_Cfg_GetTxPduRDestIdByTxPdu(forwardingHandle), E_OK);
  }
  else if (transmissionState == CDDDRM_TRANSMISSION_FAIL)
  {
    PduR_CddDrmRxIndication(CddDrm_Cfg_GetRxPduRSrcIdByTxPdu(forwardingHandle), E_NOT_OK);
    PduR_CddDrmTxConfirmation(CddDrm_Cfg_GetTxPduRDestIdByTxPdu(forwardingHandle), E_NOT_OK);
  }
# if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
  else if (transmissionState == CDDDRM_TRANSMISSION_CANCEL_RECEIVE)
  {
    PduR_CddDrmTxConfirmation(CddDrm_Cfg_GetTxPduRDestIdByTxPdu(forwardingHandle), E_NOT_OK);
  }
# endif
# if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
  else if (transmissionState == CDDDRM_TRANSMISSION_CANCEL_TRANSMIT)
  {
    PduR_CddDrmRxIndication(CddDrm_Cfg_GetRxPduRSrcIdByTxPdu(forwardingHandle), E_NOT_OK);
  }
# endif
  else
  {
    /* Handles case: ONGOING */
  }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  CddDrm_InitLowerLayerMemory()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_InitLowerLayerMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  CddDrm_LowerLayerModuleInitialized = (uint8)CDDDRM_UNINIT;
# endif
} /* CddDrm_InitLowerLayerMemory() */

/***********************************************************************************************************************
 *  CddDrm_MainFunctionLowerLayer()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_MainFunctionLowerLayer(void)
{
  /*---- - Local Variables---------------------------------------------- */
  CddDrm_TransmissionDataIterType i;
  /* ----- Implementation ----------------------------------------------- */
  /* Iterate over all the configured Tx Pdu Ids */
  for (i = 0; i< CddDrm_GetSizeOfTransmissionData(); i++)
  {
    /* Process transmission if state is CDDDRM_TXPDU_STATE_COPYMESSAGE */
    if (CddDrm_GetTxPduStateOfTransmissionData(i) == CDDDRM_TXPDU_STATE_COPYMESSAGE)
    {
      transmissionStateType lTransmissionState = CDDDRM_TRANSMISSION_ONGOING;

# if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
      if (CddDrm_IsCancelTransmitFlagOfTransmissionData(i))
      {
        lTransmissionState = CDDDRM_TRANSMISSION_CANCEL_TRANSMIT;
        CddDrm_SetTxPduStateOfTransmissionData(i, CDDDRM_TXPDU_STATE_INACTIVE);
      }
# endif
# if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
      if (CddDrm_IsCancelReceiveFlagOfTransmissionData(i))
      {
        lTransmissionState = CDDDRM_TRANSMISSION_CANCEL_RECEIVE;
        CddDrm_SetTxPduStateOfTransmissionData(i, CDDDRM_TXPDU_STATE_INACTIVE);
      }
# endif
      if (lTransmissionState == CDDDRM_TRANSMISSION_ONGOING)                                                                                         /* PRQA S 2991, 2995 */ /* MD_CDDDRM_Rule14.3_2991 */
      {
        CddDrm_LowerLayer_CopyMessage((CddDrm_SizeOfForwardingTableLLType)i);
      }
      else
      {
        CddDrm_HandleTransmissionResult((CddDrm_SizeOfForwardingTableLLType)i, lTransmissionState);                                                                        /* PRQA S 2880 */ /* MD_CDDDRM_Rule2.1_2880 */
      }
    }
  }
} /* CddDrm_MainFunctionLowerLayer() */                                                                                                        /* PRQA S 6050 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  CddDrm_InitLowerLayer()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, CDDDRM_CODE) CddDrm_InitLowerLayer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CddDrm_TransmissionDataIterType lInitIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize transmission data to default values. */
  for (lInitIndex = 0; lInitIndex < CddDrm_GetSizeOfTransmissionData(); lInitIndex++)
  {
    CddDrm_SetAvilableBufferSizeOfTransmissionData(lInitIndex, 0);

    CddDrm_SetTxPduStateOfTransmissionData(lInitIndex, CDDDRM_TXPDU_STATE_INACTIVE);

    CddDrm_SetMessageLengthOfTransmissionData(lInitIndex, 0);

    CddDrm_SetRemainingMessageLengthOfTransmissionData(lInitIndex, 0);
# if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
    CddDrm_SetCancelReceiveFlagOfTransmissionData(lInitIndex, FALSE);
# endif
# if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
    CddDrm_SetCancelTransmitFlagOfTransmissionData(lInitIndex, FALSE);
# endif
  }

# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  CddDrm_LowerLayerModuleInitialized = (uint8)CDDDRM_INIT;
# endif


} /* CddDrm_InitLowerLayer() */

/**********************************************************************************************************************
 *  CddDrm_Transmit()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_Transmit(PduIdType txPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) pduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lResult = E_NOT_OK;
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  BufReq_ReturnType lULBufferStatus;
  PduLengthType lBufferSize = 0;
  /* ----- Development Error Checks ------------------------------------- */
# if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  /*#10 Check plausibility of input parameters.*/
  if (CddDrm_LowerLayerModuleInitialized != (uint8)CDDDRM_INIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
  }
  else if (txPduId >= CDDDRM_CFG_LOWERLAYER_TXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else if (pduInfoPtr == NULL_PTR)
  {
    lErrorId = CDDDRM_E_PARAM_POINTER;
  }
  else if (pduInfoPtr->SduLength == 0U)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Call PduR to forward the transmission. */
    lULBufferStatus = PduR_CddDrmStartOfReception(CddDrm_Cfg_GetRxPduRSrcIdByTxPdu(txPduId),                                                         /* VCA_CDDDRM_PDURSTARTOFRECEPTION_CALL */
                                                  NULL_PTR,
                                                  pduInfoPtr->SduLength,
                                                  &lBufferSize);

    /* #30 Check if the reception was successfully initiated. */
    if ((lULBufferStatus == BUFREQ_OK) && (lBufferSize > 0u))
    {
      /* #31 Set values of the TxPduStruct */
      CddDrm_SetMessageLengthOfTransmissionData(txPduId, pduInfoPtr->SduLength);
      CddDrm_SetRemainingMessageLengthOfTransmissionData(txPduId, pduInfoPtr->SduLength);
      CddDrm_SetAvilableBufferSizeOfTransmissionData(txPduId, lBufferSize);
      /* #32 Set transmission state to CDDDRM_TXPDU_STATE_COPYMESSAGE. */
      CddDrm_SetTxPduStateOfTransmissionData(txPduId, CDDDRM_TXPDU_STATE_COPYMESSAGE);

# if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
      CddDrm_SetCancelReceiveFlagOfTransmissionData(txPduId, FALSE);
# endif
# if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
      CddDrm_SetCancelTransmitFlagOfTransmissionData(txPduId, FALSE);
# endif

      lResult = E_OK;
    }
    else
    {
      /* #40 Otherwise, Do nothing */
    }

  }

  /* ----- Development Error Report --------------------------------------- */
# if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_TRANSMIT, lErrorId);
  }
# else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif

  return lResult;
}

# if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_CancelTransmit()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelTransmit(PduIdType txPduId)
{
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  Std_ReturnType lResult = E_OK;
  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check plausibility of input parameters. */
#  if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_LowerLayerModuleInitialized != (uint8)CDDDRM_INIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
    lResult = E_NOT_OK;
  }
  else if (txPduId >= CDDDRM_CFG_LOWERLAYER_TXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
    lResult = E_NOT_OK;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set CancelTransmit flag. */
    CddDrm_SetCancelTransmitFlagOfTransmissionData(txPduId, TRUE);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #30 Otherwise, return development error. */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANCELTRANSMIT, lErrorId);
  }
#  else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif

  return lResult;

} /* CddDrm_CancelTransmit() */
# endif

# if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_CancelReceive()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelReceive(PduIdType rxPduId)
{
  uint8 lErrorId = CDDDRM_E_NO_ERROR;
  Std_ReturnType lResult = E_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check plausibility of input parameters. */
#  if (CDDDRM_DEV_ERROR_DETECT == STD_ON)
  if (CddDrm_LowerLayerModuleInitialized != (uint8)CDDDRM_INIT)
  {
    lErrorId = CDDDRM_E_UNINIT;
    lResult = E_NOT_OK;
  }
  else if (rxPduId >= CDDDRM_CFG_LOWERLAYER_RXPDU_COUNT)
  {
    lErrorId = CDDDRM_E_PARAM_VALUE;
    lResult = E_NOT_OK;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set CancelReceive flag. */
    CddDrm_SetCancelReceiveFlagOfTransmissionData(CddDrm_GetTxDrmIdOfRxPduRefToTxPduRefLL(rxPduId), TRUE);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CDDDRM_DEV_ERROR_REPORT == STD_ON)
  if (lErrorId != CDDDRM_E_NO_ERROR)
  {
    /* #30 Otherwise, return development error. */
    (void)Det_ReportError(CDDDRM_MODULE_ID, CDDDRM_INSTANCE_ID_DET, CDDDRM_SID_CANCELRECEIVE, lErrorId);
  }
#  else
  CDDDRM_DUMMY_STATEMENT(lErrorId);                                                                                                                  /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif

  return lResult;
} /* CddDrm_CancelReceive() */
# endif

#endif

#define CDDDRM_STOP_SEC_CODE
#include "CddDrm_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  END OF FILE: CddDrm_LowerLayer.c
 *********************************************************************************************************************/
