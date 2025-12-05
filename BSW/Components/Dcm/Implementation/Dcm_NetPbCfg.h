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
/**        \file  Dcm_NetPbCfg.h
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
#if !defined(DCM_NETPBCFG_H)
# define DCM_NETPBCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_NetPbCfgTypes.h"
# include "Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef P2CONST(NetworkHandleType, TYPEDEF, DCM_CONST_PBCFG) Dcm_PbCfgNetNetworkHandlePtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dcm_NetPbCfgInit()
 *********************************************************************************************************************/
/*! \brief          Initialization of the network Configurations parameters.
 *  \details        -
 *  \param[in]      configPtr    Pointer to a concrete configuration root
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPbCfgInit(
  Dcm_ConfigPtrType configPtr
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgIsRxPduIdValid()
 *********************************************************************************************************************/
/*! \brief          Checks the received rxPduId.
 *  \details        -
 *  \param[in]      rxPduId    The rxPduId of the request
 *  \return         TRUE       The rxPduId is valid
 *  \return         FALSE      The rxPduId is not valid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgIsRxPduIdValid(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgIsTxPduIdValid()
 *********************************************************************************************************************/
/*! \brief          Checks the received txPduId.
 *  \details        -
 *  \param[in]      txPduId    The txPduId of the request
 *  \return         TRUE       The txPduId is valid
 *  \return         FALSE      The txPduId is not valid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgIsTxPduIdValid(
  PduIdType txPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumRxPduIds()
 *********************************************************************************************************************/
/*! \brief          Gets the number of RxPduIds of the active variant.
 *  \details        -
 *  \return         Number of RxPduIds
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetNumRxPduIds(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetBufferContext()
 *********************************************************************************************************************/
/*! \brief          Returns a buffer context element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a buffer context
 *  \return         Buffer context object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetBufferContextPtrType, DCM_CODE) Dcm_NetPbCfgGetBufferContext(
  Dcm_CfgNetBufferRefOptType index
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTesterAddressOfRxPduId()
 *********************************************************************************************************************/
/*! \brief          Gets tester address of a specific rxPduId.
 *  \details        -
 *  \param[in]      rxPduId   The RxPduId of the request
 *  \return         Tester source address
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint16, DCM_CODE) Dcm_NetPbCfgGetTesterAddressOfRxPduId(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTransportObject()
 *********************************************************************************************************************/
/*! \brief          Returns a transport object element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a transport object
 *  \return         Transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetTransportObjectPtrType, DCM_CODE) Dcm_NetPbCfgGetTransportObject(
  Dcm_CfgNetTObjHandleOptType index
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetComMContext()
 *********************************************************************************************************************/
/*! \brief          Returns a ComM context element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a ComM context
 *  \return         ComM context object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetComMContextPtrType, DCM_CODE) Dcm_NetPbCfgGetComMContext(
  Dcm_CfgNetNetIdRefOptType index
  );

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetPeriodicTxObject()
 *********************************************************************************************************************/
/*! \brief          Returns a periodic transport object element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a periodic transport object
 *  \return         Periodic transport object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetPeriodicTxObjectPtrType, DCM_CODE) Dcm_NetPbCfgGetPeriodicTxObject(
  Dcm_CfgNetPTxObjHandleOptType index
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgPutToConn2TObjMap()
 *********************************************************************************************************************/
/*! \brief          Sets a transport object handle of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a transport object handle
 *  \param[in]      value    New handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPbCfgPutToConn2TObjMap(
  Dcm_NetConnRefOptType index,
  Dcm_CfgNetTObjHandleMemType value
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTObjConn2TObjMap()
 *********************************************************************************************************************/
/*! \brief          Gets a transport object handle of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a transport object handle
 *  \return         Return transport object handle for the give connection handle
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetTObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetTObjConn2TObjMap(
  Dcm_NetConnRefOptType index
  );

# if (DCM_MODE_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetComCtrlChannelState()
 *********************************************************************************************************************/
/*! \brief          Gets the communication state [Rx/Tx][On/Off] of each ComM channel known to DCM.
 *  \details        -
 *  \return         Communication state
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CommunicationModeType, DCM_CODE) Dcm_NetPbCfgGetComCtrlChannelState(
  uint8_least index
  );
# endif

# if (DCM_NET_COMMCTRL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgSetComControlChannelState()
 *********************************************************************************************************************/
/*! \brief          Sets the state of a communication control channel of the given index.
 *  \details        -
 *  \param[in]      index    Index to the communication control channel
 *  \param[in]      value    New communication mode
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetPbCfgSetComControlChannelState(
  uint8_least index,
  Dcm_CommunicationModeType value
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetBufferInfo()
*********************************************************************************************************************/
/*! \brief          Returns a buffer information element of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a buffer information element
 *  \return         Buffer info object
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_PbCfgNetBufferInfoPtrType, DCM_CODE) Dcm_NetPbCfgGetBufferInfo(
  Dcm_CfgNetBufferRefOptType index
  );

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle()
 *********************************************************************************************************************/
/*! \brief          Return the DCM periodic message index corresponding to TxPduId.
 *  \details        -
 *  \param[in]      txPduId    The txPduId of the request
 *  \return         DCM periodic message index corresponding to TxPduId.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPbCfgPeriodicMsgGetTxPduIdHandle(
  PduIdType txPduId
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnHdlOfRxPduId()
 *********************************************************************************************************************/
/*! \brief          Returns the reference to Dcm_CfgNetConnectionInfo[] entry of the rxPduID.
 *  \details        -
 *  \param[in]      rxPduId   The RxPduId of the request
 *  \return         reference to Dcm_CfgNetConnectionInfo[] entry of the rxPduID.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnHdlOfRxPduId(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnHdlOfTxPduId()
 *********************************************************************************************************************/
/*! \brief          Returns the reference to a USDT diagnostic client connection configuration entry of the TxPduID.
 *  \details        -
 *  \param[in]      txPduId   The TxPDUID of the request
 *  \return         The reference to a USDT diagnostic client connection configuration entry of the TxPduID.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnHdlOfTxPduId(
  PduIdType txPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnTesterAddress()
 *********************************************************************************************************************/
/*! \brief          Returns the tester source address corresponding to connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         TRUE       Tester source address
 *  \return         FALSE      The tester source address corresponding to connection handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint16, DCM_CODE) Dcm_NetPbCfgGetConnTesterAddress(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtIdOfConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol reference for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Protocol reference Id for a given connection handle.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetProtIdOfConnection(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetMetaDataLengthOfConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the meta data length for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Meta data length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetMetaDataLengthOfConnection(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetPerTxConfTimeOutOfConn()
 *********************************************************************************************************************/
/*! \brief          Gets the periodic TX confirmation time out for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Periodic Tx confirmation time out
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_NetPbCfgGetPerTxConfTimeOutOfConn(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetIdRefOfConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the network ID reference for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Protocol reference Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetNetIdRefOfConnection(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetworkRefByConnHdl()
 *********************************************************************************************************************/
/*! \brief          Gets the network handle table entry for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         network table entry
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetPbCfgGetNetworkRefByConnHdl(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetworkRef()
 *********************************************************************************************************************/
/*! \brief          Gets the network handle table entry.
 *  \details        -
 *  \param[in]      netHandle    The connection handle
 *  \return         network table entry
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetPbCfgGetNetworkRef(
  Dcm_CfgNetNetIdRefOptType netHandle
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnId()
 *********************************************************************************************************************/
/*! \brief          Returns the connection Id for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Connection Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint16, DCM_CODE) Dcm_NetPbCfgGetConnId(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnProp()
 *********************************************************************************************************************/
/*! \brief          Returns the connection properties for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Connection properties
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetConnPropertiesType, DCM_CODE) Dcm_NetPbCfgGetConnProp(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetEcuAddress()
 *********************************************************************************************************************/
/*! \brief          Returns the ECU address for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         ECU address
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetEcuAddress(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnTxPduIdMain()
 *********************************************************************************************************************/
/*! \brief          Return the PduR's PduId of the main diagnostic response for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Main Tx PduId
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetConnTxPduIdMain(
  Dcm_NetConnRefMemType connHdl
  );

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnAuthInfoRef()
 *********************************************************************************************************************/
/*! \brief          Gets the reference of the authentication info for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Reference to the authentication info
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetAuthInfoRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnAuthInfoRef(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnAuthInfoRefByRxPduId()
 *********************************************************************************************************************/
/*! \brief          Returns the reference to Dcm_CfgAuthMgrNetConnInfo[] corresponding to rxPduId
 *  \details        -
 *  \param[in]      rxPduId    The rxPduId of the request
 *  \return         Reference to Dcm_CfgAuthMgrNetConnInfo[] corresponding to rxPduId
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetAuthInfoRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnAuthInfoRefByRxPduId(
  PduIdType rxPduId
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnUudtFrameSize()
 *********************************************************************************************************************/
/*! \brief          Returns the diagnostic client specific UUDT frame size for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Uudt frame size for a given connection handle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_UudtFrameSizeType, DCM_CODE) Dcm_NetPbCfgGetConnUudtFrameSize(
  Dcm_NetConnRefMemType connHdl
  );
# endif

# if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTpRxPduIdMin()
 *********************************************************************************************************************/
/*! \brief          Returns the lowest CAN transport object rxPduId.
 *  \details        -
 *  \return         Lowest CAN transport object rxPduId
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetTpRxPduIdMin(
  void
  );
# endif

# if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetTpRxPduIdMax()
 *********************************************************************************************************************/
/*! \brief          Returns the highest CAN transport object RxPduId.
 *  \details        -
 *  \return         Highest CAN transport object RxPduId
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetTpRxPduIdMax(
  void
  );
# endif

# if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetDcmRxPduIdOfCanTpPduId()
 *********************************************************************************************************************/
/*! \brief          Returns the DCM related RxPduId from the CanTp PduId.
 *  \details        -
 *  \param[in]      canTpRxPduId   The CanTp RxPduId
 *  \return         DCM related RxPduId from the CanTp PduId
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetDcmRxPduIdOfCanTpPduId(
  PduIdType canTpRxPduId
  );
# endif

# if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumCanRxPduIds()
 *********************************************************************************************************************/
/*! \brief          Returns the number of Can RxPduIds configured.
 *  \details        -
 *  \return         The number of Can RxPduIds configured
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgGetNumCanRxPduIds(
  void
  );
# endif

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumPerTxObjects()
 *********************************************************************************************************************/
/*! \brief          Returns the configured maximum number of periodic transmission objects.
 *  \details        -
 *  \return         Configured maximum number of periodic transmission objects
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetNumPerTxObjects(
  void
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumConnections()
 *********************************************************************************************************************/
/*! \brief          Returns the configured maximum number of connections
 *  \details        -
 *  \return         Configured maximum number of DCM connections
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetConnRefMemType, DCM_CODE) Dcm_NetPbCfgGetNumConnections(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumTransportObjects()
 *********************************************************************************************************************/
/*! \brief          Return the configured maximum number of parallel running connections
 *  \details        -
 *  \return         The configured maximum number of parallel running connections
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetTObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetNumTransportObjects(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNetworkHdlLookUp()
 *********************************************************************************************************************/
/*! \brief          Return a pointer to the DCM relevant network handles table
 *  \details        -
 *  \return         pointer to the DCM relevant network handles table
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_PbCfgNetNetworkHandlePtrType, DCM_CODE) Dcm_NetPbCfgGetNetworkHdlLookUp(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumProtocols()
 *********************************************************************************************************************/
/*! \brief          Returns the total number of configured protocols
 *  \details        -
 *  \return         The total number of configured protocols
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetNumProtocols(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetAllComMChannelMap()
 *********************************************************************************************************************/
/*! \brief          Returns the entry of the given index of the Dcm_PbCfgNetAllComMChannelMap() abstraction.
 *  \details        -
 *  \param[in]      index    Index to a network handle
 *  \return         Network handle entry
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(NetworkHandleType, DCM_CODE) Dcm_NetPbCfgGetAllComMChannelMap(
  Dcm_CfgNetNetIdRefOptType index
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumComMChannels()
 *********************************************************************************************************************/
/*! \brief          Returns the configured maximum number of ComM channels to keep ECU awake
 *  \details        -
 *  \return         The configured maximum number of ComM channels to keep ECU awake
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(NetworkHandleType, DCM_CODE) Dcm_NetPbCfgGetNumComMChannels(
  void
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumBuffers()
 *********************************************************************************************************************/
/*! \brief          Returns the number of USDT buffers.
 *  \details        -
 *  \return         Number of USDT buffers
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetBufferRefMemType, DCM_CODE) Dcm_NetPbCfgGetNumBuffers(
  void
  );

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgPeriodicMsgGetTxPduIdByConn()
 *********************************************************************************************************************/
/*! \brief          Return the TxPduId coressponding to the connection handle
 *  \details        -
 *  \param[in]      connHdl    The connection handler
 *  \param[in]      txObjIdx   the txObj index
 *  \return         TxPduId Corresponding to a connection
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(PduIdType, DCM_CODE) Dcm_NetPbCfgPeriodicMsgGetTxPduIdByConn(
  Dcm_NetConnRefMemType         connHdl,
  Dcm_CfgNetPTxObjHandleOptType txObjIdx
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetNumAllComMChannels()
 *********************************************************************************************************************/
/*! \brief          Gets the number of channels from main connections and ComControl channels
 *  \details        -
 *  \return         Number of channels from main connections and ComControl channels
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(NetworkHandleType, DCM_CODE) Dcm_NetPbCfgGetNumAllComMChannels(
  void
  );

# if (DCM_NET_COMCTRL_ALLNET_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetComCtrlChannelListAll()
 *********************************************************************************************************************/
/*! \brief          Gets the list of all ComM channels.
 *  \details        -
 *  \return         List of all ComM channels
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetNetIdRefMemType, DCM_CODE) Dcm_NetPbCfgGetComCtrlChannelListAll(
  uint8_least index
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolId()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol Id for a given protocol.
 *  \details        -
 *  \param[in]      handle    The protocol handle
 *  \return         protocol Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(Dcm_ProtocolType, DCM_CODE) Dcm_NetPbCfgGetProtocolId(
  Dcm_NetProtRefMemType handle
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetThreadIdOfConnection()
 *********************************************************************************************************************/
/*! \brief          Gets the thread ID of connection.
 *  \details        -
 *  \param[in]      connHdl    The connection handler of the request
 *  \return         thread Id of a given connection handler
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(Dcm_ProtocolType, DCM_CODE) Dcm_NetPbCfgGetThreadIdOfConnection(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolIdOfTranspObj()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol Id of the transport object.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \return         protocol Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(Dcm_ProtocolType, DCM_CODE) Dcm_NetPbCfgGetProtocolIdOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

# if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON) || \
     (DCM_SVC_11_ECU_RESET_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetHasRespPendOnBoot()
 *********************************************************************************************************************/
/*! \brief          Returns the HasRespPendOnBoot value.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \return         HasRespPendOnBoot value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgGetHasRespPendOnBoot(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolRxTxBufferRefByRxPduId()
 *********************************************************************************************************************/
/*! \brief          Gets the RxTxBuffer handle for a given RxPduId.
 *  \details        -
 *  \param[in]      rxPduId    The rxPduId of the request
 *  \return         RxTxBuffer handle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetBufferRefMemType, DCM_CODE) Dcm_NetPbCfgGetProtocolRxTxBufferRefByRxPduId(
  PduIdType rxPduId
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetRxTxBufRefByConnHdl()
 *********************************************************************************************************************/
/*! \brief          Gets the RxTxBuffer handle for a given connection handler.
 *  \details        -
 *  \param[in]      connHdl    The connection handler of the request
 *  \return         RxTxBuffer handle
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetBufferRefMemType, DCM_CODE) Dcm_NetPbCfgGetRxTxBufRefByConnHdl(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolPriorityByConnHdl()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol priority for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handler of the request
 *  \return         Network protocol priority
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetProtPrioType, DCM_CODE) Dcm_NetPbCfgGetProtocolPriorityByConnHdl(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnProtIdOfRxPduId()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol reference for a given rxPduId.
 *  \details        -
 *  \param[in]      rxPduId    The rxPduId of the request
 *  \return         Protocol reference Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_NetProtRefMemType, DCM_CODE) Dcm_NetPbCfgGetConnProtIdOfRxPduId(
  PduIdType rxPduId
  );

# if (DCM_STATE_PROTOCOL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolStateRef()
 *********************************************************************************************************************/
/*! \brief          Gets the bit position of the current protocol in the precondition mask.
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \return         The bit position (not the bit mask) of the current protocol
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolStateRef(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolMaxLenOfTranspObj()
 *********************************************************************************************************************/
/*! \brief          Returns the maximum protocol length of the transport object
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \return         Maximum length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetProtLengthType, DCM_CODE) Dcm_NetPbCfgGetProtocolMaxLenOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolSrvAdjTime()
 *********************************************************************************************************************/
/*! \brief          Returns the server adjustment times of the given protocol.
 *  \details        -
 *  \param[in]      protocolIdx   The protocol Id relevant to the adjustment times
 *  \return         The pointer to the struct which contains the adjustment time for P2 and P2* timer
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_DiagP2TimingsConstPtrType, DCM_CODE) Dcm_NetPbCfgGetProtocolSrvAdjTime(
  Dcm_NetProtRefMemType protocolIdx
  );

# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetConnNumPeriodicTxPduIds()
 *********************************************************************************************************************/
/*! \brief          Returns the number of periodic messages of the diagnostic client.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Number of periodic messages of the diagnostic client
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetPTxObjHandleMemType, DCM_CODE) Dcm_NetPbCfgGetConnNumPeriodicTxPduIds(
  Dcm_NetConnRefMemType connHdl
  );
# endif

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolDemClientId()
 *********************************************************************************************************************/
/*! \brief          Gets the DEM Client Id for a given protocol.
 *  \details        -
 *  \param[in]      handle    The protocol handle
 *  \return         Dem client Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolDemClientId(
  Dcm_NetProtRefMemType handle
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolDemClientIdByConnHdl()
 *********************************************************************************************************************/
/*! \brief          Gets the Dem Client Id for a given connection handle.
 *  \details        -
 *  \param[in]      connHdl    The connection handle
 *  \return         Dem client Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolDemClientIdByConnHdl(
  Dcm_NetConnRefMemType connHdl
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolPriority()
 *********************************************************************************************************************/
/*! \brief          Gets the protocol priority for a given protocol reference.
 *  \details        -
 *  \param[in]      protRef    The protocol reference
 *  \return         Network protocol priority
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_CfgNetProtPrioType, DCM_CODE) Dcm_NetPbCfgGetProtocolPriority(
  Dcm_NetProtRefMemType protRef
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj()
 *********************************************************************************************************************/
/*! \brief          Returns the active thread Id of the given transport object
 *  \details        -
 *  \param[in]      pTranspObj    The transport object
 *  \return         Thread Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolThreadIdOfTranspObj(
  Dcm_NetTransportObjectConstPtrType pTranspObj
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgGetProtocolThreadId()
 *********************************************************************************************************************/
/*! \brief          Gets the thread Id for a given protocol.
 *  \details        -
 *  \param[in]      handle    The protocol handle
 *  \return         Thread Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
*********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_NetPbCfgGetProtocolThreadId(
  Dcm_NetProtRefMemType handle
  );

/**********************************************************************************************************************
 *  Dcm_NetPbCfgIsFuncRequest()
 *********************************************************************************************************************/
/*! \brief          Checks whether the request is a functional request.
 *  \details        -
 *  \param[in]      rxPduId   The rxPduId of the request
 *  \return         IsFuncReq value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_NetPbCfgIsFuncRequest(
  PduIdType rxPduId
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_NETPBCFG_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_NetPbCfg.h
 *********************************************************************************************************************/
