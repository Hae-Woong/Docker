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
/*!        \file  DoIPInt.c
 *        \brief  DoIPInt source file
 *      \details  Implementation for component DoIPInt.
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

#define DOIPINT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Cbk.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_Connection.h"
#include "DoIPInt_Tx.h"
#include "DoIPInt_Rx.h"
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
# include "DoIPInt_GenHdr.h"
# include "DoIPInt_DiagMsg.h"
# include "DoIPInt_OemSpecific.h"
#else
# include "DoIPInt_LightHdr.h"
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
#include "SoAd_Types.h"

#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* Check the version of DoIPInt header file. */
#if (  (DOIPINT_SW_MAJOR_VERSION != (7u)) \
    || (DOIPINT_SW_MINOR_VERSION != (0u)) \
    || (DOIPINT_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of DoIPInt.c and DoIPInt.h are inconsistent"
#endif /*    (DOIPINT_SW_MAJOR_VERSION != (7u))
        * || (DOIPINT_SW_MINOR_VERSION != (0u))
        * || (DOIPINT_SW_PATCH_VERSION != (0u)) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/* ISO header. */
#  define DOIPINT_HDR_LEN                               DOIPINT_DIAG_HDR_LEN        /*!< Length of header. */
# else
/* LIGHT header. */
#  define DOIPINT_HDR_LEN                               DOIPINT_LIGHT_HDR_LEN       /*!< Length of header. */
# endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

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
#define DOIPINT_START_SEC_VAR_CLEARED_8
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module. */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
DOIPINT_LOCAL VAR(uint8, DOIPINT_VAR_CLEARED) DoIPInt_State = DOIPINT_STATE_UNINIT;
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */

#define DOIPINT_STOP_SEC_VAR_CLEARED_8
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define DOIPINT_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the current configuration. */
P2CONST(DoIPInt_ConfigType, DOIPINT_VAR_CLEARED, DOIPINT_PBCFG) DoIPInt_ConfigDataPtr = NULL_PTR;

#define DOIPINT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DOIPINT_START_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_InitMemory()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set module state to uninitialized (if error detection is enabled). */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  DoIPInt_State = DOIPINT_STATE_UNINIT;
#else
  /* This dummy statement is used to avoid PClint warnings when entering/leaving critical sections. */
  DOIPINT_DUMMY_STATEMENT(DoIPInt_ConfigDataPtr); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
} /* DoIPInt_InitMemory() */

/**********************************************************************************************************************
 *  DoIPInt_Init()
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
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Init(
  P2CONST(DoIPInt_ConfigType, AUTOMATIC, DOIPINT_PBCFG) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ConnectionIterType  connectionIter;
  DoIPInt_ChannelIterType     channelIter;
  uint8                       errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameter (if error detection is enabled). */
  if ( ConfigPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_CONFIG;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize configuration pointer. */
    DoIPInt_ConfigDataPtr = ConfigPtr;

    /* #30 Iterate over all connections. */
    for ( connectionIter = 0u; connectionIter < DoIPInt_GetSizeOfConnection(); connectionIter++ )
    {
      /* #40 Initialize connection in sub-module DoIPInt_Connection. */
      DoIPInt_Connection_Init(connectionIter);

      /* #50 Initialize connection in sub-module DoIPInt_Tx. */
      DoIPInt_Tx_Init(connectionIter);

      /* #60 Initialize connection in sub-module DoIPInt_Rx. */
      DoIPInt_Rx_Init(connectionIter);

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      /* #70 Initialize connection in sub-module DoIPInt_GenHdr. */
      DoIPInt_GenHdr_Init(connectionIter);

      /* #80 Initialize connection in sub-module DoIPInt_DiagMsg. */
      DoIPInt_DiagMsg_Init(connectionIter);

      /* #90 Initialize connection in sub-module DoIPInt_OemSpecific. */
      DoIPInt_OemSpecific_Init(connectionIter);
#else
      /* #100 Initialize connection in sub-module DoIPInt_LightHdr. */
      DoIPInt_LightHdr_Init(connectionIter);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    }

    /* #110 Iterate over all channels and initialize logical addresses with their configured values. */
    for ( channelIter = 0u; channelIter < DoIPInt_GetSizeOfChannel(); channelIter++ )
    {
      DoIPInt_SetLogicalRemoteAddressOfChannelDyn(channelIter, DoIPInt_GetLogicalRemoteAddressOfChannel(channelIter));
      DoIPInt_SetLogicalLocalAddressOfChannelDyn(channelIter, DoIPInt_GetLogicalLocalAddressOfChannel(channelIter));
    }

#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
    /* #120 Set module state to initialized (if error detection is enabled). */
    DoIPInt_State = DOIPINT_STATE_INIT;
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #130 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_INIT, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
} /* DoIPInt_Init() */

/**********************************************************************************************************************
 *  DoIPInt_MainFunction()
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
FUNC(void, DOIPINT_CODE) DoIPInt_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Process any pending closing and opening of connections. */
    DoIPInt_Connection_ProcessState();

    /* #30 Process any pending transmission requests. */
    DoIPInt_Tx_ProcessTransmission();

    /* #40 Process any pending reception retries. */
    DoIPInt_Rx_ProcessRetryTimeout();

    /* #50 Process any pending message reception. */
    DoIPInt_Rx_ContinueCopyRxData();
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_MAIN_FUNCTION, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */
} /* DoIPInt_MainFunction() */

/**********************************************************************************************************************
 *  DoIPInt_TpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TpTransmit(
  PduIdType DoIPIntPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) DoIPIntPduRTxInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntPduRTxId >= DoIPInt_GetSizeOfPduRTxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPIntPduRTxInfoPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( DoIPIntPduRTxInfoPtr->SduLength == 0u )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPIntPduRTxInfoPtr->SduLength > (DoIPInt_GetMaxPduLength() - DOIPINT_HDR_LEN) )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPInt_GetApiTypeOfChannel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId)) !=
    DOIPINT_API_TYPE_TP )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Try to transmit TP-PDU. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    retVal = DoIPInt_DiagMsg_TxTpTransmit(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId),
      DoIPIntPduRTxInfoPtr->SduLength);
#else
    retVal = DoIPInt_LightHdr_TxTpTransmit(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId),
      DoIPIntPduRTxInfoPtr->SduLength);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_TRANSMIT, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_TpTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_TpCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TpCancelTransmit(
  PduIdType DoIPIntPduRTxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameter (if error detection is enabled). */
  else if ( DoIPIntPduRTxId >= DoIPInt_GetSizeOfPduRTxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPInt_GetApiTypeOfChannel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId)) !=
    DOIPINT_API_TYPE_TP )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Try to cancel transmission of TP-PDU. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    retVal = DoIPInt_DiagMsg_TxCancel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId));
#else
    retVal = DoIPInt_LightHdr_TxCancel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId));
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_CANCEL_TRANSMIT, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_TpCancelTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_TpCancelReceive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TpCancelReceive(
  PduIdType DoIPIntPduRRxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_NOT_OK;
  uint8           errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameter (if error detection is enabled). */
  else if ( DoIPIntPduRRxId >= DoIPInt_GetSizeOfPduRRxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Try to cancel reception of TP-PDU. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    retVal = DoIPInt_DiagMsg_RxCancel(DoIPInt_GetChannelIdxOfPduRRxPduIdMap(DoIPIntPduRRxId));
#else
    retVal = DoIPInt_LightHdr_RxCancel(DoIPInt_GetChannelIdxOfPduRRxPduIdMap(DoIPIntPduRRxId));
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_CANCEL_RECEIVE, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_TpCancelReceive() */

/**********************************************************************************************************************
 *  DoIPInt_IfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_IfTransmit(
  PduIdType DoIPIntPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) DoIPIntPduRTxInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = DOIPINT_E_NO_ERROR;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State != DOIPINT_STATE_INIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntPduRTxId >= DoIPInt_GetSizeOfPduRTxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPIntPduRTxInfoPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( DoIPIntPduRTxInfoPtr->SduDataPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( DoIPIntPduRTxInfoPtr->SduLength == 0u )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPInt_GetApiTypeOfChannel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId)) !=
    DOIPINT_API_TYPE_IF )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Try to transmit IF-PDU. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    retVal = DoIPInt_DiagMsg_TxIfTransmit(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId),
      DoIPIntPduRTxInfoPtr);
#else
    retVal = DoIPInt_LightHdr_TxIfTransmit(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId),
      DoIPIntPduRTxInfoPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_IF_TRANSMIT, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_IfTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_IfCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_IfCancelTransmit(
  PduIdType DoIPIntPduRTxId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = DOIPINT_E_NO_ERROR;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State != DOIPINT_STATE_INIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameter (if error detection is enabled). */
  else if ( DoIPIntPduRTxId >= DoIPInt_GetSizeOfPduRTxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPInt_GetApiTypeOfChannel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId)) !=
    DOIPINT_API_TYPE_IF )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Try to cancel transmission of IF-PDU. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    retVal = DoIPInt_DiagMsg_TxCancel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId));
#else
    retVal = DoIPInt_LightHdr_TxCancel(DoIPInt_GetChannelIdxOfPduRTxPduIdMap(DoIPIntPduRTxId));
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_IF_CANCEL_TRANSMIT, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_IfCancelTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_TransmitOemSpecificPayloadType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TransmitOemSpecificPayloadType(
  DoIPInt_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PayloadDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           errorId = DOIPINT_E_NO_ERROR;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State != DOIPINT_STATE_INIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameter (if error detection is enabled). */
  else if ( ConnectionId >= DoIPInt_GetSizeOfConnectionIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( PayloadDataPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( (PayloadDataPtr->SduDataPtr == NULL_PTR) && (PayloadDataPtr->SduLength != 0u) )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Try to transmit PDU. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    retVal = DoIPInt_OemSpecific_TxTransmit(DoIPInt_GetConnectionIdxOfConnectionIdMap(ConnectionId), PayloadType,
      PayloadDataPtr);
#else
    retVal = E_NOT_OK;
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_OEM_PAYLOAD_TRANSMIT, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
  DOIPINT_DUMMY_STATEMENT(PayloadType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

  return retVal;
} /* DoIPInt_TransmitOemSpecificPayloadType() */

#if ( DOIPINT_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  DoIPInt_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIPINT_VAR) Versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check plausibility of input parameter (if error detection is enabled). */
  if ( Versioninfo == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else
# endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set 'versioninfo' with corresponding macros from component header. */
    Versioninfo->vendorID = (DOIPINT_VENDOR_ID);
    Versioninfo->moduleID = (DOIPINT_MODULE_ID);
    Versioninfo->sw_major_version = (DOIPINT_SW_MAJOR_VERSION);
    Versioninfo->sw_minor_version = (DOIPINT_SW_MINOR_VERSION);
    Versioninfo->sw_patch_version = (DOIPINT_SW_PATCH_VERSION);
  }

  /* ----- Development Error Report --------------------------------------- */
# if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #30 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_GET_VERSION_INFO, errorId);
  }
# else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */
} /* DoIPInt_GetVersionInfo() */
#endif /* DOIPINT_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  DoIPInt_OverwriteLogicalAddresses()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_OverwriteLogicalAddresses(
  DoIPInt_ChannelIdType ChannelId,
  uint16 LogicalLocalAddress,
  uint16 LogicalRemoteAddress)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfChannelType channelIdx;
  uint8                     errorId = DOIPINT_E_NO_ERROR;
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( ChannelId >= DoIPInt_GetSizeOfChannelIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    channelIdx = DoIPInt_GetChannelIdxOfChannelIdMap(ChannelId);

    /* #30 Overwrite logical addresses. */
    DoIPInt_SetLogicalRemoteAddressOfChannelDyn(channelIdx, LogicalRemoteAddress);
    DoIPInt_SetLogicalLocalAddressOfChannelDyn(channelIdx, LogicalLocalAddress);
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_OVERWRITE_LOGICAL_ADDR, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_OverwriteLogicalAddresses() */

/**********************************************************************************************************************
 *  DoIPInt_CloseConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_CloseConnection(
  DoIPInt_ConnectionIdType ConnectionId,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = DOIPINT_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameter (if error detection is enabled). */
  else if ( ConnectionId >= DoIPInt_GetSizeOfConnectionIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Handle closing of socket connection. */
    DoIPInt_Connection_CloseConnection(ConnectionId, Abort);
    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_CLOSE_CONNECTION, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */
  return retVal;
} /* DoIPInt_CloseConnection() */

/**********************************************************************************************************************
 *  DoIPInt_SoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfConnectionType connectionIdx = DoIPInt_GetSizeOfConnection();
  uint8 errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPInt_Connection_GetConnectionIdxBySoConId(SoConId, &connectionIdx) == E_NOT_OK )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( (Mode != SOAD_SOCON_ONLINE ) && (Mode != SOAD_SOCON_RECONNECT) && (Mode != SOAD_SOCON_OFFLINE) )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#else
  if ( DoIPInt_Connection_GetConnectionIdxBySoConId(SoConId, &connectionIdx) == E_OK )
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Handle socket connection mode change. */
    /*@ assert connectionIdx < DoIPInt_GetSizeOfConnection(); */                                                       /* VCA_DOIPINT_VALID_IDX_DERIVED_BY_FUNC */
    DoIPInt_Connection_ModeChg(connectionIdx, Mode);
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_SO_CON_MODE_CHG, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */
} /* DoIPInt_SoConModeChg() */

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
/* PRQA S 3673 4 */ /* MD_MSR_Rule8.13 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_SoAdTpCopyTxData(
  PduIdType DoIPIntSoAdTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) DoIPIntSoAdTxInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DOIPINT_VAR) RetryPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8             errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntSoAdTxPduId >= DoIPInt_GetSizeOfSoAdTxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPIntSoAdTxInfoPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( (DoIPIntSoAdTxInfoPtr->SduLength > 0u) && (DoIPIntSoAdTxInfoPtr->SduDataPtr == NULL_PTR) )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( RetryPtr != NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( AvailableDataPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else
#else
  /* Use const dummy statement to prevent a self assignment with NULL_PTR. */
  DOIPINT_DUMMY_STATEMENT_CONST(RetryPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    *AvailableDataPtr = 0u;

    /* #30 Try to copy transmission data to provided buffer. */
    retVal = DoIPInt_Tx_CopyTxData(DoIPInt_GetConnectionIdxOfSoAdTxPduIdMap(DoIPIntSoAdTxPduId), DoIPIntSoAdTxInfoPtr,
      AvailableDataPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_COPY_TX_DATA, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_SoAdTpCopyTxData() */

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_SoAdTpTxConfirmation(
  PduIdType DoIPIntSoAdTxPduId,
  Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntSoAdTxPduId >= DoIPInt_GetSizeOfSoAdTxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( (Result != E_OK) && (Result != E_NOT_OK) )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Confirm transmitted PDU. */
    DoIPInt_Tx_Confirmation(DoIPInt_GetConnectionIdxOfSoAdTxPduIdMap(DoIPIntSoAdTxPduId), Result);
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_TX_CONFIRMATION, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */
} /* DoIPInt_SoAdTpTxConfirmation() */

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpStartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
/* PRQA S 3673 3 */ /* MD_MSR_Rule8.13 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_SoAdTpStartOfReception(
  PduIdType DoIPIntSoAdRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) DoIPIntSoAdRxInfoPtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8             errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntSoAdRxPduId >= DoIPInt_GetSizeOfSoAdRxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPIntSoAdRxInfoPtr != NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( TpSduLength != 0u )
  {
    /* The value 0 is required to indicate that the TP-API is used for streaming. */
    errorId = DOIPINT_E_PARAM;
  }
  else if ( AvailableDataPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else
#else
  /* PRQA S 1338, 2983, 3112 3 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  /* Use const dummy statement to prevent a self assignment with NULL_PTR. */
  DOIPINT_DUMMY_STATEMENT_CONST(DoIPIntSoAdRxInfoPtr);
  DOIPINT_DUMMY_STATEMENT(TpSduLength);
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    *AvailableDataPtr = 0u;

    /* #30 Try to start reception. */
    retVal = DoIPInt_Rx_StartOfReception(DoIPInt_GetConnectionIdxOfSoAdRxPduIdMap(DoIPIntSoAdRxPduId),
      AvailableDataPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_START_OF_RECEPTION, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_SoAdTpStartOfReception() */

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpCopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
/* PRQA S 3673 3 */ /* MD_MSR_Rule8.13 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_SoAdTpCopyRxData(
  PduIdType DoIPIntSoAdRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) DoIPIntSoAdRxInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  uint8             errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntSoAdRxPduId >= DoIPInt_GetSizeOfSoAdRxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( DoIPIntSoAdRxInfoPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( (DoIPIntSoAdRxInfoPtr->SduLength > 0u) && (DoIPIntSoAdRxInfoPtr->SduDataPtr == NULL_PTR) )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else if ( AvailableDataPtr == NULL_PTR )
  {
    errorId = DOIPINT_E_PARAM_POINTER;
  }
  else
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    *AvailableDataPtr = 0u;

    /* #30 Try to copy reception data to provided buffer. */
    retVal = DoIPInt_Rx_CopyRxData(DoIPInt_GetConnectionIdxOfSoAdRxPduIdMap(DoIPIntSoAdRxPduId), DoIPIntSoAdRxInfoPtr,
      AvailableDataPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_COPY_RX_DATA, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* DoIPInt_SoAdTpCopyRxData() */

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_SoAdTpRxIndication(
  PduIdType DoIPIntSoAdRxPduId,
  Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = DOIPINT_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state (if error detection is enabled). */
  if ( DoIPInt_State == DOIPINT_STATE_UNINIT )
  {
    errorId = DOIPINT_E_UNINIT;
  }
  /* #20 Check plausibility of input parameters (if error detection is enabled). */
  else if ( DoIPIntSoAdRxPduId >= DoIPInt_GetSizeOfSoAdRxPduIdMap() )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else if ( (Result != E_OK) && (Result != E_NOT_OK) )
  {
    errorId = DOIPINT_E_PARAM;
  }
  else
#else
  DOIPINT_DUMMY_STATEMENT(Result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Indicate that reception is finished. */
    DoIPInt_Rx_Indication(DoIPInt_GetConnectionIdxOfSoAdRxPduIdMap(DoIPIntSoAdRxPduId));
  }

  /* ----- Development Error Report ------------------------------------- */
#if ( DOIPINT_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report error to default error tracer if any occurred (if enabled). */
  if ( errorId != DOIPINT_E_NO_ERROR )
  {
    (void)Det_ReportError(DOIPINT_MODULE_ID, DOIPINT_INSTANCE_ID_DET, DOIPINT_SID_TP_RX_INDICATION, errorId);
  }
#else
  DOIPINT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_DEV_ERROR_REPORT == STD_ON */
} /* DoIPInt_SoAdTpRxIndication() */

#define DOIPINT_STOP_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 MD_DoIPInt_ObjectWithExtLinkOnlyAccessedOnce: MISRA2012 rule 8.4, rule 8.7 and rule 8.9
      Reason:     Exclusive usage in the template file. An external access to the object is required for modification
                  but since it is a template file an API for modification is not introduced.
      Risk:       None.
      Prevention: No prevention necessary.
 */

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_DOIPINT_VALID_IDX_DERIVED_BY_FUNC
    \DESCRIPTION      The index is retrieved as out-parameter by a function and is not checked against its size
                      afterwards.
    \COUNTERMEASURE   \N An internal function is used to retrieve the index and will always provide a valid index when
                         the return value is positive (E_OK). The return value and index are checked by a separate VCA
                         assertion within the called function. The assertion within the called function holds.

  \ID VCA_DOIPINT_QUEUE_IDX_VALID
    \DESCRIPTION      The used queue index (TxQueueUnusedListEndIdxOfConnectionDyn) is retrieved from the ConnectionDyn
                      array and is not checked against its size afterwards.
    \COUNTERMEASURE   \N The derived queue index is always valid by design while there are unused elements in the queue
                         (TxQueueUnusedElementsOfConnectionDyn) which is previously asserted. The validity is checked
                         by review.

  \ID VCA_DOIPINT_IDX_VALIDITY_CHECKED_CSL03
    \DESCRIPTION      The index is calculated inside the function and checked against the range of start and end index
                      which is modeled by a 0:N sorted indirection. VCA is unable to detect validity of the resulting
                      index.
    \COUNTERMEASURE   \R The indirection is modeled as 0:N sorted indirection. The calculated index will always be
                         valid when checked against the range of start and end index. This is the qualified use-case
                         CSL03 of the ComStackLib.

  \ID VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX
    \DESCRIPTION      The value pair of handle index and message handler index is retrieved from the ConnectionDyn or
                      the TxQueue array. The values are not checked against their size afterwards.
    \COUNTERMEASURE   \N The derived value pair of handle index and message handler index is always valid on the
                         ConnectionDyn after the first transmission has been started and when inserted into the TxQueue
                         and they are not reset to invalid values after finishing a transmission. Since the values are
                         derived from the TxQueue or used in an active transmission context it is asserted that the
                         value pair is valid. The validity is checked by review.

  \ID VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT
    \DESCRIPTION      A function with pointer parameters is directly called, but the function is not defined within the
                      analyzed sources. VCA is unable to determine the behavior of the function.
    \COUNTERMEASURE   \N The compiler ensures the validity of the pointer(s) because the pointer parameter is a
                         constant pointer.

  \ID VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ARGUMENT
    \DESCRIPTION      A function with pointer parameters is directly called, but the function is not defined within the
                      analyzed sources. VCA is unable to determine the behavior of the function.
    \COUNTERMEASURE   \N Validity of the pointer(s) is checked by review. Pointer type corresponds to function
                         parameter type. If NULL_PTR is used, it is in valid range as specified by the called function.

  \ID VCA_DOIPINT_OPTIONAL_FUNCTION_PTR
    \DESCRIPTION      Calling an optional function pointer stored in an array with constant index using ComStackLib.
    \COUNTERMEASURE   \S The user of MICROSAR Safe shall perform the integration (ISO 26262:6-10) and verification
                         (ISO 26262:6-11) processes as required by ISO 26262.
                         SMI-4
                      \R ComStackLib is used to check availability during runtime. This check ensures that the
                         implemented constant index 0 is valid and can be used to access the array.

  \ID VCA_DOIPINT_0_N_IND_CSL03
    \DESCRIPTION      Iteration of 0:N unsorted indirection. VCA is unable to detect validity of the resulting index.
    \COUNTERMEASURE   \N The indirection is modeled as 0:N unsorted indirection. The iteration index will always be
                         valid when iterating from start to end index. This is the qualified use-case CSL03 of the
                         ComStackLib.

  \ID VCA_DOIPINT_1_N_IND_CSL03
    \DESCRIPTION      Iteration of 1:N unsorted indirection. VCA is unable to detect validity of the resulting index.
    \COUNTERMEASURE   \N The indirection is modeled as 1:N unsorted indirection. The iteration index will always be
                         valid when iterating from start to end index. This is the qualified use-case CSL03 of the
                         ComStackLib.

  \ID VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID
    \DESCRIPTION      VStdLib_MemCpy is used to copy data. The pointer parameter of PduInfoType originates from the
                      lower layer and VCA is unable to determine the validity of the pointed to memory location and the
                      corresponding length as required by VStdLib_MemCpy.
    \COUNTERMEASURE   \S The user of MICROSAR Safe shall only pass valid pointers at all interfaces to MICROSAR Safe
                         components.
                         This valid pointer is copied to a local variable and the valid length and memory are never
                         extended. Instead, they are only shortened/shifted by the same range which is checked by
                         review. Therefore, the length and pointed to memory match and are always valid.
                         SMI-16

  \ID VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED
    \DESCRIPTION      VStdLib_MemCpy is used to copy data. The target array is a component internal buffer that is
                      accessed via ComStackLib indirections. VCA is unable to determine the validity of the pointed to
                      memory location and the corresponding length as required by VStdLib_MemCpy.
    \COUNTERMEASURE   \R The target array is a component internal buffer and its length is previously checked (inside
                         the function or when reserving the buffer for the current context) by ComStackLib
                         indirections. The data is only copied in case the target array is big enough to hold the data
                         to be copied.

  \ID VCA_DOIPINT_MEM_CPY_FIX_LENGTH
    \DESCRIPTION      VStdLib_MemCpy is used to copy data. The target array is a local variable that originates from
                      the calling function and VCA is unable to determine the validity of the pointed to memory
                      location and the corresponding length as required by VStdLib_MemCpy.
    \COUNTERMEASURE   \N The length of the target array is fix (constant macro) and so the length of the data to be
                         copied data will never be more since the fix length is the maximum length that may be
                         buffered. The target array is always big enough to hold the data to be copied.

  \ID VCA_DOIPINT_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION
    \DESCRIPTION      Compiler specific memcpy function is called and VCA is unable to recognize the specification of
                      the calling function.
    \COUNTERMEASURE   \N Specification at the calling function satisfies the requirements of memcpy.

  \ID VCA_DOIPINT_PTR_VALID
    \DESCRIPTION      SduDataPtr is used without additional check for NULL_PTR.
    \COUNTERMEASURE   \N The API is called with a valid SduDataPtr from each possible context.

  \ID VCA_DOIPINT_NO_ENTRY_FUNC
    \DESCRIPTION      Specification for entry function is unsatisfiable.
    \COUNTERMEASURE   \N The given API is no entry function. Instead, it is a function which is only relevant when the
                         OEM specific feature is enabled. Since the API is never called in variants not using this
                         feature it is mistakenly interpreted as entry function.

  \ID VCA_DOIPINT_CODE_NOT_REACHED_WHEN_NULL_PTR
    \DESCRIPTION      Reading from a null pointer has undefined behavior.
    \COUNTERMEASURE   \N The section of code is never reached when the accessed pointer is a NULL_PTR. The message
                         handler index is only set to DoIPInt_GetMsgHandlerIdxOfOemSpecificMsgHandler() when an OEM
                         specific transmission is started. If this is the case, the buffer will never be a NULL_PTR.

VCA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt.c
 *********************************************************************************************************************/
