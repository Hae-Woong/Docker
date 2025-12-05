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
/*!        \file  SoAd_TxSocketManager_Int.h
 *         \unit  TxSocketManager
 *        \brief  Internal header file of the TxSocketManager unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the TxSocketManager unit for module internal usage.
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

#if !defined (SOAD_TXSOCKETMANAGER_INT_H)
# define SOAD_TXSOCKETMANAGER_INT_H

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
 *  SoAd_TxSocketManager_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes socket connection structs related to the Tx socket manager.
 *  \details        -
 *  \param[in]      SoConIdx      Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]      PartitionIdx  Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxSocketManager_Init(
  SoAd_SocketIterType SocketIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxSocketManager_TcpTransmit()
 *********************************************************************************************************************/
/*! \brief       Transmits a PDU over TCP.
 *  \details     -
 *  \param[in]   SoConIdx           Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx       Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   AvailableLength    Available buffer length for transmission.
 *  \param[in]   ForceRetrieve      Flag that indicates if all or only some data should be retrieved from upper layer.
 *  \return      E_OK               Transmit request was accepted.
 *  \return      E_NOT_OK           Transmit request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different PDUs, FALSE for same PDU.
 *  \config      SOAD_TCP
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_TcpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint32 AvailableLength,
  boolean ForceRetrieve);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_UdpTransmit()
 *********************************************************************************************************************/
/*! \brief       Transmits a PDU over UDP.
 *  \details     -
 *  \param[in]   SoConIdx           Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx       Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   TotalLength        Length that should be transmitted.
 *  \return      E_OK               Transmit request was accepted.
 *  \return      E_NOT_OK           Transmit request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different PDUs, FALSE for same PDU.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_UdpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 TotalLength);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_Terminate()
 *********************************************************************************************************************/
/*! \brief       Terminates a transmission.
 *  \details     Active TP sessions as well as UDP/TCP transmissions are terminated.
 *  \param[in]   SoConIdx           Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx       Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different PDUs, FALSE for same PDU.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxSocketManager_Terminate(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CheckAndSetTxActive()
 *********************************************************************************************************************/
/*! \brief       Checks if no transmission is active and if socket is established and sets transmission active.
 *  \details     -
 *  \param[in]   SoConIdx           Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx       Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   SetTxActiveOnSoCon Flag that indicates if Tx active is set on SoCon too.
 *  \return      E_OK               Transmit request was accepted.
 *  \return      E_NOT_OK           Transmit request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different PDUs, FALSE for same PDU.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CheckAndSetTxActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean SetTxActiveOnSoCon);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_ResetTxActive()
 *********************************************************************************************************************/
/*! \brief       Resets transmission active state on socket.
 *  \details     Transmission active state can additionally be reset on socket connection.
 *  \param[in]   SoConIdx             Socket connection index.
 *                                    [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx         Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   ResetOnSoConRequired Flag that indicates if TxActive is reset on SoCon too.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different PDUs, FALSE for same PDU.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TxSocketManager_ResetTxActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ResetOnSoConRequired);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_IsTxActive()
 *********************************************************************************************************************/
/*! \brief        Indicates if a transmission is active on a socket connection.
 *  \details      Transmission of an nPdu can optionally be triggered immediately on call of this function to speed up
 *                the transmission.
 *  \param[in]    SoConIdx             Socket connection index.
 *                                     [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx         Partition index.
 *                                     [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    TransmitImmediately  Flag to indicate if transmission of nPdu shall be triggered immediately.
 *  \return       TRUE                 Transmission is active.
 *  \return       FALSE                No transmission is active.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_TxSocketManager_IsTxActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean TransmitImmediately);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_IsTxActiveOnSocket()
 *********************************************************************************************************************/
/*! \brief        Indicates if a transmission is active on a socket.
 *  \details      -
 *  \param[in]    SockIdx              Socket index.
 *                                     [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx         Partition index.
 *                                     [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       TRUE                 Transmission is active.
 *  \return       FALSE                Transmission is not active.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_TxSocketManager_IsTxActiveOnSocket(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CheckAndAddPduHdrSize()
 *********************************************************************************************************************/
/*! \brief         Checks if PDU header size fits into the data length and adds it.
 *  \details       -
 *  \param[in]     SoConIdx             Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]     PartitionIdx         Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in,out] DataLength           Pointer to data length.
 *  \return        E_OK                 PDU header size fits into the data length.
 *  \return        E_NOT_OK             PDU header size does not fit into the data length.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CheckAndAddPduHdrSize(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) DataLength);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_TXSOCKETMANAGER_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_TxSocketManager_Int.h
 *********************************************************************************************************************/
