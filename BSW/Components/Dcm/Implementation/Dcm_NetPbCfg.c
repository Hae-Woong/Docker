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
/**        \file  Dcm_NetPbCfg.c
 *         \unit  NetPbCfg
 *        \brief  Contains the implementation of Network Postbuild Configuration unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
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
#define DCM_NETPBCFG_SOURCE

#ifdef __PRQA__                                                                                                                                      /* COV_DCM_PRQA_UNREACHABLE XF */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetReqData" 2985 /* MD_Dcm_Redundant_2985 */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetResData" 2985 /* MD_Dcm_Redundant_2985 */
                                                                                                                                                     /* PRQA S 2991 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2992 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2995 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2996 EOF */ /* MD_Dcm_ConstExpr */
#endif
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_NetPbCfg.h"
#include "Dcm_Uti.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Network variant/non-variant configuration abstraction */
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define Dcm_PbRamNetBufferContext                                   (Dcm_NetPbCfgSingletonContext.RamVariant.BufferContexts)
# define Dcm_PbRamNetTransportObject                                 (Dcm_NetPbCfgSingletonContext.RamVariant.TranspObjects)
# define Dcm_PbRamNetComMContext                                     (Dcm_NetPbCfgSingletonContext.RamVariant.ComMContexts)
# define Dcm_PbRamNetPeriodicTxObject                                (Dcm_NetPbCfgSingletonContext.RamVariant.PeriodicTxObjects)
# define Dcm_PbRamNetConnHdl2TObjMap                                 (Dcm_NetPbCfgSingletonContext.RamVariant.ConnHdl2TObjIdMap)
# define Dcm_PbRamNetComCtrlChannels                                 (Dcm_NetPbCfgSingletonContext.RamVariant.ComCtrlChannels)
#else
# define Dcm_PbRamNetBufferContext                                   (Dcm_NetPbCfgSingletonContext.BufferContext)
# define Dcm_PbRamNetTransportObject                                 (Dcm_NetPbCfgSingletonContext.TransportObject)
# define Dcm_PbRamNetComMContext                                     (Dcm_NetPbCfgSingletonContext.ComMContext)
# define Dcm_PbRamNetPeriodicTxObject                                (Dcm_NetPbCfgSingletonContext.PeriodicTxObjects)
# define Dcm_PbRamNetConnHdl2TObjMap                                 (Dcm_NetPbCfgSingletonContext.ConnHdl2TObjIdMap)
# define Dcm_PbRamNetComCtrlChannels                                 (Dcm_NetPbCfgSingletonContext.ComCtrlChannels)
#endif

#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define Dcm_NetPbCfgBufferInfo                                      (Dcm_NetPbCfgSingletonContext.RomVariant.BufferInfo)
#else
# define Dcm_NetPbCfgBufferInfo                                      Dcm_CfgNetBufferInfo
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
 /*! Network variant/non-variant configuration abstraction */
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
# define Dcm_NetPbCfgNumRxPduIds                                     (Dcm_NetPbCfgSingletonContext.ComVariant->NumRxPduIds)                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumTxPduIds                                     (Dcm_NetPbCfgSingletonContext.ComVariant->NumTxPduIds)                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumConnections                                  (Dcm_NetPbCfgSingletonContext.ComVariant->NumConnections)                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgRxPduInfo                                       (Dcm_NetPbCfgSingletonContext.ComVariant->RxPduIdTable)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgxPdu2ConnMap                                    (Dcm_NetPbCfgSingletonContext.ComVariant->TxPduIdMap)                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgCanTpRxPduIdMin                                 (Dcm_NetPbCfgSingletonContext.ComVariant->CanTpRxPduIdMin)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgCanTpRxPduIdMax                                 (Dcm_NetPbCfgSingletonContext.ComVariant->CanTpRxPduIdMax)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumCanRxPduIds                                  (Dcm_NetPbCfgSingletonContext.ComVariant->NumCanTpRxPduIds)                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumPerTxObjects                                 (Dcm_NetPbCfgSingletonContext.ComVariant->NumPerTxObjects)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumProtocols                                    (Dcm_NetPbCfgSingletonContext.ComVariant->NumProtocols)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumComMChannels                                 (Dcm_NetPbCfgSingletonContext.ComVariant->NumComMChannels)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumBuffers                                      (Dcm_NetPbCfgSingletonContext.ComVariant->NumBuffers)                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumTransportObjects                             (Dcm_NetPbCfgSingletonContext.ComVariant->NumTranspObjects)                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNumAllComMChannels                              (Dcm_NetPbCfgSingletonContext.ComVariant->NumAllComMChannels)                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgNetworkHdlLookUp                                (Dcm_NetPbCfgSingletonContext.ComVariant->NetworkHandleLookUpTable)             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgComCtrlChannelListAll                           (Dcm_NetPbCfgSingletonContext.ComVariant->ComCtrlChannelListAll)                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgAllComMChannelMap                               (Dcm_NetPbCfgSingletonContext.ComVariant->NetAllComMChannelMap)                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgConnectionInfo                                  (Dcm_NetPbCfgSingletonContext.ComVariant->ConnectionTable)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgPerTxPduIdInfo                                  (Dcm_NetPbCfgSingletonContext.ComVariant->PerTxPduIdTable)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgProtocolInfo                                    (Dcm_NetPbCfgSingletonContext.ComVariant->ProtocolTable)                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgConnComMChannelMap                              (Dcm_NetPbCfgSingletonContext.ComVariant->NetConnComMChannelMap)                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgPerTxPdu2RsrsMap                                (Dcm_NetPbCfgSingletonContext.ComVariant->PerTxPduIdMap)                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPbCfgCanTp2DcmRxPduIdMap                             (Dcm_NetPbCfgSingletonContext.ComVariant->CanTp2DcmRxPduIdMap)                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define Dcm_NetPbCfgNumRxPduIds                                     DCM_NET_NUM_RX_PDUIDS
# define Dcm_NetPbCfgNumTxPduIds                                     DCM_NET_NUM_TX_PDUIDS
# define Dcm_NetPbCfgNumConnections                                  DCM_NET_MAX_NUM_CONNECTIONS
# define Dcm_NetPbCfgCanTpRxPduIdMin                                 DCM_NET_CANTP_RX_PDUID_MIN
# define Dcm_NetPbCfgCanTpRxPduIdMax                                 DCM_NET_CANTP_RX_PDUID_MAX
# define Dcm_NetPbCfgNumCanRxPduIds                                  DCM_NET_NUM_CANTP_RX_PDUIDS
# define Dcm_NetPbCfgNumPerTxObjects                                 DCM_NET_MAX_NUM_PERIODIC_TX_OBJ
# define Dcm_NetPbCfgNumProtocols                                    DCM_NET_NUM_PROTOCOLS
# define Dcm_NetPbCfgNumComMChannels                                 DCM_NET_MAX_NUM_COMM_CHANNELS
# define Dcm_NetPbCfgNumBuffers                                      DCM_NET_NUM_BUFFERS
# define Dcm_NetPbCfgNumTransportObjects                             DCM_NET_MAX_NUM_TRANSP_OBJECTS
# define Dcm_NetPbCfgNumAllComMChannels                              DCM_NET_MAX_NUM_ALL_COMM_CHANNELS
# define Dcm_NetPbCfgCanTp2DcmRxPduIdMap                             Dcm_CfgNetCanTp2DcmRxPduIdMap
# define Dcm_NetPbCfgRxPduInfo                                       Dcm_CfgNetRxPduInfo
# define Dcm_NetPbCfgxPdu2ConnMap                                    Dcm_CfgNetTxPduInfo
# define Dcm_NetPbCfgNetworkHdlLookUp                                Dcm_CfgNetNetworkHandleLookUpTable
# define Dcm_NetPbCfgComCtrlChannelListAll                           Dcm_CfgNetComCtrlChannelListAll
# define Dcm_NetPbCfgAllComMChannelMap                               Dcm_CfgNetAllComMChannelMap
# define Dcm_NetPbCfgConnectionInfo                                  Dcm_CfgNetConnectionInfo
# define Dcm_NetPbCfgPerTxPduIdInfo                                  Dcm_CfgNetPeriodicTxPduIds
# define Dcm_NetPbCfgProtocolInfo                                    Dcm_CfgNetProtocolInfo
# define Dcm_NetPbCfgConnComMChannelMap                              Dcm_CfgNetConnComMChannelMap
# define Dcm_NetPbCfgPerTxPdu2RsrsMap                                Dcm_CfgNetPeriodicTxPduInfo
#endif

#define Dcm_NetPbCfgGetConnectionInfoOfRxPduId(rxPduId)              (Dcm_NetPbCfgConnectionInfo[Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId)])          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetPbCfgGetProtObj(protRef)                              (&Dcm_NetPbCfgProtocolInfo[(protRef)])                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetPbCfgGetProtObjOfConnection(connHdl)                  (Dcm_NetPbCfgGetProtObj(Dcm_NetPbCfgConnectionInfo[connHdl].ProtRef))           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 /*! Control states of the network sub-component */
struct DCM_NETPBCFGSINGLETONCONTEXTTYPE_TAG
{
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_CfgVarMgrComVariantType   ComVariant;                                                           /*!< Points to the COM-criteria */
#endif
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
  Dcm_CfgNetRamVariantInfoType  RamVariant;                                                           /*!< Relocatable RAM for the network sub-component */
  Dcm_CfgNetRomVariantInfoType  RomVariant;                                                           /*!< Relocatable ROM for the network sub-component */
#endif
  DCM_PB_RAM_DATA_DECL(Dcm_NetBufferContextType, BufferContext, DCM_NET_NUM_BUFFERS)                  /*!< Control states of the USDT message buffers (main and additional) */
  DCM_PB_RAM_DATA_DECL(Dcm_NetTransportObjectType, TransportObject, DCM_NET_MAX_NUM_TRANSP_OBJECTS)   /*!< Transport objects for simultaneous servicing of multiple clients */
  DCM_PB_RAM_DATA_DECL(Dcm_NetComMContextType, ComMContext, DCM_NET_MAX_NUM_COMM_CHANNELS)            /*!< Control states of ComM status notifications per DCM related com-channel (i.e. channel on which DCM communicates with a client */
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  DCM_PB_RAM_DATA_DECL(Dcm_NetPeriodicTxObjectType, PeriodicTxObjects, DCM_NET_MAX_NUM_PERIODIC_TX_OBJ) /*!< Periodic message control objects */
#endif
  DCM_PB_RAM_DATA_DECL(Dcm_CfgNetTObjHandleMemType, ConnHdl2TObjIdMap, DCM_NET_MAX_NUM_CONNECTIONS)   /*!< A map of type <key = connectionHdl, value = TransportObject>. Used for fast transport object allocation */
#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
  DCM_PB_RAM_DATA_DECL(Dcm_CommunicationModeType, ComCtrlChannels, DCM_NET_MAX_NUM_ALL_COMM_CHANNELS) /*!< Communication state [Rx/Tx][On/Off] of each ComM channel known to DCM */
#endif
};
typedef struct DCM_NETPBCFGSINGLETONCONTEXTTYPE_TAG Dcm_NetPbCfgSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Singleton context of NetPbCfg unit */
DCM_LOCAL VAR(Dcm_NetPbCfgSingletonContextType, DCM_VAR_NOINIT) Dcm_NetPbCfgSingletonContext;

#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dcm_NetPbCfgInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPbCfgInit(
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);

#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_NetPbCfgSingletonContext.ComVariant = configPtr->ComVariant;
#endif
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
  Dcm_NetPbCfgSingletonContext.RamVariant = configPtr->RamVariant->NetworkRam;
  Dcm_NetPbCfgSingletonContext.RomVariant = configPtr->RomVariant->NetworkRom;
#endif
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgIsRxPduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgIsRxPduIdValid(
  PduIdType rxPduId
  )
{
  boolean lResult = TRUE;
  if( (rxPduId >= Dcm_NetPbCfgNumRxPduIds)
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
    ||(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId) >= Dcm_NetPbCfgNumConnections)
#endif
    )
  {
    lResult = FALSE;
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgIsTxPduIdValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgIsTxPduIdValid(
  PduIdType txPduId
  )
{
  boolean lResult = TRUE;
  if( (txPduId >= Dcm_NetPbCfgNumTxPduIds)
#if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
    ||(Dcm_NetPbCfgxPdu2ConnMap[txPduId] >= Dcm_NetPbCfgNumConnections)
#endif
    )
  {
    lResult = FALSE;
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumRxPduIds()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetNumRxPduIds(
  void
  )
{
  return Dcm_NetPbCfgNumRxPduIds;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetBufferContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetBufferContextPtrType, DCM_CODE) Dcm_NetPbCfgGetBufferContext(
  Dcm_CfgNetBufferRefOptType index
  )
{
  Dcm_NetBufferContextPtrType lResult;
  if(Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumBuffers))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetBufferContext[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetBufferContext[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTesterAddressOfRxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DCM_CODE) Dcm_NetPbCfgGetTesterAddressOfRxPduId(
  PduIdType rxPduId
  )
{
  return Dcm_NetPbCfgConnectionInfo[Dcm_NetPbCfgRxPduInfo[rxPduId].ConnRef].ClientSrcAddr;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTransportObject()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_NetPbCfgGetTransportObject(
  Dcm_CfgNetTObjHandleOptType index
  )
{
  Dcm_NetTransportObjectPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumTransportObjects))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetTransportObject[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetTransportObject[index];
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetComMContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetComMContextPtrType, DCM_CODE) Dcm_NetPbCfgGetComMContext(
  Dcm_CfgNetNetIdRefOptType index
  )
{
  Dcm_NetComMContextPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumComMChannels))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetComMContext[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetComMContext[index];
  }

  return lResult;
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetPeriodicTxObject()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetPeriodicTxObjectPtrType, DCM_CODE) Dcm_NetPbCfgGetPeriodicTxObject(
  Dcm_CfgNetPTxObjHandleOptType index
  )
{
  Dcm_NetPeriodicTxObjectPtrType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumPerTxObjects))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_PbRamNetPeriodicTxObject[0];
  }
  else
  {
    lResult = &Dcm_PbRamNetPeriodicTxObject[index];
  }

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgPutToConn2TObjMap()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPbCfgPutToConn2TObjMap(
  Dcm_NetConnRefOptType index,
  Dcm_CfgNetTObjHandleMemType value
  )
{
  if(Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumConnections))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_PbRamNetConnHdl2TObjMap[index] = value;                                                                                                      /* SBSW_DCM_POINTER_WRITE_TOBJHANDLE */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTObjConn2TObjMap()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetTObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetTObjConn2TObjMap(
  Dcm_NetConnRefOptType index
  )
{
  return Dcm_PbRamNetConnHdl2TObjMap[index];
}

#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetComCtrlChannelState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CommunicationModeType, DCM_CODE) Dcm_NetPbCfgGetComCtrlChannelState(
  uint8_least index
  )
{
  return Dcm_PbRamNetComCtrlChannels[index];
}
#endif

#if (DCM_NET_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgSetComControlChannelState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPbCfgSetComControlChannelState(
  uint8_least index,
  Dcm_CommunicationModeType value
  )
{
  if (Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumAllComMChannels))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_PbRamNetComCtrlChannels[index] = value;                                                                                                      /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_COMCONTROLCHANNEL */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetBufferInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_PbCfgNetBufferInfoPtrType, DCM_CODE) Dcm_NetPbCfgGetBufferInfo(
  Dcm_CfgNetBufferRefOptType index
  )
{
  Dcm_PbCfgNetBufferInfoPtrType lResult;

  if (Dcm_DebugDetectRuntimeError(index >= Dcm_NetPbCfgNumBuffers))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = &Dcm_NetPbCfgBufferInfo[0];
  }
  else
  {
    lResult = &Dcm_NetPbCfgBufferInfo[index];
  }

  return lResult;
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle(
  PduIdType txPduId
  )
{
  return Dcm_NetPbCfgPerTxPdu2RsrsMap[txPduId];
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnHdlOfRxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnHdlOfRxPduId(
  PduIdType rxPduId
  )
{
  return Dcm_NetPbCfgRxPduInfo[rxPduId].ConnRef;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnHdlOfTxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnHdlOfTxPduId(
  PduIdType txPduId
  )
{
  return Dcm_NetPbCfgxPdu2ConnMap[txPduId];
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnTesterAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DCM_CODE) Dcm_NetPbCfgGetConnTesterAddress(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].ClientSrcAddr;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtIdOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetProtIdOfConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].ProtRef;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetMetaDataLengthOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetMetaDataLengthOfConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].MetaDataLength;
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetPerTxConfTimeOutOfConn()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_NetPbCfgGetPerTxConfTimeOutOfConn(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].PeriodicTxConfTimeout;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetIdRefOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetNetIdRefOfConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].NetworkIdRef;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetworkRefByConnHdl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetPbCfgGetNetworkRefByConnHdl(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnComMChannelMap[Dcm_NetPbCfgConnectionInfo[connHdl].NetworkIdRef];
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetworkRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetPbCfgGetNetworkRef(
  Dcm_CfgNetNetIdRefOptType netHandle
  )
{
  return Dcm_NetPbCfgConnComMChannelMap[netHandle];
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DCM_CODE) Dcm_NetPbCfgGetConnId(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].ConnectionId;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnProp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetConnPropertiesType, DCM_CODE) Dcm_NetPbCfgGetConnProp(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].Properties;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetEcuAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetEcuAddress(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].EcuAddress;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnTxPduIdMain()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetConnTxPduIdMain(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].TxPduIdMain;
}

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnAuthInfoRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetAuthInfoRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnAuthInfoRef(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].AuthInfoRef;
}
#endif

#if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnAuthInfoRefByRxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetAuthInfoRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnAuthInfoRefByRxPduId(
  PduIdType rxPduId
  )
{
  return Dcm_NetPbCfgGetConnectionInfoOfRxPduId(rxPduId).AuthInfoRef;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnUudtFrameSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_UudtFrameSizeType, DCM_CODE) Dcm_NetPbCfgGetConnUudtFrameSize(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].UudtFrameSize;
}
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTpRxPduIdMin()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetTpRxPduIdMin(
  void
  )
{
  return Dcm_NetPbCfgCanTpRxPduIdMin;
}
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTpRxPduIdMax()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetTpRxPduIdMax(
  void
  )
{
  return Dcm_NetPbCfgCanTpRxPduIdMax;
}
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetDcmRxPduIdOfCanTpPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetDcmRxPduIdOfCanTpPduId(
  PduIdType canTpRxPduId
  )
{
  return Dcm_NetPbCfgCanTp2DcmRxPduIdMap[canTpRxPduId-Dcm_NetPbCfgCanTpRxPduIdMin];
}
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumCanRxPduIds()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetNumCanRxPduIds(
  void
  )
{
  return Dcm_NetPbCfgNumCanRxPduIds;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumPerTxObjects()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetNumPerTxObjects(
  void
  )
{
  return Dcm_NetPbCfgNumPerTxObjects;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumConnections()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPbCfgGetNumConnections(
  void
  )
{
  return Dcm_NetPbCfgNumConnections;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumTransportObjects()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetTObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetNumTransportObjects(
  void
  )
{
  return Dcm_NetPbCfgNumTransportObjects;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetworkHdlLookUp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_PbCfgNetNetworkHandlePtrType, DCM_CODE) Dcm_NetPbCfgGetNetworkHdlLookUp(
  void
  )
{
  return Dcm_NetPbCfgNetworkHdlLookUp;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumProtocols()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetNumProtocols(
  void
  )
{
  return Dcm_NetPbCfgNumProtocols;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetAllComMChannelMap()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(NetworkHandleType, DCM_CODE) Dcm_NetPbCfgGetAllComMChannelMap(
  Dcm_CfgNetNetIdRefOptType index
  )
{
  return Dcm_NetPbCfgAllComMChannelMap[index];
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumComMChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(NetworkHandleType, DCM_CODE) Dcm_NetPbCfgGetNumComMChannels(
  void
  )
{
  return Dcm_NetPbCfgNumComMChannels;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumBuffers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetBufferRefMemType, DCM_CODE) Dcm_NetPbCfgGetNumBuffers(
  void
  )
{
  return Dcm_NetPbCfgNumBuffers;
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgPeriodicMsgGetTxPduIdByConn()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgPeriodicMsgGetTxPduIdByConn(
  Dcm_NetConnRefMemType         connHdl,
  Dcm_CfgNetPTxObjHandleOptType txObjIdx
  )
{
  return Dcm_NetPbCfgPerTxPduIdInfo[Dcm_NetPbCfgConnectionInfo[connHdl].PeriodicTxRef + txObjIdx];
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumAllComMChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(NetworkHandleType, DCM_CODE) Dcm_NetPbCfgGetNumAllComMChannels(
  void
  )
{
  return Dcm_NetPbCfgNumAllComMChannels;
}

#if (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetComCtrlChannelListAll()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetPbCfgGetComCtrlChannelListAll(
  uint8_least index
  )
{
  return Dcm_NetPbCfgComCtrlChannelListAll[index];
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_ProtocolType, DCM_CODE) Dcm_NetPbCfgGetProtocolId(
  Dcm_NetProtRefMemType handle
  )
{
  return Dcm_NetPbCfgProtocolInfo[handle].Id;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetThreadIdOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_ProtocolType, DCM_CODE) Dcm_NetPbCfgGetThreadIdOfConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(connHdl)->ThreadId;
 }

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolIdOfTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_ProtocolType, DCM_CODE) Dcm_NetPbCfgGetProtocolIdOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(pTranspObj->ConnHdl)->Id;
}

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON) || \
    (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetHasRespPendOnBoot()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgGetHasRespPendOnBoot(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(pTranspObj->ConnHdl)->HasRespPendOnBoot;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolRxTxBufferRefByRxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetBufferRefMemType, DCM_CODE) Dcm_NetPbCfgGetProtocolRxTxBufferRefByRxPduId(
  PduIdType rxPduId
  )
{
  Dcm_NetConnRefMemType lConnHdl = Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId);
  return Dcm_NetPbCfgGetRxTxBufRefByConnHdl(lConnHdl);
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetRxTxBufRefByConnHdl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetBufferRefMemType, DCM_CODE) Dcm_NetPbCfgGetRxTxBufRefByConnHdl(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(connHdl)->RxTxBufferRef;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolPriorityByConnHdl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetProtPrioType, DCM_CODE) Dcm_NetPbCfgGetProtocolPriorityByConnHdl(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(connHdl)->Priority;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnProtIdOfRxPduId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnProtIdOfRxPduId(
  PduIdType rxPduId
  )
{
  return Dcm_NetPbCfgGetConnectionInfoOfRxPduId(rxPduId).ProtRef;
}

#if (DCM_STATE_PROTOCOL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolStateRef()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolStateRef(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(pTranspObj->ConnHdl)->NetStateRef;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolMaxLenOfTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetProtLengthType, DCM_CODE) Dcm_NetPbCfgGetProtocolMaxLenOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(pTranspObj->ConnHdl)->MaxLength;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolSrvAdjTime()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_DiagP2TimingsConstPtrType, DCM_CODE) Dcm_NetPbCfgGetProtocolSrvAdjTime(
  Dcm_NetProtRefMemType protocolIdx
  )
{
  return &Dcm_NetPbCfgProtocolInfo[protocolIdx].SrvAdjTime;
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnNumPeriodicTxPduIds()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetConnNumPeriodicTxPduIds(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgConnectionInfo[connHdl].NumPeriodicTxPduIds;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolDemClientId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolDemClientId(
  Dcm_NetProtRefMemType handle
  )
{
  return Dcm_NetPbCfgProtocolInfo[handle].DemClientId;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolDemClientIdByConnHdl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolDemClientIdByConnHdl(
  Dcm_NetConnRefMemType connHdl
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(connHdl)->DemClientId;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolPriority()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgNetProtPrioType, DCM_CODE) Dcm_NetPbCfgGetProtocolPriority(
  Dcm_NetProtRefMemType protRef
  )
{
  return Dcm_NetPbCfgProtocolInfo[protRef].Priority;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetPbCfgGetProtObjOfConnection(pTranspObj->ConnHdl)->ThreadId;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolThreadId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolThreadId(
  Dcm_NetProtRefMemType handle
  )
{
  return Dcm_NetPbCfgProtocolInfo[handle].ThreadId;
}

/**********************************************************************************************************************
 *  Dcm_NetPbCfgIsFuncRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgIsFuncRequest(
  PduIdType rxPduId
  )
{
  return Dcm_NetPbCfgRxPduInfo[rxPduId].IsFuncReq;
}

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_NetPbCfg.c
 *********************************************************************************************************************/
