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
/**        \file  Dcm_Net.h
 *         \unit  Net
 *        \brief  Contains public types, function declarations and inline function definitions of Network unit.
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
#if !defined(DCM_NET_H)
# define DCM_NET_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_NetTypes.h"
# include "Dcm_NetPbCfg.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Calculate the maximum number of events related to USDT transport objects, needed by a network related task */
# if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
#  define DCM_NET_MAX_NUM_EVENT_TRANSP_OBJECTS                       32u                            /*!< Take the worst case, limited to 32 for a bit mask usage (i.e. task events) */
# else
#  define DCM_NET_MAX_NUM_EVENT_TRANSP_OBJECTS                       DCM_NET_MAX_NUM_TRANSP_OBJECTS /*!< Allocate as many as needed by the concrete configuration */
# endif

/*! ComM channel message communication ability states */
# define DCM_NET_COMM_STATE_FLAG_RX_DIS                              ((Dcm_NetComMStateType)0x00u ) /*!< Message reception is disabled */
# define DCM_NET_COMM_STATE_FLAG_TX_DIS                              ((Dcm_NetComMStateType)0x00u ) /*!< Message transmission is disabled */
# define DCM_NET_COMM_STATE_FLAG_TX_EN                               ((Dcm_NetComMStateType)0x01u ) /*!< Message transmission is enabled */
# define DCM_NET_COMM_STATE_FLAG_RX_EN                               ((Dcm_NetComMStateType)0x02u ) /*!< Message reception is enabled */

# define DCM_NET_INVALID_TOBJID                                      (Dcm_NetPbCfgGetNumTransportObjects())
# define DCM_NET_INVALID_CONNHDL                                     (Dcm_NetPbCfgGetNumConnections())
# define DCM_NET_INVALID_PROTID                                      (Dcm_NetPbCfgGetNumProtocols())
# define DCM_NET_INVALID_PTXOBJ_HANDLE                               (Dcm_NetPbCfgGetNumPerTxObjects())
# define DCM_NET_INVALID_RXPDUID                                     (Dcm_NetPbCfgGetNumRxPduIds())

/*! Calculates a combined reception and transmission message communication state */
# define DCM_NET_COMM_STATE_COMPOSER(rx,tx)                          ((Dcm_NetComMStateType)((rx)|(tx)))                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! Presets of all possible message communication states in both directions (Rx and Tx) */
# define DCM_NET_COMM_STATE_RX_DIS_TX_DIS                            (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_DIS,DCM_NET_COMM_STATE_FLAG_TX_DIS)) /*!< Message reception and transmission are disabled (NoCommunicationMode) */
# define DCM_NET_COMM_STATE_RX_DIS_TX_EN                             (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_DIS,DCM_NET_COMM_STATE_FLAG_TX_EN))  /*!< Message reception is disabled, but transmission is enabled */
# define DCM_NET_COMM_STATE_RX_EN_TX_DIS                             (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_EN, DCM_NET_COMM_STATE_FLAG_TX_DIS)) /*!< Message reception is enabled, but transmission is disabled (SilentCommunicationMode) */
# define DCM_NET_COMM_STATE_RX_EN_TX_EN                              (DCM_NET_COMM_STATE_COMPOSER(DCM_NET_COMM_STATE_FLAG_RX_EN, DCM_NET_COMM_STATE_FLAG_TX_EN))  /*!< Message reception and transmission are enabled (FullCommunicationMode) */

/*! USDT transport object flags */
# define DCM_NET_TOBJ_FLAG_NONE                                      ((Dcm_NetTransportObjectFlagType)0x0000u)  /*!< No flags set */
# define DCM_NET_TOBJ_FLAG_INTERNAL                                  ((Dcm_NetTransportObjectFlagType)0x0001u)  /*!< The transport object is used for internal (virtual) request (e.g. RoE) */
# define DCM_NET_TOBJ_FLAG_COPYHEAD                                  ((Dcm_NetTransportObjectFlagType)0x0002u)  /*!< The diagnostic request head shall be copied into a temporary buffer */
# define DCM_NET_TOBJ_FLAG_BUSY                                      ((Dcm_NetTransportObjectFlagType)0x0004u)  /*!< Current diagnostic request reception is a pseudo-parallel one */
# define DCM_NET_TOBJ_FLAG_CANCELED                                  ((Dcm_NetTransportObjectFlagType)0x0008u)  /*!< Current message transmission (Rx/Tx) was canceled */
# define DCM_NET_TOBJ_FLAG_RCRRP_TX_REQ                              ((Dcm_NetTransportObjectFlagType)0x0010u)  /*!< RCR-RP state: just requested to be sent */
# define DCM_NET_TOBJ_FLAG_RCRRP_ON_TX                               ((Dcm_NetTransportObjectFlagType)0x0020u)  /*!< RCR-RP state: currently on transmission */
# define DCM_NET_TOBJ_FLAG_RCRRP_SENT                                ((Dcm_NetTransportObjectFlagType)0x0040u)  /*!< RCR-RP state: at least once has been sent */
# define DCM_NET_TOBJ_FLAG_RCRRP_TYPE_APPL                           ((Dcm_NetTransportObjectFlagType)0x0080u)  /*!< RCR-RP state: current one was triggered by the application */
# define DCM_NET_TOBJ_FLAG_OBSOLETE                                  ((Dcm_NetTransportObjectFlagType)0x0100u)  /*!< Transport object is obsolete and shall be canceled at later time. The difference to FLAG_CANCELED is that the ongoing transmission is not affected */
# define DCM_NET_TOBJ_FLAG_IGNORE                                    ((Dcm_NetTransportObjectFlagType)0x0200u)  /*!< Transport object shall not restart S3 and Keep-Alive Timers */

/*! USDT transport object response types */
# define DCM_NET_TOBJ_RESTYPE_NONE                                   ((Dcm_NetResponseType)0u)  /*!< No response is ongoing */
# define DCM_NET_TOBJ_RESTYPE_LINEAR                                 ((Dcm_NetResponseType)1u)  /*!< Final response (linear buffer) */
# define DCM_NET_TOBJ_RESTYPE_PAGED                                  ((Dcm_NetResponseType)2u)  /*!< Final response (paged-buffer) */
# define DCM_NET_TOBJ_RESTYPE_RCRRP                                  ((Dcm_NetResponseType)3u)  /*!< RCR-RP response */
# define DCM_NET_TOBJ_RESTYPE_FBL_FINALRSP                           ((Dcm_NetResponseType)4u)  /*!< Unsolicited response sent during jump from Fbl */
# define DCM_NET_TOBJ_RESTYPE_SIMPLE                                 ((Dcm_NetResponseType)5u)  /*!< Simply respond (without processing) e.g. final negative response NRC 0x21 for pseudo parallel client handling or protocol switch rejection */
# define DCM_NET_TOBJ_RESTYPE_ROE_INTMRSP                            ((Dcm_NetResponseType)6u)  /*!< Intermediate response for Svc86 */

/*! Diagnostic client USDT connection properties */
# define DCM_NET_CONN_PROP_NONE                                      ((Dcm_CfgNetConnPropertiesType)0x00u)  /*!< Nothing special */
# define DCM_NET_CONN_PROP_TX_ON_FUNC_RX                             ((Dcm_CfgNetConnPropertiesType)0x01u)  /*!< No response to be sent on functional requests over this connection */
# define DCM_NET_CONN_PROP_NO_MAIN_TX                                ((Dcm_CfgNetConnPropertiesType)0x02u)  /*!< The USDT connection does not contain a PduRTxPduId */
# define DCM_NET_CONN_PROP_READY_INDICATION_ENABLED                  ((Dcm_CfgNetConnPropertiesType)0x04u)  /*!< Channel ready indication dis-/enabled for the USDT connection */

/*! State definitions for periodic transport object */
# define DCM_NET_PERIODIC_TX_STATE_FREE                              ((Dcm_NetPerTxObjStateMemType)0)
# define DCM_NET_PERIODIC_TX_STATE_RESERVED                          ((Dcm_NetPerTxObjStateMemType)1)
# define DCM_NET_PERIODIC_TX_STATE_QUEUED                            ((Dcm_NetPerTxObjStateMemType)2)
# define DCM_NET_PERIODIC_TX_STATE_ONTX                              ((Dcm_NetPerTxObjStateMemType)3)

/*! AR 4.1.2+ PduR API argument values adaption */
# define DCM_NET_ARENV_NTFRSLT_OK                                    E_OK
# define DCM_NET_ARENV_NTFRSLT_NOT_OK                                E_NOT_OK

/*! USDT transport object states */
# define DCM_NET_TOBJ_STATE_FREE                                     ((Dcm_NetTransportObjectStateType)0x00u)  /*!< Free to be used */
# define DCM_NET_TOBJ_STATE_ONRX                                     ((Dcm_NetTransportObjectStateType)0x01u)  /*!< USDT reception in progress */
# define DCM_NET_TOBJ_STATE_RX_END                                   ((Dcm_NetTransportObjectStateType)0x02u)  /*!< USDT reception already finished */
# define DCM_NET_TOBJ_STATE_PREPTX                                   ((Dcm_NetTransportObjectStateType)0x04u)  /*!< Ready for USDT transmission */
# define DCM_NET_TOBJ_STATE_ONTX                                     ((Dcm_NetTransportObjectStateType)0x08u)  /*!< USDT transmission in progress */
# define DCM_NET_TOBJ_STATE_READY                                    ((Dcm_NetTransportObjectStateType)0x10u)  /*!< Transmission (Rx/Tx) finished but transport object still in use */
# define DCM_NET_TOBJ_STATE_RESERVED                                 ((Dcm_NetTransportObjectStateType)0x20u)  /*!< Transport object is allocated, but not yet in use (transmission) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_NetBufferLock(bufferHdl)                                (Dcm_NetBufferUsageEnter((bufferHdl)))                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_NetBufferRelease(bufferHdl)                             (Dcm_NetBufferUsageLeave((bufferHdl)))                                          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! USDT buffer management */
# define Dcm_NetBufferIsFree(bufferHdl)                              (Dcm_NetPbCfgGetBufferContext((bufferHdl))->IsInUseCnt == 0u)                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/*! Periodic message transmitter control states */
struct DCM_NETPERIODICTXCONTEXTTYPE_TAG
{
#  if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
    volatile Dcm_TmrTimerCntrMemType DelayTimer[DCM_NET_DELAY_BULK_TRANSMISSION]; /*!< Array of delay timers that assure gap times between periodic DIDs */
#  endif
  uint16                           ClientSrcAddr;    /*!< Client source address. Required only when generic connection is used */
#  if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
  Dcm_CfgNetPTxObjHandleMemType    DelayCntr;        /*!< Number of remaining messages that can be dispatched before a delay is required */
  uint8                            NextDelayTimer;   /*!< Index of next delay timer to use */
  uint8                            FastestRate;      /*!< The fastest rate in which periodic DIDs are currently scheduled */
#  endif
  Dcm_NetConnRefMemType            ConnHdlInUse;      /*!< Current owner (diagnostic client) of the periodic transmitter */
  Dcm_CfgNetPTxObjHandleMemType    NextTxObjectHdl;  /*!< Pointer to the next periodic message to be sent. Scope is only the current periodic transmitter owners list of periodic messages */
  Dcm_CfgNetPTxObjHandleMemType    TxObjectSentCntr; /*!< Keeps track of the already sent periodic messages. Scope is only the current periodic transmitter owners list of periodic messages */
};
typedef struct DCM_NETPERIODICTXCONTEXTTYPE_TAG Dcm_NetPeriodicTxContextType;
typedef P2VAR(Dcm_NetPeriodicTxContextType, TYPEDEF, DCM_RAM_PBCFG) Dcm_NetPeriodicTxContextPtrType;
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if(DCM_NET_GENERIC_CONNECTION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetIsGenericConnection()
 *********************************************************************************************************************/
/*! \brief          Checks whether the connection is generic.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         boolean value on whether connection is generic.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetIsGenericConnection(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSupportedFor()
 *********************************************************************************************************************/
/*! \brief          Returns if periodic messages are supported for the given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         TRUE       The periodic messages are supported
 *  \return         FALSE      The periodic messages are not supported
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetPeriodicMsgSupportedFor(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetConnection()
 *********************************************************************************************************************/
/*! \brief          Returns the current owner (diagnostic client) of the periodic transmitter.
 *  \details        -
 *  \return         Current owner (diagnostic client) of the periodic transmitter
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPeriodicMsgGetConnection(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetConnection()
 *********************************************************************************************************************/
/*! \brief          Set the current owner (diagnostic client) of the periodic transmitter.
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetConnection(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetClientSrcAddr()
 *********************************************************************************************************************/
/*! \brief          Returns the diagnostic client's unique source address (identifier).
 *  \details        -
 *  \return         Client's unique source address (identifier)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint16, DCM_CODE) Dcm_NetPeriodicMsgGetClientSrcAddr(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetClientSrcAddr()
 *********************************************************************************************************************/
/*! \brief          Set the diagnostic client's unique source address (identifier).
 *  \details        -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetClientSrcAddr(
  uint16 clientSrcAddr
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetSetSessionConnection()
 *********************************************************************************************************************/
/*! \brief          Set the owner of the non-default session.
 *  \details        -
 *  \param[in]      connHdl    The connectionId of the owner
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetSetSessionConnection(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetGetSessionClientSrcAddr()
 *********************************************************************************************************************/
/*! \brief          Returns the source address of the non-default session owner.
 *  \details        -
 *  \return         The source address of the owner
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint16, DCM_CODE) Dcm_NetGetSessionClientSrcAddr(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetSetSessionClientSrcAddr()
 *********************************************************************************************************************/
/*! \brief          Set the source address of the non-default session owner.
 *  \details        -
 *  \param[in]      clientSrcAddress    The source address of the owner
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetSetSessionClientSrcAddr(
  uint16 clientSrcAddress
  );

/**********************************************************************************************************************
 *  Dcm_NetGetKeepAlive()
 *********************************************************************************************************************/
/*! \brief          Returns the keep alive guard.
 *  \details        -
 *  \return         Keep alive guard
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (boolean, DCM_CODE) Dcm_NetGetKeepAlive(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetSetKeepAlive()
 *********************************************************************************************************************/
/*! \brief          Set the keep alive guard status.
 *  \details        -
 *  \param[in]      keepAlive    Keep alive guard
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (void, DCM_CODE) Dcm_NetSetKeepAlive(
  boolean keepAlive
  );

/**********************************************************************************************************************
 *  Dcm_NetSetRxAllowed()
 *********************************************************************************************************************/
/*! \brief          Sets the RxAllowed value.
 *  \details        -
 *  \param[in]      rxAllowed    Value to be set.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC (void, DCM_CODE) Dcm_NetSetRxAllowed(
  boolean rxAllowed
  );

/**********************************************************************************************************************
 *  Dcm_NetGetSessionConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the session connection.
 *  \details        -
 *  \return         session connection
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetSessionConnection(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetGetProtIdOfActiveProtocol()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol Id of the active protocol.
 *  \details        -
 *  \return         protocol Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetGetProtIdOfActiveProtocol(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetSetProtIdOfActiveProtocol()
 *********************************************************************************************************************/
/*! \brief          Set the protocol Id of the active protocol.
 *  \details        -
 *  \param[in]      protId                 protocol identifier
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetSetProtIdOfActiveProtocol(
  Dcm_NetProtRefMemType protId
  );

/**********************************************************************************************************************
 *  Dcm_NetSetActiveProtocolConnHdl()
 *********************************************************************************************************************/
/*! \brief          Set the protocol Id of the active protocol using connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetSetActiveProtocolConnHdl(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetIsComMChannelTransmissionReady()
 *********************************************************************************************************************/
/*! \brief          Checks wether the ComM Channel of the given transport object is ready for transmission.
 *  \details        If ComM State == FullCom and if necessary ComM Availability == Ready return true.
 *  \param[in]      pTranspObj    The pointer to a transport object
 *  \return         TRUE          ComM Channel is in FullCom and Ready
 *  \return         FALSE         ComM Channel is not in FullCom or not Ready
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetIsComMChannelTransmissionReady(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgGetTxBuffer()
 *********************************************************************************************************************/
/*! \brief          Return the tx buffer of the periodic message with the given index.
 *  \details        -
 *  \param[in]      perTxObjHandle    Handle of periodic transport object to be transmitted
 *  \return         Tx buffer of periodic transport object
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            The periodic message shall first be allocated via Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
FUNC(Dcm_MsgType, DCM_CODE) Dcm_NetPeriodicMsgGetTxBuffer(
  Dcm_CfgNetPTxObjHandleOptType perTxObjHandle
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgAllocate()
 *********************************************************************************************************************/
/*! \brief          Periodic message allocation.
 *  \details        Allocates a resource for a periodic message.
 *  \param[in]      Did     DID of the message that is allocated
 *  \return         A periodic Tx handle
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPeriodicMsgAllocate(
  uint16 Did
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgRelease()
 *********************************************************************************************************************/
/*! \brief          Releases a concrete periodic message resource.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgRelease(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgResetConnection()
 *********************************************************************************************************************/
/*! \brief          Resets the periodic message connection.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgResetConnection(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgStopDid()
 *********************************************************************************************************************/
/*! \brief          Stops transmission of DID and also removes queued transport objects.
 *  \details        -
 *  \param[in]      Did      DID that is to be stopped
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgStopDid(
  uint16 Did
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgTransmit()
 *********************************************************************************************************************/
/*! \brief          Transmits a periodic message.
 *  \details        -
 *  \param[in]      pPeriodicTxObj    The periodic transport object
 *  \param[in]      length            The periodic response message length
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgTransmit(
  Dcm_NetPeriodicTxObjectPtrType pPeriodicTxObj,
  PduLengthType length
  );
# endif

# if ( (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON) || (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON) || (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON) )
/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByConnectionId()
 *********************************************************************************************************************/
/*! \brief          Tries to find the corresponding connection of a connection handle.
 *  \details        -
 *  \param[in]      connectionId                 Connection identifier
 *  \return         connection handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByConnectionId(
  uint16 connectionId
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetGetConnHdlByTesterAddress()
 *********************************************************************************************************************/
/*! \brief          Tries to find the corresponding connection of a diagnostic client.
 *  \details        -
 *  \param[in]      testerAddress    The diagnostic client address to look for
 *  \return         connection handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetGetConnHdlByTesterAddress(
  uint16 testerAddress
  );

/**********************************************************************************************************************
 *  Dcm_NetGetComState()
 *********************************************************************************************************************/
/*! \brief          Returns the communication state for the given RxPduId.
 *  \details        -
 *  \param[in]      rxPduId    The RxPDUID of the request
 *  \return         The communication state
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetComMStateType, DCM_CODE) Dcm_NetGetComState(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageLeave()
 *********************************************************************************************************************/
/*! \brief          Decrements the buffer usage counter.
 *  \details        -
 *  \param[in]      bufferHdl    buffer handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetBufferUsageLeave(
  Dcm_CfgNetBufferRefMemType bufferHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetBufferUsageEnter()
 *********************************************************************************************************************/
/*! \brief          Increments the buffer usage counter.
 *  \details        -
 *  \param[in]      bufferHdl    buffer handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetBufferUsageEnter(
  Dcm_CfgNetBufferRefMemType bufferHdl
  );

# if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetRxIndInternal()
 *********************************************************************************************************************/
/*! \brief          Provides a means for virtual/internal diagnostic request.
 *  \details        This API is generally used for RoE (SID 0x86) purposes.
 *  \param[in]      rxPduId            The RxPDUID of the request
 *  \param[in]      sourceAddress      Client or tester address
 *  \param[in]      data               The data of the request
 *  \param[in]      length             The length of the request
 *  \return         DCM_E_OK           Reception was successful
 *  \return         DCM_E_NOT_OK       Reception was not successful
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_NetRxIndInternal(
  PduIdType rxPduId,
  uint16 sourceAddress,
  Dcm_MsgType data,
  PduLengthType length
  );
# endif

# if (DCM_NET_UUDT_DELAYTIMER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPeriodicMsgSetFastestRate()
 *********************************************************************************************************************/
/*! \brief          Sets a new fastest rate.
 *  \details        -
 *  \param[in]      Rate    The periodic rate
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPeriodicMsgSetFastestRate(
  Dcm_Svc2ASchedRateOptType Rate
  );
# endif

# if (DCM_NET_CONN_LOCK_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetLockConnection()
 *********************************************************************************************************************/
/*! \brief          Lock a connection.
 *  \details        -
 *  \param[in]      Rate    The periodic rate
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_NetLockConnection(
  Dcm_NetConnRefMemType connHdl,
  CONSTP2VAR(Dcm_NetTransportObjectPtrType, AUTOMATIC, AUTOMATIC) pTranspObj
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetComposeAddNegResponse()
 *********************************************************************************************************************/
/*! \brief          Composes a negative response.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \param[in]      resType       The response type
 *  \param[in]      nrc           The negative response code
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetComposeAddNegResponse(
  Dcm_NetTransportObjectPtrType pTranspObj,
  Dcm_NetResponseType resType,
  Dcm_NegativeResponseCodeType nrc
  );

/**********************************************************************************************************************
 *  Dcm_NetComMActiveDiagnostic()
 *********************************************************************************************************************/
/*! \brief          Informs the ComM component about an active network.
 *  \details        -
 *  \param[in]      netHdl    The network handle
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetComMActiveDiagnostic(
  NetworkHandleType netHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetRegisterActiveConnection()
 *********************************************************************************************************************/
/*! \brief          Registers an active connection.
 *  \details        Function will be called from Dcm_NetStartOfReception.
 *  \param[in]      pTranspObj    The transport object
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterActiveConnection()
 *********************************************************************************************************************/
/*! \brief          Unregister a connection.
 *  \details        Unregister an active connection referenced by the given transport object.
 *  \param[in]      pTranspObj    The transport object to be released
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetUnRegisterActiveConnection(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetRegisterComMActivity()
 *********************************************************************************************************************/
/*! \brief          Tries to register a ComM channel activity.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object which starts diagnostic job
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetRegisterComMActivity(
  Dcm_NetTransportObjectPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterComMActivity()
 *********************************************************************************************************************/
/*! \brief          Tries to unregister a ComM channel activity.
 *  \details        -
 *  \param[in]      connHdl    The connection which has finished its job
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Must be called only for external diagnostic requests
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetUnRegisterComMActivity(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetUnRegisterAllComMActivity()
 *********************************************************************************************************************/
/*! \brief          Tries to unregister all ComM channels activities.
 *  \details        -
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetUnRegisterAllComMActivity(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetWriteAddBufferU8At()
 *********************************************************************************************************************/
/*! \brief          Writes into the additional buffer of a transport object.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \param[in]      index         Index to the additional buffer
 *  \param[in]      value         New value of the transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetWriteAddBufferU8At(
  Dcm_NetTransportObjectPtrType pTranspObj,
  uint8_least index,
  Dcm_MsgItemType value
  );

/**********************************************************************************************************************
 *  Dcm_NetInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the network sub-module.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetInit(
  Dcm_ConfigPtrType configPtr
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_NET_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Net.h
 *********************************************************************************************************************/
