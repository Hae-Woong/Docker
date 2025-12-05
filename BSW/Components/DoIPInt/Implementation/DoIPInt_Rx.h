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
/*!        \file  DoIPInt_Rx.h
 *        \brief  Declarations for sub-module DoIP_Rx of component DoIPInt.
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

#if !defined (DOIPINT_RX_H)
# define DOIPINT_RX_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Types.h"

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
 *  DoIPInt_Rx_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related variables used in DoIPInt_Rx sub-module.
 *  \details      -
 *  \param[in]    ConnectionIter          Connection iterator.
 *                                        [range: ConnectionIter < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-DoIPIntInitialization
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessRetryTimeout()
 *********************************************************************************************************************/
/*! \brief        Processes the reception retry timeout counter on the connections.
 *  \details      The timeout is implemented as counter which is modified in each main function cycle.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ProcessRetryTimeout(void);

/**********************************************************************************************************************
 *  DoIPInt_Rx_Terminate()
 *********************************************************************************************************************/
/*! \brief        Terminates pending reception on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_Terminate(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Rx_StartOfReception()
 *********************************************************************************************************************/
/*! \brief        Indicates start of reception.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: IN:  AvailableDataPtr != NULL_PTR
 *                                                OUT: undefined]
 *  \return       BUFREQ_OK               Reception request was accepted.
 *  \return       BUFREQ_E_NOT_OK         Reception request was not accepted.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_StartOfReception(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Rx_CheckOrStartRetryTimeout()
 *********************************************************************************************************************/
/*! \brief        Checks the value of the retry timeout and starts it in case it is not yet running and the connection
 *                is open.
 *  \details      In case it is detected that the connection is not open the timeout value is not started and E_OK is
 *                returned. The handling of the closed connection is done in a different context.
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       E_OK                    Timeout is still running, has just been started or connection is not open.
 *  \return       E_NOT_OK                Timeout occurred and has now been stopped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Rx_CheckOrStartRetryTimeout(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Rx_ResetRetryTimeout()
 *********************************************************************************************************************/
/*! \brief        Resets the retry timeout of the connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ResetRetryTimeout(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Rx_SetPending()
 *********************************************************************************************************************/
/*! \brief        Sets reception to pending on a connection to enable reception retry in main function context.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_SetPending(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Rx_ResetPending()
 *********************************************************************************************************************/
/*! \brief        Resets pending reception on a connection to disable reception retry in main function context.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ResetPending(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Rx_CopyRxData()
 *********************************************************************************************************************/
/*! \brief        Copies data for reception from provided buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr              Provides the buffer and the number of bytes to be copied.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: IN:  AvailableDataPtr != NULL_PTR
 *                                                OUT: undefined]
 *  \return       BUFREQ_OK               Data has been copied completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_CopyRxData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Rx_ContinueCopyRxData()
 *********************************************************************************************************************/
/*! \brief        Continues copying of received user data.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ContinueCopyRxData(void);

/**********************************************************************************************************************
 *  DoIPInt_Rx_Indication()
 *********************************************************************************************************************/
/*! \brief        Receives indication for reception caused by closed socket connection.
 *  \details      No functional implementation required since on socket connection mode change DoIPInt_Rx_Terminate is
 *                called and indicates the same event.
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_Indication(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Rx_Cancel()
 *********************************************************************************************************************/
/*! \brief        Forwards reception cancellation request to the SoAd.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Rx_Cancel(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_RX_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Rx.h
 *********************************************************************************************************************/
