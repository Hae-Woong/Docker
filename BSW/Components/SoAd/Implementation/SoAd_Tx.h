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
/*!        \file  SoAd_Tx.h
 *        \brief  Socket Adaptor header file
 *
 *      \details  Vector static code header file for AUTOSAR Socket Adaptor sub-component Tx.
 *
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

#if !defined (SOAD_TX_H)
# define SOAD_TX_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SoAd.h"

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

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_Tx_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes Tx structs for PduRoutes, NPdus and IP fragmentation buffer.
 *  \details      -
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Tx_Init(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_Tx_InitSoCon()
 *********************************************************************************************************************/
/*! \brief        Initializes Tx structs on a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Tx_InitSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_Tx_IfTransmit()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU over all related PduRouteDests.
 *  \details      -
 *  \param[in]    PduRouteIdx         PDU route index.
 *                                    [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr          Pointer to PDU.
 *  \param[out]   ErrorIdPtr          Pointer to error identifier (set if error occurred).
 *  \return       E_OK                SoConId is valid and corresponding PduRouteDest was found.
 *  \return       E_NOT_OK            SoConId is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_IfTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_Tx_IfTransmitPduRouteDest()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU on a socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx   PduRouteDest index.
 *                                  [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr        Pointer to PDU.
 *  \param[out]   ErrorIdPtr        Pointer to error identifier (set if error occurred).
 *  \return       E_OK              Transmit request was accepted.
 *  \return       E_NOT_OK          Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_IfTransmitPduRouteDest(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_Tx_IfReserveTriggerBuf()
 *********************************************************************************************************************/
/*! \brief          Reserves and returns a trigger transmit buffer for a PduRoute.
 *  \details        -
 *  \param[in]      PduRouteIdx       PDU route index.
 *                                    [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]      PartitionIdx      Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in,out]  LengthPtr         Length of trigger transmit buffer to be reserved.
 *                                    [range: in:   LengthPtr != NULL_PTR, *LengthPtr > 0,
 *                                                  *LengthPtr == 0 for unspecific length
 *                                            out:  *LengthPtr > 0 for unspecific length]
 *  \param[out]     TriggerBufPtrPtr  Pointer to pointer to reserved trigger transmit buffer.
 *                                    [range: TriggerBufPtrPtr != NULL_PTR]
 *  \return         E_OK              Trigger transmit buffer reservation succeeded and output parameter are set.
 *  \return         E_NOT_OK          Trigger transmit buffer reservation failed and output parameter are not set.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_IfReserveTriggerBuf(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) TriggerBufPtrPtr);

/**********************************************************************************************************************
 *  SoAd_Tx_IfReleaseTriggerBuf()
 *********************************************************************************************************************/
/*! \brief        Releases a reserved trigger transmit buffer.
 *  \details      -
 *  \param[in]    PduRouteIdx       PDU route index.
 *                                  [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Tx_IfReleaseTriggerBuf(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_Tx_TpTransmit()
 *********************************************************************************************************************/
/*! \brief        Transmits a TP-PDU over the related PduRouteDest.
 *  \details      -
 *  \param[in]    PduRouteIdx         PDU route index.
 *                                    [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr          Pointer to PDU.
 *  \return       E_OK                Transmission request was successful.
 *  \return       E_NOT_OK            Transmission request was not successful.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_TpTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_Tx_TpCancelTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission cancellation of a specific TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx   PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK          Transmit cancellation request was accepted.
 *  \return       E_NOT_OK      Transmit cancellation request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_TpCancelTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_ScheduleTxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Handles TxConfirmation in main function context for IF-API on UDP socket connections.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxIf_ScheduleTxConfirmation(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_Tx_UdpImmediateIfTxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms transmission of data for a UDP socket connection with immediate IF TxConfirmation.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      ISR2
 *  \reentrant    TRUE for different socket connections, FALSE for same socket connection.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_Tx_UdpImmediateIfTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_Tx_IsRouteGrpActive()
 *********************************************************************************************************************/
/*! \brief        Indicates if a routing group transmission is active on a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       TRUE            Transmission is active.
 *  \return       FALSE           No transmission is active.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_Tx_IsRouteGrpActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an IF-PDU on a UDP socket connection.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK     Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxIf_UdpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_TcpCopyTxData()
 *********************************************************************************************************************/
/*! \brief        Copies data to provided transmission buffer of an IF-PDU on a TCP socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    BufPtr          Pointer to buffer of provided transmission buffer.
 *  \param[in]    BufLength       Length of provided transmission buffer.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxIf_TcpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpCopyTxData()
 *********************************************************************************************************************/
/*! \brief        Copies data to provided transmission buffer of a TP-PDU on a TCP socket connection.
 *  \details      -
 *  \param[in]    SoConIdx          Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]    PduRouteDestIdx   PduRouteDest index.
 *                                  [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    BufPtr            Pointer to buffer of provided transmission buffer.
 *  \param[in]    BufLength         Length of provided transmission buffer.
 *  \return       BUFREQ_OK         Copy request accepted.
 *  \return       BUFREQ_E_NOT_OK   Copy request not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxTp_TcpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpCopyTxData()
 *********************************************************************************************************************/
/*! \brief        Copies data to provided transmission buffer of a TP-PDU on a UDP socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    BufPtr          Pointer to buffer of provided transmission buffer.
 *  \param[in]    BufLength       Length of provided transmission buffer.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxTp_UdpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_Terminate()
 *********************************************************************************************************************/
/*! \brief        Terminates active Tx TP session on a socket connection.
 *  \details      -
 *  \param[in]    TxMgtIdx      Management index of the transmission.
 *                              [range: TxMgtIdx < SoAd_GetSizeOfTxMgt()]
 *  \param[in]    PduRouteIdx   PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxTp_Terminate(
  SoAd_SizeOfTxMgtType TxMgtIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTp_ScheduleTransmission()
 *********************************************************************************************************************/
/*! \brief        Handles TP transmission in main function context.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxTp_ScheduleTransmission(
  SoAd_SizeOfInstanceMapType InstMapIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxTcp_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer.
 *  \details        -
 *  \param[in]      SoConIdx          Socket connection index.
 *                                    [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx      Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr            Pointer to buffer of provided transmission buffer.
 *  \param[in]      BufLength         Length of provided transmission buffer.
 *  \return         BUFREQ_OK         Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK   Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         SOAD_TCP
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxTcp_CopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength);

/**********************************************************************************************************************
 *  SoAd_TxTcp_Terminate()
 *********************************************************************************************************************/
/*! \brief        Terminates active Tx TCP session on a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxTcp_Terminate(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_TxConfirmation
 *********************************************************************************************************************/
/*! \brief        Removes elements from queue which have been sent and calls the corresponding notification of user.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTcpTxQueueUsedOfSoCon()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    Len           Length of confirmed transmission data.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_TxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType Len);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxUdp_ScheduleTransmission()
 *********************************************************************************************************************/
/*! \brief        Handles UDP transmission in main function context.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxUdp_ScheduleTransmission(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_TxUdp_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer.
 *  \details        -
 *  \param[in]      SoConIdx          Socket connection index.
 *                                    [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx      Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr            Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr      Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK         Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK   Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxUdp_CopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxUdp_Terminate()
 *********************************************************************************************************************/
/*! \brief        Terminates active Tx UDP session on a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxUdp_Terminate(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_IsTxPending()
 *********************************************************************************************************************/
/*! \brief        Returns if a nPdu transmission is pending.
 *  \details      Transmission of an nPdu can optionally be triggered immediately on call of this function to speed up
 *                the transmission.
 *  \param[in]    SoConIdx             Socket connection index.
 *                                     [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx         Partition index.
 *                                     [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    TransmitImmediately  Flag to indicate if transmission of nPdu shall be triggered immediately.
 *  \return       TRUE                 Transmission is pending.
 *  \return       FALSE                Transmission is not pending.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_TxNPduUdp_IsTxPending(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean TransmitImmediately);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_TX_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Tx.h
 *********************************************************************************************************************/
