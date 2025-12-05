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
/**        \file  Dcm_Net.c
 *         \unit  Net
 *        \brief  Contains the implementation of Network unit.
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
#define DCM_NET_SOURCE

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
#include "Dcm_Net.h"
#include "Dcm_NetPbCfg.h"
#include "Dcm_AuthMgr.h"
#include "Dcm_Debug.h"
#include "Dcm_Diag.h"
#include "Dcm_PagedBuffer.h"
#include "Dcm_State.h"
#include "Dcm_Tmr.h"
#include "Dcm_Tsk.h"
#include "Dcm_Uti.h"
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Svc86.h"
#endif
#include "ComM_Dcm.h"
#include "PduR_Dcm.h"
#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
# include "Dcm_Svc2ASchd.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! ComM channel message communication availability states */
#define DCM_NET_COMM_CHANNEL_READY                                   ((Dcm_NetComMAvailabilityType)TRUE)  /*!< ComM Channel is ready */
#define DCM_NET_COMM_CHANNEL_NOT_READY                               ((Dcm_NetComMAvailabilityType)FALSE) /*!< ComM Channel is not ready */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
# define Dcm_NetPeriodicMsgGetNumMsgsFor(connHdl)                    (Dcm_NetPbCfgGetConnNumPeriodicTxPduIds((connHdl)))                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgGetTxPduIdCurrConn(txObjIdx)              (Dcm_NetPbCfgPeriodicMsgGetTxPduIdByConn(Dcm_NetPeriodicMsgGetConnection(),(txObjIdx))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgNumMsgsCurrConnection()                   (Dcm_NetPeriodicMsgGetNumMsgsFor(Dcm_NetPeriodicMsgGetConnection()))            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgGetNextMsgHdl()                           (Dcm_NetSingletonContext.PeriodicTxContext.NextTxObjectHdl)                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgResetNextMsgHdl()                         ((Dcm_NetSingletonContext.PeriodicTxContext.NextTxObjectHdl) = 0u)              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgIncNextMsgHdl()                           (++(Dcm_NetSingletonContext.PeriodicTxContext.NextTxObjectHdl))                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgGetTxObjectSentCntr()                     (Dcm_NetSingletonContext.PeriodicTxContext.TxObjectSentCntr)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgResetTxObjectSentCntr()                   ((Dcm_NetSingletonContext.PeriodicTxContext.TxObjectSentCntr) = 0u)             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgIncTxObjectSentCntr()                     (++(Dcm_NetSingletonContext.PeriodicTxContext.TxObjectSentCntr))                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPeriodicMsgResetConnectionOwner()                    (Dcm_NetPeriodicMsgSetConnection(DCM_NET_INVALID_CONNHDL))                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
#  define Dcm_NetPeriodicMsgGetDelayCntr()                           (Dcm_NetSingletonContext.PeriodicTxContext.DelayCntr)                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgIncDelayCntr()                           (++(Dcm_NetSingletonContext.PeriodicTxContext.DelayCntr))                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgDecDelayCntr()                           (--(Dcm_NetSingletonContext.PeriodicTxContext.DelayCntr))                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgInitDelayCntr()                          ((Dcm_NetSingletonContext.PeriodicTxContext.DelayCntr) = DCM_NET_DELAY_BULK_TRANSMISSION) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgResetNextDelayTimer()                    ((Dcm_NetSingletonContext.PeriodicTxContext.NextDelayTimer) = 0u)               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgGetNextDelayTimer()                      (Dcm_NetSingletonContext.PeriodicTxContext.NextDelayTimer)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgGetFastestRate()                         (Dcm_NetSingletonContext.PeriodicTxContext.FastestRate)                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetPeriodicMsgInitFastestRate()                        ((Dcm_NetSingletonContext.PeriodicTxContext.FastestRate) = DCM_SVC2A_RATE_TYPE_SLOW) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_NetGetPeriodicDelayTime(Rate)                          (Dcm_CfgNetPeriodicDelayTime[Rate])                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_NetPeriodicMsgGetDelayCntr()                           0u
#  define Dcm_NetPeriodicMsgIncDelayCntr()                           /* not used */
#  define Dcm_NetPeriodicMsgDecDelayCntr()                           /* not used */
#  define Dcm_NetPeriodicMsgInitDelayCntr()                          /* not used */
#  define Dcm_NetPeriodicMsgGetFastestRate()                         DCM_SVC2A_RATE_TYPE_SLOW
#  define Dcm_NetPeriodicMsgInitFastestRate()                        /* not used */
#  define Dcm_NetGetPeriodicDelayTime(Rate)                          0u
# endif
# define Dcm_NetPTxObjTimerGet(pPeriodicTxObj)                       ((pPeriodicTxObj)->Timer)                                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetPTxObjTimerExpired(pPeriodicTxObj)                   (Dcm_NetPTxObjTimerGet(pPeriodicTxObj) == 0u)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif /* (DCM_NET_PERIODIC_TX_ENABLED == STD_ON) */

#define Dcm_NetGetComStateByChannelId(channelId)                     (Dcm_NetPbCfgGetComMContext((channelId))->ComState)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComStateByConnection(connHdl)                      (Dcm_NetGetComStateByChannelId(Dcm_NetPbCfgGetNetIdRefOfConnection(connHdl)))   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComStateByRxPduId(rxPduId)                         (Dcm_NetGetComStateByConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId)))      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComAvailabilityByChannelId(channelId)              (Dcm_NetPbCfgGetComMContext((channelId))->ComAvailability)                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetComAvailabilityByConnection(connHdl)               (Dcm_NetGetComAvailabilityByChannelId(Dcm_NetPbCfgGetNetIdRefOfConnection(connHdl))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetProtIdOfSessionConnection()                        (Dcm_NetPbCfgGetProtIdOfConnection(Dcm_NetGetSessionConnection()))              /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_NetGetProtIdOfTranspObj(pTranspObj)                      (Dcm_NetPbCfgGetProtIdOfConnection((pTranspObj)->ConnHdl))                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */


#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
# define Dcm_NetGetThreadIdOfSessionConnection()                     (Dcm_NetPbCfgGetThreadIdOfConnection(Dcm_NetGetSessionConnection()))            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
/*! During prioritization in default session, the session connection is not initialized, so map to DCM_THREAD_ID_BASIC here directly */
# define Dcm_NetGetThreadIdOfSessionConnection()                     (DCM_THREAD_ID_BASIC)                                                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif
#define Dcm_NetIsPropertieSet(pTranspObj, prop)                      (Dcm_UtiBitOpTest(Dcm_CfgNetConnPropertiesType, Dcm_NetPbCfgGetConnProp((pTranspObj->ConnHdl)), (prop))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Control states of the network sub-component */
struct DCM_NETCONTEXTTYPE_TAG
{
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  Dcm_NetPeriodicTxContextType  PeriodicTxContext;                                                    /*!< Composition relation to the periodic message transmitter */
#endif
  Dcm_NetProtRefMemType         ActiveProtocol;                                                       /*!< The protocol handle last activated */
  Dcm_NetConnRefMemType         NumActiveConnections;                                                 /*!< Current number of simultaneously serviced clients/connections */
  Dcm_NetConnRefMemType         SessionConnection;                                                    /*!< Owner (client) of the non-default session last started */
  uint16                        SessionClientSrcAddr;                                                 /*!< Source address of client which started the non-default session */
  Dcm_NetActiveDiagnosticType   ActiveDiagnostic;                                                     /*!< Guard for prevention of DCMs automatic ComM diag-activate signal notification (this state is managed by the application) */
  boolean                       RxAllowed;                                                            /*!< Guard for global blocking of any request reception */
  boolean                       KeepAlive;                                                            /*!< Guard for prevention of diag-inactive notifications from DCM to ComM (e.g. in the default-session) */
};
typedef struct DCM_NETCONTEXTTYPE_TAG Dcm_NetSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled()
 *********************************************************************************************************************/
/*! \brief          Returns DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED as boolean.
 *  \details        -
 *  \return         TRUE         DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED is STD_ON
 *  \return         FALSE        DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED is STD_OFF
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetCommonInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the common network related states.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetCommonInit(
  Dcm_ConfigPtrType configPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetComMInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the ComM related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetComMInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetBufferInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the buffer related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetBufferInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetConnectionInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the connection related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetConnectionInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetTranspObjInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the transport object related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTranspObjInit(
  void
  );

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the periodic message related states.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgInit(
  void
  );
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetOnRequestDetection()
 *********************************************************************************************************************/
/*! \brief          Cancels any pending job if detected another ECU to be requested from the same client.
 *  \details        Function will be called from Dcm_NetStartOfReception.
 *  \param[in]      connHdl        The connection ID of the foreign ECU request
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetOnRequestDetection(
  Dcm_NetConnRefMemType connHdl
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationFindWinner()
 *********************************************************************************************************************/
/*! \brief          Determination of request with highest priority.
 *  \details        -
 *  \param[in]      ev                      The active event(s) of the task Dcm_NetTaskRx
 *  \param[out]     pBusyEvents             Bitfield for busy requests
 *  \return         ID of the winner
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetTObjHandleOptType, DCM_CODE) Dcm_NetRxPrioritisationFindWinner(
  Dcm_TskTaskEvOptType ev,
  Dcm_TskTaskEvPtrType pBusyEvents
  );

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessWinner()
 *********************************************************************************************************************/
/*! \brief          Processing of the request with the highest priority.
 *  \details        -
 *  \param[in]      pContext      Pointer to the context
 *  \param[in]      pTranspObj    The transport object
 *  \param[in,out]  pBusyEvents   Bitfield for busy requests
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessWinner(
  Dcm_ContextPtrType pContext,
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_TskTaskEvPtrType pBusyEvents
  );

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessLoosers()
 *********************************************************************************************************************/
/*! \brief          Processing of requests that were preempted.
 *  \details        -
 *  \param[in]      BusyEvents             Bitfield for busy requests
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessLoosers(
  Dcm_TskTaskEvOptType BusyEvents
  );

#if (DCM_NET_PROTOCOL_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetExecStartProtocolOp()
 *********************************************************************************************************************/
/*! \brief          Executes a protocol start up.
 *  \details        -
 *  \param[in]      pTranspObj     The transport object
 *  \return         TRUE           Proceed with protocol starting
 *  \return         FALSE          Skip any further service processing after return
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetExecStartProtocolOp(
  Dcm_NetTransportObjectPtrType pTranspObj
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetProcessStartProtocol()
 *********************************************************************************************************************/
/*! \brief          Starts a new protocol.
 *  \details        Delegates a new request with a new protocol and terminates the old one.
 *  \param[in]      pTranspObj            The transport object
 *  \param[in]      stopActiveProtocol    Whether the active protocol shall be stopped or not
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetProcessStartProtocol(
  Dcm_NetTransportObjectPtrType pTranspObj,
  boolean stopActiveProtocol
  );

/**********************************************************************************************************************
 *  Dcm_NetTransmitUsdtResponse()
 *********************************************************************************************************************/
/*! \brief          Start transmission on this connection.
 *  \details        Start an USDT transmission over a specific connection.
 *  \param[in]      pTranspObj            The transport object to be used during the transmission.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            All the items of the structure must be validly initialized:
 *                  hBuffer               Shall be the TxBuffer handle
 *                  pduInfo.SduDataPtr    Shall point to the TxBuffer
 *                  pduInfo.SduLength     Shall contain the length of the data to be sent
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTransmitUsdtResponse(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationNotOk()
 *********************************************************************************************************************/
/*! \brief          Indication of finished reception with no success.
 *  \details        This function is called by Dcm_NetTpRxIndication.
 *  \param[in]      pTranspObj    The transport object
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationNotOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationOk()
 *********************************************************************************************************************/
/*! \brief          Indication of finished reception with success.
 *  \details        This function is called by Dcm_NetTpRxIndication.
 *  \param[in]      pTranspObj    The transport object
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerGet()
 *********************************************************************************************************************/
/*! \brief          Get remaining delay time.
 *  \details        Returns the remaining delay time of the given timer
 *  \param[in]      timer       ID of the timer
 *  \return         Remaining delay time
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerCntrOptType, DCM_CODE) Dcm_NetDelayTimerGet(
  uint8_least timer
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerSet()
 *********************************************************************************************************************/
/*! \brief          Set delay timeout.
 *  \details        Sets the timout for the given delay timer
 *  \param[in]      timer       ID of the timer
 *  \param[in]      timeout     Timeout value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetDelayTimerSet(
  uint8_least timer,
  Dcm_TmrTimerCntrOptType timeout
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerExpired()
 *********************************************************************************************************************/
/*! \brief          Check if the delay timer timed out.
 *  \details        Returns whether the given delay timer timed out
 *  \param[in]      timer       ID of the timer
 *  \return         TRUE        Timer has expired
 *  \return         FALSE       Timer is running
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetDelayTimerExpired(
  uint8_least timer
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerSet()
 *********************************************************************************************************************/
/*! \brief          Set the timeout for the given periodic transport object.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \param[in]      timeout           Timout of the timer
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerSet(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStart()
 *********************************************************************************************************************/
/*! \brief          Start the timer for the given periodic transport object.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \param[in]      timeout           Timout of the timer
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStart(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStop()
 *********************************************************************************************************************/
/*! \brief          Stop the timer for the given periodic transport object.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStop(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgIncNextDelayTimer()
 *********************************************************************************************************************/
/*! \brief          Set delay timer index to next timer.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgIncNextDelayTimer(
  void
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxDelayTimerStart()
 *********************************************************************************************************************/
/*! \brief          Starts a delay timer for periodic message transmission.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxDelayTimerStart(
  void
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutTxObjectHandler()
 *********************************************************************************************************************/
/*! \brief          Executes jobs on timeout of a transport object timer.
 *  \details        -
 *  \return         Flag whether timer needs to be reloaded
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutTxObjectHandler(
  void
  );
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutDelayTimerHandler()
 *********************************************************************************************************************/
/*! \brief          Executes jobs on timeout of a delay timer.
 *  \details        -
 *  \return         Flag whether timer needs to be reloaded
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutDelayTimerHandler(
  void
  );
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmitTxObj()
 *********************************************************************************************************************/
/*! \brief          Transmits a periodic message.
 *  \details        -
 *  \param[in]      perTxObjHandle    Handle of periodic transport object to be transmitted
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            The periodic message shall first be allocated via Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmitTxObj(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  );
#endif

/**********************************************************************************************************************
 *  Dcm_NetIsPduInfoValid()
 *********************************************************************************************************************/
/*! \brief          Checks the received PduInfo.
 *  \details        -
 *  \param[in]      info       Message context (data and length of the portion)
 *  \return         TRUE       The info element is valid
 *  \return         FALSE      The info element is not valid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsPduInfoValid(
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
  );

/**********************************************************************************************************************
 *  Dcm_NetCopyRxData()
 *********************************************************************************************************************/
/*! \brief          Copies request data from the lower layer.
 *  \details        This function is called by Dcm_CopyRxData.
 *  \param[in]      pTranspObj         The transport object
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[out]     bufferSizePtr      Available buffer size
 *  \return         BUFREQ_OK          A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK    No free buffer is available - ignore request
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyRxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  );

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndication()
 *********************************************************************************************************************/
/*! \brief          Indication of finished reception.
 *  \details        This function is called by Dcm_TpRxIndication.
 *  \param[in]      rxPduId    The RxPDUID of the request.
 *  \param[in]      result     Transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndication(
  PduIdType rxPduId,
  Std_ReturnType result
  );

/**********************************************************************************************************************
 *  Dcm_NetCopyLinearTxData()
 *********************************************************************************************************************/
/*! \brief          Copies transmit data in linear form to the lower layer.
 *  \details        This function is called by Dcm_NetCopyTxData for the linear response data case.
 *  \param[in]      pTranspObj          The transport object
 *  \param[in]      info                Pointer to a PduInfoType, which indicates the number of bytes to be copied
 *                                      (SduLength) and the location where the data have to be copied to (SduDataPtr).
 *                                      An SduLength of 0 is possible in order to poll the available transmit data
 *                                      count. In this case no data are to be copied and SduDataPtr might be invalid.
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call
 *  \return         BUFREQ_OK           A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK     Some error occurred (i.e. internal error detected)
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyLinearTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetCopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies transmit data to the lower layer.
 *  \details        This function is called by Dcm_CopyTxData.
 *  \param[in]      pTranspObj          The transport object
 *  \param[in]      info                Pointer to a PduInfoType, which indicates the number of bytes to be copied
 *                                      (SduLength) and the location where the data have to be copied to (SduDataPtr).
 *                                      An SduLength of 0 is possible in order to poll the available transmit data
 *                                      count. In this case no data are to be copied and SduDataPtr might be invalid.
 *  \param[in]      availableDataPtr    Remaining Tx data after completion of this call.
 *  \return         BUFREQ_OK           Response data is available - all requested data are copied
 *  \return         BUFREQ_E_BUSY       The required amount of data is not available - try later again
 *  \return         BUFREQ_E_NOT_OK     Some error occurred (e.g. no more data, internal error detected etc.)
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetGetComStateByTranspObject()
 *********************************************************************************************************************/
/*! \brief          Gets the communication state for a transport object.
 *  \details        -
 *  \param[in]      pTranspObj    The pointer to a transport object
 *  \return         The communication state of the assigned transport object
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_NetComMStateType, DCM_CODE) Dcm_NetGetComStateByTranspObject(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetGetComAvailabilityByTranspObj()
 *********************************************************************************************************************/
/*! \brief          Gets the communication availability for a transport object.
 *  \details        This flag represents if a tester is (re-)connected to the ECU. It is only relevant for connections
 *                  which support the "channel ready indication" (DCM_NET_CONN_PROP_READY_INDICATION_ENABLED). It is
 *                  initialized as FALSE and must be set by Dcm_SetChannelReady to TRUE.
 *  \param[in]      pTranspObj    The pointer to a transport object
 *  \return         The communication availability of the assigned transport object
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_NetComMAvailabilityType, DCM_CODE) Dcm_NetGetComAvailabilityByTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetReloadAuthTimer()
 *********************************************************************************************************************/
/*! \brief          Reload the Authentication Timer if neccessary.
 *  \details        -
 *  \param[in]      pTranspObj    The pointer to a transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetReloadAuthTimer(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetUsdtTxConfirmation()
 *********************************************************************************************************************/
/*! \brief          Transport object transmission finalization.
 *  \details        Finalization of a transmitted USDT message.
 *  \param[in]      pTranspObj    The transport object used during transmission
 *  \param[in]      result        The transmission result
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUsdtTxConfirmation(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType result
  );

/**********************************************************************************************************************
 *  Dcm_NetAllocateOrGetTranspObject()
 *********************************************************************************************************************/
/*! \brief          Transport object allocation.
 *  \details        Allocates a transport object for a concrete client connection.
 *  \param[in]      connHdl            The connection identifier
 *  \param[out]     pTranspObj         The allocated transport object
 *  \return         BUFREQ_OK          The operation was successful
 *  \return         BUFREQ_E_NOT_OK    The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Must be called within a critical section to guarantee unique result
 *********************************************************************************************************************/
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetAllocateOrGetTranspObject(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetGetTranspObjOfConnection()
 *********************************************************************************************************************/
/*! \brief          Returns the transport object of the given connection.
 *  \details        -
 *  \param[in]      connHdl            The connection identifier
 *  \param[out]     pTranspObj         The transport object assigned to the given connection
 *  \return         BUFREQ_OK          The operation was successful
 *  \return         BUFREQ_E_NOT_OK    The given index was invalid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            Since only getter, can be called outside of a critical section, but for consistent results do call within critical section
 *********************************************************************************************************************/
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetGetTranspObjOfConnection(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetTranspObjRelease()
 *********************************************************************************************************************/
/*! \brief          Release transport object resource.
 *  \details        Releases a given transport object.
 *  \param[in]      pTranspObj    The transport object to be released
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetTranspObjRelease(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetStartOfReception()
 *********************************************************************************************************************/
/*! \brief          Start of new request reception.
 *  \details        This function is called by Dcm_StartOfReception.
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[in]      tpSduLength        The total length of the TP message
 *  \param[out]     bufferSizePtr      Available buffer size
 *  \param[in]      addTObjFlags       Additional transport object flags from caller
 *  \return         BUFREQ_OK          A free buffer is available - start copy data
 *  \return         BUFREQ_E_NOT_OK    No free buffer is available - ignore request
 *  \return         BUFREQ_E_OVFL      The request message is too long for the configured buffer
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetStartOfReception(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  PduLengthType tpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr,
  Dcm_NetTransportObjectFlagType addTObjFlags
  );

/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveRequest()
 *********************************************************************************************************************/
/*! \brief          Check whether request is functional TesterPresent with SPRMIB set.
 *  \details        -
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      info               Message context (data and length of the portion)
 *  \param[in]      neededLength       The length to be a TesterPresent  message
 *  \return         TRUE               Request is a functional TesterPresent with SPRMIB set
 *  \return         FALSE              Request is not a functional TesterPresent with SPRMIB set
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveRequest(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  uint8 neededLength
  );

/**********************************************************************************************************************
 *  Dcm_NetSetComModeEntered()
 *********************************************************************************************************************/
/*! \brief          Internal utility for updating the new communication state.
 *  \details        -
 *  \param[in]      networkId      The communication channel on which this state is now active
 *  \param[in]      newComState    The new state of the communication channel (e.g. DCM_NET_COMM_STATE_RX_DIS_TX_DIS)
 *  \context        ISR1|ISR2
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetSetComModeEntered(
  NetworkHandleType networkId,
  Dcm_NetComMStateType newComState
  );

/**********************************************************************************************************************
 *  Dcm_NetSetChannelReady()
 *********************************************************************************************************************/
/*! \brief          Internal utility for updating the new communication availability.
 *  \details        -
 *  \param[in]      networkId      The communication channel on which this state is now active
 *  \context        ISR1|ISR2
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetSetChannelReady(
  NetworkHandleType networkId
  );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of Net unit */
DCM_LOCAL VAR(Dcm_NetSingletonContextType, DCM_VAR_NOINIT) Dcm_NetSingletonContext;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled(
  void
  )
{
  return (DCM_NET_KEEP_ALIVE_TIME_START_ON_FUNC_3E_80_ENABLED == STD_ON);
}

/**********************************************************************************************************************
 *  Dcm_NetCommonInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetCommonInit(
  Dcm_ConfigPtrType configPtr
  )
{
  Dcm_NetPbCfgInit(configPtr);                                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

  Dcm_NetSingletonContext.NumActiveConnections = 0u;
  Dcm_NetSingletonContext.ActiveProtocol = DCM_NET_INVALID_PROTID;
  Dcm_NetSingletonContext.ActiveDiagnostic = DCM_NET_COMM_ACTIVE;
  Dcm_NetSingletonContext.RxAllowed = TRUE;
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
  Dcm_NetSingletonContext.KeepAlive = FALSE;
#endif

  /* Initialization value should never be used, nevertheless initialize this variable for better robustness */
  Dcm_NetSingletonContext.SessionConnection = 0u;
}

/**********************************************************************************************************************
 *  Dcm_NetComMInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetComMInit(
  void
  )
{
  Dcm_CfgNetNetIdRefOptType lNetHandleIter;

  for(lNetHandleIter = 0; lNetHandleIter < Dcm_NetPbCfgGetNumComMChannels(); ++lNetHandleIter)
  {
    Dcm_NetComMContextPtrType pComMContext;

    pComMContext = Dcm_NetPbCfgGetComMContext(lNetHandleIter);

    pComMContext->ComState           = DCM_NET_COMM_STATE_RX_DIS_TX_DIS;                                                                             /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    pComMContext->ComAvailability    = DCM_NET_COMM_CHANNEL_NOT_READY;                                                                               /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    pComMContext->RegisteredNetworks = 0u;                                                                                                           /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetBufferInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetBufferInit(
  void
  )
{
  Dcm_CfgNetBufferRefOptType lBufferIter;

  for(lBufferIter = 0; lBufferIter < Dcm_NetPbCfgGetNumBuffers(); ++lBufferIter)
  {
    Dcm_NetBufferContextPtrType pBufferContext;

    pBufferContext = Dcm_NetPbCfgGetBufferContext(lBufferIter);

    pBufferContext->IsInUseCnt = 0u;                                                                                                                 /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetConnectionInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetConnectionInit(
  void
  )
{
  Dcm_NetConnRefOptType lConnIter;

  for(lConnIter = 0; lConnIter < Dcm_NetPbCfgGetNumConnections(); ++lConnIter)
  {
    Dcm_NetPbCfgPutToConn2TObjMap(lConnIter, DCM_NET_INVALID_TOBJID);
  }
}

/**********************************************************************************************************************
 *  Dcm_NetTranspObjInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTranspObjInit(
  void
  )
{
  Dcm_CfgNetTObjHandleOptType lTObjIter;

  for(lTObjIter = 0; lTObjIter < Dcm_NetPbCfgGetNumTransportObjects(); ++lTObjIter)
  {
    Dcm_NetTransportObjectPtrType pTranspObj;

    pTranspObj = Dcm_NetPbCfgGetTransportObject(lTObjIter);

    pTranspObj->ResType  = DCM_NET_TOBJ_RESTYPE_NONE;                                                                                                /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pTranspObj->State    = DCM_NET_TOBJ_STATE_FREE;                                                                                                  /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pTranspObj->Flags    = DCM_NET_TOBJ_FLAG_NONE;                                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    Dcm_NetWriteAddBufferU8At(pTranspObj, 0, 0x7Fu); /* always used for negative responses */                                                        /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
    pTranspObj->Handle   = (Dcm_CfgNetTObjHandleMemType)lTObjIter;                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
  }
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgInit(
  void
  )
{
  /* Initialize periodic transmission manager state machine */
  Dcm_NetPeriodicMsgResetConnection();
  Dcm_NetPeriodicMsgInitDelayCntr();                                                                                                                 /* SBSW_DCM_GLOBAL_PTR_WRITE */
  Dcm_NetPeriodicMsgInitFastestRate();                                                                                                               /* SBSW_DCM_GLOBAL_PTR_WRITE */
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  {
    /* Initialization of delay timers */
    uint8_least lDelayTmrIter;
    for (lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
    {
      Dcm_NetDelayTimerSet(lDelayTmrIter, 0);
    }

    Dcm_NetPeriodicMsgResetNextDelayTimer();                                                                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
  }
# endif
  {
    Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;
    for(lPerTxObjIter = 0; lPerTxObjIter < Dcm_NetPbCfgGetNumPerTxObjects(); ++lPerTxObjIter)
    {
      Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

      pPeriodicTxObj = Dcm_NetPbCfgGetPeriodicTxObject(lPerTxObjIter);

      /* Initialize the transport object */
      pPeriodicTxObj->Timer = 0u;                                                                                                                    /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_FREE;                                                                                        /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      pPeriodicTxObj->TxPduInfo.SduDataPtr = pPeriodicTxObj->TxBuffer;                                                                               /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
    }
  }
}
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetOnRequestDetection()
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
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetOnRequestDetection(                                                                                     /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;
  BufReq_ReturnType             lBufReqResult;

  /* Retrieve the transport object from the given connection */
  lBufReqResult = Dcm_NetGetTranspObjOfConnection(connHdl, &pTranspObj);                                                                             /* SBSW_DCM_POINTER_FORWARD_STACK */

  /* Check whether a transport object is in use */
  if(lBufReqResult == BUFREQ_OK)
  {
    /* Enter critical section (Reason: the transport object may not be modified from outside (e.g. Dcm_TpTxConfirmation releases it) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    /*  Retrieve again the transport object from connection to guarantee it has still the same state */
    lBufReqResult = Dcm_NetGetTranspObjOfConnection(connHdl, &pTranspObj);                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

    /* Check the connection still uses the same transport object (it is always 1:1 association between connection and a transport object) */
    if(lBufReqResult == BUFREQ_OK)                                                                                                                   /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY TX */
    {
      /* Check whether the connection is free */
      if(pTranspObj->State != DCM_NET_TOBJ_STATE_FREE)
      {
        /* Check if suppression of responses on functional addressed requests is enabled */
        if( (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_TX_ON_FUNC_RX))
          &&(Dcm_NetPbCfgIsFuncRequest(pTranspObj->RxPduId) == TRUE) )                                                                               /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        {
          /*
           * Interruption of functional addressed request is not necessary because response will already be
           * suppressed
           */
        }
        /* Otherwise, interrupt ongoing service processing */
        else
        {
          /* If a new request is being received: */
          if(pTranspObj->State == DCM_NET_TOBJ_STATE_ONRX)
          {
            /* Isolate this connection */
            pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_CANCELED;                                                                                         /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */

            /* Terminate the ongoing reception immediately */
            (void)PduR_DcmCancelReceive(pTranspObj->RxPduId);
          }
          /* Otherwise, let the task cancel any pending jobs */
          else
          {
            /* Mark transport object as obsolete. This is used to distinguish between two allocations of the same transport object before the Dcm_NetTaskTaMonitor is called */
            pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_OBSOLETE;                                                                                         /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
            Dcm_TskSetEvent(DCM_TSK_ID_NET_TA_MONITOR, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle));
          }
        }
      }/* else - nothing to interrupt */
    }/* else - request no more in progress -> ignore */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }/* else - no request in progress or different tester -> ignore */
}
#endif

#if (DCM_NET_PROTOCOL_NOTIFICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetExecStartProtocolOp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetExecStartProtocolOp(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  boolean lResult = TRUE;

  for (uint16_least lIter = 0u; lIter < DCM_CFGNETPROTOCOLNOTIFICATIONINFO_SIZE; ++lIter)                                                            /* PRQA S 2842 */ /* MD_Dcm_DerefInvalidPointer */
  {
    if(Dcm_CfgNetProtocolNotificationInfo[lIter].StartFunc(Dcm_NetPbCfgGetProtocolId(Dcm_NetGetProtIdOfTranspObj(pTranspObj))
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_450)
                                                          ,Dcm_NetPbCfgGetConnId(pTranspObj->ConnHdl)
                                                          ,pTranspObj->ClientSrcAddr
# endif
                                                          ) != DCM_E_OK)                                                                             /* SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC */
    {
      lResult = FALSE;
      break;
    }
  }

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetProcessStartProtocol()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetProcessStartProtocol(                                                                                   /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_NetTransportObjectPtrType pTranspObj,
  boolean stopActiveProtocol
  )
{
#if (DCM_NET_PROTOCOL_NOTIFICATION_ENABLED == STD_ON)
  if(!Dcm_NetExecStartProtocolOp(pTranspObj))                                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    /* Neg response 0x22 */
    Dcm_NetComposeAddNegResponse(pTranspObj
                                ,DCM_NET_TOBJ_RESTYPE_SIMPLE
                                ,DCM_E_CONDITIONSNOTCORRECT);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
  }
  else
#endif
  {
    if(stopActiveProtocol == TRUE)                                                                                                                   /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
    {
      uint8 lThreadId = Dcm_NetGetThreadIdOfSessionConnection();

      Dcm_DiagCancelProcessing(lThreadId);

      /* According to AR DCM SWS the session switch is executed at start protocol, not at stop! */
      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_WORK, DCM_TSK_EV_DIAG_WORK_INT_SET2DEF, lThreadId);
    } /* else - no protocol enabled -> no need to reset anything in case that parallelization is not enabled */

    Dcm_NetSingletonContext.ActiveProtocol = Dcm_NetGetProtIdOfTranspObj(pTranspObj); /* Set new active protocol */

    /* Update P2 timings at once */
    Dcm_DiagSetP2Timings(0, Dcm_NetGetProtIdOfTranspObj(pTranspObj), Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj));                        /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_DiagRxIndication(pTranspObj);                                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationFindWinner()
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
 */
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetTObjHandleOptType, DCM_CODE) Dcm_NetRxPrioritisationFindWinner(
  Dcm_TskTaskEvOptType ev,
  Dcm_TskTaskEvPtrType pBusyEvents
  )
{
  Dcm_TskTaskEvOptType lEv = ev;
  uint16 lMaxPrio = 0x100u;/* lower than the lowest configurable priority (256 > 255) */

  Dcm_CfgNetTObjHandleOptType lTObjCntr = 0u;
  Dcm_CfgNetTObjHandleOptType lTranspObjWinnerId = DCM_NET_INVALID_TOBJID;

  *pBusyEvents = 0;                                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  DCM_TSK_EVENT_SCAN_LOOP(lEv)
  {
    /* Scan all arrived transport objects */
    if (Dcm_TskIsLocalEventSet(lEv, 0x01u)) /* start scan from the LSB since TObjects are allocated by reversed iteration and thus -> first allocated (last checked here) will be the winner */
    {
      uint8 tmpProtPrio = Dcm_NetPbCfgGetProtocolPriorityByConnHdl(Dcm_NetPbCfgGetTransportObject(lTObjCntr)->ConnHdl);                              /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */

      /* Higher priority */
      if(lMaxPrio <= tmpProtPrio)
      {
        /* Lower priority than current request/pending response */
        *pBusyEvents |= Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, lTObjCntr);                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      else
      {
        if(lTranspObjWinnerId < DCM_NET_INVALID_TOBJID)
        {
          /* Mark busy for the last winner */
          *pBusyEvents |= Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, lTranspObjWinnerId);                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }

        /* Update maximum priority */
        lMaxPrio = tmpProtPrio;

        /* potential winner found */
        lTranspObjWinnerId = lTObjCntr;
      }
    }
    ++lTObjCntr;
  }
  return lTranspObjWinnerId;
}

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessWinner()
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
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessWinner(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_TskTaskEvPtrType pBusyEvents
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if(Dcm_NetGetProtIdOfActiveProtocol() != Dcm_NetGetProtIdOfTranspObj(pTranspObj))/* the winner is a different protocol as the old one -> interrupt the old protocol */
  {
    /* very first protocol activation - start the protocol/send NRC 0x21 responses */
    if( (Dcm_NetGetProtIdOfActiveProtocol() >= DCM_NET_INVALID_PROTID)
#if (DCM_NET_PROTOCOL_PARALLELISATION_ENABLED == STD_ON)                                                                                             /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
      ||(Dcm_StateIsDefaultSessionPending() == TRUE) /* if default session ... */                                                                    /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#endif
      )
    {
      Dcm_NetProcessStartProtocol(pTranspObj, FALSE);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
#if (DCM_NET_PROTOCOL_PARALLELISATION_ENABLED == STD_ON)
      if( (Dcm_NetPbCfgGetProtocolPriority(Dcm_NetGetProtIdOfTranspObj(pTranspObj))
           < Dcm_NetPbCfgGetProtocolPriority(Dcm_NetGetProtIdOfSessionConnection())) /* new one is higher priority than session owner */
#else
      if( (Dcm_NetPbCfgGetProtocolPriority(Dcm_NetGetProtIdOfTranspObj(pTranspObj))
           < Dcm_NetPbCfgGetProtocolPriority(Dcm_NetGetProtIdOfActiveProtocol())) /* new one is higher priority */
        ||( (((DCM_DIAG_QUEUE_FLAG_IS_ACTIVE | DCM_DIAG_QUEUE_FLAG_IS_WAITING)
              & Dcm_DiagGetQueuedSetFlags(pContext->ThreadId)) == 0u ) /* if idle or post-processing (considered for back-to-back requests) ... */   /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
          &&(Dcm_StateIsDefaultSessionPending() == TRUE) ) /* ... still default session - continue with new request */                               /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#endif
        )
      {
        Dcm_NetProcessStartProtocol(pTranspObj, TRUE);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      else
      {
        /* Mark busy for the winner */
        *pBusyEvents |= Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle);                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
  }
  else /* same protocol - proceed with the new request/NRC 0x21 responses */
  {
    Dcm_DiagRxIndication(pTranspObj);                                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetRxPrioritisationProcessLoosers()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetRxPrioritisationProcessLoosers(
  Dcm_TskTaskEvOptType BusyEvents
  )
{
  Dcm_CfgNetTObjHandleOptType lTObjCntr = 0u;
  Dcm_TskTaskEvOptType        lBusyEvents = BusyEvents;

  DCM_TSK_EVENT_SCAN_LOOP(lBusyEvents)
  {
    /* Check who did just loose */
    if (Dcm_TskIsLocalEventSet(lBusyEvents, 0x01u))
    {
      Dcm_NetTransportObjectPtrType pTranspObj = Dcm_NetPbCfgGetTransportObject(lTObjCntr);

      Dcm_NetBufferRelease(pTranspObj->BufferHdl);
      pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_IGNORE;                                                                                                 /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
      pTranspObj->Flags |= DCM_NET_TOBJ_FLAG_BUSY;                                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
#else
# if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
      if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u) /* internal requests do not manage the ComM state */
# endif
      {
        Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);
      }
      Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                               /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
#endif
    }
    ++lTObjCntr;
  }
}

/**********************************************************************************************************************
 *  Dcm_NetTransmitUsdtResponse()
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
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTransmitUsdtResponse(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  boolean doCallTxConfirmation = TRUE;

  /* The Tx path must be still "ready" here. if not, check the caller - it shall take care about this state! */
  Dcm_DebugAssert((pTranspObj->State == DCM_NET_TOBJ_STATE_PREPTX), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                          /* COV_DCM_RTM_DEV_DEBUG TX */

  /* Prepare the given transport object for USDT response transmission (set all relevant attributes to their initial values) */
  pTranspObj->BuffPos = 0u;                                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->State = DCM_NET_TOBJ_STATE_ONTX;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

  /* If the USDT response shall be suppressed for any non-diagnostic but transport protocol reasons (e.g. no response on functional requests or no TxPduId configured), do: */
  if( (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_TX_ON_FUNC_RX))
    &&(Dcm_NetPbCfgIsFuncRequest(pTranspObj->RxPduId) == TRUE)                                                                                       /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
    &&(pTranspObj->ResType != DCM_NET_TOBJ_RESTYPE_FBL_FINALRSP)
#endif
    )
  {
    lStdResult = DCM_E_OK; /* No response on functional requests required -> leave with success */
  }
  else if (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_NO_MAIN_TX))
  {
    lStdResult = DCM_E_OK; /* No associated TxPduId -> leave with success */
  }
  else
  {
    /* Otherwise, if the communication mode on the ComM channel associated with the transport object still allows any response transmissions: */
    if((DCM_NET_COMM_STATE_FLAG_TX_EN & Dcm_NetGetComStateByTranspObject(pTranspObj)) != 0u)                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      PduInfoType    lTpTransmitData;
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      uint8          lMetaData[DCM_NET_METADATA_LENGTH];
#endif

#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      /* In case of generic connection, the transmit data references the meta data, otherwise, it references the payload. */
      if(Dcm_NetIsGenericConnection(pTranspObj->ConnHdl))
      {
        lMetaData[DCM_NET_METADATA_SA_OFFSET] = Dcm_NetPbCfgGetEcuAddress(pTranspObj->ConnHdl);                                                      /* SBSW_DCM_LOCAL_ARRAY_WRITE */
        lMetaData[DCM_NET_METADATA_TA_OFFSET] = (uint8)pTranspObj->ClientSrcAddr; /* safe cast, since ClientSrcAddr is source address of MetaData because ConnHdl is generic */ /* SBSW_DCM_LOCAL_ARRAY_WRITE */
        lTpTransmitData.SduDataPtr = &lMetaData[0];
      }
      else
#endif
      {
        lTpTransmitData.SduDataPtr = pTranspObj->BuffInfo.SduDataPtr;
      }

      /* The length of the Sdu does not consider the meta data. */
      lTpTransmitData.SduLength = pTranspObj->BuffInfo.SduLength;

      lStdResult = PduR_DcmTransmit(Dcm_NetPbCfgGetConnTxPduIdMain(pTranspObj->ConnHdl)
                                   ,&lTpTransmitData);                                                                                               /* SBSW_DCM_POINTER_FORWARD_STACK */

      /* If this transmission request was accepted, wait for the Tx-Confirmation from the XxxTp */
      if(lStdResult == E_OK)
      {
        doCallTxConfirmation = FALSE;
      }
    }
  }
  /* Otherwise, take the shortcut and loop-back through the USDT message confirmation with failed result (this response had to be sent, but couldn't and no retry is allowed (SWS)) */

  /* Make a loop-back and finalize transmission */
  if(doCallTxConfirmation == TRUE)
  {
    Dcm_NetUsdtTxConfirmation(pTranspObj, lStdResult);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationNotOk()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationNotOk(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) == 0u) /* consider only real buffer usage */                                                       /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
#endif
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
  }
  Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndicationOk()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndicationOk(                                                                                       /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u) /* CancelReceive was called before -> ignore request although reception couldn't be canceled */
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
#endif
  {
    boolean keepTranspObj;
    keepTranspObj = Dcm_DiagAcceptNewRequest(pTranspObj);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    pTranspObj->State = DCM_NET_TOBJ_STATE_RX_END;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

    if(keepTranspObj == TRUE)
    {
      Dcm_NetRegisterComMActivity(pTranspObj);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) != 0u)
      {
        Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_USDT);
      }
      else
#endif
      {
        /* Store SID for RCR-RP/busy on protocol prioritisation responses */
        Dcm_NetWriteAddBufferU8At(pTranspObj, 1u, pTranspObj->RxData[0]);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        /* Signal processing start */
        Dcm_TskSetEventByThread(DCM_TSK_ID_NET_RX, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle), Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj)); /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
    }
    else
    {
      if (Dcm_NetIsKeepAliveTimeStartOnFunc3E80Enabled())
      {
        if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_IGNORE) == 0u)
        {
          /* Register at ComM */
          Dcm_NetRegisterComMActivity(pTranspObj);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
          Dcm_NetSingletonContext.KeepAlive = TRUE;
          /* Unregister at ComM */
          Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);
        }
      }
      Dcm_NetTpRxIndicationNotOk(pTranspObj);                                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
}

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerGet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_TmrTimerCntrOptType, DCM_CODE) Dcm_NetDelayTimerGet(
  uint8_least timer
  )
{
  return Dcm_NetSingletonContext.PeriodicTxContext.DelayTimer[timer];
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetDelayTimerSet(
  uint8_least timer,
  Dcm_TmrTimerCntrOptType timeout
  )
{
  if (Dcm_DebugDetectRuntimeError(timer >= DCM_NET_DELAY_BULK_TRANSMISSION))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    Dcm_NetSingletonContext.PeriodicTxContext.DelayTimer[timer] = (Dcm_TmrTimerCntrMemType)timeout;                                                  /* SBSW_DCM_POINTER_WRITE_DELAYTIMER */
  }
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetDelayTimerExpired()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetDelayTimerExpired(
  uint8_least timer
  )
{
  return (boolean)(Dcm_NetSingletonContext.PeriodicTxContext.DelayTimer[timer] == 0u);                                                               /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerSet(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  )
{
  Dcm_SplitTaskEnterCS();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  pPeriodicTxObj->Timer = timeout;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_SplitTaskLeaveCS();
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStart(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  Dcm_TmrTimerCntrMemType timeout
  )
{
  Dcm_NetPTxObjTimerSet(pPeriodicTxObj, timeout);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (!Dcm_TmrIsTimerRunning(DCM_TMR_ID_PERIODIC_TX))
  {
    Dcm_TmrStartTimer(DCM_TMR_ID_PERIODIC_TX, 1u);
  }
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxObjTimerStop()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxObjTimerStop(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  )
{
  Dcm_NetPTxObjTimerSet(pPeriodicTxObj, 0u);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgIncNextDelayTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgIncNextDelayTimer(
  void
  )
{
  ++Dcm_NetSingletonContext.PeriodicTxContext.NextDelayTimer;                                                                                        /* SBSW_DCM_GLOBAL_PTR_WRITE */ /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */
  if (Dcm_NetPeriodicMsgGetNextDelayTimer() >= DCM_NET_DELAY_BULK_TRANSMISSION)
  {
    Dcm_NetPeriodicMsgResetNextDelayTimer();                                                                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
  }
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPTxDelayTimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPTxDelayTimerStart(
  void
  )
{
  Dcm_NetDelayTimerSet(Dcm_NetPeriodicMsgGetNextDelayTimer()
                      ,Dcm_NetGetPeriodicDelayTime(Dcm_NetPeriodicMsgGetFastestRate()));
  Dcm_NetPeriodicMsgIncNextDelayTimer();

  /* Make sure system timer for periodic tx is running */
  Dcm_TmrStartTimer(DCM_TMR_ID_PERIODIC_TX, 1u);
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutTxObjectHandler()
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
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutTxObjectHandler(
  void
  )
{
  Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;
  boolean                       lReloadTimer = FALSE;

  for(lPerTxObjIter = 0; lPerTxObjIter < Dcm_NetPbCfgGetNumPerTxObjects(); ++lPerTxObjIter)
  {
    Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

    pPeriodicTxObj = Dcm_NetPbCfgGetPeriodicTxObject(lPerTxObjIter);
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    Dcm_UtiEnterCriticalSection(); /* protect against high level interrupts from the network interface */
    if(!Dcm_NetPTxObjTimerExpired(pPeriodicTxObj))
    {
      Dcm_NetPTxObjTimerSet(pPeriodicTxObj, (Dcm_TmrTimerCntrMemType)(Dcm_NetPTxObjTimerGet(pPeriodicTxObj) - 1u));                                  /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      if(Dcm_NetPTxObjTimerExpired(pPeriodicTxObj))
      {
        (void)PduR_DcmCancelTransmit(Dcm_NetPeriodicMsgGetTxPduIdCurrConn(lPerTxObjIter)); /* try to cancel still ongoing Tx */                      /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
        Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                   /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      }
      else
      {
        lReloadTimer = TRUE; /* at least one active timer - reload timer */
      }
    }
    Dcm_UtiLeaveCriticalSection();
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
  }

  return lReloadTimer;
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutDelayTimerHandler()
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
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_OnTimeoutDelayTimerHandler(
  void
  )
{
  uint8_least lDelayTmrIter;
  boolean     lReloadTimer = FALSE;
  boolean     lSetTxEvent = FALSE;
  for(lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
  {
    if(Dcm_NetDelayTimerExpired(lDelayTmrIter) == FALSE)
    {
      Dcm_NetDelayTimerSet(lDelayTmrIter, (Dcm_TmrTimerCntrOptType)(Dcm_NetDelayTimerGet(lDelayTmrIter)-1u));
      if(Dcm_NetDelayTimerExpired(lDelayTmrIter) == TRUE)
      {
        Dcm_NetPeriodicMsgIncDelayCntr();                                                                                                            /* SBSW_DCM_GLOBAL_PTR_WRITE */ /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */
        lSetTxEvent = TRUE;
      }
      else
      {
        lReloadTimer = TRUE; /* at least one active timer - reload timer */
      }
    }
  }

  if (lSetTxEvent == TRUE)
  {
    Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_UUDT);
  }

  return lReloadTimer;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmitTxObj()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmitTxObj(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  )
{
  Std_ReturnType lStdResult;
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

  pPeriodicTxObj = Dcm_NetPbCfgGetPeriodicTxObject(perTxObjHandle);

  if ((DCM_NET_COMM_STATE_FLAG_TX_EN & Dcm_NetGetComStateByConnection(Dcm_NetPeriodicMsgGetConnection())) != 0u)
  {
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
    if (Dcm_NetPeriodicMsgGetDelayCntr() == 0u)
    {
      /* Retry sending */
      lStdResult = DCM_E_OK;
    }
    else
# endif
    {
      P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pPduInfo;

      Dcm_NetPeriodicMsgDecDelayCntr();                                                                                                              /* SBSW_DCM_GLOBAL_PTR_WRITE */ /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */

      pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_ONTX;                                                                                        /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
      /* Note: In case the following two lines are not executed within 100ms, the timeout may occur before the message
       * was sent (only if split tasks used). This may lead to inconsistent periodic data, since the transmitted
       * message is free to be reused again while it is on transmission. The design avoiding this very rare scenario
       * will increase the software complexity unnecessarily. Entering such conditions means that the ECU is not
       * capable of holding the periodic rates too.
       */
      Dcm_NetPTxObjTimerStart(pPeriodicTxObj, Dcm_NetPbCfgGetPerTxConfTimeOutOfConn(Dcm_NetPeriodicMsgGetConnection()));                             /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */

      pPduInfo = &(pPeriodicTxObj->TxPduInfo);
      lStdResult = PduR_DcmTransmit(Dcm_NetPeriodicMsgGetTxPduIdCurrConn(perTxObjHandle), pPduInfo);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
  else
  {
    lStdResult = DCM_E_NOT_OK;
  }

  if(lStdResult != DCM_E_OK)
  {
    /* Do not retry transmission -> make a loop-back and finalize transmission */
    Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                       /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetIsPduInfoValid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_NetIsPduInfoValid(
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info
  )
{
  boolean lResult;

  if(info == NULL_PTR)
  {
    lResult = FALSE;
  }
  else if( (info->SduLength  != 0u)
         &&(info->SduDataPtr == NULL_PTR) )
  {
    lResult = FALSE;
  }
  else
  {
    lResult = TRUE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetCopyRxData()
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
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyRxData(                                                                                /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* If the XxxTp (e.g. CanTp or FrTp) requests Dcm to copy any data: */
  if(info->SduLength != 0u) /* any payload now? */
  {
    /* ----- Development Error Checks -------------------------------------- */
    if (Dcm_DebugDetectError(pTranspObj->BuffPos >= pTranspObj->RxLength))
    {
      lErrorId = DCM_E_PARAM;
    }
    else if(Dcm_DebugDetectError(((PduLengthType)(pTranspObj->RxLength) - (PduLengthType)(pTranspObj->BuffPos)) < info->SduLength))
    {
      lErrorId = DCM_E_PARAM;
    }
    else
    {
      /* ----- Implementation ---------------------------------------------- */
#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
      /* If connection has been yet canceled (if RxPath supports cancellation), do return failed copy data result */
      if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u)
      {
        lResult = BUFREQ_E_NOT_OK;
      }
      else
#endif
      {

        /* If the transmission is for a main-client (full request reception), do copy the data portion provided by the XxxTp */
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
        if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) != 0u)
        {
          /* For the very first call of this function for this request reception, do copy the request head (SID and SubFunction) for later usage (i.e. for 0x3E 0x80 detection) */
          if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_COPYHEAD) != 0u)
          {
            Dcm_UtiBitOpClr(Dcm_NetTransportObjectFlagType, pTranspObj->Flags, DCM_NET_TOBJ_FLAG_COPYHEAD);  /* Toggle flag for next calls to not extract any SID/SUBSID data */ /* SBSW_DCM_PARAM_PTR_WRITE */
            Dcm_NetWriteAddBufferU8At(pTranspObj, 1, info->SduDataPtr[0]); /* copy the SID */                                                        /* PRQA S 2822 */ /* MD_Dcm_2822 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
            Dcm_NetWriteAddBufferU8At(pTranspObj, 2, info->SduDataPtr[1]); /* copy the SUBSID */                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
          }
        }
        else
#endif
        {
          Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

          lBufferInfo = Dcm_NetPbCfgGetBufferInfo(pTranspObj->BufferHdl);

          Dcm_UtiMemCopySafe(info->SduDataPtr
                            ,lBufferInfo->BufferPtr
                            ,pTranspObj->BuffPos
                            ,lBufferInfo->Size
                            ,info->SduLength);                                                                                                       /* SBSW_DCM_POINTER_WRITE_BUFFERINFO */
        }
        /* Update the transport object's buffer-copy-progress */
        pTranspObj->BuffPos += (Dcm_CfgNetBufferSizeMemType)(info->SduLength);                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

        /* Report the remaining available DCM buffer size to the XxxTp */
        *bufferSizePtr = (PduLengthType)(pTranspObj->RxLength) - (PduLengthType)(pTranspObj->BuffPos);                                               /* SBSW_DCM_PARAM_PTR_WRITE */
        lResult = BUFREQ_OK;
      }
    }
    /* Otherwise if connection has been canceled, return negative result to signal the XxxTp to abort the transmission (if RxPath supports cancellation) */
  }
  else
  {
    /* Otherwise, the XxxTp requests Dcm only to report the available buffer size  */
    *bufferSizePtr = (PduLengthType)(pTranspObj->RxLength) - (PduLengthType)(pTranspObj->BuffPos); /* Notify lower layer about available buffer space */ /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = BUFREQ_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYRXDATA, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetTpRxIndication()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetTpRxIndication(
  PduIdType rxPduId,
  Std_ReturnType result
  )
{
  Dcm_NetTransportObjectPtrType pTranspObj;
  BufReq_ReturnType             lBufReqResult;
  lBufReqResult = Dcm_NetGetTranspObjOfConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId), &pTranspObj);                                            /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_FORWARD_STACK */

  /* If no transport object has been associated with this connection yet, do ignore this indication */
  if(lBufReqResult == BUFREQ_OK)
  {
    /* Otherwise, if the transport object is determined to belong to the expected request, that means: */
    if( (pTranspObj->State == DCM_NET_TOBJ_STATE_ONRX) /* expected state for Rx indication? */
      &&(rxPduId == pTranspObj->RxPduId) ) /* Consider also any nested request on different TP connection but from the same tester! */
    {
      if(result != DCM_NET_ARENV_NTFRSLT_OK)
      {
        Dcm_NetTpRxIndicationNotOk(pTranspObj);                                                                                                      /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
      else
      {
        Dcm_NetTpRxIndicationOk(pTranspObj);                                                                                                         /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
    }/* Otherwise, just ignore this call since this API call is most probably due to a (concurrent request (i.e. 0x3E 0x80) from the same tester) OR
      * (indication of unexpected connection) */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetCopyLinearTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyLinearTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError( (info->SduLength != 0u)
                         &&(pTranspObj->BuffPos >= pTranspObj->BuffInfo.SduLength) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError((PduLengthType)(pTranspObj->BuffInfo.SduLength - pTranspObj->BuffPos) < info->SduLength))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Copy requested amount of data to the XxxTp */
    Dcm_UtiMemCopySafe(&(pTranspObj->BuffInfo.SduDataPtr[pTranspObj->BuffPos])
                      ,info->SduDataPtr
                      ,0u
                      ,info->SduLength
                      ,info->SduLength);                                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    /* Update Tx-buffer read progress */
    pTranspObj->BuffPos += (Dcm_CfgNetBufferSizeMemType)(info->SduLength);                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Report remaining amount of data in the Tx-buffer */
    *availableDataPtr = (PduLengthType)(pTranspObj->BuffInfo.SduLength - pTranspObj->BuffPos);                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = BUFREQ_OK;
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYTXDATA, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetCopyTxData(
  Dcm_NetTransportObjectPtrType pTranspObj,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  )
{
  BufReq_ReturnType lResult;

  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u)
  {
    lResult = BUFREQ_E_NOT_OK;
  }
  else
  {
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
    if(pTranspObj->ResType == DCM_NET_TOBJ_RESTYPE_PAGED)
    {
      lResult = Dcm_PagedBufferCopyData(Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(pTranspObj), info, availableDataPtr);                             /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
#endif
    {
      lResult = Dcm_NetCopyLinearTxData(pTranspObj, info, availableDataPtr);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetComStateByTranspObject()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_NetComMStateType, DCM_CODE) Dcm_NetGetComStateByTranspObject(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetGetComStateByConnection((pTranspObj)->ConnHdl);
}

/**********************************************************************************************************************
 *  Dcm_NetGetComAvailabilityByTranspObj()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_NetComMAvailabilityType, DCM_CODE) Dcm_NetGetComAvailabilityByTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  return Dcm_NetGetComAvailabilityByConnection((pTranspObj)->ConnHdl);
}

/**********************************************************************************************************************
 *  Dcm_NetReloadAuthTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_NetReloadAuthTimer(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
#if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
  if ( (Dcm_NetPbCfgGetConnAuthInfoRef(pTranspObj->ConnHdl) != DCM_AUTHMGR_INVALID_AUTH_INFO_REF) &&
        ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u) )
  {
    Dcm_AuthMgrTimerReload(Dcm_NetPbCfgGetConnAuthInfoRef(pTranspObj->ConnHdl), DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT);
  }
#else
  DCM_IGNORE_UNREF_PARAM(pTranspObj);
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_NetAllocateOrGetTranspObject()
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
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetAllocateOrGetTranspObject(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  )
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK; /* No free transport object found */

  if(Dcm_DebugDetectRuntimeError(connHdl >= Dcm_NetPbCfgGetNumConnections()))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    if(Dcm_NetPbCfgGetTObjConn2TObjMap(connHdl) >= DCM_NET_INVALID_TOBJID)/* check if already assigned */
    {
      Dcm_CfgNetTObjHandleOptType lTObjIter;

      for(lTObjIter = 0; lTObjIter < Dcm_NetPbCfgGetNumTransportObjects(); ++lTObjIter)
      {
        if(Dcm_NetPbCfgGetTransportObject(lTObjIter)->State == DCM_NET_TOBJ_STATE_FREE)
        {
          Dcm_NetPbCfgPutToConn2TObjMap(connHdl, (Dcm_CfgNetTObjHandleMemType)lTObjIter); /* Assign ownership information */
          *pTranspObj = Dcm_NetPbCfgGetTransportObject(lTObjIter);                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
          lResult = BUFREQ_OK;
          break;
        }/* Transport Object is in use - check next one */
      }
    }
    else
    {
      *pTranspObj = Dcm_NetPbCfgGetTransportObject(Dcm_NetPbCfgGetTObjConn2TObjMap(connHdl));                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetTranspObjOfConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetGetTranspObjOfConnection(
  Dcm_NetConnRefOptType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  )
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;

  if(Dcm_DebugDetectRuntimeError(connHdl >= Dcm_NetPbCfgGetNumConnections()))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    if(Dcm_NetPbCfgGetTObjConn2TObjMap(connHdl) < DCM_NET_INVALID_TOBJID)
    {
      *pTranspObj = Dcm_NetPbCfgGetTransportObject(Dcm_NetPbCfgGetTObjConn2TObjMap(connHdl));                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetTranspObjRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetTranspObjRelease(
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
  /* If the transport object is still in use: */
  if(pTranspObj->State != DCM_NET_TOBJ_STATE_FREE) /* assure the object has validly initialized fields */
  {
    /* Its states are valid and can be used if needed */
    /* Reset only the relevant states of the transport object */
    Dcm_NetPbCfgPutToConn2TObjMap(pTranspObj->ConnHdl, DCM_NET_INVALID_TOBJID); /* Clear the ownership information */
    pTranspObj->ResType  = DCM_NET_TOBJ_RESTYPE_NONE;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    pTranspObj->Flags    = DCM_NET_TOBJ_FLAG_NONE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    /* Reset the guard state (object in use) at last to avoid critical section usage */
    pTranspObj->State    = DCM_NET_TOBJ_STATE_FREE;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  }
  /* Otherwise nothing to do */
}

/**********************************************************************************************************************
 *  Dcm_NetUsdtTxConfirmation()
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetUsdtTxConfirmation(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetTransmissionResultType result
  )
{
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_CANCELED) != 0u)
  {
    Dcm_NetBufferRelease(pTranspObj->BufferHdl);
    /* ComM is already released in the DiagPostProcessing task */
    Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    if(pTranspObj->ResType == DCM_NET_TOBJ_RESTYPE_SIMPLE)
    {
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
      if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u)
#endif
      {
        Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl);
      }
      Dcm_NetUnRegisterActiveConnection(pTranspObj);/* no further processing needed */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      Dcm_DiagOnTxFinished(pTranspObj, result);                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_NetIsKeepAliveRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(boolean, DCM_CODE) Dcm_NetIsKeepAliveRequest(
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  uint8 neededLength
  )
{
  boolean lIsTesterPresent = FALSE;
  if (info != NULL_PTR)                                                                                                                              /* PRQA S 2822 */ /* MD_Dcm_2822 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    if (info->SduLength == neededLength)
    {
      if ((Dcm_NetPbCfgIsFuncRequest(rxPduId) == TRUE)
         && (info->SduDataPtr[0] == 0x3Eu) && (info->SduDataPtr[1] == 0x80u))                                                                        /* PRQA S 2822 */ /* MD_Dcm_2822 */
      {
        lIsTesterPresent = TRUE;
      }
    }
  }
  return lIsTesterPresent;
}

/**********************************************************************************************************************
 *  Dcm_NetSetComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetSetComModeEntered(
  NetworkHandleType networkId,
  Dcm_NetComMStateType newComState
  )
{
  Dcm_PbCfgNetNetworkHandlePtrType lNetHdlLookup = Dcm_NetPbCfgGetNetworkHdlLookUp();
  /* used to always verify for valid parameter */
  sint16_least                     lDcmComMHandle = Dcm_UtiLookUpUint8(lNetHdlLookup, networkId);                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  if(lDcmComMHandle >= 0) /* consider only DCM relevant channels */
  {
    Dcm_NetComMContextPtrType pComMContext;

    pComMContext = Dcm_NetPbCfgGetComMContext((Dcm_CfgNetNetIdRefOptType)lDcmComMHandle);

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    pComMContext->ComState = newComState;                                                                                                            /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }/* else - not a DCM channel -> ignore */
}

/**********************************************************************************************************************
 *  Dcm_NetSetChannelReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_NetSetChannelReady(
  NetworkHandleType networkId
  )
{
  Dcm_PbCfgNetNetworkHandlePtrType lNetHdlLookup = Dcm_NetPbCfgGetNetworkHdlLookUp();
  /* used to always verify for valid parameter */
  sint16_least lDcmComMHandle = Dcm_UtiLookUpUint8(lNetHdlLookup, networkId);                                                                        /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  if(lDcmComMHandle >= 0) /* consider only DCM relevant channels */
  {
    Dcm_NetComMContextPtrType pComMContext;

    pComMContext = Dcm_NetPbCfgGetComMContext((Dcm_CfgNetNetIdRefOptType)lDcmComMHandle);

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    pComMContext->ComAvailability = DCM_NET_COMM_CHANNEL_READY;                                                                                      /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }/* else - not a DCM channel -> ignore */
}

/**********************************************************************************************************************
 *  Dcm_NetStartOfReception()
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
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetStartOfReception(                                                                                 /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
  PduIdType rxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,
  PduLengthType tpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr,
  Dcm_NetTransportObjectFlagType addTObjFlags
  )
{
  BufReq_ReturnType lResult = BUFREQ_E_NOT_OK;
  Dcm_NetConnRefMemType lConnHdl = Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId);                                                                         /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  DCM_IGNORE_UNREF_PARAM(info);

  /* If the communication state allows any diagnostic request reception (at least SilentCommunicationMode): */
  if( ((DCM_NET_COMM_STATE_FLAG_RX_EN & Dcm_NetGetComStateByConnection(lConnHdl)) != 0u)
    &&(Dcm_NetSingletonContext.RxAllowed == TRUE)
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
    &&( (!Dcm_NetIsGenericConnection(lConnHdl))                                                                                                      /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
      ||(Dcm_NetPbCfgIsFuncRequest(rxPduId) == TRUE)                                                                                                 /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
      ||(info->SduDataPtr[info->SduLength-1u] == Dcm_NetPbCfgGetEcuAddress(lConnHdl)) )                                                              /* PRQA S 2822, 3415 */ /* MD_Dcm_2822, MD_Dcm_Rule13.5 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
#endif
    )
  {
    Dcm_NetTransportObjectPtrType pTranspObj;

    /* Enter critical section (Reason: Needed only on multi-channel DCM configurations to guarantee the correct
     * transport object allocation and initialization) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    /* Allocate new or get existing, already associated with this DcmRxPduId transport object */
    /* If allocation or getter succeeded: */
    if(Dcm_NetAllocateOrGetTranspObject(lConnHdl, &pTranspObj) == BUFREQ_OK)                                                                         /* SBSW_DCM_POINTER_FORWARD_STACK */
    {
      /* If the transport object is allocated as a new one: */
      if(pTranspObj->State == DCM_NET_TOBJ_STATE_FREE)
      {
        Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;

        pTranspObj->BufferHdl = Dcm_NetPbCfgGetRxTxBufRefByConnHdl(lConnHdl);                                                                        /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        lBufferInfo = Dcm_NetPbCfgGetBufferInfo(pTranspObj->BufferHdl);

        /* If the request message length fits the Rx-buffer associated with the DcmRxPduId: */
        if(tpSduLength <= lBufferInfo->Size)
        {
          /* Prepare for positive result or NRC 0x21 response: */
          /* Prepare result to be reported for the available Rx-buffer size */
          *bufferSizePtr = lBufferInfo->Size;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

          /* Initialize the transport object */
          pTranspObj->RxPduId  = rxPduId;                                                                                                            /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->ConnHdl   = (Dcm_NetConnRefMemType)lConnHdl;                                                                                   /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */

          /* Set up Rx message context */
          pTranspObj->RxLength = (Dcm_CfgNetBufferSizeMemType)tpSduLength;/* safe cast, since tpSduLength <= bufferSize = value fits Dcm_CfgNetBufferSizeMemType */ /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->BuffPos  = 0u; /* start writing from the buffer begin */                                                                       /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->State    = DCM_NET_TOBJ_STATE_ONRX;                                                                                            /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          pTranspObj->Flags    = addTObjFlags;                                                                                                       /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */

#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
          /* #20 If the transport object uses generic connection, set meta data. */
          if (Dcm_NetIsGenericConnection(lConnHdl))
          {
            /* Source address of the request shall be the target address of the response. */
            pTranspObj->ClientSrcAddr = info->SduDataPtr[info->SduLength-(PduLengthType)2u];                                                         /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          }
          else
#endif
          {
            pTranspObj->ClientSrcAddr = Dcm_NetPbCfgGetConnTesterAddress(lConnHdl);                                                                  /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          }

          /* #30 If the new request can be received - that is: */
          if( Dcm_NetBufferIsFree(pTranspObj->BufferHdl) /* Is the buffer free? */
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
            &&( (Dcm_StateIsDefaultSessionPending() == TRUE)/* a default session is/will be active */                                                /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
              ||( (Dcm_NetGetSessionConnection() == lConnHdl)
                &&(Dcm_NetGetSessionClientSrcAddr() == pTranspObj->ClientSrcAddr) ) /* the owner of the (non-default) session */
              ||(Dcm_NetPbCfgGetProtocolPriorityByConnHdl(lConnHdl)                                                                                  /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
                < Dcm_NetPbCfgGetProtocolPriorityByConnHdl(Dcm_NetGetSessionConnection())) /* higher priority -> let it through */                   /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
              )
#endif
            )
          {
            /* Lock the buffer and allow further request reception */
            pTranspObj->RxData = lBufferInfo->BufferPtr;                                                                                             /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
            Dcm_NetBufferLock(pTranspObj->BufferHdl); /* the buffer is in use now */
            lResult = BUFREQ_OK;
          }
#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
          else
          {
            /* Otherwise, prepare for pseudo request reception (only SID and sub-function will be copied) (if multiple client environment enabled) */
            pTranspObj->RxData = &pTranspObj->AddBuffer[1];                                                                                          /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
            /* no free "real buffer" object - ignore or simulate reception for NRC 0x21 */
            pTranspObj->Flags |= (Dcm_NetTransportObjectFlagType)(DCM_NET_TOBJ_FLAG_BUSY | DCM_NET_TOBJ_FLAG_COPYHEAD | DCM_NET_TOBJ_FLAG_IGNORE);   /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
            lResult = BUFREQ_OK;
          }
#endif
        }
        else
        {
          /* Otherwise, reject the message reception with the result "message too long" */
          lResult = BUFREQ_E_OVFL;
        }

        /* #40 If the message reception is to be continued, register the client connection as active  */
        if(lResult == BUFREQ_OK)
        {
          Dcm_NetRegisterActiveConnection(pTranspObj);                                                                                               /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        }
        else
        {
          Dcm_NetReloadAuthTimer(pTranspObj);                                                                                                        /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
          /* Otherwise, release the allocated transport object */
          Dcm_NetTranspObjRelease(pTranspObj);/* no further processing needed */                                                                     /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        }
      }/* Otherwise, the transport object is already in use (i.e. parallel request from the same tester):
        *   - reject the message reception if no functional TesterPresent (checked later on) */
    }/* Otherwise, no free transport object found (more clients than transport objects configured, legal use case
      * to save resources) - reject the message reception if no functional TesterPresent (checked later on) */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();

    if (lResult == BUFREQ_E_NOT_OK)
    {
      uint8 lLengthForTesterPresent = 2u; /* 0x3E 0x80 */

#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      if (Dcm_NetIsGenericConnection(lConnHdl))
      {
        lLengthForTesterPresent += DCM_NET_METADATA_LENGTH;
      }
#endif

      if (Dcm_NetIsKeepAliveRequest(rxPduId, info, lLengthForTesterPresent))                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
      {
        /* functional Tester Present as a parallel request */
        *bufferSizePtr = 2u;                                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
        lResult = BUFREQ_OK;
      }
    }
  } /* Otherwise, reject the message reception */
  return lResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if(DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetIsGenericConnection()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetIsGenericConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  return (Dcm_NetPbCfgGetMetaDataLengthOfConnection(connHdl) == DCM_NET_METADATA_LENGTH);
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSupportedFor()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetPeriodicMsgSupportedFor(
  Dcm_NetConnRefMemType connHdl
  )
{
  return (Dcm_NetPeriodicMsgGetNumMsgsFor(connHdl) != 0u);
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPeriodicMsgGetConnection(
  void
  )
{
  return Dcm_NetSingletonContext.PeriodicTxContext.ConnHdlInUse;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_NetSingletonContext.PeriodicTxContext.ConnHdlInUse = connHdl;                                                                                  /* SBSW_DCM_GLOBAL_PTR_WRITE */
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetClientSrcAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DCM_CODE) Dcm_NetPeriodicMsgGetClientSrcAddr(
  void
  )
{
  return Dcm_NetSingletonContext.PeriodicTxContext.ClientSrcAddr;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetClientSrcAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetClientSrcAddr(
  uint16 clientSrcAddr
  )
{
  Dcm_NetSingletonContext.PeriodicTxContext.ClientSrcAddr = clientSrcAddr;                                                                           /* SBSW_DCM_GLOBAL_PTR_WRITE */
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetSetSessionConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetSessionConnection(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_NetSingletonContext.SessionConnection = connHdl;
}

/**********************************************************************************************************************
 *  Dcm_NetGetSessionClientSrcAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DCM_CODE) Dcm_NetGetSessionClientSrcAddr(
  void
  )
{
  return Dcm_NetSingletonContext.SessionClientSrcAddr;
}

/**********************************************************************************************************************
 *  Dcm_NetSetSessionClientSrcAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetSessionClientSrcAddr(
  uint16 clientSrcAddress
  )
{
  Dcm_NetSingletonContext.SessionClientSrcAddr = clientSrcAddress;
}

/**********************************************************************************************************************
 *  Dcm_NetGetKeepAlive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetGetKeepAlive(
  void
  )
{
  return Dcm_NetSingletonContext.KeepAlive;
}

/**********************************************************************************************************************
 *  Dcm_NetSetKeepAlive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetKeepAlive(
  boolean keepAlive
  )
{
  Dcm_NetSingletonContext.KeepAlive = keepAlive;
}

/**********************************************************************************************************************
 *  Dcm_NetSetRxAllowed()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetRxAllowed(
  boolean rxAllowed
  )
{
  Dcm_NetSingletonContext.RxAllowed = rxAllowed;
}

/**********************************************************************************************************************
 *  Dcm_NetGetSessionConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetSessionConnection(
  void
  )
{
  return Dcm_NetSingletonContext.SessionConnection;
}

/**********************************************************************************************************************
 *  Dcm_NetGetProtIdOfActiveProtocol()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetGetProtIdOfActiveProtocol(
  void
  )
{
  return Dcm_NetSingletonContext.ActiveProtocol;
}

/**********************************************************************************************************************
 *  Dcm_NetSetProtIdOfActiveProtocol()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetProtIdOfActiveProtocol(
  Dcm_NetProtRefMemType protId
  )
{
  Dcm_NetSingletonContext.ActiveProtocol = protId;
}

/**********************************************************************************************************************
 *  Dcm_NetSetActiveProtocolConnHdl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetSetActiveProtocolConnHdl(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_NetSingletonContext.ActiveProtocol = Dcm_NetPbCfgGetProtIdOfConnection(connHdl);
}

/**********************************************************************************************************************
 *  Dcm_NetIsComMChannelTransmissionReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_NetIsComMChannelTransmissionReady(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  )
{
  boolean retVal = TRUE;
  if ((DCM_NET_COMM_STATE_FLAG_TX_EN & Dcm_NetGetComStateByTranspObject(pTranspObj)) == 0u)                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    retVal = FALSE;
  }
  else
  {
    Dcm_NetComMAvailabilityType lComAvailability = Dcm_NetGetComAvailabilityByTranspObj(pTranspObj);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    if (Dcm_NetIsPropertieSet(pTranspObj, DCM_NET_CONN_PROP_READY_INDICATION_ENABLED) &&
       (lComAvailability == DCM_NET_COMM_CHANNEL_NOT_READY))
    {
      retVal = FALSE;
    }
  }
  return retVal;
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetTxBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_MsgType, DCM_CODE) Dcm_NetPeriodicMsgGetTxBuffer(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  )
{
  return Dcm_NetPbCfgGetPeriodicTxObject(perTxObjHandle)->TxBuffer;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPeriodicMsgAllocate(
  uint16 Did
  )
{
  Dcm_CfgNetPTxObjHandleMemType perTxObjHandle = DCM_NET_INVALID_PTXOBJ_HANDLE;

  /* If all queued messages were sent, reset counters (continue with highest prio UUDT) */
  if(Dcm_NetPeriodicMsgGetNextMsgHdl() == Dcm_NetPeriodicMsgGetTxObjectSentCntr())
  {
    /* Note: No critical section needed to avoid interrupt by Dcm_NetPeriodicMsgRelease because at this point, all
    *       messages are already sent and so no more notification interrupt can occur */
    Dcm_NetPeriodicMsgResetNextMsgHdl();                                                                                                             /* SBSW_DCM_GLOBAL_PTR_WRITE */
    Dcm_NetPeriodicMsgResetTxObjectSentCntr();                                                                                                       /* SBSW_DCM_GLOBAL_PTR_WRITE */
  }

  /* If there is a message ID left that has a lower priority: */
  if(Dcm_NetPeriodicMsgGetNextMsgHdl() < Dcm_NetPeriodicMsgNumMsgsCurrConnection())
  {
    Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

    perTxObjHandle = (Dcm_CfgNetPTxObjHandleMemType)Dcm_NetPeriodicMsgGetNextMsgHdl();
    pPeriodicTxObj = Dcm_NetPbCfgGetPeriodicTxObject(perTxObjHandle);
    pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_RESERVED; /* Mark tx object as reserved */                                                     /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
    pPeriodicTxObj->Did = Did; /* Store the DID */                                                                                                   /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
    Dcm_NetPeriodicMsgIncNextMsgHdl(); /* Increment message handle for next message */                                                               /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_GLOBAL_PTR_WRITE */
  }

  return perTxObjHandle;
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgRelease(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  )
{
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  if(pPeriodicTxObj->State == DCM_NET_PERIODIC_TX_STATE_ONTX)
  {
    Dcm_NetPTxDelayTimerStart(); /* start delay timer */
  }
# endif

  Dcm_NetPTxObjTimerStop(pPeriodicTxObj);                                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
  pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_FREE;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */

  Dcm_NetPeriodicMsgIncTxObjectSentCntr(); /* Update sent message statistics */                                                                      /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_GLOBAL_PTR_WRITE */
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgResetConnection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgResetConnection(
  void
  )
{
  Dcm_NetPeriodicMsgResetNextMsgHdl();                                                                                                               /* SBSW_DCM_GLOBAL_PTR_WRITE */
  Dcm_NetPeriodicMsgResetTxObjectSentCntr();                                                                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
  Dcm_NetPeriodicMsgResetConnectionOwner();
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgStopDid()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgStopDid(
  uint16 Did
  )
{
  Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;
  for(lPerTxObjIter = 0; lPerTxObjIter < Dcm_NetPeriodicMsgGetNextMsgHdl(); lPerTxObjIter++)
  {
    Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj;

    pPeriodicTxObj = Dcm_NetPbCfgGetPeriodicTxObject(lPerTxObjIter);

    if((pPeriodicTxObj->State != DCM_NET_PERIODIC_TX_STATE_FREE)
      && (pPeriodicTxObj->Did == Did))
    {
      /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
      Dcm_UtiEnterCriticalSection(); /* Enter critical section (Reason: Protect against high level interrupts from the network interface) */
      switch (pPeriodicTxObj->State)
      {
        case DCM_NET_PERIODIC_TX_STATE_ONTX:
          (void)PduR_DcmCancelTransmit(Dcm_NetPeriodicMsgGetTxPduIdCurrConn(lPerTxObjIter)); /* try to cancel still ongoing Tx */                    /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
          /* fall through */
        case DCM_NET_PERIODIC_TX_STATE_RESERVED:                                                                                                     /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
        case DCM_NET_PERIODIC_TX_STATE_QUEUED:
# endif
          Dcm_NetPeriodicMsgRelease(pPeriodicTxObj);                                                                                                 /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
          break;
        default:
          /* nothing to do */
          break;
      }
      Dcm_UtiLeaveCriticalSection();
      /*=================================*
      END CRITICAL SECTION
      *=================================*/
    }
  }
}
#endif

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmit(                                                                                                     /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  PduLengthType length
  )
{
# if (DCM_NET_PERIODIC_MSG_PADDING_ENABLED == STD_ON)
  Dcm_UudtFrameSizeType lUudtFrameSize = Dcm_NetPbCfgGetConnUudtFrameSize(Dcm_NetPeriodicMsgGetConnection());
# endif
  PduLengthType lLength = length;

  Dcm_DebugAssert((Dcm_NetPeriodicMsgGetConnection() < DCM_NET_INVALID_CONNHDL), DCM_SID_INTERNAL, DCM_E_PARAM);                                     /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */ /* COV_DCM_RTM_DEV_DEBUG TX */

# if (DCM_NET_PERIODIC_MSG_PADDING_ENABLED == STD_ON)
  if(Dcm_DebugDetectRuntimeError(lUudtFrameSize > DCM_NET_PERIODIC_BUFFER_SIZE))                                                                     /* COV_DCM_RTM_RUNTIME_CHECK XF */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    for (; lLength < lUudtFrameSize; lLength++)
    {
      pPeriodicTxObj->TxBuffer[lLength] = DCM_NET_PERIODIC_MSG_PADDING_PATTERN;                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
# endif

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  /* In case of generic connection, append the meta data at the end of the paylaod. */
  if(Dcm_NetIsGenericConnection(Dcm_NetPeriodicMsgGetConnection()))
  {
    if(Dcm_DebugDetectRuntimeError(lLength > (PduLengthType)(DCM_NET_PERIODIC_BUFFER_SIZE - DCM_NET_METADATA_LENGTH)))                               /* COV_DCM_RTM_RUNTIME_CHECK XF */
    {
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    }
    else
    {
      pPeriodicTxObj->TxBuffer[lLength] = Dcm_NetPbCfgGetEcuAddress(Dcm_NetSingletonContext.PeriodicTxContext.ConnHdlInUse);                         /* SBSW_DCM_PARAM_PTR_WRITE */
      lLength++;
      pPeriodicTxObj->TxBuffer[lLength] = (Dcm_MsgItemType)Dcm_NetPeriodicMsgGetClientSrcAddr();                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      lLength++;
    }
  }
# endif

  pPeriodicTxObj->TxPduInfo.SduLength = lLength;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  pPeriodicTxObj->State = DCM_NET_PERIODIC_TX_STATE_QUEUED;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_TskSetEvent(DCM_TSK_ID_NET_TX, DCM_TSK_EV_NET_TX_SEND_UUDT);
}
#endif

#if ( (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON) || (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON) || (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON) )
/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByConnectionId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByConnectionId(
  uint16 connectionId
  )
{
  Dcm_NetConnRefOptType lConnIter;

  Dcm_NetConnRefMemType lResult = DCM_NET_INVALID_CONNHDL;
  Dcm_NetConnRefOptType lNumConnections = lResult;

  for(lConnIter = 0; lConnIter < lNumConnections; ++lConnIter)
  {
    if(connectionId == Dcm_NetPbCfgGetConnId((Dcm_NetConnRefMemType)lConnIter))
    {
      lResult = (Dcm_NetConnRefMemType)lConnIter;
      break;
    }
  }
  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByTesterAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByTesterAddress(
  uint16 testerAddress
  )
{
  Dcm_NetConnRefOptType lConnIter;

  Dcm_NetConnRefMemType lResult = DCM_NET_INVALID_CONNHDL;
  Dcm_NetConnRefOptType lNumConnections = lResult;

  for(lConnIter = 0; lConnIter < lNumConnections; ++lConnIter)
  {
    if (testerAddress == Dcm_NetPbCfgGetConnTesterAddress(((Dcm_NetConnRefMemType)lConnIter)))
    {
      lResult = (Dcm_NetConnRefMemType)lConnIter;
      break;
    }
  }
  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetGetComState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_NetComMStateType, DCM_CODE) Dcm_NetGetComState(
  PduIdType rxPduId
  )
{
  Dcm_NetComMStateType lResult  = DCM_NET_COMM_STATE_RX_DIS_TX_DIS;                                                                                  /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8                lErrorId = DCM_E_NO_ERROR;

  DCM_IGNORE_UNREF_PARAM(rxPduId);

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(Dcm_NetPbCfgIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = Dcm_NetGetComStateByRxPduId(rxPduId);                                                                                                  /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_INTERNAL, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageLeave()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetBufferUsageLeave(
  Dcm_CfgNetBufferRefMemType bufferHdl
  )
{
  Dcm_NetPbCfgGetBufferContext(bufferHdl)->IsInUseCnt--;                                                                                             /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageEnter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetBufferUsageEnter(
  Dcm_CfgNetBufferRefMemType bufferHdl
  )
{
  Dcm_NetPbCfgGetBufferContext(bufferHdl)->IsInUseCnt++;                                                                                             /* PRQA S 3387 */ /* MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
}

#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetRxIndInternal()
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_NetRxIndInternal(                                                                                                 /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
  PduIdType rxPduId,
  uint16 sourceAddress,
  Dcm_MsgType data,                                                                                                                                  /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  PduLengthType length
  )
{
  PduLengthType                               bufferSize;
  Dcm_NetConnRefMemType                       lConnHdl;
  Std_ReturnType                              lResult = DCM_E_NOT_OK;
  P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC)  pInfo;
# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  uint8                                       lMetaData[DCM_NET_METADATA_LENGTH];
  PduInfoType                                 lInfo;
# endif

  DCM_IGNORE_UNREF_PARAM(sourceAddress);

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  if(Dcm_NetIsGenericConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId)))
  {
    lMetaData[DCM_NET_METADATA_SA_OFFSET] = (uint8)sourceAddress;                                                                                    /* SBSW_DCM_LOCAL_ARRAY_WRITE */
    lMetaData[DCM_NET_METADATA_TA_OFFSET] = Dcm_NetPbCfgGetEcuAddress(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId));                                     /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_LOCAL_ARRAY_WRITE */
    lInfo.SduDataPtr = lMetaData;
    lInfo.SduLength = DCM_NET_METADATA_LENGTH;
    pInfo = &lInfo;
  }
  else
# endif
  {
    pInfo = NULL_PTR;
  }

# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
  Dcm_DebugAssert((Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId) < Dcm_NetPbCfgGetNumConnections()), DCM_SID_INTERNAL, DCM_E_PARAM);                      /* COV_DCM_RTM_DEV_DEBUG TX */
# endif

  /* start virtual request; */
  lConnHdl = Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId);

  if (((DCM_NET_COMM_STATE_FLAG_RX_EN & Dcm_NetGetComStateByConnection(lConnHdl)) != 0u)
    && (Dcm_NetSingletonContext.RxAllowed == TRUE))
  {
    BufReq_ReturnType bufReq = Dcm_NetStartOfReception(rxPduId, pInfo, length, &bufferSize, DCM_NET_TOBJ_FLAG_INTERNAL);                             /* SBSW_DCM_POINTER_FORWARD_STACK */
    Std_ReturnType    notifResult = DCM_NET_ARENV_NTFRSLT_NOT_OK;
    /* If reception accepted: */
    if (bufReq == BUFREQ_OK)
    {
      Dcm_NetTransportObjectPtrType pTranspObj;
      bufReq = Dcm_NetGetTranspObjOfConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId), &pTranspObj);                                               /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_FORWARD_STACK */

      if (bufReq == BUFREQ_OK)                                                                                                                       /* COV_DCM_RTM_UNREACHABLE XF */
      {
# if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
        /* If not a pseudo parallel request */
        if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_BUSY) == 0u)
# endif
        {
          PduInfoType pduInfo;

          pduInfo.SduDataPtr = data;
          pduInfo.SduLength = length;

          /* Copy received data */
          bufReq = Dcm_CopyRxData(rxPduId, &pduInfo, &bufferSize);                                                                                   /* SBSW_DCM_POINTER_FORWARD_STACK */
          /* If data copy succeeded: */
          if (bufReq == BUFREQ_OK)                                                                                                                   /* COV_DCM_RTM_UNREACHABLE TX */
          {
            /* Notify DCM about successful reception */
            notifResult = DCM_NET_ARENV_NTFRSLT_OK;
          }/* else - Dcm_CopyRxData failed -> exit with Dcm_TpRxIndication(failed) */
        }/* else - a pseudo parallel request will not be processed over an internal request */
      }
      else
      {
        /* Check for valid call state */
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      }
      /* Notify DCM about reception accomplishment only in case the reception was accepted */
      Dcm_TpRxIndication(rxPduId, notifResult);
    }/* else - no reception possible exit with fail */

    if (notifResult == DCM_NET_ARENV_NTFRSLT_OK)
    {
      lResult = DCM_E_OK;
    }
    else
    {
      /* NOT_OK is mapped to pending result to signal caller to try again later */
      lResult = DCM_E_PENDING;
    }
  }
  /* Return the operation result to the caller */
  return lResult;
}
#endif

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetTaskTaMonitor()
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
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetTaskTaMonitor(                                                                                                           /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_CfgNetTObjHandleOptType lTObjCntr;
  Dcm_TskTaskEvOptType        lEv = pEventContext->Ev;

  /* Check whether any request which has to be canceled is in progress */
  if(((DCM_DIAG_QUEUE_FLAG_IS_ACTIVE | DCM_DIAG_QUEUE_FLAG_IS_WAITING) & Dcm_DiagGetQueuedSetFlags(pContext->ThreadId)) != 0u)
  {

    /* Enter critical section (Reason: the transport object may not be modified from outside) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/

    /* If a request is in progress */
    if((DCM_DIAG_QUEUE_FLAG_IS_ACTIVE & Dcm_DiagGetQueuedSetFlags(pContext->ThreadId)) != 0u)
    {
      Dcm_TskTaskEvMemType tObjEv = Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvOptType, Dcm_DiagGetTranspObj(pContext->ThreadId)->Handle);

      /* If the ongoing request is from the same tester as the new request with the foreign N_TA */
      if (Dcm_TskIsLocalEventSet(lEv, tObjEv))
      {
        /* Cancel the request processing */
        Dcm_DiagCancelProcessing(DCM_THREAD_ID_BASIC);
      }/* else - post processing or idle are to be handled in a normal way */
    }

    /* If a waiting new request is in the queue */
    if((DCM_DIAG_QUEUE_FLAG_IS_WAITING & Dcm_DiagGetQueuedSetFlags(pContext->ThreadId)) != 0u)
    {
       Dcm_TskTaskEvMemType tObjEv = Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvOptType, Dcm_DiagGetQueuedTranspObj(pContext->ThreadId)->Handle);

       /* If the queued request is from the same tester as the new request with the foreign N_TA */
       if (Dcm_TskIsLocalEventSet(lEv, tObjEv))
       {
         /* Cancel the queued request in the next task cycle */
         pEventContext->PostEv = tObjEv;/* retry later */                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
       }
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }

  /* Scan all affected transport objects */
  lTObjCntr = 0u;
  DCM_TSK_EVENT_SCAN_LOOP(lEv)
  {
    if (Dcm_TskIsLocalEventSet(lEv, 0x01u))
    {
      Dcm_NetTransportObjectPtrType pTranspObj = Dcm_NetPbCfgGetTransportObject(lTObjCntr);

      /* If a received but not yet processed request was interrupted by another request with a foreign N_TA */
      if( ( pTranspObj->State == DCM_NET_TOBJ_STATE_RX_END)
        &&((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_OBSOLETE) != 0u ) ) /* This is still the original transport object marked to be canceled in the Dcm_OnRequestIndication */
      {
        /* Prevent the processing of the received but not yet processed request */
        Dcm_TskClrEventByThread(DCM_TSK_ID_NET_RX, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvMemType, pTranspObj->Handle), DCM_THREAD_ID_BASIC);

        /* Deallocate the transport object and free the connection */
        Dcm_NetUnRegisterComMActivity(pTranspObj->ConnHdl); /* Deallocate transport object */
        Dcm_NetBufferRelease(pTranspObj->BufferHdl);
        Dcm_NetUnRegisterActiveConnection(pTranspObj); /* No further processing needed */                                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
      } /* Otherwise, connection is not obsolete or will be canceled during service processing (already in state
         * DCM_NET_TOBJ_STATE_READY) */
    }
    ++lTObjCntr;
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetTaskRx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetTaskRx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_TskTaskEvMemType          lBusyEvents;
  Dcm_CfgNetTObjHandleOptType   lTranspObjWinnerId;
  Dcm_NetTransportObjectPtrType pTranspObj;

  lTranspObjWinnerId = Dcm_NetRxPrioritisationFindWinner(pEventContext->Ev, &lBusyEvents);                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

  pTranspObj = Dcm_NetPbCfgGetTransportObject(lTranspObjWinnerId);

  Dcm_NetRxPrioritisationProcessWinner(pContext, pTranspObj, &lBusyEvents);                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
  Dcm_NetRxPrioritisationProcessLoosers(lBusyEvents);
}

/**********************************************************************************************************************
 *  Dcm_NetTaskTx()
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
 */
FUNC(void, DCM_CODE) Dcm_NetTaskTx(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);

  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_NET_TX_SEND_USDT))
  {
    {
      Dcm_CfgNetTObjHandleOptType lTObjIter;

      /* find candidate for NRC 0x21 or final response */
      for (lTObjIter = 0; lTObjIter < Dcm_NetPbCfgGetNumTransportObjects(); ++lTObjIter)
      {
        Dcm_NetTransportObjectPtrType pTranspObj;
        pTranspObj = Dcm_NetPbCfgGetTransportObject(lTObjIter);

#if (DCM_NET_MULTI_CLIENT_ENABLED == STD_ON)
        if ((pTranspObj->State == DCM_NET_TOBJ_STATE_RX_END) /* Completely received ... */
          && ((DCM_NET_TOBJ_FLAG_BUSY & pTranspObj->Flags) != 0u))/* ... a parallel request */
        {
          /* Send NRC 0x21 response */
          Dcm_NetComposeAddNegResponse(pTranspObj, DCM_NET_TOBJ_RESTYPE_SIMPLE, DCM_E_BUSYREPEATREQUEST);                                            /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
#endif

        if (pTranspObj->State == DCM_NET_TOBJ_STATE_PREPTX)
        {
          Dcm_NetTransmitUsdtResponse(pTranspObj);                                                                                                   /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
        }
      }
    }
  }

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_NET_TX_SEND_UUDT))
  {
    Dcm_CfgNetPTxObjHandleOptType lPerTxObjIter;

    for (lPerTxObjIter = 0; lPerTxObjIter < Dcm_NetPeriodicMsgGetNextMsgHdl(); lPerTxObjIter++)
    {
      if (Dcm_NetPbCfgGetPeriodicTxObject(lPerTxObjIter)->State == DCM_NET_PERIODIC_TX_STATE_QUEUED)
      {
        Dcm_NetPeriodicMsgTransmitTxObj(lPerTxObjIter);
      }
    }
  }
#endif
}
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutPeriodicTx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutPeriodicTx(
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_TmrTimerCntrMemType lReloadTick;
  boolean                 lDoReload;

  DCM_IGNORE_UNREF_PARAM(threadId);

  lDoReload = Dcm_OnTimeoutTxObjectHandler();
# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  if (!lDoReload)
  {
    lDoReload = Dcm_OnTimeoutDelayTimerHandler();
  }
# endif

  lReloadTick = (Dcm_TmrTimerCntrMemType)((lDoReload == TRUE)?1u:0u);

  return lReloadTick;
}
#endif

#if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetFastestRate()
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
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetFastestRate(                                                                                               /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_Svc2ASchedRateOptType Rate
  )
{
  /* If the new rate differs from the previous rate */
  if (Dcm_NetPeriodicMsgGetFastestRate() != Rate)
  {
    Dcm_TmrTimerCntrMemType lRateDiff;
    uint8_least lDelayTmrIter;

    /* If changing to a faster rate */
    if (Rate > Dcm_NetPeriodicMsgGetFastestRate())
    {
      lRateDiff = (Dcm_TmrTimerCntrMemType) ( Dcm_NetGetPeriodicDelayTime(Dcm_NetPeriodicMsgGetFastestRate())
                                            - Dcm_NetGetPeriodicDelayTime(Rate) );

      for (lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
      {
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_SplitTaskEnterCS(); /* Protect against interrupt from timer task */
        /* If delay timer is set */
        if (Dcm_NetDelayTimerExpired(lDelayTmrIter) == FALSE)
        {
          if (Dcm_NetDelayTimerGet(lDelayTmrIter) <= lRateDiff)
          {
            Dcm_NetDelayTimerSet(lDelayTmrIter, 1); /* immediate timeout */
          }
          else
          {
            /* decrease timer value by rate difference */
            Dcm_NetDelayTimerSet(lDelayTmrIter, (Dcm_TmrTimerCntrOptType)(Dcm_NetDelayTimerGet(lDelayTmrIter) - lRateDiff));
          }
        }
        Dcm_SplitTaskLeaveCS();
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
      }
    }
    else /* Otherwise (changing to a slower rate) */
    {
      lRateDiff = (Dcm_TmrTimerCntrMemType) ( Dcm_NetGetPeriodicDelayTime(Rate)
                                            - Dcm_NetGetPeriodicDelayTime(Dcm_NetPeriodicMsgGetFastestRate()) );

      for (lDelayTmrIter = 0; lDelayTmrIter < DCM_NET_DELAY_BULK_TRANSMISSION; ++lDelayTmrIter)
      {
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
        Dcm_SplitTaskEnterCS(); /* Protect against interrupt from timer task */
        /* If delay timer is set */
        if (Dcm_NetDelayTimerExpired(lDelayTmrIter) == FALSE)
        {
          /* increase timer value by rate difference */
          Dcm_NetDelayTimerSet(lDelayTmrIter, (Dcm_TmrTimerCntrOptType)(Dcm_NetDelayTimerGet(lDelayTmrIter) + lRateDiff));
        }
        Dcm_SplitTaskLeaveCS();
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
      }
    }

    Dcm_NetSingletonContext.PeriodicTxContext.FastestRate = (Dcm_Svc2ASchedRateMemType)Rate;                                                         /* SBSW_DCM_GLOBAL_PTR_WRITE */
  }
}
#endif

#if (DCM_NET_CONN_LOCK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetLockConnection()
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
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetLockConnection(
  Dcm_NetConnRefMemType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  )
{
  BufReq_ReturnType lResult;

  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Try allocating a new or getting an already allocated transport object */
  lResult = Dcm_NetAllocateOrGetTranspObject(connHdl, pTranspObj);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lResult == BUFREQ_OK)
  {
    if((*pTranspObj)->State == DCM_NET_TOBJ_STATE_FREE)
    {
      /* Setup the transport object */
      (*pTranspObj)->ConnHdl = connHdl;                                                                                                              /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
      (*pTranspObj)->BufferHdl = Dcm_NetPbCfgGetRxTxBufRefByConnHdl(connHdl);                                                                        /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */

      /* Is the buffer still free ? */
      if(Dcm_NetBufferIsFree((*pTranspObj)->BufferHdl))
      {
        Dcm_NetBufferLock((*pTranspObj)->BufferHdl); /* the buffer is in use now */
        (*pTranspObj)->State = DCM_NET_TOBJ_STATE_RESERVED;                                                                                          /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        lResult = BUFREQ_OK;
      }
      else
      {
        Dcm_NetTranspObjRelease(*pTranspObj);                                                                                                        /* SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ */
        lResult = BUFREQ_E_NOT_OK;
      }
    }
    else
    {
      lResult = BUFREQ_E_NOT_OK;
    }
  }/* else - nothing found to be free */
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_NetComposeAddNegResponse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetComposeAddNegResponse(                                                                                                   /* COV_DCM_RTM_NO_RCR_RP_TX */
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetResponseType resType,
  Dcm_NegativeResponseCodeType nrc
  )
{
  Dcm_NetWriteAddBufferU8At(pTranspObj, 2, nrc); /* NRC = nrc, 0x7F and SID are already copied! */                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  pTranspObj->BuffInfo.SduDataPtr = pTranspObj->AddBuffer; /* use the temp buffer to free the main buffer now */                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->BuffInfo.SduLength  = 3u;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->ResType             = resType;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  pTranspObj->State               = DCM_NET_TOBJ_STATE_PREPTX;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_NetComMActiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetComMActiveDiagnostic(
  NetworkHandleType netHdl
  )
{
  NetworkHandleType lNetworkHandle = Dcm_NetPbCfgGetAllComMChannelMap(Dcm_NetPbCfgGetNetworkRef(netHdl));
  ComM_DCM_ActiveDiagnostic(lNetworkHandle);
}

/**********************************************************************************************************************
 *  Dcm_NetRegisterActiveConnection()
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
 */
FUNC(void, DCM_CODE) Dcm_NetRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pTranspObj);
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u)/* internal requests do not manage the S3 timer */
#endif
  {
    Dcm_DebugAssert((Dcm_NetSingletonContext.NumActiveConnections != Dcm_UtiMaxValueOfUintType(uint8)), DCM_SID_INTERNAL, DCM_E_PARAM);              /* COV_DCM_RTM_DEV_DEBUG TX */

    /* Enter critical section (Reason: protect the statistic counter against parallel request notifications or transmission confirmations, executed from within interrupts ) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
     BEGIN CRITICAL SECTION
     *=================================*/
    /* Reset the idle connection deauthentication timer for this connection */
#if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
    /* Check if authentication restriction is supported for this connection */
    if(Dcm_NetPbCfgGetConnAuthInfoRef(pTranspObj->ConnHdl) != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
      Dcm_AuthMgrTimerReset(Dcm_NetPbCfgGetConnAuthInfoRef(pTranspObj->ConnHdl));
    }
#endif

    if(Dcm_NetSingletonContext.NumActiveConnections == 0u)
    {
      Dcm_TmrStopTimer(DCM_TMR_ID_S3);
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
      Dcm_TmrStopTimer(DCM_TMR_ID_KEEP_ALIVE);
#endif
    }

    /* Register a new active connection */
    ++Dcm_NetSingletonContext.NumActiveConnections;
    /*=================================*
     END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterActiveConnection()
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
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetUnRegisterActiveConnection(                                                                                              /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_NetTransportObjectPtrType pTranspObj
  )
{
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u)
  {
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc86TxEnd();
# endif
  }
  else
#endif
  {
    Dcm_DebugAssert((Dcm_NetSingletonContext.NumActiveConnections != 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                                            /* COV_DCM_RTM_DEV_DEBUG TX */

    Dcm_UtiEnterCriticalSection();
    /*=================================*
     BEGIN CRITICAL SECTION
     *=================================*/
    /* Start the idle connection timer for the authenticated session connection */
#if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
    /* Check if authentication restriction is supported for this connection */
    if (Dcm_NetPbCfgGetConnAuthInfoRef(pTranspObj->ConnHdl) != DCM_AUTHMGR_INVALID_AUTH_INFO_REF)
    {
      Dcm_AuthMgrTimerStart(Dcm_NetPbCfgGetConnAuthInfoRef(pTranspObj->ConnHdl), DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT);
    }
#endif

    /* If this was not a functional request from different tester with 0x3E 0x80 or a denied parallel tester request */
    if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_IGNORE) == 0u)
    {
      /* The timers shall be restarted later with the original timer values (when the last connection goes inactive) */
      Dcm_TmrSetTimer(DCM_TMR_ID_S3, DCM_DIAG_TIME_S3);
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
      Dcm_TmrSetTimer(DCM_TMR_ID_KEEP_ALIVE, DCM_NET_KEEP_ALIVE_TIME);
#endif
    }

    /* Un-register an active connection */
    --Dcm_NetSingletonContext.NumActiveConnections;

    if(Dcm_NetSingletonContext.NumActiveConnections == 0u)
    {
      /* Start or resume the S3 timer and keep-alive timer with the last set values */
      Dcm_TmrResumeTimer(DCM_TMR_ID_S3);
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
      Dcm_TmrResumeTimer(DCM_TMR_ID_KEEP_ALIVE);
#endif
    }

    /*=================================*
     END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
  Dcm_NetTranspObjRelease(pTranspObj);                                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_NetRegisterComMActivity()
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
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetRegisterComMActivity(
  Dcm_NetTransportObjectPtrType pTranspObj                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pTranspObj);
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  if((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) == 0u)/* internal requests do not manage the ComM activity */
#endif
  {
    Dcm_CfgNetNetIdRefMemType lNetHandle;
    Dcm_NetComMContextPtrType pComMContext;

    lNetHandle   = Dcm_NetPbCfgGetNetIdRefOfConnection(pTranspObj->ConnHdl);
    pComMContext = Dcm_NetPbCfgGetComMContext(lNetHandle);

    Dcm_DebugAssert((pComMContext->RegisteredNetworks != Dcm_UtiMaxValueOfUintType(Dcm_NetConnRefMemType)), DCM_SID_INTERNAL, DCM_E_PARAM);          /* COV_DCM_RTM_DEV_DEBUG TX */

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    if(pComMContext->RegisteredNetworks == 0u)
    {
      /* activate only if default session is active, otherwise already active */
      if(   Dcm_StateIsDefaultSessionActive()
        && (Dcm_NetSingletonContext.ActiveDiagnostic == DCM_NET_COMM_ACTIVE)
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
        && (Dcm_NetSingletonContext.KeepAlive == FALSE)
#endif
        )
      {
        /* Register one connection */
        Dcm_NetComMActiveDiagnostic(lNetHandle);
      }
    }

    /* Register one connection */
    ++pComMContext->RegisteredNetworks;                                                                                                              /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterComMActivity()
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
 */
FUNC(void, DCM_CODE) Dcm_NetUnRegisterComMActivity(
  Dcm_NetConnRefMemType connHdl
  )
{
  Dcm_CfgNetNetIdRefMemType lNetHandle;
  Dcm_NetComMContextPtrType pComMContext;
  NetworkHandleType         lNetworkHandleEntry;
  lNetHandle   = Dcm_NetPbCfgGetNetIdRefOfConnection(connHdl);
  pComMContext = Dcm_NetPbCfgGetComMContext(lNetHandle);
  lNetworkHandleEntry = Dcm_NetPbCfgGetAllComMChannelMap(Dcm_NetPbCfgGetNetworkRefByConnHdl(connHdl));

  Dcm_DebugAssert((pComMContext->RegisteredNetworks != 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                                                          /* COV_DCM_RTM_DEV_DEBUG TX */

  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  /* Unregister one connection */
  --pComMContext->RegisteredNetworks;                                                                                                                /* SBSW_DCM_POINTER_WRITE_COMMCONTEXT */

  if(pComMContext->RegisteredNetworks == 0u)
  {
    /* return only if default session is active */
    if (Dcm_StateIsDefaultSessionActive()
#if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
       &&(Dcm_NetSingletonContext.KeepAlive == FALSE)
#endif
       )
    {
      ComM_DCM_InactiveDiagnostic(lNetworkHandleEntry);
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterAllComMActivity()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetUnRegisterAllComMActivity(
  void
  )
{
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/
  {
    Dcm_CfgNetNetIdRefOptType lNetHandleIter;
    NetworkHandleType lNetworkHandleEntry;
    for(lNetHandleIter = 0; lNetHandleIter < Dcm_NetPbCfgGetNumComMChannels(); ++lNetHandleIter)
    {
      /* unregister only if no connection is ongoing */
      if(Dcm_NetPbCfgGetComMContext(lNetHandleIter)->RegisteredNetworks == 0u)
      {
        lNetworkHandleEntry = Dcm_NetPbCfgGetAllComMChannelMap(Dcm_NetPbCfgGetNetworkRef(lNetHandleIter));
        ComM_DCM_InactiveDiagnostic(lNetworkHandleEntry);
      }
    }
  }
  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_NetWriteAddBufferU8At()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetWriteAddBufferU8At(
  Dcm_NetTransportObjectPtrType pTranspObj,
  uint8_least index,
  Dcm_MsgItemType value
  )
{
  if(Dcm_DebugDetectRuntimeError(index >= DCM_NET_TOBJ_ADDBUFFER_SIZE))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    pTranspObj->AddBuffer[index] = value;                                                                                                            /* SBSW_DCM_POINTER_WRITE_TOBJ_ADDBUFFER */
  }
}

/**********************************************************************************************************************
 *  Dcm_NetInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_NetInit(
  Dcm_ConfigPtrType configPtr
  )
{
  /* First initialize common variables */
  Dcm_NetCommonInit(configPtr);                                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Now initialize all remaining network related sub-elements. The order of execution shall not be relevant */
  Dcm_NetComMInit();                                                                                                                                 /* PRQA S 2987 */ /* MD_Dcm_2987 */
  Dcm_NetBufferInit();
  Dcm_NetConnectionInit();                                                                                                                           /* PRQA S 2987 */ /* MD_Dcm_2987 */
  Dcm_NetTranspObjInit();
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  Dcm_NetPeriodicMsgInit();
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_GetActiveProtocol()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetActiveProtocol(
  P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_APPL_DATA) ActiveProtocol                                                                                   /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  )
{
  uint8 lErrorId;

  DCM_IGNORE_UNREF_PARAM(ActiveProtocol);

  /* ----- Development Error Checks ---------------------------------------- */
#if (DCM_NET_PROTOCOL_PARALLELISATION_ENABLED == STD_ON)
  lErrorId = DCM_E_INVALID_CONFIG;
#else
  lErrorId = DCM_E_NO_ERROR;

  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(ActiveProtocol == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if(Dcm_NetGetProtIdOfActiveProtocol() >= DCM_NET_INVALID_PROTID)                                                                                 /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
    {
      *ActiveProtocol = DCM_NO_ACTIVE_PROTOCOL;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      *ActiveProtocol = Dcm_NetPbCfgGetProtocolId(Dcm_NetGetProtIdOfActiveProtocol());                                                               /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
#endif

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETACTIVEPROTOCOL, lErrorId);

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_GetTesterSourceAddress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetTesterSourceAddress(
  PduIdType DcmRxPduId,
  P2VAR(uint16, AUTOMATIC, DCM_APPL_DATA) TesterSourceAddress
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;                                                                                                            /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsRxPduIdValid(DcmRxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(TesterSourceAddress == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  else if(Dcm_DebugDetectError(Dcm_NetIsGenericConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(DcmRxPduId))))
  {
    lErrorId = DCM_E_PARAM;
  }
#endif
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = DCM_E_OK;
    *TesterSourceAddress = Dcm_NetPbCfgGetConnTesterAddress(Dcm_NetPbCfgGetConnHdlOfRxPduId(DcmRxPduId));                                            /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETTESTERSOURCEADDRESS, lErrorId);

  return lResult;
}

#if (DCM_DIAG_VIRTUAL_REQUEST_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessVirtualRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ProcessVirtualRequest(
  PduIdType RxPduId,
  Dcm_MsgType Data,
  PduLengthType Length
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsRxPduIdValid(RxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Data == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  /* if (length > <BufferSize>)
  {
    This check is done through the Dcm_NetRxIndInternal API!
  }
  else */
  {
    /* ----- Implementation ------------------------------------------------ */
    uint16 lClientSrcAddr;

# if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
    if(Dcm_NetIsGenericConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(RxPduId)))
    {
      /* The client source address is stored behind payload data */
      lClientSrcAddr = Data[Length];
    }
    else
# endif
    {
      lClientSrcAddr = Dcm_NetPbCfgGetConnTesterAddress(Dcm_NetPbCfgGetConnHdlOfRxPduId(RxPduId));
    }

    lResult = Dcm_NetRxIndInternal(RxPduId, lClientSrcAddr, Data, Length);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Pending result should not be forwarded to external caller */
    if (lResult == DCM_E_PENDING)
    {
      lResult = DCM_E_NOT_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_PROCESSVIRTUALREQUEST, lErrorId);

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_SetActiveDiagnostic()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(
  boolean active
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (active != DCM_NET_COMM_ACTIVE) && (active != DCM_NET_COMM_NOT_ACTIVE) ))
  {
    lErrorId = DCM_E_PARAM;                                                                                                                          /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Enter critical section (Reason: API is reentrant) */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    /* Apply new ComM management state */
    Dcm_NetSingletonContext.ActiveDiagnostic = active;
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETACTIVEDIAGNOSTIC, lErrorId);

  return DCM_E_OK;
}

/**********************************************************************************************************************
 *  Dcm_GetRequestKind()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetRequestKind(                                                                                                   /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
  uint16 TesterSourceAddress,
  P2VAR(Dcm_RequestKindType, AUTOMATIC, DCM_APPL_DATA) RequestKind
  )
{
  Std_ReturnType lResult  = DCM_E_NOT_OK;
  uint8          lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(RequestKind == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetConnRefMemType lConnHdl;

    lConnHdl = Dcm_NetGetConnHdlByTesterAddress(TesterSourceAddress);

    /* Check if the tester source address is a valid one */
    if (lConnHdl < DCM_NET_INVALID_CONNHDL)
    {
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
      if (Dcm_DebugDetectError(Dcm_NetIsGenericConnection(lConnHdl)))
      {
        lErrorId = DCM_E_PARAM;
      }
      else
#endif
      {
        Dcm_NetTransportObjectPtrType pTranspObj;
        BufReq_ReturnType             lBufReqResult;

        /* Assure that no interrupt will change the state of the conn2tobj map and the tobj state/flags */
        Dcm_UtiEnterCriticalSection();
        /*=================================*
          BEGIN CRITICAL SECTION
          *=================================*/
        lBufReqResult = Dcm_NetGetTranspObjOfConnection(lConnHdl, &pTranspObj);                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

        if (lBufReqResult == BUFREQ_OK)
        {
          /* If for this tester any request is ongoing: */
          if (pTranspObj->State != DCM_NET_TOBJ_STATE_FREE)
          {
#if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
            /* Determine the kind of the request */
            if ((pTranspObj->Flags & DCM_NET_TOBJ_FLAG_INTERNAL) != 0u)
            {
              *RequestKind = DCM_REQ_KIND_ROE;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
            }
            else
#endif
            {
              *RequestKind = DCM_REQ_KIND_EXTERNAL;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
            }
          }
          else
          {
            /* Otherwise consider this tester idling */
            *RequestKind = DCM_REQ_KIND_NONE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
          }
        }
        else
        {
          *RequestKind = DCM_REQ_KIND_NONE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
        }
        /*=================================*
          END CRITICAL SECTION
          *=================================*/
        Dcm_UtiLeaveCriticalSection();

        lResult = DCM_E_OK;
      }
    } /* else - result already set to NOT_OK */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETREQUESTKIND, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_ComM_NoComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetComModeEntered(networkId, DCM_NET_COMM_STATE_RX_DIS_TX_DIS);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COMMENTERNOCOMMODE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_ComM_SilentComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetComModeEntered(networkId, DCM_NET_COMM_STATE_RX_EN_TX_DIS);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COMMENTERSILENTCOMMODE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_ComM_FullComModeEntered()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetComModeEntered(networkId, DCM_NET_COMM_STATE_RX_EN_TX_EN);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COMMENTERFULLCOMMODE, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_SetChannelReady()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_SetChannelReady(
  NetworkHandleType networkId
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetSetChannelReady(networkId);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_SETCHANNELREADY, lErrorId);
}

#if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnRequestDetection()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_OnRequestDetection(
  PduIdType canTpRxPduId,
  uint8 tpAddrExtension
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;
  PduIdType lCanTpRxPduIdMax = Dcm_NetPbCfgGetTpRxPduIdMax();
  PduIdType lCanTpRxPduIdMin = Dcm_NetPbCfgGetTpRxPduIdMin();

  /* ----- Development Error Checks ---------------------------------------- */
  /* Check initialization state */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    /* Check whether the CanTpPduId is between the upper and lower bound of DCM CanTpPduIds */
    if(
# if (DCM_VARMGR_MULTI_COM_ENABLED == STD_ON)
        (Dcm_NetPbCfgGetNumCanRxPduIds() > 0u) &&  /* Check for a non-empty map */
# endif
        ( (canTpRxPduId >= lCanTpRxPduIdMin)
        &&(canTpRxPduId <= lCanTpRxPduIdMax) )
      )
    {
      PduIdType lDcmRxPduId;

      /* Retrieve the DCM RxPduId from the CanTp PduId */
      lDcmRxPduId = Dcm_NetPbCfgGetDcmRxPduIdOfCanTpPduId(canTpRxPduId);

      /* Check for DCM connection on this SDU */
      if(lDcmRxPduId < Dcm_NetPbCfgGetNumRxPduIds())
      {
        Dcm_NetConnRefMemType lConnHdl = Dcm_NetPbCfgGetConnHdlOfRxPduId(lDcmRxPduId);

        /* Compare the N_TA with the own ECU address. React only on external ECU requests */
        if(Dcm_NetPbCfgGetEcuAddress(lConnHdl) != tpAddrExtension)
        {
          Dcm_NetOnRequestDetection(lConnHdl);
        }/* else - a DcmPduId and the same N_TA -> request will be ignored on task level */
      }/* else - not a DcmPduId or a functional DcmPduId -> ignore */
    }/* else - not a DcmPduId -> ignore */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_ONREQUESTDETECTION, lErrorId);
}
#endif

/**********************************************************************************************************************
 *  Dcm_StartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception(                                                                                              /* PRQA S 6080 */ /* MD_MSR_STMIF */
  PduIdType rxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  PduLengthType tpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(bufferSizePtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
#if (DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
  /* info is not NULL_PTR when the connection is generic. */
  else if(Dcm_DebugDetectError( (info == NULL_PTR) &&  Dcm_NetIsGenericConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId)) ))                       /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError(((info != NULL_PTR) /* info pointer check done again to avoid a Misra justification */
                                && Dcm_NetIsGenericConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId))                                              /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
                                && ((info->SduLength < DCM_NET_METADATA_LENGTH)
                                   || ((info->SduLength > DCM_NET_METADATA_LENGTH) && (tpSduLength < (info->SduLength-DCM_NET_METADATA_LENGTH)))
                                   ))))
  {
    lErrorId = DCM_E_PARAM;
  }
#endif
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lResult = Dcm_NetStartOfReception(rxPduId, info, tpSduLength, bufferSizePtr, DCM_NET_TOBJ_FLAG_NONE);                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_STARTOFRECEPTION, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_CopyRxData()
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
 *
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData(                                                                                                    /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
  PduIdType rxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) bufferSizePtr
  )
{
  BufReq_ReturnType  lResult  = BUFREQ_E_NOT_OK;                                                                                                     /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8              lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsPduInfoValid(info) == FALSE))                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(bufferSizePtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTransportObjectPtrType pTranspObj;

    lResult = Dcm_NetGetTranspObjOfConnection(Dcm_NetPbCfgGetConnHdlOfRxPduId(rxPduId), &pTranspObj);                                                /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_FORWARD_STACK */

    DCM_IGNORE_UNREF_PARAM(rxPduId);

    /* Check always for valid call state - if free or ready -> ignore call
    * This check is necessary in case Dcm_Init is called some time during ongoing transmission and in case of a parallel functional TesterPresent
    */
    if ((lResult == BUFREQ_OK)
      && (pTranspObj->State == DCM_NET_TOBJ_STATE_ONRX)
      && (rxPduId == pTranspObj->RxPduId))
    {
      lResult = Dcm_NetCopyRxData(pTranspObj, info, bufferSizePtr);                                                                                  /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
    }
    else if (Dcm_NetIsKeepAliveRequest(rxPduId, info, 2u))                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      *bufferSizePtr = 0u;                                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
    else if (info->SduLength == 0u)
    {
      *bufferSizePtr = 2u;                                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = BUFREQ_OK;
    }
    else
    {
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYRXDATA, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_TpRxIndication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TpRxIndication(
  PduIdType rxPduId,
  Std_ReturnType result
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsRxPduIdValid(rxPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTpRxIndication(rxPduId, result);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TPRXINDICATION, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData(                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
  PduIdType txPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info ,                                                                                                /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) retry ,                                                                                             /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr
  )
{
  BufReq_ReturnType lResult  = BUFREQ_E_NOT_OK;                                                                                                      /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8             lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_NetIsPduInfoValid(info) == FALSE))                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(availableDataPtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError( (retry != NULL_PTR) /* else - if(retry == NULL_PTR) -> not used => OK */
                              &&(retry->TpDataState != TP_DATACONF) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTransportObjectPtrType pTranspObj;
    Dcm_NetConnRefMemType         lConnHdl = Dcm_NetPbCfgGetConnHdlOfTxPduId(txPduId);

    lResult = Dcm_NetGetTranspObjOfConnection(lConnHdl, &pTranspObj);                                                                                /* SBSW_DCM_POINTER_FORWARD_STACK */

    DCM_IGNORE_UNREF_PARAM(txPduId);
    DCM_IGNORE_UNREF_PARAM(retry);

    if(lResult == BUFREQ_OK)
    {
      if(Dcm_DebugDetectError(pTranspObj->State != DCM_NET_TOBJ_STATE_ONTX))                                                                         /* COV_DCM_RTM_UNREACHABLE_COMPLEXITY XF */
      {
        lResult  = BUFREQ_E_NOT_OK;
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        lResult = Dcm_NetCopyTxData(pTranspObj, info, availableDataPtr);                                                                             /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
    }
    else
    {
      /* Check for valid call state */
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_COPYTXDATA, lErrorId);

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_TpTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(
  PduIdType txPduId,
  Std_ReturnType result
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  DCM_IGNORE_UNREF_PARAM(txPduId);

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetTransportObjectPtrType pTranspObj;
    BufReq_ReturnType             lResult;
    Dcm_NetConnRefMemType         lConnHdl = Dcm_NetPbCfgGetConnHdlOfTxPduId(txPduId);

    lResult = Dcm_NetGetTranspObjOfConnection(lConnHdl, &pTranspObj);                                                                                /* SBSW_DCM_POINTER_FORWARD_STACK */

    if(lResult == BUFREQ_OK)
    {
      if(Dcm_DebugDetectError(pTranspObj->State != DCM_NET_TOBJ_STATE_ONTX))
      {
        lErrorId = DCM_E_ILLEGAL_STATE;
      }
      else
      {
        Dcm_NetUsdtTxConfirmation(pTranspObj, result);                                                                                               /* SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION */
      }
    }
    else
    {
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TPTXCONFIRMATION, lErrorId);
}

/**********************************************************************************************************************
 *  Dcm_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TxConfirmation(                                                                                                             /* PRQA S 6050 */ /* MD_MSR_STCAL */
  PduIdType txPduId
  )
{
#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
  uint8                         lErrorId = DCM_E_NO_ERROR;
  Dcm_NetConnRefMemType         lConnHdl = Dcm_NetPbCfgGetConnHdlOfTxPduId(txPduId);

  DCM_IGNORE_UNREF_PARAM(txPduId);

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgGetConnection() != lConnHdl))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle(txPduId) >= Dcm_NetPeriodicMsgNumMsgsCurrConnection()))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_NetPeriodicMsgRelease(Dcm_NetPbCfgGetPeriodicTxObject(Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle(txPduId)));                                    /* SBSW_DCM_POINTER_WRITE_PERTXOBJ */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TXCONFIRMATION, lErrorId);
#else
  /* Function is not used since 0x2A or periodic messages are not available.
   * Note: PduR may still need this API to be able to link in case periodic messages are available, but SID 0x2A shall
   * not be supported.
   */
  DCM_IGNORE_UNREF_PARAM(txPduId);
#endif /* (DCM_NET_PERIODIC_TX_ENABLED == STD_ON) */
}

#if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_TriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerTransmit(                                                                                                  /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
  PduIdType txPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) pTxPduInfo
  )
{
  Std_ReturnType                lResult  = E_NOT_OK;                                                                                                 /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  uint8                         lErrorId = DCM_E_NO_ERROR;
  Dcm_NetConnRefMemType         lConnHdl = Dcm_NetPbCfgGetConnHdlOfTxPduId(txPduId);

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPbCfgIsTxPduIdValid(txPduId) == FALSE))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(pTxPduInfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(pTxPduInfo->SduDataPtr == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgGetConnection() != lConnHdl))
  {
    lErrorId = DCM_E_ILLEGAL_STATE;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_CfgNetPTxObjHandleMemType lPerTxObjHandle;

    lPerTxObjHandle = (Dcm_CfgNetPTxObjHandleMemType)Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle(txPduId);

    if(Dcm_DebugDetectError(Dcm_NetPeriodicMsgNumMsgsCurrConnection() <= lPerTxObjHandle))
    {
      lErrorId = DCM_E_ILLEGAL_STATE;
    }
    else
    {
      P2CONST(PduInfoType, AUTOMATIC, DCM_VAR_NOINIT) txPduInfoPtr = &(Dcm_NetPbCfgGetPeriodicTxObject(lPerTxObjHandle)->TxPduInfo);

      Dcm_UtiMemCopySafe(txPduInfoPtr->SduDataPtr
                        ,pTxPduInfo->SduDataPtr
                        ,0u
                        ,pTxPduInfo->SduLength
                        ,txPduInfoPtr->SduLength);                                                                                                   /* SBSW_DCM_COMB_PTR_FORWARD */

      pTxPduInfo->SduLength = txPduInfoPtr->SduLength;                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = E_OK;
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_TRIGGERTRANSMIT, lErrorId);

  return lResult;
}
#endif /* (DCM_NET_PERIODIC_TX_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Net.c
 *********************************************************************************************************************/
