/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_Connection.h
 *        \brief  Declarations for sub-module DoIP_Connection of component DoIPInt.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (DOIPINT_CONNECTION_H)
# define DOIPINT_CONNECTION_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Types.h"
# include "SoAd_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_Connection_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related variables used in DoIPInt_Connection sub-module.
 *  \details      -
 *  \param[in]    ConnectionIter          Connection iterator.
 *                                        [range: ConnectionIter < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-DoIPIntInitialization
 *  \spec
 *    requires ConnectionIter < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_Connection_GetConnectionIdxBySoConId()
 *********************************************************************************************************************/
/*! \brief        Searches for the connection with the requested socket connection index and returns its index.
 *  \details      -
 *  \param[in]    SoConId                 Socket connection index.
 *  \param[out]   ConnectionIdxPtr        Pointer to index of connection.
 *                                        [range: IN:  ConnectionIdxPtr != NULL_PTR
 *                                                OUT: *ConnectionIdxPtr < DoIPInt_GetSizeOfConnection()]
 *  \return       E_OK                    Connection found and ConnectionIdxPtr contains a valid index.
 *  \return       E_NOT_OK                No connection found. ConnectionIdxPtr is set to size of connection array.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdxPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Connection_GetConnectionIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(DoIPInt_SizeOfConnectionType, AUTOMATIC, DOIPINT_VAR) ConnectionIdxPtr);

/**********************************************************************************************************************
 *  DoIPInt_Connection_IsOpened()
 *********************************************************************************************************************/
/*! \brief        Indicates if connection is opened.
 *  \details      Considers socket connection state and requested state on connection.
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       TRUE                    Connection is open.
 *  \return       FALSE                   Connection is not open.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, DOIPINT_CODE) DoIPInt_Connection_IsOpened(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Connection_IsClosedOrRequested()
 *********************************************************************************************************************/
/*! \brief        Indicates if connection is closed or requested to be closed.
 *  \details      Considers current socket connection state, the state requested at SoAd and the state requested by
 *                DoIPInt's user.
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       TRUE                    Connection is closed/requested to be closed.
 *  \return       FALSE                   Connection is not closed/requested to be closed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, DOIPINT_CODE) DoIPInt_Connection_IsClosedOrRequested(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Connection_ProcessState()
 *********************************************************************************************************************/
/*! \brief        Processes the connection state of all connections. Closes connections with pending close request and
 *                reopens connections if required.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_ProcessState(void);

/**********************************************************************************************************************
 *  DoIPInt_Connection_CloseConnection()
 *********************************************************************************************************************/
/*! \brief        Calls the socket adaptor to close a socket connection.
 *  \details      -
 *  \param[in]    ConnectionId            Connection identifier.
 *                                        [range: ConnectionId < DoIPInt_GetSizeOfConnectionIdMap()]
 *  \param[in]    Abort                   Flag to close socket connection immediately.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionId, TRUE otherwise
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionId < DoIPInt_GetSizeOfConnectionIdMap();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_CloseConnection(
  DoIPInt_ConnectionIdType ConnectionId,
  boolean Abort);

/**********************************************************************************************************************
 *  DoIPInt_Connection_ModeChg()
 *********************************************************************************************************************/
/*! \brief        Receives notification for socket connection mode change.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Mode                    New socket connection mode.
 *                                        [range: Mode == SOAD_SOCON_*]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_ModeChg(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  SoAd_SoConModeType Mode);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_CONNECTION_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Connection.h
 *********************************************************************************************************************/
