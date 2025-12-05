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
/*!        \file  SoAd_Tx.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component Tx.
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

/* PRQA S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */

#define SOAD_TX_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_EventQueue.h"
#include "SoAd_TimeoutList.h"
#include "SoAd_SoCon.h"
#include "SoAd_RouteGrp.h"
#include "SoAd_Util.h"
#include "SoAd_Tx.h"
#include "SoAd_State_Int.h"
#include "SoAd_RemoteAddr_Int.h"
#include "SoAd_TxSocketManager_Int.h"
#include "SoAd_Priv.h"

#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
#include "IpBase.h"
#include "vstdlib.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/*! Length if meta data. */
#define SOAD_TX_META_DATA_LEN                       2u
/*! Number of retries to send an nPdu if nPdu becomes larger (caused by larger PDUs in trigger transmit calls). */
#define SOAD_N_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT    2u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (SOAD_LOCAL) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL static
#endif

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */ /*lint !e451 */

/**********************************************************************************************************************
 *  SoAd_Tx_CopyTxDataPduHeader()
 *********************************************************************************************************************/
/*! \brief          Copies PDU header to provided transmission buffer.
 *  \details        Uses "BufLengthPtr" to decrement length of available provided transmission buffer. Sets "BufPtrPtr"
 *                  to the position after PDU header.
 *  \param[in]      SoConIdx          Socket connection index.
 *                                    [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PduRouteDestIdx   PduRouteDest index.
 *                                    [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]      PartitionIdx      Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in,out]  BufPtrPtr         Pointer to pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr      Pointer to length of provided transmission buffer.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Tx_CopyTxDataPduHeader(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) BufPtrPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckParameter()
 *********************************************************************************************************************/
/*! \brief      Checks transmission parameter for an IF-PDU.
 *  \details    -
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  PduLength       PDU length.
 *  \return     E_OK            Parameters are valid for transmission.
 *  \return     E_NOT_OK        Parameters are not valid for transmission.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckParameter(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxPduLength()
 *********************************************************************************************************************/
/*! \brief        Cheks the PDU length of an IF-PDU requested for transmission.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            PDU length is valid.
 *  \return       E_NOT_OK        PDU length is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxPduLength(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxState()
 *********************************************************************************************************************/
/*! \brief        Checks the transmission state for an IF-PDU if transmission is allowed or not.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK            Transmission is allowed.
 *  \return       E_NOT_OK        Transmission is prohibited.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxState(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Transmits a valid IF-PDU over all related PduRouteDests.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            SoConId is valid and corresponding PduRouteDest was found.
 *  \return       E_NOT_OK        SoConId is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \trace        DSGN-SoAdTransmissionFanOutIfPdu
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPdu(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_GetPduRouteDestIdxByMetaData()
 *********************************************************************************************************************/
/*! \brief        Returns the PduRouteDest of a PduRoute which matches the SoConId in the meta data if configured.
 *  \details      -
 *  \param[in]    PduRouteIdx         PDU route index.
 *                                    [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr          Pointer to PDU.
 *  \param[out]   PduRouteDestIdxPtr  Pointer to index of PduRouteDest.
 *  \return       E_OK                SoConId is valid and corresponding PduRouteDest was found.
 *  \return       E_NOT_OK            SoConId is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \trace        DSGN-SoAdTransmissionOnSpecificSocketConnection, DSGN-SoAdMetaData
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_GetPduRouteDestIdxByMetaData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(SoAd_SizeOfPduRouteDestType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestIdxPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitOverSpecificPduRouteDest()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU over specific PDU route destination.
 *  \details      -
 *  \param[in]    PduRouteDestIdx   PduRouteDest index.
 *                                  [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr        Pointer to PDU.
 *                                  [range: PduInfoPtr != NULL_PTR]
 *  \param[out]   ErrorIdPtr        Pointer to error identifier (set if error occurred).
 *  \return       E_OK              Transmit request was accepted.
 *  \return       E_NOT_OK          Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitOverSpecificPduRouteDest(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_GetPduDataViaTriggerTransmit()
 *********************************************************************************************************************/
/*! \brief        Retrieves PDU data via trigger transmit to a trigger transmit buffer.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   PduInfoPtr      Pointer to PDU.
 *                                [range: PduInfoPtr != NULL_PTR]
 *  \return       E_OK            PDU data retrieved successfully.
 *  \return       E_NOT_OK        PDU data could not be retrieved.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_GetPduDataViaTriggerTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitOverAllPduRouteDest()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU on all related socket connections.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            Transmit request was accepted.
 *  \return       E_NOT_OK        Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitOverAllPduRouteDest(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPduRouteDest()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU on a socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            Transmit request was accepted.
 *  \return       E_NOT_OK        Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPduRouteDest(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_TriggerTransmitCheckParameter()
 *********************************************************************************************************************/
/*! \brief      Checks parameter after call to trigger transmit for an IF-PDU on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx          Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx      Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  BufLength         Length of provided transmission buffer.
 *  \param[in]  PduInfoPtr        Pointer to PDU copied in trigger transmit call.
 *  \param[in]  ExactLength       Indicates if copied PDU shall fit exactly into the provided buffer.
 *  \param[in]  ConsiderPduHeader Indicates if BufLength contains the PDU header.
 *  \return     E_OK              Trigger transmit call returned valid parameter.
 *  \return     E_NOT_OK          Trigger transmit call returned invalid parameter.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TriggerTransmitCheckParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 BufLength,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  boolean ExactLength,
  boolean ConsiderPduHeader);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_TcpTransmit()
 *********************************************************************************************************************/
/*! \brief      Transmits an IF-PDU on a TCP socket connection.
 *  \details    -
 *  \param[in]  PduRouteDestIdx PduRouteDest index.
 *                              [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  PduInfoPtr      Pointer to PDU.
 *  \return     E_OK            Transmit processed successfully.
 *  \return     E_NOT_OK        Transmit failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TcpTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_TcpPrepareTransmit()
 *********************************************************************************************************************/
/*! \brief      Prepares transmission of an IF-PDU on a TCP socket connection.
 *  \details    -
 *  \param[in]  PduRouteDestIdx PduRouteDest index.
 *                              [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  PduInfoPtr      Pointer to PDU.
 *  \return     E_OK            Transmit prepared successfully.
 *  \return     E_NOT_OK        Transmit preparation failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TcpPrepareTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_TcpCallTransmit()
 *********************************************************************************************************************/
/*! \brief      Calls transmission service of lower layer to transmit an IF-PDU on a TCP socket connection.
 *  \details    -
 *  \param[in]  PduRouteDestIdx PduRouteDest index.
 *                              [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK            Transmit processed successfully.
 *  \return     E_NOT_OK        Transmit failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TcpCallTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_TcpFinishTransmit()
 *********************************************************************************************************************/
/*! \brief      Finishes transmission of an IF-PDU on a TCP socket connection.
 *  \details    -
 *  \param[in]  PduRouteDestIdx PduRouteDest index.
 *                              [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_TcpFinishTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmit()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU on a UDP socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitNPdu()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU as an nPdu on a UDP socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest(),
 *                                SoAd_IsNPduUdpTxUsedOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \trace        DSGN-SoAdTransmissionNPdu
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitNPdu(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitNPduQueue()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU as an nPdu stored in a queue on a UDP socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest(),
 *                                SoAd_IsNPduUdpTxUsedOfSoCon(),
 *                                SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitNPduQueue(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitNPduBuffer()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU as an nPdu stored in a buffer on a UDP socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest(),
 *                                SoAd_IsNPduUdpTxUsedOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitNPduBuffer(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU as a single PDU on a UDP socket connection.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitPdu(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitPduFragment()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU as a single PDU on a UDP socket connection using a IP fragmentation buffer.
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest(),
 *                                SoAd_IsIpFragMgtUsedOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \param[out]   ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitPduFragment(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitPduDirect()
 *********************************************************************************************************************/
/*! \brief        Transmits an IF-PDU as a single PDU on a UDP socket connection using data pointer directly (i.e. no
 *                additional buffer is used).
 *  \details      -
 *  \param[in]    PduRouteDestIdx PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \return       E_OK            Transmit processed successfully.
 *  \return       E_NOT_OK        Transmit failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitPduDirect(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxDataTrigger()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an IF-PDU on a UDP socket connection retrieved via
 *                  trigger transmit call.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PduRouteIdx         PDU route index.
 *                                      [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK     Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxIf_UdpCopyTxDataTrigger(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxDataTriggerFinish()
 *********************************************************************************************************************/
/*! \brief          Finishes transmission of an IF-PDU on a UDP socket connection retrieved via trigger transmit.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      SoConIdx        Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx    Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr          Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr    Pointer to length of provided transmission buffer.
 *  \param[in]      PduInfoPtr      Pointer to PDU copied in trigger transmit call.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_UdpCopyTxDataTriggerFinish(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxDataDirect()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an IF-PDU on a UDP socket connection sent directly
 *                  without trigger transmit.
 *  \details        -
 *  \param[in]      SoConIdx        Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx    Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr          Pointer to buffer of provided transmission buffer.
 *  \param[in]      BufLength       Length of provided transmission buffer.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_UdpCopyTxDataDirect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength);

/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConf()
 *********************************************************************************************************************/
/*! \brief      Adds a pending TxConfirmation for IF-API to a specific socket connection.
 *  \details    Handles TxConfirmations generated by SoAd itself and called in context of SoAd_TxConfirmation.
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConf(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConfUdp()
 *********************************************************************************************************************/
/*! \brief      Adds a pending TxConfirmation for IF-API to a specific UDP socket connection.
 *  \details    Handles TxConfirmations generated by SoAd itself and called in context of SoAd_TxConfirmation.
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConfUdp(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConfTcp()
 *********************************************************************************************************************/
/*! \brief      Adds a pending TxConfirmation for IF-API to a specific TCP socket connection.
 *  \details    Handles TxConfirmations generated by SoAd itself and called in context of SoAd_TxConfirmation.
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConfTcp(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxIf_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Calls TxConfirmation for IF-API if pending. This function is used in case TxConfirmation shall be
 *              called in main function context by default.
 *  \details    -
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxIf_TxConfirmation(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckAndClearPendingTxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Checks if a TxConfirmation for IF-API is pending which shall be called in main function context by
 *              default. Afterwards, it clears this pending TxConfirmation and succeeds only if no further
 *              TxConfirmation (e.g. in context of SoAd_TxConfirmation) is missing.
 *  \details    -
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckAndClearPendingTxConfirmation(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIf_CallTxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Calls TxConfirmation for IF-PDU if configured.
 *  \details    -
 *  \param[in]  PduRouteIdx     PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_CallTxConfirmation(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_TcpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms IF transmission on a TCP socket connection.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PduRouteIdx   PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    ByResetSoCon  Indicates if confirmation is requested by resetting socket connection on close.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxIf_TcpTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ByResetSoCon);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckParameter()
 *********************************************************************************************************************/
/*! \brief        Checks transmission parameter for a TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            Parameters are valid for transmission.
 *  \return       E_NOT_OK        Parameters are not valid for transmission.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckParameter(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_TransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Transmits a TP-PDU over the related PduRouteDest.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr      Pointer to PDU.
 *  \return       E_OK            Transmission request was successful.
 *  \return       E_NOT_OK        Transmission request was not successful.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TransmitPdu(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckAndSetProtocolParameter()
 *********************************************************************************************************************/
/*! \brief        Checks and sets protocol specific parameters for a transmission of a TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduLength       PDU length.
 *  \return       E_OK            Parameters are valid and set for transmission.
 *  \return       E_NOT_OK        Parameters are not valid and therefore not set for transmission.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckAndSetProtocolParameter(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_PrepareTransmitPdu()
 *********************************************************************************************************************/
/*! \brief        Prepares socket connection to perform a transmission of a TP-PDU.
 *  \details      -
 *  \param[in]    PduRouteIdx     PDU route index.
 *                                [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduLength       PDU length.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_PrepareTransmitPdu(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength);

/**********************************************************************************************************************
 *  SoAd_TxTp_CallCopyTxData()
 *********************************************************************************************************************/
/*! \brief        Calls CopyTxData function of upper layer.
 *  \details      -
 *  \param[in]    PduRouteIdx       PDU route index.
 *                                  [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    PduInfoPtr        Pointer to PDU.
 *  \param[out]   BufferSizePtr     Pointer to available data length.
 *  \return       BUFREQ_OK         Copy request accepted.
 *  \return       BUFREQ_E_BUSY     Required amount of data is not available.
 *  \return       BUFREQ_E_NOT_OK   Copy request not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxTp_CallCopyTxData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr);

/**********************************************************************************************************************
 *  SoAd_TxTp_CancelTransmit()
 *********************************************************************************************************************/
/*! \brief      Requests PDU transmission cancellation which is handled later in main function context.
 *  \details    -
 *  \param[in]  PduRouteIdx   PDU route index.
 *                            [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK          Cancellation request succeeded.
 *  \return     E_NOT_OK      Cancellation request failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CancelTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTp_TransmitPduRouteDest()
 *********************************************************************************************************************/
/*! \brief      Transmits a TP-PDU on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \return     E_OK            Transmit processed successfully or it will be continued in main function context.
 *  \return     E_NOT_OK        Transmit failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TransmitPduRouteDest(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_HandleCancellation()
 *********************************************************************************************************************/
/*! \brief      Handles TP reception cancellation in main function context.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]  PduRouteDestIdx PduRouteDest index.
 *                              [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_HandleCancellation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmit()
 *********************************************************************************************************************/
/*! \brief      Transmits a TP-PDU on a TCP socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \return     E_OK            Transmit processed successfully.
 *  \return     E_NOT_OK        Transmit failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TcpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpCalculateLength()
 *********************************************************************************************************************/
/*! \brief        Calculates length of available data to be transmitted.
 *  \details      -
 *  \param[in]    SoConIdx            Socket connection index.
 *                                    [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   AvailableBufLenPtr  Pointer to available upper layer TP transmission buffer.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpCalculateLength(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) AvailableBufLenPtr);

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmitOnSocket()
 *********************************************************************************************************************/
/*! \brief          Handles transmission of a TP-PDU on a TCP socket connection.
 *  \details        -
 *  \param[in]      SoConIdx        Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx    Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      AvailableBufLen Available buffer length for transmission.
 *  \param[in]      CalledByMain    Indicates if transmission is in context of main function instead of transmit
 *                                  service.
 *  \return         E_OK            Transmit handled successfully.
 *  \return         E_NOT_OK        Transmit failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config         SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TcpTransmitOnSocket(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType AvailableBufLen,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmitHandleSuccess()
 *********************************************************************************************************************/
/*! \brief      Handles TP session in TCP socket connection if transmission to TcpIp was successful.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpTransmitHandleSuccess(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmitHandleFail()
 *********************************************************************************************************************/
/*! \brief      Handles TP session in TCP socket connection if transmission to TcpIp has failed.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpTransmitHandleFail(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms successful TP transmission on a TCP socket connection.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]    PduRouteIdx   PDU route index.
 *                              [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpTransmit()
 *********************************************************************************************************************/
/*! \brief      Transmits a TP-PDU on a UDP socket connection.
 *  \details    Copies PDU to TP transmission buffer first and triggers transmission when copied completely.
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon(),
 *                              SoAd_IsTpTxBufferConfigUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \return     E_OK            Transmit processed successfully.
 *  \return     E_NOT_OK        Transmit failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_UdpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpCopyPduHdr()
 *********************************************************************************************************************/
/*! \brief      Copies PDU header to TP transmission buffer.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon(),
 *                              SoAd_IsTpTxBufferConfigUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_UdpCopyPduHdr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpCopyPdu()
 *********************************************************************************************************************/
/*! \brief      Copies PDU to TP transmission buffer.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon(),
 *                              SoAd_IsTpTxBufferConfigUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \return     E_OK            PDU copied successfully.
 *  \return     E_NOT_OK        PDU copy failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_UdpCopyPdu(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpTransmitOnSocket()
 *********************************************************************************************************************/
/*! \brief      Transmits data from TP transmission buffer over a UDP socket conenction.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon(),
 *                              SoAd_IsTpTxBufferConfigUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \return     E_OK            Transmisssion succeeded.
 *  \return     E_NOT_OK        Transmission failed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_UdpTransmitOnSocket(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain);

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpTerminateTransmit()
 *********************************************************************************************************************/
/*! \brief      Terminates a TP session for transmission on a UDP socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon(),
 *                              SoAd_IsTpTxBufferConfigUsedOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CalledByMain    Indicates if transmission is in context of main function instead of transmit service.
 *  \param[in]  Result          Indicates result of transmission. Range: E_OK, E_NOT_OK.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxTp_UdpTerminateTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain,
  Std_ReturnType Result);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_Init()
 *********************************************************************************************************************/
/*! \brief      Initializes Tx TCP queue on a specific socket connection if configured.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_Init(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_Clear()
 *********************************************************************************************************************/
/*! \brief      Clears Tx TCP queue on a specific socket connection if configured.
 *  \details    Calls additionally IF TxConfirmation if pending.
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_Clear(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_ReserveNextElement()
 *********************************************************************************************************************/
/*! \brief      Reserves next free Tx TCP queue entry to store a transmission.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTcpTxQueueUsedOfSoCon()]
 *  \param[in]  PduRouteIdx   PDU route index.
 *                            [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK          Element reserved.
 *  \return     E_NOT_OK      No elements left or an element is already reserved.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTcpQueue_ReserveNextElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_GetNextElement()
 *********************************************************************************************************************/
/*! \brief      Gets next free Tx TCP queue entry to store a transmission if not already reserved.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out] WriteIdx      Pointer to next write index.
 *  \return     E_OK          Next element available.
 *  \return     E_NOT_OK      Last element is already reserved.
 *  \pre        Has to be called in critical section.
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTcpQueue_GetNextElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SizeOfTcpTxQueueDataType, AUTOMATIC, SOAD_APPL_DATA) WriteIdx);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_SetElement()
 *********************************************************************************************************************/
/*! \brief      Sets a previously reserved element in Tx TCP queue.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTcpTxQueueUsedOfSoCon()]
 *  \param[in]  PduRouteIdx   PDU route index.
 *                            [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Len           Length of transmission data.
 *  \param[in]  Release       Release or set the reserved element.
 *  \return     E_OK          Reserved element released or set.
 *  \return     E_NOT_OK      No reserved element found and element is neither released nor set.
 *  \pre        An element has to be reserved with SoAd_TxTcpQueue_ReserveNextElement().
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTcpQueue_SetElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType Len,
  boolean Release);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_GetTotalLength()
 *********************************************************************************************************************/
/*! \brief      Returns the total length of all set queue elements.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     TotalLen      Total length of all set queue elements.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(uint32, SOAD_CODE) SoAd_TxTcpQueue_GetTotalLength(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_CallTxConfirmation
 *********************************************************************************************************************/
/*! \brief        Calls the transmission confirmation notification of user.
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]    PduRouteIdx   PDU route index.
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_CallTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_HandleTransmission()
 *********************************************************************************************************************/
/*! \brief      Handles nPdu transmission in main function context.
 *  \details    -
 *  \param[in]  InstMapIdx   Instance Map index.
 *                           [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_HandleTransmission(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_Transmit()
 *********************************************************************************************************************/
/*! \brief      Transmits an nPdu.
 *  \details    -
 *  \param[in]  NPduIdx           NPdu index.
 *                                [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PartitionIdx      Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \note       This function must be called inside a critical section to ensure that transmission requests are not
 *              rejected when a transmission in main function context is interrupted by a transmission request from a
 *              different context.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_Transmit(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_SetTriggerTimeout()
 *********************************************************************************************************************/
/*! \brief      Sets timeout for nPdu if configured.
 *  \details    -
 *  \param[in]  NPduIdx           NPdu index.
 *                                [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PduRouteDestIdx   PduRouteDest index.
 *                                [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx      Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK              Timeout set successfully.
 *  \return     E_NOT_OK          Timeout list cannot store timeout.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_SetTriggerTimeout(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_Clear()
 *********************************************************************************************************************/
/*! \brief      Clears an nPdu and calls the corresponding TxConfirmations if required.
 *  \details    -
 *  \param[in]  NPduIdx           NPdu index.
 *                                [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PartitionIdx      Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_Clear(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueAddOrUpdateElement()
 *********************************************************************************************************************/
/*! \brief      Adds or updates a PDU as element to an nPdu queue.
 *  \details    -
 *  \param[in]  PduRouteDestIdx         PduRouteDest index.
 *                                      [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest(),
 *                                      SoAd_IsNPduUdpTxUsedOfSoCon(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  PduInfoPtr              Pointer to PDU.
 *  \return     SOAD_NPDU_OK            Added or updated element to nPdu successfully.
 *  \return     SOAD_NPDU_OVFL_NEW      New PDU exceeds nPdu size or queue size.
 *  \return     SOAD_NPDU_OVFL_UPDATE   Update of PDU in nPdu exceeds nPdu size.
 *  \return     SOAD_NPDU_NOT_OK        PDU could not be added/updated.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueAddOrUpdateElement(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueUpdateElement()
 *********************************************************************************************************************/
/*! \brief      Updates a PDU as element to an nPdu queue.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PduRouteDestIdx         PduRouteDest index.
 *                                      [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  DataLen                 Length of PDU considering PDU header.
 *  \param[out] ElementUpdated          Indicates if element is updated.
 *  \return     SOAD_NPDU_OK            Updated element in nPdu successfully or no element for PDU exists.
 *  \return     SOAD_NPDU_OVFL_UPDATE   Update of PDU in nPdu exceeds nPdu size.
 *  \return     SOAD_NPDU_NOT_OK        Update of PDU not possible since transmission is pending.
 *  \pre        Has to be called in critical section.
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueUpdateElement(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType DataLen,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_VAR) ElementUpdated);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueAddElement()
 *********************************************************************************************************************/
/*! \brief      Adds a PDU as element to an nPdu queue.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PduRouteDestIdx         PduRouteDest index.
 *                                      [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  DataLen                 Length of PDU considering PDU header.
 *  \param[in]  PduInfoPtr              Pointer to PDU.
 *  \return     SOAD_NPDU_OK            Added element to nPdu successfully.
 *  \return     SOAD_NPDU_OVFL_NEW      New PDU exceeds nPdu size or queue size.
 *  \return     SOAD_NPDU_NOT_OK        Cannot create nPdu since timeout list size is not sufficient to store nPdu.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueAddElement(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType DataLen,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCheckSize()
 *********************************************************************************************************************/
/*! \brief      Checks if a new PDU exceeds the nPdu size or the queue size.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  DataLen                 Length of PDU considering PDU header.
 *  \return     SOAD_NPDU_OK            Added element to nPdu successfully.
 *  \return     SOAD_NPDU_OVFL_NEW      New PDU exceeds nPdu size or queue size.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCheckSize(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType DataLen);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTransmit()
 *********************************************************************************************************************/
/*! \brief      Transmits an nPdu stored in a queue.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueTransmit(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueuePrepareTransmitSegment()
 *********************************************************************************************************************/
/*! \brief      Prepares a transmission of an nPdu segment stored in a queue.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK                    Transmission prepared successfully.
 *  \return     E_NOT_OK                Transmission not possible and nPdu is flushed if no retry is left.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueuePrepareTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueuePrepareTransmitSegmentFragment()
 *********************************************************************************************************************/
/*! \brief      Prepares a transmission of an nPdu segment stored in a queue using an IP fragmentation buffer.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx(),
 *                                      SoAd_IsIpFragMgtUsedOfSoCon()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK                    Transmission prepared successfully.
 *  \return     E_NOT_OK                Transmission not possible.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueuePrepareTransmitSegmentFragment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueuePrepareTransmitSegmentDirect()
 *********************************************************************************************************************/
/*! \brief      Prepares a transmission of an nPdu segment stored in a queue which is sent directly (i.e. no additional
 *              buffer is used).
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueuePrepareTransmitSegmentDirect(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCheckAndSetTransmitState()
 *********************************************************************************************************************/
/*! \brief      Checks if a transmission is possible and set transmission to active if so.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK                    Transmission possible and set to active.
 *  \return     E_NOT_OK                Transmission not possible and nPdu is flushed if no retry is left.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCheckAndSetTransmitState(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCheckRetry()
 *********************************************************************************************************************/
/*! \brief      Checks if a retry is left so that transmission of nPdu segment is allowed.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK                    Transmission allowed.
 *  \return     E_NOT_OK                Transmission not allowed.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCheckRetry(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow()
 *********************************************************************************************************************/
/*! \brief      Sums up all PDUs in queue until nPdu overflow occurs.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     NPduLengthUpToOverflow  Length of all PDUs in nPdu until nPdu overflow occurs.
 *                                      [range: NPduLengthUpToOverflow > 0,
 *                                      NPduLengthUpToOverflow < SoAd_GetNPduUdpTxBufferMinOfSocketUdp()]
 *  \pre        Must be called in case of overflow only. This guarantees that at least two PDUs are in queue since one
 *              PDU cannot exceed the nPdu size.
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(uint16, SOAD_CODE) SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTransmitSegment()
 *********************************************************************************************************************/
/*! \brief      Requests to transmit an nPdu segment stored in a queue and checks if the transmission succeeded to
 *              enable retry of transmission in context of main function.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK                    Transmission succeeded.
 *  \return     E_NOT_OK                Transmission failed.
 *  \pre        Transmission has to be prepared via SoAd_TxNPduUdp_QueuePrepareTransmitSegment().
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTryTransmitSegment()
 *********************************************************************************************************************/
/*! \brief      Tries to transmit an nPdu segment stored in a queue.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx(),
 *                                      SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out] RetryTransmit           Indicates if the transmission shall be retried.
 *                                      [range: TRUE, FALSE]
 *  \return     E_OK                    Transmission allowed.
 *  \return     E_NOT_OK                Transmission not allowed.
 *  \pre        RetryTransmit must be initialized to FALSE by the calling context since it is only set to TRUE inside
 *              this API when retry of transmission is required.
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueTryTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SOAD_P2VAR(boolean) RetryTransmit);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueFinishTransmitSegment()
 *********************************************************************************************************************/
/*! \brief      Finishes nPdu segment transmission by calling TxConfirmation for all transmitted PDU elements in queue.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueFinishTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTxConfirmation()
 *********************************************************************************************************************/
/*! \brief      Handles TxConfirmation for a PDU located in an nPdu queue.
 *  \details    -
 *  \param[in]  SoConIdx                Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PduRouteIdx             PDU route index.
 *                                      [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  AllowReconnect          Indicates if socket connection reconnection is allowed if required.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean AllowReconnect);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_BufferAddElement()
 *********************************************************************************************************************/
/*! \brief      Adds a PDU as element to an nPdu buffer.
 *  \details    -
 *  \param[in]  PduRouteIdx             PDU route index.
 *                                      [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]  PduRouteDestIdx         PduRouteDest index.
 *                                      [range: PduRouteDestIdx < SoAd_GetSizeOfPduRouteDest(),
 *                                      SoAd_IsNPduUdpTxUsedOfSoCon()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  PduInfoPtr              Pointer to PDU.
 *  \return     SOAD_NPDU_OK            Added element to nPdu successfully.
 *  \return     SOAD_NPDU_OVFL_NEW      New PDU exceeds nPdu size.
 *  \return     SOAD_NPDU_NOT_OK        Cannot create nPdu since timeout list size is not sufficient to store nPdu.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_BufferAddElement(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_BufferSend()
 *********************************************************************************************************************/
/*! \brief      Sends an nPdu stored in a buffer.
 *  \details    -
 *  \param[in]  NPduIdx                 NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PartitionIdx            Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_BufferSend(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an nPdu in context of SoAd_CopyTxData.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon(),
 *                                      SoAd_IsNPduUdpTxUsedOfSoCon()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK     Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_CopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an nPdu queue in context of SoAd_CopyTxData.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      NPduIdx             NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK     Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different nPdu indexes, FALSE for same nPdu index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyTxData(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_BufferCopyTxData()
 *********************************************************************************************************************/
/*! \brief      Copies data to provided transmission buffer of an nPdu buffer in context of SoAd_CopyTxData.
 *  \details    -
 *  \param[in]  NPduIdx         NPdu index.
 *                              [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  BufPtr          Pointer to buffer of provided transmission buffer.
 *  \param[in]  BufLength       Length of provided transmission buffer.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different nPdu indexes, FALSE for same nPdu index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_BufferCopyTxData(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBuffer()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an nPdu queue.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      NPduIdx             NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK     Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different nPdu indexes, FALSE for same nPdu index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBuffer(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferFromBuffer()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an nPdu queue element stored in nPdu buffer.
 *  \details        Uses "BufLengthPtr" to decrement length of available provided transmission buffer.
 *  \param[in]      NPduQueueIdx        NPdu queue index.
 *                                      [range: NPduQueueIdx < SoAd_GetSizeOfNPduUdpTxQueue()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_OVFL       Provided buffer is not sufficient.
 *  \return         BUFREQ_E_NOT_OK     Copy request rejected (length does not fit and dynamic length is disabled).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different nPdu indexes, FALSE for same nPdu index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferFromBuffer(
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTrigger()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer of an nPdu queue element to be retrieved via trigger
 *                  transmit.
 *  \details        Uses "BufLengthPtr" to decrement length of available provided transmission buffer.
 *  \param[in]      NPduIdx             NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]      NPduQueueIdx        NPdu queue index.
 *                                      [range: NPduQueueIdx < SoAd_GetSizeOfNPduUdpTxQueue()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK           Copy request accepted.
 *  \return         BUFREQ_E_OVFL       Copy request not accepted (provided buffer is not sufficient).
 *  \return         BUFREQ_E_NOT_OK     Copy request not accepted (length does not fit and dynamic length is disabled).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different nPdu indexes, FALSE for same nPdu index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTrigger(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerPrepare()
 *********************************************************************************************************************/
/*! \brief          Prepares to copy data to provided transmission buffer of an nPdu queue element to be retrieved via
 *                  trigger transmit.
 *  \details        -
 *  \param[in]      NPduIdx             NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in]      BufLength           Length of provided transmission buffer.
 *  \param[in,out]  PduInfoPtr          Pointer to provided transmission buffer used in trigger transmit call.
 *  \return         BUFREQ_OK           Preparation succeeded.
 *  \return         BUFREQ_E_OVFL       Preparation failed because provided buffer size is not sufficient.
 *  \return         BUFREQ_E_NOT_OK     Preparation failed (length does not fit and dynamic length is disabled).
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerPrepare(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerCall()
 *********************************************************************************************************************/
/*! \brief          Calls trigger transmit to copy data to provided transmission buffer of an nPdu queue element to be
 *                  retrieved via trigger transmit.
 *  \details        Uses "BufLengthPtr" to decrement length of available provided transmission buffer.
 *  \param[in]      NPduIdx             NPdu index.
 *                                      [range: NPduIdx < SoAd_GetSizeOfNPduUdpTx()]
 *  \param[in]      NPduQueueIdx        NPdu queue index.
 *                                      [range: NPduQueueIdx < SoAd_GetSizeOfNPduUdpTxQueue()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *  \param[in,out]  PduInfoPtr          Pointer to provided transmission buffer used in trigger transmit call.
 *  \return         BUFREQ_OK           Trigger transmit call succeeded.
 *  \return         BUFREQ_E_OVFL       Trigger transmit call failed.
 *  \return         BUFREQ_E_NOT_OK     Trigger transmit call returned invalid parameter.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerCall(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerAddPduHeader()
 *********************************************************************************************************************/
/*! \brief          Adds PDU header to provided transmission buffer of an nPdu queue element to be retrieved via
 *                  trigger transmit.
 *  \details        Uses "BufLengthPtr" to decrement length of available provided transmission buffer.
 *  \param[in]      NPduQueueIdx        NPdu queue index.
 *                                      [range: NPduQueueIdx < SoAd_GetSizeOfNPduUdpTxQueue()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr              Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr        Pointer to length of provided transmission buffer.
 *                                      [range: *BufLengthPtr >= SOAD_PDU_HDR_SIZE]
 *  \param[in]      PduInfoPtr          Pointer to PDU copied in trigger transmit call.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerAddPduHeader(
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr);

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes all IP fragmentation buffers.
 *  \details        -
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIpFragBuf_Init(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Reserve()
 *********************************************************************************************************************/
/*! \brief          Reserves and returns an IP fragmentation buffer for a socket connection.
 *  \details        -
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsIpFragMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      Length              Length of IP fragmentation buffer to be reserved.
 *                                      [range: Length > 0]
 *  \param[out]     IpFragBufPtrPtr     Pointer to pointer to reserved IP fragmentation buffer.
 *                                      [range: IpFragBufPtrPtr != NULL_PTR]
 *  \return         E_OK                IP fragmentation buffer reservation succeeded and IpFragBufPtrPtr is set.
 *  \return         E_NOT_OK            IP fragmentation buffer reservation failed and IpFragBufPtrPtr is not set.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIpFragBuf_Reserve(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint32 Length,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) IpFragBufPtrPtr);

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Set()
 *********************************************************************************************************************/
/*! \brief          Sets the copied length of an IP fragmentation buffer (i.e. indicates buffer usage).
 *  \details        -
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsIpFragMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      Length              Length of copied data.
 *                                      [range: Length > 0, Length <= Length in SoAd_TxIpFragBuf_Reserve()]
 *  \pre            SoAd_TxIpFragBuf_Reserve() has been called successfully previously.
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIpFragBuf_Set(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 Length);

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Get()
 *********************************************************************************************************************/
/*! \brief          Returns the reserved IP fragmentation buffer and the length.
 *  \details        In case SoAd_TxIpFragBuf_Set() was called the set length is returned. Otherwise, the length set in
 *                  SoAd_TxIpFragBuf_Reserve() is returned.
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsIpFragMgtUsedOfSoCon()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]     LengthPtr           Pointer to length of reserved IP fragmentation buffer.
 *                                      [range: LengthPtr != NULL_PTR]
 *  \param[out]     IpFragBufPtrPtr     Pointer to pointer to reserved IP fragmentation buffer.
 *                                      [range: IpFragBufPtrPtr != NULL_PTR]
 *  \return         E_OK                IP fragmentation buffer has been reserved and is returned successfully. The
 *                                      output parameter are valid.
 *  \return         E_NOT_OK            IP fragmentation buffer has not been reserved previously and the output
 *                                      parameter are not set.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIpFragBuf_Get(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) IpFragBufPtrPtr);

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Release()
 *********************************************************************************************************************/
/*! \brief          Releases a reserved IP fragmentation buffer.
 *  \details        -
 *  \param[in]      SoConIdx            Socket connection index.
 *                                      [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]      PartitionIdx        Partition index.
 *                                      [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIpFragBuf_Release(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIfTriggerBuf_Init()
 *********************************************************************************************************************/
/*! \brief          Initializes all trigger transmit buffers.
 *  \details        -
 *  \param[in]      PartitionIdx  Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIfTriggerBuf_Init(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_TxIfTriggerBuf_Reserve()
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
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIfTriggerBuf_Reserve(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) TriggerBufPtrPtr);

/**********************************************************************************************************************
 *  SoAd_TxIfTriggerBuf_Release()
 *********************************************************************************************************************/
/*! \brief          Releases a reserved trigger transmit buffer.
 *  \details        -
 *  \param[in]      PduRouteIdx       PDU route index.
 *                                    [range: PduRouteIdx < SoAd_GetSizeOfPduRoute()]
 *  \param[in]      PartitionIdx      Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIfTriggerBuf_Release(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_Tx_CopyTxDataPduHeader()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_Tx_CopyTxDataPduHeader(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) BufPtrPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType      txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  PduLengthType             bytesCopied;
  PduLengthType             bytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate data length which is already copied to lower layer. */
  bytesCopied = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SoAd_GetTxBytesPendingOfTxMgt(
    txMgtIdx, PartitionIdx));

  /* #20 Check if PDU header is not yet copied. */
  if ( bytesCopied < SOAD_PDU_HDR_SIZE )
  {
    /* #30 Calculate PDU header snippet length which can be copied in this context. */
    bytesToCopy = SOAD_PDU_HDR_SIZE - bytesCopied;
    if ( bytesToCopy > *BufLengthPtr )
    {
      bytesToCopy = (PduLengthType)*BufLengthPtr;
    }

    /* #40 Copy PDU header snippet of PduRouteDest to provided buffer. */
    /* PRQA S 4391 2 */ /* MD_SoAd_GenericWiderTypeCast */
    SoAd_Util_CopyPduHdr2Buf(SoAd_GetTxPduHdrIdOfPduRouteDest(PduRouteDestIdx, PartitionIdx),                          /* SBSW_SOAD_POINTER_FORWARD */
      (SoAd_PduHdrLenType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SOAD_PDU_HDR_SIZE),
      *BufPtrPtr,
      bytesCopied,
      bytesToCopy);

    /* #50 Decrement pending data of PDU. */
    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) - bytesToCopy,       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      PartitionIdx);

    /* #60 Decrement provided buffer length. */
    *BufLengthPtr -= (uint16)bytesToCopy;                                                                              /* SBSW_SOAD_POINTER_WRITE */

    /* #70 Update buffer pointer to position after PDU header. */
    *BufPtrPtr = &(*BufPtrPtr)[bytesToCopy];                                                                           /* SBSW_SOAD_POINTER_WRITE */
  }
} /* SoAd_Tx_CopyTxDataPduHeader() */

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckParameter(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check module state. */
  if ( SoAd_State_GetState() == SOAD_STATE_INIT )
  {
    /* #20 Check PDU length. */
    if ( SoAd_TxIf_CheckTxPduLength(PduRouteIdx, PartitionIdx, PduLength) == E_OK )
    {
      /* #30 Check transmission state. */
      if ( SoAd_TxIf_CheckTxState(PduRouteIdx, PartitionIdx) == E_OK )
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* SoAd_TxIf_CheckParameter() */

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxPduLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 3206 2 */ /* MD_SoAd_UnusedParameter */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxPduLength(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8           minLength;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate minimum length in case PDU Header option is used. */
  if ( SoAd_IsAllPduRouteDestWithPduHdrOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    if ( SoAd_IsMetaDataTxEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
    {
      minLength = 2u;
    }
    else
    {
      minLength = 0u;
    }
  }
  /* #11 Calculate minimum length in case PDU Header option is not used otherwise. */
  else
  {
    if ( SoAd_IsMetaDataTxEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
    {
      minLength = 3u;
    }
    else
    {
      minLength = 1u;
    }
  }

  /* #20 Validate minimum length. */
  if ( PduLength >= minLength )
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_TxIf_CheckTxPduLength() */

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckTxState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckTxState(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  Std_ReturnType              retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if a TxConfirmation is pending on the requested PDU */
  if ( !SoAd_IsNPduUdpTxUsedOfSoCon(soConIdx, PartitionIdx) ||
    (SoAd_GetIfTriggerTransmitModeOfPduRoute(PduRouteIdx, PartitionIdx) != SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI) )
  {
    if ( (SoAd_GetPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx) != 0u) ||
      (SoAd_IsPendingTxConfMainOfPduRouteDyn(PduRouteIdx, PartitionIdx)) )
    {
      retVal = E_NOT_OK;

      /* switch off TxConfirmation if faster transmission is required */
    }
  }

  return retVal;
} /* SoAd_TxIf_CheckTxState() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPdu()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPdu(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType                 pduInfo;
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetSizeOfPduRouteDest(PartitionIdx);
  Std_ReturnType              retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Make sure that data pointer is set to NULL_PTR in case of trigger transmit. */
  if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(PduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_NONE )
  {
    pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
  }
  else
  {
    pduInfo.SduDataPtr = NULL_PTR;
  }
  pduInfo.SduLength = PduInfoPtr->SduLength;

  /* #20 Check if meta data are used to send over a specific PduRouteDest. */
  if ( SoAd_IsMetaDataTxEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    /* #200 Get PduRouteDest by extracting SoConId from meta data. */
    if ( SoAd_TxIf_GetPduRouteDestIdxByMetaData(PduRouteIdx, PartitionIdx, PduInfoPtr, &pduRouteDestIdx) == E_OK )     /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
    {
      /* #2000 Remove meta data from PDU payload. */
      pduInfo.SduLength -= SOAD_TX_META_DATA_LEN;

      /* #2001 Transmit the PDU over specific PduRouteDest. */
      retVal = SoAd_TxIf_TransmitOverSpecificPduRouteDest(pduRouteDestIdx, PartitionIdx, &pduInfo, ErrorIdPtr);        /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  /* #21 Send over all PduRouteDests otherwise. */
  else
  {
    /* #210 Check if SoAd is forced to call trigger transmit only once. */
    if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(PduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_SINGLE )
    {
      /* #2100 Check if PDU data can be retrieved via trigger transmit. */
      if ( SoAd_TxIf_GetPduDataViaTriggerTransmit(PduRouteIdx, PartitionIdx, &pduInfo) == E_OK )                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        /* #21000 Transmit the PDU over all related PduRouteDests. */
        retVal = SoAd_TxIf_TransmitOverAllPduRouteDest(PduRouteIdx, PartitionIdx, &pduInfo, ErrorIdPtr);               /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */

        /* #21001 Release the retrieved PDU data again. */
        SoAd_TxIfTriggerBuf_Release(PduRouteIdx, PartitionIdx);
      }
      /* #2101 Reject transmission otherwise. */
      else
      {
        retVal = E_NOT_OK;
      }
    }
    /* #211 Transmit the PDU over all related PduRouteDests otherwise. */
    else
    {
      retVal = SoAd_TxIf_TransmitOverAllPduRouteDest(PduRouteIdx, PartitionIdx, &pduInfo, ErrorIdPtr);                 /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
    }
  }

  /* #30 Call TxConfirmation in this context if transmission optimization is enabled. */
  if ( SoAd_IsTxOptimizedOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    SoAd_TxIf_TxConfirmation(PduRouteIdx, PartitionIdx);
  }

  return retVal;
} /* SoAd_TxIf_TransmitPdu() */

/**********************************************************************************************************************
 *  SoAd_TxIf_GetPduRouteDestIdxByMetaData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_GetPduRouteDestIdxByMetaData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(SoAd_SizeOfPduRouteDestType, AUTOMATIC, SOAD_APPL_DATA) PduRouteDestIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PduRouteDestIterType pduRouteDestIter;
  SoAd_SoConIdType          soConId;
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Extract SoConId from meta data depending of TriggerTransmitMode. */
  /* PRQA S 4391 1 */ /* MD_SoAd_GenericWiderTypeCast */
  if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(PduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_NONE )
  {
    IPBASE_GET_UINT16(PduInfoPtr->SduDataPtr,                                                                          /* SBSW_SOAD_POINTER_FORWARD */
      (uint32_least)((uint32_least)PduInfoPtr->SduLength - SOAD_TX_META_DATA_LEN), soConId);
  }
  else
  {
    IPBASE_GET_UINT16(PduInfoPtr->SduDataPtr, 0u, soConId);                                                            /* SBSW_SOAD_POINTER_FORWARD */
  }

  /* #11 Check if extracted SoConId is valid. */
  if ( (soConId < SoAd_GetSizeOfSoConMap())
#if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    && !SoAd_IsInvalidHndOfSoConMap(soConId)
#endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    )
  {
    /* #110 Iterate over all PduRouteDests to find the PduRouteDest with extracted SoConId. */
    for ( pduRouteDestIter = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
      pduRouteDestIter < SoAd_GetPduRouteDestEndIdxOfPduRoute(PduRouteIdx, PartitionIdx);
      pduRouteDestIter++ )
    {
      if ( (SoAd_SoConIdType)soConId == SoAd_GetSoConIdOfSoCon(SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIter,
            PartitionIdx), PartitionIdx) )
      {
        *PduRouteDestIdxPtr = (SoAd_SizeOfPduRouteDestType)pduRouteDestIter;                                           /* SBSW_SOAD_POINTER_WRITE */
        retVal = E_OK;
        break;
      }
    }
  }

  return retVal;
} /* SoAd_TxIf_GetPduRouteDestIdxByMetaData() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitOverSpecificPduRouteDest()
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
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitOverSpecificPduRouteDest(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  Std_ReturnType          retVal;

  /* #10 Check if any routing group is enabled on the PduRouteDest. */
  if ( SoAd_RouteGrp_CheckAnyRoutGrpOnPduRouteDestEnabled(PduRouteDestIdx, PartitionIdx) == TRUE )
  {
    /* #100 Check if SoAd is forced to call trigger transmit only once. */
    if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_SINGLE )
    {
      /* #1000 Check if PDU data can be retrieved via trigger transmit. */
      if ( SoAd_TxIf_GetPduDataViaTriggerTransmit(pduRouteIdx, PartitionIdx, PduInfoPtr) == E_OK )                     /* SBSW_SOAD_POINTER_FORWARD */
      {
        /* #10000 Transmit the PDU over PduRouteDest. */
        retVal = SoAd_TxIf_TransmitPduRouteDest(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                /* SBSW_SOAD_POINTER_FORWARD */

        /* #10001 Release the retrieved PDU data again. */
        SoAd_TxIfTriggerBuf_Release(pduRouteIdx, PartitionIdx);
      }
      /* #1001 Reject transmission otherwise. */
      else
      {
        retVal = E_NOT_OK;
      }
    }
    /* #101 Transmit the PDU over PduRouteDest otherwise. */
    else
    {
      retVal = SoAd_TxIf_TransmitPduRouteDest(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                  /* SBSW_SOAD_POINTER_FORWARD */
    }
  }
  /* #11 Reject transmission request otherwise. */
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* SoAd_TxIf_TransmitOverSpecificPduRouteDest() */

/**********************************************************************************************************************
 *  SoAd_TxIf_GetPduDataViaTriggerTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_GetPduDataViaTriggerTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType     pduInfo;
  PduLengthType   length;
  Std_ReturnType  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
  pduInfo.SduLength = PduInfoPtr->SduLength;

  /* #10 Try to reserve a trigger transmit buffer. */
  if ( SoAd_TxIfTriggerBuf_Reserve(PduRouteIdx, PartitionIdx, &pduInfo.SduLength, &pduInfo.SduDataPtr) == E_OK )       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    length = pduInfo.SduLength;

    /* #20 Call trigger transmit to copy PDU to trigger transmit buffer. */
    if ( SoAd_GetIfTriggerTransmitCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(         /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), &pduInfo) == E_OK )
    {
      /* #30 Check if returned parameters are valid. */
      if ( (pduInfo.SduDataPtr != NULL_PTR) && (pduInfo.SduLength != 0u) && (pduInfo.SduLength <= length) )
      {
        /* PRQA S 2916 1 */ /* MD_SoAd_CopyingToPointerWithGreaterLifetime */
        PduInfoPtr->SduDataPtr = pduInfo.SduDataPtr;                                                                   /* SBSW_SOAD_POINTER_WRITE */
        PduInfoPtr->SduLength = pduInfo.SduLength;                                                                     /* SBSW_SOAD_POINTER_WRITE */
        retVal = E_OK;
      }
    }

    /* #40 Release trigger transmit buffer if trigger transmit call failed or returned invalid parameters. */
    if ( retVal != E_OK )
    {
      SoAd_TxIfTriggerBuf_Release(PduRouteIdx, PartitionIdx);
    }
  }

  return retVal;
} /* SoAd_TxIf_GetPduDataViaTriggerTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitOverAllPduRouteDest()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitOverAllPduRouteDest(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PduRouteDestIterType   pduRouteDestIter;
  SoAd_SizeOfPduRouteDestType pduRouteDestsTransmitted = 0u;
  SoAd_SizeOfPduRouteDestType pduRouteDestsRejected = 0u;
  Std_ReturnType              retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all PduRouteDests. */
  for ( pduRouteDestIter = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
    pduRouteDestIter < SoAd_GetPduRouteDestEndIdxOfPduRoute(PduRouteIdx, PartitionIdx);
    pduRouteDestIter++ )
  {
    /* #20 Check if at least one routing group is enabled on the PduRouteDest. */
    if ( SoAd_RouteGrp_CheckAnyRoutGrpOnPduRouteDestEnabled((SoAd_SizeOfPduRouteDestType)pduRouteDestIter,
        PartitionIdx) == TRUE )
    {
      /* #30 Try to transmit the PDU over the PduRouteDest. */
      if ( SoAd_TxIf_TransmitPduRouteDest((SoAd_SizeOfPduRouteDestType)pduRouteDestIter, PartitionIdx, PduInfoPtr,     /* SBSW_SOAD_POINTER_FORWARD */
          ErrorIdPtr) == E_OK )
      {
        pduRouteDestsTransmitted++;
      }
      else
      {
        pduRouteDestsRejected++;
      }
    }
  }

  /* #40 Assume the transmission as successful if at least one transmission succeeded while none failed or optimistic
   *     transmission is enabled. */
  if ( (pduRouteDestsTransmitted > 0u) &&
    ((pduRouteDestsRejected == 0u) || SoAd_IsIfOptimisticTransmitOfPduRoute(PduRouteIdx, PartitionIdx)) )
  {
    retVal = E_OK;
  }
  /* #41 Assume the transmission as failed due to disabled routing groups or failed transmission(s) otherwise. */
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* SoAd_TxIf_TransmitOverAllPduRouteDest() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TransmitPduRouteDest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TransmitPduRouteDest(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType  soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the connection states and that the remote address is not forced to be released. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  if ( (SoAd_SoCon_CheckConnectionStates(soConIdx, PartitionIdx) == E_OK) &&
    (SoAd_RemoteAddr_IsForceRelease(soConIdx, PartitionIdx) == FALSE) )
  {
#if ( SOAD_TCP == STD_ON )
    /* #20 Handle transmission for TCP socket connection. */
    if ( SoAd_IsSocketTcpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx), PartitionIdx) )
    {
      retVal = SoAd_TxIf_TcpTransmit(PduRouteDestIdx, PartitionIdx, PduInfoPtr);                                       /* SBSW_SOAD_POINTER_FORWARD */
    }
    else
#endif /* SOAD_TCP == STD_ON */
    /* #21 Handle transmission for UDP socket connection otherwise. */
    {
      retVal = SoAd_TxIf_UdpTransmit(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                           /* SBSW_SOAD_POINTER_FORWARD */
    }
  }

  return retVal;
} /* SoAd_TxIf_TransmitPduRouteDest() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TriggerTransmitCheckParameter()
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
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TriggerTransmitCheckParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 BufLength,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  boolean ExactLength,
  boolean ConsiderPduHeader)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16          maxLen;
  uint16          minLen;
  Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate maximum and minimum expected length if PDU header is used. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #100 Calculate maximum expected length if PDU header shall be considered in provided buffer. */
    if ( ConsiderPduHeader == TRUE )
    {
      maxLen = (uint16)(BufLength - SOAD_PDU_HDR_SIZE);
    }
    /* #101 Calculate maximum expected length if PDU header shall not be considered in provided buffer otherwise. */
    else
    {
      maxLen = (uint16)(BufLength);
    }
    minLen = 0u;
  }
  /* #11 Calculate maximum and minimum expected length without considering PDU header otherwise. */
  else
  {
    maxLen = BufLength;
    minLen = 1u;
  }

  /* #20 Check if buffer pointer is invalid. */
  if ( PduInfoPtr->SduDataPtr == NULL_PTR )
  {
    retVal = E_NOT_OK;
  }
  /* #21 Check if buffer length is invalid in case upper layer shall copy the provided buffer size otherwise. */
  else if ( (ExactLength == TRUE) && (PduInfoPtr->SduLength != maxLen) )
  {
    retVal = E_NOT_OK;
  }
  /* #22 Check if buffer length is invalid in case upper layer shall copy up to the provided buffer size otherwise. */
  else if ( (ExactLength == FALSE) && ((PduInfoPtr->SduLength < minLen) || (PduInfoPtr->SduLength > maxLen)) )
  {
    retVal = E_NOT_OK;
  }
  /* #23 Return success otherwise. */
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_TxIf_TriggerTransmitCheckParameter() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_TcpTransmit()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TcpTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType  soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check socket and set transmission active on socket connection and on socket. */
  if ( SoAd_TxSocketManager_CheckAndSetTxActive(soConIdx, PartitionIdx, TRUE) == E_OK )
  {
    /* #20 Prepare transmission. */
    if ( SoAd_TxIf_TcpPrepareTransmit(PduRouteDestIdx, PartitionIdx, PduInfoPtr) == E_OK )                             /* SBSW_SOAD_POINTER_FORWARD */
    {
      /* #30 Transmit IF-PDU on TCP socket connection. */
      retVal = SoAd_TxIf_TcpCallTransmit(PduRouteDestIdx, PartitionIdx);
    }

    /* #40 Finish transmission. */
    SoAd_TxIf_TcpFinishTransmit(PduRouteDestIdx, PartitionIdx);

    /* #50 Deactivate transmission on socket connection and on socket. */
    SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);
  }

  return retVal;
} /* SoAd_TxIf_TcpTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TcpPrepareTransmit()
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TcpPrepareTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  PduLengthType           dataLen = PduInfoPtr->SduLength;
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission via trigger transmit (multiple calls allowed) is configured. */
  if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
  {
    PduInfoType   pduInfo;
    PduLengthType triggerBufLength;

    pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
    pduInfo.SduLength = PduInfoPtr->SduLength;

# if ( SOAD_TX_DYN_LEN == STD_OFF )
    /* #20 Check if PDU data has been retrieved already (e.g. in case of routing group transmit). */
    if ( pduInfo.SduDataPtr != NULL_PTR )
    {
      retVal = E_OK;
    }
    else
# endif /* SOAD_TX_DYN_LEN == STD_OFF */
    /* #21 Try to get a trigger transmit buffer otherwise. */
    if ( SoAd_TxIfTriggerBuf_Reserve(pduRouteIdx, PartitionIdx, &pduInfo.SduLength, &pduInfo.SduDataPtr) == E_OK )     /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      triggerBufLength = pduInfo.SduLength;

      /* #22 Call trigger transmit and check if call succeeds. */
      if ( SoAd_GetIfTriggerTransmitCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(       /* SBSW_SOAD_FUNCTION_PTR */
        SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), &pduInfo) == E_OK )
      {
        /* #23 Update data length. */
        dataLen = pduInfo.SduLength;

        /* #24 Check if data copied by upper layer via trigger transmit is valid. */
        if ( SoAd_TxIf_TriggerTransmitCheckParameter(soConIdx, PartitionIdx, (uint16)triggerBufLength, &pduInfo, FALSE,/* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
          FALSE) == E_OK )
        {
          retVal = E_OK;
        }
      }
    }
    else
    {
      /* Nothing to do. */
    }

    /* #30 Prepare for call to CopyTxData if all previous actions succeeded. */
    if ( retVal == E_OK )
    {
      /* #40 Add PDU header size if valid. */
      if ( SoAd_TxSocketManager_CheckAndAddPduHdrSize(soConIdx, PartitionIdx, &dataLen) == E_OK )                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, PduRouteDestIdx, PartitionIdx);                                       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, dataLen, PartitionIdx);                                                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, dataLen, PartitionIdx);                                                /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, pduInfo.SduDataPtr, PartitionIdx);                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
  else
  /* #11 Prepare for call to CopyTxData for direct transmission otherwise. */
  {
    /* #12 Add PDU header size if valid. */
    if ( SoAd_TxSocketManager_CheckAndAddPduHdrSize(soConIdx, PartitionIdx, &dataLen) == E_OK )                        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #13 Prepare for call to CopyTxData if all previous actions succeeded. */
      SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, PduRouteDestIdx, PartitionIdx);                                         /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, PduInfoPtr->SduDataPtr, PartitionIdx);                                         /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, dataLen, PartitionIdx);                                                      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, dataLen, PartitionIdx);                                                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

      retVal = E_OK;
    }
  }

  return retVal;
} /* SoAd_TxIf_TcpPrepareTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TcpCallTransmit()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_TcpCallTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to reserve an element in the Tx TCP queue. */
  if ( SoAd_TxTcpQueue_ReserveNextElement(soConIdx, pduRouteIdx, PartitionIdx) == E_OK )
  {
    /* #20 Try to transmit data and check if transmission succeeds. */
    if ( SoAd_TxSocketManager_TcpTransmit(soConIdx, PartitionIdx,
        (uint32)SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx), TRUE) == E_OK )
    {
      /* #30 Try to set reserved element in the Tx TCP queue. */
      if ( SoAd_TxTcpQueue_SetElement(soConIdx, pduRouteIdx, PartitionIdx, SoAd_GetTxBytesLenOfTxMgt(txMgtIdx,
        PartitionIdx), FALSE) == E_OK )
      {
        /* #40 Add a pending TxConfirmation for the transmitted PDU. */
        SoAd_TxIf_AddPendingConf(pduRouteIdx, soConIdx, PartitionIdx);

        retVal = E_OK;
      }
      else
      {
        /* Nothing to do. */
      }
    }
    /* #21 Remove reserved element from Tx TCP queue otherwise. */
    else
    {
      /* To check result of releasing element is not required. */
      (void)SoAd_TxTcpQueue_SetElement(soConIdx, pduRouteIdx, PartitionIdx, 0, TRUE);
    }
  }

  return retVal;
} /* SoAd_TxIf_TcpCallTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxIf_TcpFinishTransmit()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_TcpFinishTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission via trigger transmit (multiple calls allowed) is configured. */
  if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
  {
    /* #20 Release trigger transmit buffer. */
    SoAd_TxIfTriggerBuf_Release(pduRouteIdx, PartitionIdx);
  }

  /* #30 Finish transmission of PduRouteDest on socket connection. */
  SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, NULL_PTR, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

} /* SoAd_TxIf_TcpFinishTransmit() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmit(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  Std_ReturnType       retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update alive timeout. */
  SoAd_SoCon_UpdateUdpAliveTimeout(soConIdx, PartitionIdx);

  /* #20 Handle transmission if nPdu is configured. */
  if ( SoAd_IsNPduUdpTxUsedOfSoCon(soConIdx, PartitionIdx) )
  {
    retVal = SoAd_TxIf_UdpTransmitNPdu(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                         /* SBSW_SOAD_POINTER_FORWARD */
  }
  else
  /* #21 Handle transmission of single PDU otherwise. */
  {
    retVal = SoAd_TxIf_UdpTransmitPdu(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                          /* SBSW_SOAD_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitNPdu()
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
/* PRQA S 3673 5 */ /* MD_SoAd_ConstCausedByUnused */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitNPdu(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType     soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfNPduUdpTxType nPduIdx = SoAd_GetNPduUdpTxIdxOfSoCon(soConIdx, PartitionIdx);
  PduLengthType            dataLen = PduInfoPtr->SduLength;
  Std_ReturnType           retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check and add PDU header size. */
  if ( SoAd_TxSocketManager_CheckAndAddPduHdrSize(soConIdx, PartitionIdx, &dataLen) == E_OK )                          /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #20 Check if PDU size is valid for nPdu. */
    if ( dataLen <= SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx) )
    {
      /* #30 Transmit via nPdu queue. */
      if ( SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx(nPduIdx, PartitionIdx) )
      {
        retVal = SoAd_TxIf_UdpTransmitNPduQueue(PduRouteDestIdx, PartitionIdx, PduInfoPtr);                            /* SBSW_SOAD_POINTER_FORWARD */
      }
      /* #31 Transmit via nPdu buffer. */
      else
      {
        retVal = SoAd_TxIf_UdpTransmitNPduBuffer(PduRouteDestIdx, PartitionIdx, PduInfoPtr);                           /* SBSW_SOAD_POINTER_FORWARD */
      }
    }
    /* #21 Set DET error since too much TCP transmissions are active or too much PduRouteDests are configured. */
    else
    {
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
      *ErrorIdPtr = SOAD_E_NOBUFS;                                                                                     /* SBSW_SOAD_POINTER_WRITE */
#else
      SOAD_DUMMY_STATEMENT(ErrorIdPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    }
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmitNPdu() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitNPduQueue()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitNPduQueue(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfNPduUdpTxType  nPduIdx = SoAd_GetNPduUdpTxIdxOfSoCon(soConIdx, PartitionIdx);
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to add or update an element for his PDU. */
  switch ( SoAd_TxNPduUdp_QueueAddOrUpdateElement(PduRouteDestIdx, PartitionIdx, PduInfoPtr) )                         /* SBSW_SOAD_POINTER_FORWARD */
  {
    /* #100 Handle nPdu in case adding new PDU leads to an overflow. */
    case SOAD_NPDU_OVFL_NEW:
    {
      /* #1000 Transmit nPdu. */
      SoAd_TxNPduUdp_QueueTransmit(nPduIdx, PartitionIdx);

      /* #1001 Try to add new PDU to nPdu again. */
      if ( SoAd_TxNPduUdp_QueueAddOrUpdateElement(PduRouteDestIdx, PartitionIdx, PduInfoPtr) == SOAD_NPDU_OK )         /* SBSW_SOAD_POINTER_FORWARD */
      {
        /* #1002 Transmit nPdu now if trigger mode is set to always. */
        if ( SoAd_GetTxUdpTriggerModeOfPduRouteDest(PduRouteDestIdx, PartitionIdx) == SOAD_TX_UDP_TRIGGER_ALWAYS )
        {
          SoAd_TxNPduUdp_QueueTransmit(nPduIdx, PartitionIdx);
        }
        retVal = E_OK;
      }
      break;
    }
    /* #101 Handle nPdu in case update of a PDU leads to an overflow. */
    case SOAD_NPDU_OVFL_UPDATE:
    {
      /* #1010 Transmit nPdu. */
      SoAd_TxNPduUdp_QueueTransmit(nPduIdx, PartitionIdx);
      retVal = E_OK;
      break;
    }
    /* #102 Handle nPdu in case new PDU is added to nPdu successfully. */
    case SOAD_NPDU_OK:
    {
      /* #1020 Transmit nPdu now if trigger mode is set to always. */
      if ( SoAd_GetTxUdpTriggerModeOfPduRouteDest(PduRouteDestIdx, PartitionIdx) == SOAD_TX_UDP_TRIGGER_ALWAYS )
      {
        SoAd_TxNPduUdp_QueueTransmit(nPduIdx, PartitionIdx);
      }
      retVal = E_OK;
      break;
    }
    /* #103 Reject transmission if PDU could not be added to nPdu. */
    default: /* SOAD_NPDU_NOT_OK */
    {
      /* Nothing to do. */
      break;
    }
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmitNPduQueue() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitNPduBuffer()
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
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitNPduBuffer(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType   pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfNPduUdpTxType  nPduIdx = SoAd_GetNPduUdpTxIdxOfSoCon(soConIdx, PartitionIdx);
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to add PDU to nPdu buffer. */
  switch ( SoAd_TxNPduUdp_BufferAddElement(pduRouteIdx, PduRouteDestIdx, PartitionIdx, PduInfoPtr) )                   /* SBSW_SOAD_POINTER_FORWARD */
  {
    /* #100 Handle nPdu in case adding new PDU leads to an overflow. */
    case SOAD_NPDU_OVFL_NEW:
    {
      /* #1000 Transmit nPdu. */
      SoAd_TxNPduUdp_BufferSend(nPduIdx, PartitionIdx);

      /* #1001 Try to add new PDU to nPdu again. */
      if ( SoAd_TxNPduUdp_BufferAddElement(pduRouteIdx, PduRouteDestIdx, PartitionIdx, PduInfoPtr) == SOAD_NPDU_OK )   /* SBSW_SOAD_POINTER_FORWARD */
      {
        /* #1002 Transmit nPdu now if trigger mode is set to always. */
        if ( SoAd_GetTxUdpTriggerModeOfPduRouteDest(PduRouteDestIdx, PartitionIdx) == SOAD_TX_UDP_TRIGGER_ALWAYS )
        {
          SoAd_TxNPduUdp_BufferSend(nPduIdx, PartitionIdx);
        }
        retVal = E_OK;
      }
      break;
    }
    /* #101 Handle nPdu in case new PDU is added to nPdu successfully. */
    case SOAD_NPDU_OK:
    {
      /* #1010 Transmit nPdu now if trigger mode is set to always. */
      if ( SoAd_GetTxUdpTriggerModeOfPduRouteDest(PduRouteDestIdx, PartitionIdx) == SOAD_TX_UDP_TRIGGER_ALWAYS )
      {
        SoAd_TxNPduUdp_BufferSend(nPduIdx, PartitionIdx);
      }
      retVal = E_OK;
      break;
    }
    /* #102 Reject transmission if PDU could not be added to nPdu. */
    default: /* SOAD_NPDU_NOT_OK */
    {
      /* Nothing to do. */
      break;
    }
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmitNPduBuffer() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitPdu(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check socket and set transmission active an socket connection and on socket. */
  if ( SoAd_TxSocketManager_CheckAndSetTxActive(soConIdx, PartitionIdx, TRUE) == E_OK )
  {
    /* #20 Transmit PDU via IP fragmentation buffer when using trigger transmit with IP fragmentation. */
    if ( (SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI) &&
      SoAd_IsIpFragMgtUsedOfSoCon(soConIdx, PartitionIdx) )
    {
      retVal = SoAd_TxIf_UdpTransmitPduFragment(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                /* SBSW_SOAD_POINTER_FORWARD */
    }
    /* #21 Transmit PDU directly otherwise. */
    else
    {
      retVal = SoAd_TxIf_UdpTransmitPduDirect(PduRouteDestIdx, PartitionIdx, PduInfoPtr);                              /* SBSW_SOAD_POINTER_FORWARD */
    }

    /* #30 Deactivate transmission on socket connection and on socket. */
    SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmitPdu() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitPduFragment()
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
/* PRQA S 3673 5 */ /* MD_SoAd_ConstCausedByUnused */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitPduFragment(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType               pduInfo = { NULL_PTR, 0u };
  SoAd_SizeOfPduRouteType   pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfUpperLayerType upperLayerIdx = SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx);
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  pduInfo.SduLength = PduInfoPtr->SduLength;

  /* #10 Try to reserve an IP fragmentation buffer and check if reservation succeeds. */
  if ( SoAd_TxIpFragBuf_Reserve(soConIdx, PartitionIdx, PduInfoPtr->SduLength, &pduInfo.SduDataPtr) == E_OK )          /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #20 Call trigger transmit to copy data to IP fragmentation buffer and check if call succeeds. */
    if ( SoAd_GetIfTriggerTransmitCbkOfUpperLayer(upperLayerIdx)(                                                      /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), &pduInfo) == E_OK )
    {
      /* #200 Check if upper layer returned valid parameter. */
      retVal = SoAd_TxIf_TriggerTransmitCheckParameter(soConIdx, PartitionIdx, (uint16)PduInfoPtr->SduLength, &pduInfo,/* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
        FALSE, FALSE);
    }

    /* #30 Transmit PDU using IP fragmentation buffer if trigger transmit is valid. */
    if ( retVal == E_OK )
    {
      retVal = SoAd_TxIf_UdpTransmitPduDirect(PduRouteDestIdx, PartitionIdx, &pduInfo);                                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    }

    /* #40 Release IP fragmentation buffer. */
    SoAd_TxIpFragBuf_Release(soConIdx, PartitionIdx);
  }
  /* #11 Set DET error otherwise. */
  else
  {
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
    *ErrorIdPtr = SOAD_E_NOBUFS;                                                                                       /* SBSW_SOAD_POINTER_WRITE */
#else
    SOAD_DUMMY_STATEMENT(ErrorIdPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmitPduFragment() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpTransmitPduDirect()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_UdpTransmitPduDirect(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  PduLengthType           locPduLen = PduInfoPtr->SduLength;
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add PDU header size if PDU is valid. */
  if ( (SoAd_TxSocketManager_CheckAndAddPduHdrSize(soConIdx, PartitionIdx, &locPduLen) == E_OK)                        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#if ( SOAD_MAX_PDU_LENGTH == SOAD_UINT32 )
  && (locPduLen <= SOAD_MAX_UDP_PDU_LEN)
#endif /* SOAD_MAX_PDU_LENGTH == SOAD_UINT32 */
    )
  {
    /* #20 Prepare transmission via CopyTxData. */
    SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, PduRouteDestIdx, PartitionIdx);                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, PduInfoPtr->SduDataPtr, PartitionIdx);                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, locPduLen, PartitionIdx);                                                      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, locPduLen, PartitionIdx);                                                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

    /* #30 Try to transmit PDU over UDP and check if transmission succeeds. */
    /* PRQA S 310 2 */ /* MD_SoAd_PointerCastOnStruct */
    if ( SoAd_TxSocketManager_UdpTransmit(soConIdx, PartitionIdx,
      (uint16)SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx)) == E_OK )
    {
      /* #40 Add a pending TxConfirmation for the transmitted PDU. */
      SoAd_TxIf_AddPendingConf(pduRouteIdx, soConIdx, PartitionIdx);

      retVal = E_OK;
    }

    /* #50 Finish transmission on socket connection. */
    SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, NULL_PTR, PartitionIdx);                                                         /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  }

  return retVal;
} /* SoAd_TxIf_UdpTransmitPduDirect() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxDataTrigger()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxIf_UdpCopyTxDataTrigger(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType               pduInfo;
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfUpperLayerType upperLayerIdx = SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  BufReq_ReturnType         retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare provided buffer for trigger transmit call. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    pduInfo.SduDataPtr = &BufPtr[SOAD_PDU_HDR_SIZE];
    /* PRQA S 4391 1 */ /* MD_SoAd_GenericWiderTypeCast */
    pduInfo.SduLength = (PduLengthType)(*BufLengthPtr - SOAD_PDU_HDR_SIZE);
  }
  else
  {
    pduInfo.SduDataPtr = BufPtr;
    pduInfo.SduLength = (PduLengthType)*BufLengthPtr;
  }

  /* #20 Call trigger transmit and check if call succeeds. */
  if ( SoAd_GetIfTriggerTransmitCbkOfUpperLayer(upperLayerIdx)(                                                        /* SBSW_SOAD_FUNCTION_PTR */
    SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), &pduInfo) == E_OK )
  {
    /* #200 Check if upper layer returned valid parameter. */
#if ( SOAD_TX_DYN_LEN == STD_ON )
    if ( SoAd_TxIf_TriggerTransmitCheckParameter(SoConIdx, PartitionIdx, *BufLengthPtr, &pduInfo, FALSE, TRUE) ==      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      E_OK )
#else
    if ( SoAd_TxIf_TriggerTransmitCheckParameter(SoConIdx, PartitionIdx, *BufLengthPtr, &pduInfo, TRUE, TRUE) == E_OK )/* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_TX_DYN_LEN == STD_ON */
    {
      /* #2000 Finish copy and update copied length if required. */
      SoAd_TxIf_UdpCopyTxDataTriggerFinish(SoConIdx, PartitionIdx, BufPtr, BufLengthPtr, &pduInfo);                    /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */

      retVal = BUFREQ_OK;
    }
  }

  return retVal;
} /* SoAd_TxIf_UdpCopyTxDataTrigger() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxDataTriggerFinish()
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
/* PRQA S 3206, 3673 5 */ /* MD_SoAd_UnusedParameter, MD_SoAd_ConstCausedByUnused */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_UdpCopyTxDataTriggerFinish(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
#if ( SOAD_TX_DYN_LEN == STD_ON )
  PduLengthType         oldPduLength = SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx);
#endif /* SOAD_TX_DYN_LEN == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Indicate that no further data is pending for this transmission. */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

#if ( SOAD_TX_DYN_LEN == STD_ON )
  /* #20 Update length of whole PDU (if CopyTxData with dynamic length is enabled). */
  SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, PduInfoPtr->SduLength, PartitionIdx);                                            /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

  /* #30 Check if PDU header is used. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #300 Consider PDU header length when updating length if whole PDU. */
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) + SOAD_PDU_HDR_SIZE,         /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      PartitionIdx);

    /* #301 Update PDU header if length has changed during trigger transmit call. */
    if ( PduInfoPtr->SduLength != (oldPduLength - SOAD_PDU_HDR_SIZE) )
    {
      SoAd_Util_UpdatePduHdrLen(PduInfoPtr->SduLength, BufPtr);                                                        /* SBSW_SOAD_POINTER_FORWARD_EXPECTED_SIZE_TX */
    }
  }

  /* #40 Return updated length (if CopyTxData with dynamic length is enabled). */
  *BufLengthPtr = (uint16)SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx);                                           /* SBSW_SOAD_POINTER_WRITE */
#else
  SOAD_DUMMY_STATEMENT(BufPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  SOAD_DUMMY_STATEMENT(BufLengthPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  SOAD_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_TX_DYN_LEN == STD_ON */
} /* SoAd_TxIf_UdpCopyTxDataTriggerFinish() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxDataDirect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_UdpCopyTxDataDirect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2CONST(uint8)   txBufPtr;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  PduLengthType         bytesCopied;
  uint16                bytesToCopy;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate buffer index and length to prepare for copying. */
  bytesCopied = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SoAd_GetTxBytesPendingOfTxMgt(
    txMgtIdx, PartitionIdx));
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    if ( bytesCopied < SOAD_PDU_HDR_SIZE )
    {
      bytesToCopy = 0u;
      bytesCopied = 0u;
    }
    else
    {
      bytesToCopy = BufLength;
      bytesCopied -= SOAD_PDU_HDR_SIZE;
    }
  }
  else
  {
    bytesToCopy = BufLength;
  }

  /* #20 Copy PDU data. */
  txBufPtr = SoAd_GetTxBufPtrOfTxMgt(txMgtIdx, PartitionIdx);
  /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
  VStdMemCpy(                                                                                                          /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
    SOAD_A_P2VAR(void)&BufPtr[0u],
    SOAD_A_P2CONST(void)&txBufPtr[bytesCopied],
    bytesToCopy);

  /* #30 Decrement pending data length. */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)(SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    bytesToCopy), PartitionIdx);
} /* SoAd_TxIf_UdpCopyTxDataDirect() */

/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConf()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConf(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TxConfirmation is enabled. */
  if ( SoAd_IsTxConfEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
#if ( SOAD_TCP == STD_ON )
    /* #20 Check if socket type is UDP. */
    if ( SoAd_IsSocketUdpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
#endif /* SOAD_TCP == STD_ON */
    {
      /* #200 Add pending TxConfirmation on UDP socket connection. */
      SoAd_TxIf_AddPendingConfUdp(PduRouteIdx, SoConIdx, PartitionIdx);
    }
#if ( SOAD_TCP == STD_ON )
    else
    /* #30 Assume socket type is TCP otherwise (if enabled). */
    {
      /* #300 Add pending TxConfirmation on TCP socket connection. */
      SoAd_TxIf_AddPendingConfTcp(PduRouteIdx, PartitionIdx);
    }
#endif /* SOAD_TCP == STD_ON */
  }
} /* SoAd_TxIf_AddPendingConf() */

/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConfUdp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConfUdp(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if nPdu queue is used. */
  if ( SoAd_IsNPduUdpTxUsedOfSoCon(SoConIdx, PartitionIdx) &&
    (SoAd_GetIfTriggerTransmitModeOfPduRoute(PduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI) )
  {
    /* #100 Increment pending TxConfirmation counter to confirm after TriggerTransmit call. */
    SoAd_IncPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx);                                                  /* SBSW_SOAD_PARAMETER_IDX */
  }
  /* #11 Check if UdpImmediateIfTxConfirmation is disabled otherwise (if feature is enabled at all). */
  else if ( !SoAd_IsImmedIfTxConfOfSocketUdp(socketUdpIdx, PartitionIdx) )
  {
    /* #110 Set flag to call TxConfirmation in next main function. */
    /* May be set multiple times if multiple PduRoutesDests are configured with this confirmation strategy. */
    SoAd_SetPendingTxConfMainOfPduRouteDyn(PduRouteIdx, TRUE, PartitionIdx);                                           /* SBSW_SOAD_PARAMETER_IDX */
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE, PduRouteIdx,
      SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
  }
  else
  {
    /* nothing to do */
  }
} /* SoAd_TxIf_AddPendingConfUdp() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_AddPendingConfTcp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_AddPendingConfTcp(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set flag to call TxConfirmation immediately if optimized transmission is used. */
  if ( SoAd_IsTxOptimizedOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    SoAd_SetPendingTxConfMainOfPduRouteDyn(PduRouteIdx, TRUE, PartitionIdx);                                           /* SBSW_SOAD_PARAMETER_IDX */
  }
  /* #20 Increment pending TxConfirmation counter to confirm in SoAd_TxConfirmation otherwise. */
  else
  {
    SoAd_IncPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx);                                                  /* SBSW_SOAD_PARAMETER_IDX */
  }
} /* SoAd_TxIf_AddPendingConfTcp() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxIf_TxConfirmation()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxIf_TxConfirmation(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PduRouteDestIterType pduRouteDestIter;
  SoAd_SizeOfSoConType      soConIdx;
  SoAd_SizeOfSoConGrpType   soConGrpIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all PduRouteDests. */
  for ( pduRouteDestIter = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
    pduRouteDestIter < SoAd_GetPduRouteDestEndIdxOfPduRoute(PduRouteIdx, PartitionIdx);
    pduRouteDestIter++ )
  {
    soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIter, PartitionIdx);
    soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);

    /* #20 Check if PduRouteDest is configured for TCP or UDP immediate TxConfirmation is disabled. */
    if (
#if ( SOAD_TCP == STD_ON )
      !SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) ||
#endif /* SOAD_TCP == STD_ON */
      !SoAd_IsImmedIfTxConfOfSocketUdp(SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx))
    {
      /* #30 Check if only a TxConfirmation is pending which is handled in main function by default. */
      if ( SoAd_TxIf_CheckAndClearPendingTxConfirmation(PduRouteIdx, PartitionIdx) == E_OK )
      {
        /* #40 Reconnect socket connection if socket connection has auto setup enabled and wildcards configured. */
#if ( SOAD_TCP == STD_ON )
        if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
        {
          SoAd_SoCon_RequestReconnect(soConIdx, PartitionIdx);
        }
        else
#endif /* SOAD_TCP == STD_ON */
        {
          SoAd_SoCon_Reconnect(soConIdx, PartitionIdx);
        }

        /* #50 Call TxConfirmation. */
        SoAd_TxIf_CallTxConfirmation(PduRouteIdx, PartitionIdx);

        break;
      }
    }
  }
} /* SoAd_TxIf_TxConfirmation() */

/**********************************************************************************************************************
 *  SoAd_TxIf_CheckAndClearPendingTxConfirmation()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIf_CheckAndClearPendingTxConfirmation(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that any interruption occurs. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if a TxConfirmation is pending for this PduRoute which is handled in main function by default. */
  if ( SoAd_IsPendingTxConfMainOfPduRouteDyn(PduRouteIdx, PartitionIdx) )
  {
    /* #30 Indicate that no TxConfirmation is pending anymore which is handled in main function by default. */
    SoAd_SetPendingTxConfMainOfPduRouteDyn(PduRouteIdx, FALSE, PartitionIdx);                                          /* SBSW_SOAD_PARAMETER_IDX */

    /* #40 Check if now no TxConfirmation is pending at all for this PduRoute. */
    if ( SoAd_GetPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx) == 0u )
    {
      /* #50 Indicate that TxConfirmation can be called. */
      retVal = E_OK;
    }
  }
  /* #60 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxIf_CheckAndClearPendingTxConfirmation() */

/**********************************************************************************************************************
 *  SoAd_TxIf_CallTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIf_CallTxConfirmation(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TxConfirmation is enabled for this PDU. */
  if ( SoAd_IsTxConfEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    /* #20 Call TxConfirmation of upper layer. */
    SoAd_GetIfTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(               /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx));
  }
} /* SoAd_TxIf_CallTxConfirmation() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_TcpTxConfirmation()
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
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxIf_TcpTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ByResetSoCon)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TxConfirmation is enabled for this PDU. */
  if ( SoAd_IsTxConfEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    /* #20 Enter critical section to prevent that other confirmation interrupts and TxConfirmation is called twice. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Check if a pending TxConfirmation is left. */
    if ( SoAd_GetPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx) > 0u )
    {
      /* #40 Decrement pending confirmation counter. */
      SoAd_DecPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx);                                                /* SBSW_SOAD_PARAMETER_IDX */

      /* #50 Check if no confirmation is pending (i.e. transmission confirmed on all PduRouteDests). */
      if ( (SoAd_GetPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx) == 0u) &&
        !SoAd_IsPendingTxConfMainOfPduRouteDyn(PduRouteIdx, PartitionIdx) )
      {
        /* #60 Check if confirmation is not requested by resetting socket connection on close. */
        if ( ByResetSoCon == FALSE )
        {
          /* #600 Reconnect socket connection if socket connection has auto setup enabled and wildcards configured. */
          SoAd_SoCon_RequestReconnect(SoConIdx, PartitionIdx);
        }

        /* #70 Leave critical section to be able to call TxConfirmation of upper layer. */
        SOAD_END_CRITICAL_SECTION();

        /* #80 Call TxConfirmation of upper layer. */
        SoAd_GetIfTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(           /* SBSW_SOAD_FUNCTION_PTR */
          SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx));
      }
      /* #51 Leave critical section otherwise. */
      else
      {
        SOAD_END_CRITICAL_SECTION();
      }
    }
    /* #31 Leave critical section otherwise. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }
  }
} /* SoAd_TxIf_TcpTxConfirmation() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckParameter(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check module state. */
  if ( SoAd_State_GetState() == SOAD_STATE_INIT )
  {
    /* #20 Check PDU length. */
    if ( PduLength > 0u )
    {
      /* #30 Check socket connection state. */
      /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
      if ( (SoAd_SoCon_GetCloseMode(soConIdx, PartitionIdx) == SOAD_CLOSE_NONE) &&
        (SoAd_SoCon_GetMode(soConIdx, PartitionIdx) == SOAD_SOCON_ONLINE) )
      {
        /* #40 Check if the remote address of the socket connection is not forced to be released. */
        if ( SoAd_RemoteAddr_IsForceRelease(soConIdx, PartitionIdx) == FALSE )
        {
          retVal = E_OK;
        }
      }
    }
  }

  return retVal;
} /* SoAd_TxTp_CheckParameter() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TransmitPdu()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TransmitPdu(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any routing group is enabled for this PduRouteDest. */
  if ( SoAd_RouteGrp_CheckAnyRoutGrpOnPduRouteDestEnabled(pduRouteDestIdx, PartitionIdx) == TRUE )
  {
    /* #20 Enter critical section to prevent interruption by another transmission request. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Check if corresponding socket connection is not used for another transmission and if socket is open. */
    if ( SoAd_TxSocketManager_CheckAndSetTxActive(soConIdx, PartitionIdx, FALSE) == E_OK )
    {
      /* #40 Check and set protocol (TCP/UDP) specific parameter. */
      if ( SoAd_TxTp_CheckAndSetProtocolParameter(PduRouteIdx, PartitionIdx, PduInfoPtr->SduLength) == E_OK )
      {
        /* #50 Prepare transmission on socket connection. */
        SoAd_TxTp_PrepareTransmitPdu(PduRouteIdx, PartitionIdx, PduInfoPtr->SduLength);

        retVal = E_OK;
      }
      /* #60 Reset transmission active on socket otherwise. */
      else
      {
        SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, FALSE);
      }
    }

    /* #70 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }

  /* #80 Check if transmission is prepared. */
  if ( retVal == E_OK )
  {
    /* #90 Handle transmission in this context if TpOptimized is enabled for this PDU. */
    if ( SoAd_IsTxOptimizedOfPduRoute(PduRouteIdx, PartitionIdx) )
    {
      retVal = SoAd_TxTp_TransmitPduRouteDest(soConIdx, PartitionIdx, FALSE);
    }
    /* #91 Add event to corresponding event queue to handle transmission in main function context otherwise. */
    else
    {
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, soConIdx,
        SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
    }
  }

  return retVal;
} /* SoAd_TxTp_TransmitPdu() */

/**********************************************************************************************************************
 *  SoAd_TxTp_CheckAndSetProtocolParameter()
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
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CheckAndSetProtocolParameter(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
#if ( SOAD_TCP == STD_ON )
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
#endif /* SOAD_TCP == STD_ON */
  PduLengthType               locPduLen = PduLength;
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add PDU header size if required and valid. */
  if ( SoAd_TxSocketManager_CheckAndAddPduHdrSize(soConIdx, PartitionIdx, &locPduLen) == E_OK )                        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
#if ( SOAD_TCP == STD_ON )
    /* #20 Handle transmission over TCP. */
    if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      /* #200 Check if TxQueue can handle new request. */
      if ( SoAd_TxTcpQueue_ReserveNextElement(soConIdx, PduRouteIdx, PartitionIdx) == E_OK )
      {
        /* #2000 Set element considering local PDU length. */
        retVal = SoAd_TxTcpQueue_SetElement(soConIdx, PduRouteIdx, PartitionIdx, locPduLen, FALSE);
      }
    }
    else
#endif /* SOAD_TCP == STD_ON */
      /* #30 Handle transmission over UDP. */
    {
      SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(soConIdx, PartitionIdx);

#if ( SOAD_MAX_PDU_LENGTH == SOAD_UINT32 )
      /* #300 Check if PDU length is valid. */
      if ( locPduLen <= SOAD_MAX_UDP_PDU_LEN )
#endif /* SOAD_MAX_PDU_LENGTH == SOAD_UINT32 */
      {
        /* #3000 Check if TP Tx buffer size is sufficient. */
        if ( PduLength <=
          (SoAd_SizeOfTpTxBufferType)(SoAd_GetTpTxBufferEndIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx) -
            SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx)) )
        {
          /* #30000 Update Alive Supervision Timeout. */
          SoAd_SoCon_UpdateUdpAliveTimeout(soConIdx, PartitionIdx);

          retVal = E_OK;
        }
      }
    }
  }
  return retVal;
} /* SoAd_TxTp_CheckAndSetProtocolParameter() */

/**********************************************************************************************************************
 *  SoAd_TxTp_PrepareTransmitPdu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_PrepareTransmitPdu(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType PduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  PduLengthType               locPduLen = PduLength;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Consider length of PDU header if enabled. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx), PartitionIdx) )
  {
    locPduLen += SOAD_PDU_HDR_SIZE;
  }

  /* #20 Set transmission management structure. */
  SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, pduRouteDestIdx, PartitionIdx);                                             /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, locPduLen, PartitionIdx);                                                        /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, locPduLen, PartitionIdx);                                                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
} /* SoAd_TxTp_PrepareTransmitPdu() */

/**********************************************************************************************************************
 *  SoAd_TxTp_CallCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxTp_CallCopyTxData(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) BufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfUpperLayerType upperLayerIdx = SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  BufReq_ReturnType         retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call CopyTxData function with non-constant pointer if configured. */
  if ( SoAd_GetTpCopyTxDataCbkOfUpperLayer(upperLayerIdx) != NULL_PTR )
  {
    retVal = SoAd_GetTpCopyTxDataCbkOfUpperLayer(upperLayerIdx)(                                                       /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), PduInfoPtr, NULL_PTR, BufferSizePtr);
  }
  /* #20 Call CopyTxData function with constant pointer otherwise. */
  else
  {
    retVal = SoAd_GetTpCopyTxDataConstCbkOfUpperLayer(upperLayerIdx)(
      SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), PduInfoPtr, NULL_PTR, BufferSizePtr);                  /* SBSW_SOAD_FUNCTION_PTR */
  }

  return retVal;
} /* SoAd_TxTp_CallCopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxTp_CancelTransmit()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_CancelTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestStartIdxOfPduRoute(PduRouteIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent interruption while getting PDU route destination index and while cancel flag
   *     is set dependent on transmission state. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if PDU transmission is active on socket connection. */
  if ( pduRouteDestIdx == SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx) )
  {
    /* #30 Set flag to perform cancellation. */
    SoAd_SetCancelRequestedOfTxMgt(txMgtIdx, TRUE, PartitionIdx);                                                      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

    /* #40 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #50 Set event to handle PDU (i.e. cancellation) in main function. */
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, soConIdx,
      SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);

    retVal = E_OK;
  }
  /* #21 Leave critical section otherwise. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }

  return retVal;
} /* SoAd_TxTp_CancelTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TransmitPduRouteDest()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TransmitPduRouteDest(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType       retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent interruption while getting parameters and preparing for transmission. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if TP PDU transmission is active. */
  if ( SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx) != SoAd_GetSizeOfPduRouteDest(PartitionIdx) )
  {
    /* #30 Check if socket connection is not requested to be closed. */
    if ( SoAd_SoCon_GetCloseMode(SoConIdx, PartitionIdx) == SOAD_CLOSE_NONE )
    {
      /* #40 Handle transmit cancellation if requested. */
      if ( SoAd_IsCancelRequestedOfTxMgt(txMgtIdx, PartitionIdx) )
      {
        SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);

        /* #400 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        SoAd_TxTp_HandleCancellation(SoConIdx, pduRouteDestIdx, PartitionIdx);
      }
      /* #41 Continue transmission otherwise. */
      else
      {
        SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

        /* #410 Assert that no transmission is currently active on the socket connection. */
        if ( !SoAd_IsTxActiveOfSoConDyn(SoConIdx, PartitionIdx) )
        {
          /* #4100 Set the active transmission flag on the socket connection. */
          SoAd_SetTxActiveOfSoConDyn(SoConIdx, TRUE, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */

          /* #4101 Leave critical section. */
          SOAD_END_CRITICAL_SECTION();

#if ( SOAD_TCP == STD_ON )
          /* #4102 Continue transmission for TCP (if enabled). */
          if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
          {
            retVal = SoAd_TxTp_TcpTransmit(SoConIdx, PartitionIdx, CalledByMain);
          }
          else
#endif /* SOAD_TCP == STD_ON */
          /* #4103 Continue transmission for UDP otherwise. */
          {
            retVal = SoAd_TxTp_UdpTransmit(SoConIdx, PartitionIdx, CalledByMain);
          }
          /* #4104 Reset the active transmission flag on the socket connection. */
          SoAd_SetTxActiveOfSoConDyn(SoConIdx, FALSE, PartitionIdx);                                                   /* SBSW_SOAD_PARAMETER_IDX */
        }
        /* #411 Add event to corresponding event queue to handle transmission in main function context otherwise. */
        else
        {
          SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(
              soConGrpIdx, PartitionIdx), PartitionIdx);

          /* #4111 Leave critical section. */
          SOAD_END_CRITICAL_SECTION();

          retVal = E_OK;
        }
      }
    }
    /* #31 Leave critical section otherwise. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }
  }
  /* #21 Leave critical section otherwise. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }

  return retVal;
} /* SoAd_TxTp_TransmitPduRouteDest() */

/**********************************************************************************************************************
 *  SoAd_TxTp_HandleCancellation()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_HandleCancellation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType            txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType         pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType         soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfTpTxBufferConfigType tpBufferConfigIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Release local TP transmission buffer. */
#if ( SOAD_TCP == STD_ON )
  if ( SoAd_IsTpTxBufferConfigUsedOfSoCon(SoConIdx, PartitionIdx) )
#endif /* SOAD_TCP == STD_ON */
  {
    tpBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);                                    /* SBSW_SOAD_OPTIONAL_CHECKED_PREPROC_CSL03_CSL05 */
    SoAd_SetTpTxBufferIdxOfTpTxBufferConfigDyn(tpBufferConfigIdx, SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(        /* SBSW_SOAD_OPTIONAL_CHECKED_PREPROC_CSL03_CSL05 */
      tpBufferConfigIdx, PartitionIdx), PartitionIdx);
  }

#if ( SOAD_TCP == STD_ON )
  /* #20 Release Tx TCP queue element (if enabled). */
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    /* To check result of releasing element is not required. */
    (void)SoAd_TxTcpQueue_SetElement(SoConIdx, pduRouteIdx, PartitionIdx, 0u, TRUE);
  }
#endif /* SOAD_TCP == STD_ON */

  /* #30 Reset TP session. */
  SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                               /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetCancelRequestedOfTxMgt(txMgtIdx, FALSE, PartitionIdx);                                                       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

  /* #40 Request to close the socket connection in main function context. */
  SoAd_SoCon_RequestClose(SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx,
    SOAD_CLOSE_OFFLINE_RESET, FALSE);

  /* #50 Call upper layer TpTxConfirmation with negative result. */
  SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(                 /* SBSW_SOAD_FUNCTION_PTR */
    SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), E_NOT_OK);
} /* SoAd_TxTp_HandleCancellation() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmit()
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
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TcpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  PduInfoType                 pduInfo = { NULL_PTR, 0u };
  PduLengthType               availableBufLen = 0u;
  PduLengthType               txBytesPending;
  boolean                     continueTransmission = TRUE;
  Std_ReturnType              retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate until PDU is completely transmitted. */
  while ( SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) > 0u )
  {
    /* #20 Store the current amount of pending Tx bytes. */
    txBytesPending = SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx);

    /* #30 Call CopyTxData and check if call succeeds. */
    if ( SoAd_TxTp_CallCopyTxData(pduRouteIdx, PartitionIdx, &pduInfo, &availableBufLen) == BUFREQ_OK )                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #300 Calculate length of data that can be transmitted now. */
      SoAd_TxTp_TcpCalculateLength(SoConIdx, PartitionIdx, &availableBufLen);                                          /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

      /* #301 Handle transmission to TcpIp if upper layer has data for transmission available. */
      if ( availableBufLen > 0u )
      {
        retVal = SoAd_TxTp_TcpTransmitOnSocket(SoConIdx, PartitionIdx, availableBufLen, CalledByMain);

        /* #3010 Indicate to continue transmission in main function in case that no further bytes were copied. */
        if ( SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) == txBytesPending )
        {
          SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, SoConIdx,
            SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);

          continueTransmission = FALSE;
        }
      }
      /* #302 Indicate to continue transmission in main function otherwise. */
      else
      {
        SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, SoConIdx,
          SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);

        continueTransmission = FALSE;
      }
    }
    /* #31 Handle case if call to CopyTxData failed otherwise. */
    else
    {
      /* #310 Request to close the socket connection in main function context. */
      SoAd_SoCon_RequestClose(SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx,
        SOAD_CLOSE_OFFLINE_RESET, FALSE);

      /* #311 Return that transmission has failed. */
      continueTransmission = FALSE;
      retVal = E_NOT_OK;
    }

    /* #40 Stop transmission in this context if transmission failed or transmission currently cannot be proceeded. */
    if ( continueTransmission == FALSE )
    {
      break;
    }
  }

  return retVal;
} /* SoAd_TxTp_TcpTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpCalculateLength()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpCalculateLength(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) AvailableBufLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  PduLengthType         maxExpectedBufLen;
  PduLengthType         pduHdrRestLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if upper layer has available buffer. */
  if ( *AvailableBufLenPtr > 0u )
  {
    /* #20 Set maximum expected available buffer size to pending data size. */
    maxExpectedBufLen = SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx);

    /* #30 Check if PDU header is enabled. */
    if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
    {
      /* #40 Get pending PDU header length. */
      pduHdrRestLen = SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx,
        PartitionIdx);
      if ( pduHdrRestLen > SOAD_PDU_HDR_SIZE )
      {
        pduHdrRestLen = 0u;
      }
      else
      {
        pduHdrRestLen = SOAD_PDU_HDR_SIZE - pduHdrRestLen;
      }

      /* #50 Add pending PDU header length to available buffer size. */
      *AvailableBufLenPtr += pduHdrRestLen;                                                                            /* SBSW_SOAD_POINTER_WRITE */
    }

    /* #60 Overwrite available upper layer buffer size if it exceeds the maximum expected available buffer size. */
    if ( *AvailableBufLenPtr > maxExpectedBufLen )
    {
      *AvailableBufLenPtr = maxExpectedBufLen;                                                                         /* SBSW_SOAD_POINTER_WRITE */
    }
  }
} /* SoAd_TxTp_TcpCalculateLength() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmitOnSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_TcpTransmitOnSocket(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType AvailableBufLen,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to transmit data and handle TP session if transmission succeeded. */
  if ( SoAd_TxSocketManager_TcpTransmit(SoConIdx, PartitionIdx, (uint32)AvailableBufLen, FALSE) == E_OK )
  {
    SoAd_TxTp_TcpTransmitHandleSuccess(SoConIdx, PartitionIdx);
    retVal = E_OK;
  }
  /* #20 Handle failed TP session otherwise. */
  else
  {
    SoAd_TxTp_TcpTransmitHandleFail(SoConIdx, PartitionIdx, CalledByMain);
  }

  return retVal;
} /* SoAd_TxTp_TcpTransmitOnSocket() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmitHandleSuccess()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpTransmitHandleSuccess(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission is finsihed. */
  if ( SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) == 0u )
  {
    /* #20 Check if immediate TP TxConfirmation is enabled. */
    if ( SoAd_IsImmedTpTxConfOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
    {
      /* #30 Reset TP session. */
      SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

      SoAd_TxSocketManager_ResetTxActive(SoConIdx, PartitionIdx, FALSE);

      /* #40 Reconnect socket connection if socket connection has auto setup enabled and wildcards configured. */
      SoAd_SoCon_RequestReconnect(SoConIdx, PartitionIdx);

      /* #50 Call TpTxConfirmation of upper layer with positive result. */
      SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(             /* SBSW_SOAD_FUNCTION_PTR */
        SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), E_OK);
    }
  }
} /* SoAd_TxTp_TcpTransmitHandleSuccess() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTransmitHandleFail()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpTransmitHandleFail(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Indicate to close socket connection in main function if transmission has already started. */
  if ( SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) != SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) )
  {
    /* #100 Request to close the socket connection in main function context. */
    SoAd_SoCon_RequestClose(SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx,
      SOAD_CLOSE_OFFLINE_RESET, FALSE);
  }
  /* #11 Stop transmission of this PDU otherwise. */
  else
  {
    /* To check result of releasing element is not required. */
    (void)SoAd_TxTcpQueue_SetElement(SoConIdx, pduRouteIdx, PartitionIdx, 0u, TRUE);
  }

  /* #20 Reset TP session. */
  SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                               /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

  SoAd_TxSocketManager_ResetTxActive(SoConIdx, PartitionIdx, FALSE);

  /* #30 Call TpTxConfirmation of upper layer with negative result if transmission is continued in main function. */
  if ( CalledByMain == TRUE )
  {
    SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(               /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), E_NOT_OK);
  }
} /* SoAd_TxTp_TcpTransmitHandleFail() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpTxConfirmation()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_TcpTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if immediate TxConfirmation for TP-API is disabled. */
  if ( !SoAd_IsImmedTpTxConfOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
  {
    /* #20 Check if cancellation is not requested. */
    if ( !SoAd_IsCancelRequestedOfTxMgt(txMgtIdx, PartitionIdx) )
    {
      /* #30 Reconnect socket connection if socket connection was opened on reception. */
      SoAd_SoCon_RequestReconnect(SoConIdx, PartitionIdx);

      /* #40 Finish TP transmission. */
      SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

      SoAd_TxSocketManager_ResetTxActive(SoConIdx, PartitionIdx, FALSE);

      /* #50 Call TxConfirmation of user. */
      SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(             /* SBSW_SOAD_FUNCTION_PTR */
        SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), E_OK);
    }
  }
} /* SoAd_TxTp_TcpTxConfirmation() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_UdpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType            txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update UDP alive supervision timeout. */
  SoAd_SoCon_UpdateUdpAliveTimeout(SoConIdx, PartitionIdx);

  /* #20 Copy PDU header to TP transmission buffer. */
  SoAd_TxTp_UdpCopyPduHdr(SoConIdx, PartitionIdx);

  /* #30 Copy PDU to TP transmission buffer. */
  if ( SoAd_TxTp_UdpCopyPdu(SoConIdx, PartitionIdx, CalledByMain) == E_OK )
  {
    /* #40 Transmit PDU over UDP socket if PDU is copied completely to TP transmission buffer. */
    if ( SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) ==
      (SoAd_SizeOfTpTxBufferType)(SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx, PartitionIdx) -
        SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx)) )
    {
      retVal = SoAd_TxTp_UdpTransmitOnSocket(SoConIdx, PartitionIdx, CalledByMain);
    }
    else
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* SoAd_TxTp_UdpTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpCopyPduHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTp_UdpCopyPduHdr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType            txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType     pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PDU header is enabled. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #20 Check if PDU header is not copied yet. */
    if ( SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx, PartitionIdx) ==
      SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx) )
    {
      /* #30 Copy PDU header to TP transmission buffer. */
      /* PRQA S 4391 2 */ /* MD_SoAd_GenericWiderTypeCast */
      SoAd_Util_CopyPduHdr2Buf(SoAd_GetTxPduHdrIdOfPduRouteDest(pduRouteDestIdx, PartitionIdx),                        /* SBSW_SOAD_OPTIONAL_POINTER_FORWARD_CSL03_CSL05 */
        (SoAd_PduHdrLenType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SOAD_PDU_HDR_SIZE),
        SoAd_GetAddrTpTxBuffer(SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx),
          PartitionIdx), 0u, SOAD_PDU_HDR_SIZE);

      SoAd_SetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx,                                                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        (SoAd_TpTxBufferIdxOfTpTxBufferConfigDynType)(SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx,
            PartitionIdx) + SOAD_PDU_HDR_SIZE), PartitionIdx);
    }
  }
} /* SoAd_TxTp_UdpCopyPduHdr() */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpCopyPdu()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_UdpCopyPdu(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType                     pduInfo = { NULL_PTR, 0u };
  PduLengthType                   availableBufLen = 0u;
  PduLengthType                   maxExpectedBufLen;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType            txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType     pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType         pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType         soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);
  boolean                         continueTransmission = TRUE;
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate until PDU is copied completely to TP transmission buffer. */
  while ( SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) >
    (SoAd_SizeOfTpTxBufferType)(SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx, PartitionIdx) -
      SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx)) )
  {
    /* #20 Call CopyTxData and check if call succeeds. */
    if ( SoAd_TxTp_CallCopyTxData(pduRouteIdx, PartitionIdx, &pduInfo, &availableBufLen) == BUFREQ_OK )                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #30 Increment offset of buffer by copied data length. */
      SoAd_SetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx,                                                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        (SoAd_TpTxBufferIdxOfTpTxBufferConfigDynType)(SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx,
          PartitionIdx) + pduInfo.SduLength), PartitionIdx);

      /* #40 Check if data is still pending. */
      if ( SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) >
        (SoAd_SizeOfTpTxBufferType)(SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx, PartitionIdx) -
          SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx)) )
      {
        /* #50 Set maximum expected available upper layer buffer size to pending PDU data. */
        maxExpectedBufLen = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) -
          (SoAd_SizeOfTpTxBufferType)(SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx, PartitionIdx) -
            SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx)));

        /* #60 Overwrite available upper layer buffer size if it exceeds the maximum expected available buffe size. */
        if ( availableBufLen > maxExpectedBufLen )
        {
          availableBufLen = maxExpectedBufLen;
        }

        /* #70 Set pointer to TP transmission buffer and length for next CopyTxData call if buffer is available. */
        if ( availableBufLen > 0u )
        {
          pduInfo.SduDataPtr = SoAd_GetAddrTpTxBuffer(SoAd_GetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx,
            PartitionIdx), PartitionIdx);
          pduInfo.SduLength = availableBufLen;
        }
        /* #71 Indicate to stop transmission in this context and continue transmission in main function otherwise. */
        else
        {
          SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, SoConIdx,
            SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);

          continueTransmission = FALSE;
        }
      }

      retVal = E_OK;
    }
    /* #21 Handle case if call to CopyTxData failed otherwise. */
    else
    {
      /* #80 Terminate TP session. */
      SoAd_TxTp_UdpTerminateTransmit(SoConIdx, PartitionIdx, CalledByMain, E_NOT_OK);

      /* #90 Return that transmission has failed. */
      continueTransmission = FALSE;
    }

    /* #100 Stop transmission in this context if CopyTxData failed or no data for transmission is available. */
    if ( continueTransmission == FALSE )
    {
      break;
    }
  }

  return retVal;
} /* SoAd_TxTp_UdpCopyPdu() */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpTransmitOnSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTp_UdpTransmitOnSocket(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType            txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare transmission of TP-PDU over UDP. */
  SoAd_SetTxBufPtrOfTxMgt(txMgtIdx,                                                                                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_GetAddrTpTxBuffer(SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx),
      PartitionIdx), PartitionIdx);

  /* #20 Try to transmit PDU. */
  if ( SoAd_TxSocketManager_UdpTransmit(SoConIdx, PartitionIdx,
      (uint16)SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx)) == E_OK )
  {
    /* #30 Terminate TP session positively. */
    SoAd_TxTp_UdpTerminateTransmit(SoConIdx, PartitionIdx, CalledByMain, E_OK);

    retVal = E_OK;
  }
  /* #40 Terminate TP session negatively in case transmission did not succeed otherwise. */
  else
  {
    SoAd_TxTp_UdpTerminateTransmit(SoConIdx, PartitionIdx, CalledByMain, E_NOT_OK);
  }

  return retVal;
} /* SoAd_TxTp_UdpTransmitOnSocket() */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpTerminateTransmit()
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
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxTp_UdpTerminateTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean CalledByMain,
  Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType            txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType     pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType         pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Release TP transmission buffer. */
  SoAd_SetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx,                                                      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx), PartitionIdx);

  /* #20 Reset TP session. */
  SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                               /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

  SoAd_TxSocketManager_ResetTxActive(SoConIdx, PartitionIdx, FALSE);

  /* #30 Check if transmission succeeded. */
  if ( Result == E_OK )
  {
    /* #300 Call TpTxConfirmation of upper layer with positive result. */
    SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(               /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), E_OK);

    /* #301 Reconnect socket connection if socket connection has auto setup enabled and wildcards configured. */
    SoAd_SoCon_Reconnect(SoConIdx, PartitionIdx);
  }
  /* #31 Check if transmission context is main function otherwise. */
  else if ( CalledByMain == TRUE )
  {
    /* #310 Call TpTxConfirmation of upper layer with negative result if transmission context is main function. */
    SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(               /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), E_NOT_OK);
  }
  /* #32 No further action required otherwise. */
  else
  {
    /* Nothing to do. */
  }
} /* SoAd_TxTp_UdpTerminateTransmit() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_Init(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_TcpTxQueueDataIterType tcpTxQueueDataIter;
  SoAd_SizeOfTcpTxQueueType   tcpTxQueueIdx = SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Tx TCP queue is configured. */
  if ( SoAd_IsTcpTxQueueUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    /* #20 Initialize all queue elements. */
    for ( tcpTxQueueDataIter = SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx);
      tcpTxQueueDataIter < SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx);
      tcpTxQueueDataIter++ )
    {
      SoAd_SetPduRouteIdxOfTcpTxQueueData(tcpTxQueueDataIter, SoAd_GetSizeOfPduRoute(PartitionIdx), PartitionIdx);     /* SBSW_SOAD_CSL03_CSL05 */
      SoAd_SetLenOfTcpTxQueueData(tcpTxQueueDataIter, 0u, PartitionIdx);                                               /* SBSW_SOAD_CSL03_CSL05 */
    }

    /* #30 Initialize management struct. */
    SoAd_SetTcpTxQueueDataIdxOfTcpTxQueueMgt(tcpTxQueueIdx, SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx,  /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
      PartitionIdx), PartitionIdx);
    SoAd_SetLvlOfTcpTxQueueMgt(tcpTxQueueIdx, 0u, PartitionIdx);                                                       /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
    SoAd_SetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, 0u, PartitionIdx);                                                    /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
  }
} /* SoAd_TxTcpQueue_Init() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_Clear()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_Clear(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_TcpTxQueueDataIterType tcpTxQueueDataIter;
  SoAd_SizeOfTcpTxQueueType   tcpTxQueueIdx;
  SoAd_SizeOfPduRouteType     pduRouteIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that call is interrupted and Tx TCP Queue becomes inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if Tx TCP queue is configured. */
  if ( SoAd_IsTcpTxQueueUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    tcpTxQueueIdx = SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx);

    /* #30 Initialize all queue elements. */
    for ( tcpTxQueueDataIter = SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx);
      tcpTxQueueDataIter < SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx);
      tcpTxQueueDataIter++ )
    {
      pduRouteIdx = SoAd_GetPduRouteIdxOfTcpTxQueueData(tcpTxQueueDataIter, PartitionIdx);

      /* #300 Check if IF TxConfirmation is pending. */
      if ( (pduRouteIdx != SoAd_GetSizeOfPduRoute(PartitionIdx)) &&
        (SoAd_GetUpperLayerApiOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_UL_API_IF) )
      {
        /* #3000 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #3001 Call IF TxConfirmation. */
        SoAd_TxIf_TcpTxConfirmation(SoConIdx, pduRouteIdx, PartitionIdx, TRUE);

        /* #3002 Enter critical section again. */
        SOAD_BEGIN_CRITICAL_SECTION();
      }

      SoAd_SetPduRouteIdxOfTcpTxQueueData(tcpTxQueueDataIter, SoAd_GetSizeOfPduRoute(PartitionIdx), PartitionIdx);     /* SBSW_SOAD_CSL02_CSL05 */
      SoAd_SetLenOfTcpTxQueueData(tcpTxQueueDataIter, 0u, PartitionIdx);                                               /* SBSW_SOAD_CSL02_CSL05 */
    }

    /* #40 Initialize management struct. */
    SoAd_SetTcpTxQueueDataIdxOfTcpTxQueueMgt(tcpTxQueueIdx, SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx,  /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
      PartitionIdx), PartitionIdx);
    SoAd_SetLvlOfTcpTxQueueMgt(tcpTxQueueIdx, 0u, PartitionIdx);                                                       /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
    SoAd_SetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, 0u, PartitionIdx);                                                    /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_TxTcpQueue_Clear() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_ReserveNextElement()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTcpQueue_ReserveNextElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTcpTxQueueType     tcpTxQueueIdx = SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfTcpTxQueueDataType writeIdx = SoAd_GetTcpTxQueueDataIdxOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx);
  Std_ReturnType                retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that call is interrupted and Tx TCP queue becomes inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if TCP Tx queue has elements left. */
  /* PRQA S 4391 2 */ /* MD_SoAd_GenericWiderTypeCast */
  if ( SoAd_GetLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) <
    (SoAd_LvlOfTcpTxQueueMgtType)(SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx) -
      SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx)) )
  {
    /* #30 Get next element index if queue is filled. */
    if ( SoAd_GetLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) > 0u )
    {
      retVal = SoAd_TxTcpQueue_GetNextElement(SoConIdx, PartitionIdx, &writeIdx);                                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    }
    /* #31 Use current element index if queue is empty. */
    else
    {
      retVal = E_OK;
    }
  }

  /* #40 Reserve element if free element index was found. */
  if ( retVal == E_OK )
  {
    SoAd_IncLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetPduRouteIdxOfTcpTxQueueData(writeIdx, PduRouteIdx, PartitionIdx);                                          /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
    SoAd_SetLenOfTcpTxQueueData(writeIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxTcpQueue_ReserveNextElement() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_GetNextElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTcpQueue_GetNextElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SizeOfTcpTxQueueDataType, AUTOMATIC, SOAD_APPL_DATA) WriteIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfTcpTxQueueType     tcpTxQueueIdx = SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfTcpTxQueueDataType lastLvl;
  SoAd_SizeOfTcpTxQueueDataType lastWriteIdx;
  Std_ReturnType                retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get last write index considering queue wrap around. */
  /* PRQA S 4391 1 */ /* MD_SoAd_GenericWiderTypeCast */
  lastLvl = (SoAd_SizeOfTcpTxQueueDataType)(SoAd_GetLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) - 1u);

  lastWriteIdx = (SoAd_SizeOfTcpTxQueueDataType)SoAd_Util_GetNextArrayElement(SoAd_GetTcpTxQueueDataIdxOfTcpTxQueueMgt(
      tcpTxQueueIdx, PartitionIdx), lastLvl, SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx),
    SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx));

  /* #20 Check if last index is completely written and new entry is possible. */
  if ( SoAd_GetLenOfTcpTxQueueData(lastWriteIdx, PartitionIdx) != 0u )
  {
    /* #200 Get new write index considering queue wrap around. */
    *WriteIdx = (SoAd_SizeOfTcpTxQueueDataType)SoAd_Util_GetNextArrayElement(lastWriteIdx, 1u,                         /* SBSW_SOAD_POINTER_WRITE */
      SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx),
      SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx));

    retVal = E_OK;
  }

  return retVal;
} /* SoAd_TxTcpQueue_GetNextElement() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_SetElement()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxTcpQueue_SetElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType Len,
  boolean Release)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTcpTxQueueType     tcpTxQueueIdx = SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfTcpTxQueueDataType writeIdx;
  SoAd_LvlOfTcpTxQueueMgtType   lvl;
  Std_ReturnType                retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that call is interrupted and Tx TCP queue becomes inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if queue is filled by at least one reserved element. */
  lvl = SoAd_GetLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx);
  if ( lvl > 0u )
  {
    /* #30 Get last element index considering queue wrap around. */
    lvl--;

    writeIdx = (SoAd_SizeOfTcpTxQueueDataType)SoAd_Util_GetNextArrayElement(SoAd_GetTcpTxQueueDataIdxOfTcpTxQueueMgt(
        tcpTxQueueIdx, PartitionIdx), lvl, SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx),
      SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx));

    /* #40 Check if element is reserved. */
    if ( PduRouteIdx == SoAd_GetPduRouteIdxOfTcpTxQueueData(writeIdx, PartitionIdx) )
    {
      /* #400 Release element if required. */
      if ( Release == TRUE )
      {
        SoAd_SetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, SoAd_GetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
          SoAd_GetLenOfTcpTxQueueData(writeIdx, PartitionIdx), PartitionIdx);
        SoAd_SetLenOfTcpTxQueueData(writeIdx, 0u, PartitionIdx);                                                       /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
        SoAd_DecLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx);                                                       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetPduRouteIdxOfTcpTxQueueData(writeIdx, SoAd_GetSizeOfPduRoute(PartitionIdx), PartitionIdx);             /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
      }
      /* #401 Set element if required. */
      else
      {
        SoAd_SetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, SoAd_GetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) + Len, /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
          PartitionIdx);
        SoAd_SetLenOfTcpTxQueueData(writeIdx, Len, PartitionIdx);                                                      /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
      }

      retVal = E_OK;
    }
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxTcpQueue_SetElement() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_GetTotalLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(uint32, SOAD_CODE) SoAd_TxTcpQueue_GetTotalLength(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 totalLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return total length of Tx TCP queue. */
  totalLen = SoAd_GetTotLenOfTcpTxQueueMgt(SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx);

  return totalLen;
} /* SoAd_TxTcpQueue_GetTotalLength() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_CallTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_CallTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PDU route index is valid. */
  if ( PduRouteIdx < SoAd_GetSizeOfPduRoute(PartitionIdx) )
  {
    /* #20 Confirm PDU transmission for TP-PDU. */
    if ( SoAd_GetUpperLayerApiOfPduRoute(PduRouteIdx, PartitionIdx) == SOAD_UL_API_TP )
    {
      SoAd_TxTp_TcpTxConfirmation(SoConIdx, PduRouteIdx, PartitionIdx);
    }
    /* #30 Confirm PDU transmission for IF-PDU otherwise. */
    else
    {
      SoAd_TxIf_TcpTxConfirmation(SoConIdx, PduRouteIdx, PartitionIdx, FALSE);
    }
  }
} /* SoAd_TxTcpQueue_CallTxConfirmation() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_HandleTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_HandleTransmission(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                      elementIdx = 0u;
  uint32                      handleIdx = 0u;
  SoAd_SizeOfNPduUdpTxType    nPduIdx;
  SoAd_PartitionConfigIdxType partitionIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all timeouts in this main function cycle. */
  while ( SoAd_TimeoutList_CheckElements(SOAD_TIMEOUT_LIST_N_PDU, &elementIdx, &handleIdx, InstMapIdx) == E_OK )       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    nPduIdx = (SoAd_SizeOfNPduUdpTxType)handleIdx;
    partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);

    /* #20 Enter critical section to prevent that other transmit requests interrupt this transmission. */
    SOAD_BEGIN_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

    /* #30 Try to transmit nPdu. */
    SoAd_TxNPduUdp_Transmit(nPduIdx, partitionIdx);

    /* #40 Leave critical section. */
    SOAD_END_CRITICAL_SECTION_1(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
  }
} /* SoAd_TxNPduUdp_HandleTransmission() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_Transmit()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_Transmit(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if nPdu still contains PDUs. */
  if ( SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) > 0u )
  {
    /* #20 Start transmission if not started before. */
    if ( SoAd_GetTransmissionStateOfNPduUdpTxDyn(NPduIdx, PartitionIdx) == SOAD_NPDU_UDP_TX_STATE_NONE )
    {
      SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_ACTIVE, PartitionIdx);                   /* SBSW_SOAD_PARAMETER_IDX */

      /* #200 Set retry counter for nPdu queue. */
      if ( SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx(NPduIdx, PartitionIdx) )
      {
        SoAd_SetRetryCntOfNPduUdpTxDyn(NPduIdx, SOAD_N_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN, PartitionIdx);                 /* SBSW_SOAD_PARAMETER_IDX */
      }
    }

    /* #30 Handle transmission of nPdu. */
    if ( SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx(NPduIdx, PartitionIdx) )
    {
      /* #300 Handle transmission of nPdu queue. */
      SoAd_TxNPduUdp_QueueTransmit(NPduIdx, PartitionIdx);
    }
    else
    {
      /* #301 Handle transmission of nPdu buffer otherwise. */
      SoAd_TxNPduUdp_BufferSend(NPduIdx, PartitionIdx);
    }
  }
  /* #11 Remove timeout from list otherwise. */
  else
  {
    SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
    SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
    SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

    SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, instanceIdx, PartitionIdx);
  }
} /* SoAd_TxNPduUdp_Transmit() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_SetTriggerTimeout()
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
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_SetTriggerTimeout(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                    currentTimeoutCnt = 0u;
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType   instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType  socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PduRouteDest specific timeout is configured. */
  if ( SoAd_GetUdpTriggerTimeoutOfPduRouteDest(PduRouteDestIdx, PartitionIdx) > 0u  )
  {
    /* #20 Get timeout value if already running. */
    if ( SoAd_TimeoutList_GetCurrentTimeout(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, &currentTimeoutCnt,                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      SoAd_GetCounterOfInstanceDyn(instanceIdx, PartitionIdx), PartitionIdx) == E_OK )
    {
      /* #30 Update timeout if configured timeout is smaller than current timeout value. */
      if ( SoAd_GetUdpTriggerTimeoutOfPduRouteDest(PduRouteDestIdx, PartitionIdx) < currentTimeoutCnt )
      {
        (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, SoAd_GetUdpTriggerTimeoutOfPduRouteDest(
            PduRouteDestIdx, PartitionIdx), instanceIdx, PartitionIdx);
      }
    }
    /* #21 Set timeout otherwise. */
    else
    {
      retVal = SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, SoAd_GetUdpTriggerTimeoutOfPduRouteDest(
        PduRouteDestIdx, PartitionIdx), instanceIdx, PartitionIdx);
    }
  }
  /* #11 Use socket connection specific timeout otherwise. */
  else
  {
    /* #40 Get timeout value if already running. */
    if ( SoAd_TimeoutList_GetCurrentTimeout(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, &currentTimeoutCnt,                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      SoAd_GetCounterOfInstanceDyn(instanceIdx, PartitionIdx), PartitionIdx) == E_OK )
    {
      /* #50 Update timeout if configured timeout is smaller than current timeout value. */
      if ( SoAd_GetUdpTriggerTimeoutOfSocketUdp(socketUdpIdx, PartitionIdx) < currentTimeoutCnt )
      {
        (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, SoAd_GetUdpTriggerTimeoutOfSocketUdp(
            socketUdpIdx, PartitionIdx), instanceIdx, PartitionIdx);
      }
    }
    /* #41 Set timeout otherwise. */
    else
    {
      retVal = SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, SoAd_GetUdpTriggerTimeoutOfSocketUdp(
          socketUdpIdx, PartitionIdx), instanceIdx, PartitionIdx);
    }
  }

  return retVal;
} /* SoAd_TxNPduUdp_SetTriggerTimeout() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_Clear()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_Clear(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset management structs. */
  SoAd_SetTotalLenOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_PARAMETER_IDX */
  SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_NONE, PartitionIdx);                         /* SBSW_SOAD_PARAMETER_IDX */

  /* #20 Remove timeout from list. */
  SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, instanceIdx, PartitionIdx);

  /* #30 Clear nPdu queue. */
  if ( SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx(NPduIdx, PartitionIdx) )
  {
    SoAd_NPduUdpTxQueueIterType nPduUdpTxQueueIter;

    /* #300 Iterate over all queue elements. */
    for ( nPduUdpTxQueueIter = SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
      nPduUdpTxQueueIter < SoAd_GetNPduUdpTxQueueEndIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
      nPduUdpTxQueueIter++ )
    {
      SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter,
        PartitionIdx);

      /* #3000 Call TxConfirmation if element is set. */
      if ( pduRouteDestIdx != SoAd_GetSizeOfPduRouteDest(PartitionIdx) )
      {
        SoAd_TxNPduUdp_QueueTxConfirmation(soConIdx, SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx),
          PartitionIdx, FALSE);
      }

      /* #3001 Reset element struct. */
      SoAd_SetNPduUdpTxBufferIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter, 0u, PartitionIdx);                                /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
      SoAd_SetCopiedOfNPduUdpTxQueue(nPduUdpTxQueueIter, FALSE, PartitionIdx);                                         /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
      SoAd_SetLenOfNPduUdpTxQueue(nPduUdpTxQueueIter, 0u, PartitionIdx);                                               /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
      SoAd_SetPduRouteDestIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter, SoAd_GetSizeOfPduRouteDest(PartitionIdx),            /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
        PartitionIdx);
    }

    /* #301 Clear nPdu fill level. */
    SoAd_SetLvlOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                              /* SBSW_SOAD_PARAMETER_IDX */
  }

  /* #40 Clear nPdu buffer. */
  if ( SoAd_IsNPduUdpTxBufferUsedOfNPduUdpTx(NPduIdx, PartitionIdx) )
  {
    SoAd_SetBufLenOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_PARAMETER_IDX */
  }

  /* #50 Release IP fragmentation buffer. */
  SoAd_TxIpFragBuf_Release(soConIdx, PartitionIdx);
} /* SoAd_TxNPduUdp_Clear() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueAddOrUpdateElement()
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
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueAddOrUpdateElement(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  PduLengthType             dataLen = PduInfoPtr->SduLength;
  SoAd_SizeOfNPduUdpTxType  nPduIdx = SoAd_GetNPduUdpTxIdxOfSoCon(soConIdx, PartitionIdx);
  boolean                   elementUpdated = FALSE;
  SoAd_NPduReturnType       nPduRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to write nPdu atomic. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Calculate PDU length considering PDU header. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx), PartitionIdx) )
  {
    dataLen += SOAD_PDU_HDR_SIZE;
  }

  /* #30 Check and update if PDU is already in queue. */
  nPduRetVal = SoAd_TxNPduUdp_QueueUpdateElement(nPduIdx, PduRouteDestIdx, PartitionIdx, dataLen, &elementUpdated);    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* #40 Try to add element if PDU is not updated. */
  if ( (nPduRetVal != SOAD_NPDU_NOT_OK ) && (elementUpdated == FALSE) )
  {
    /* #50 Try to add element if nPdu transmission is not active. */
    if ( SoAd_GetTransmissionStateOfNPduUdpTxDyn(nPduIdx, PartitionIdx) == SOAD_NPDU_UDP_TX_STATE_NONE )
    {
      nPduRetVal = SoAd_TxNPduUdp_QueueAddElement(nPduIdx, PduRouteDestIdx, PartitionIdx, dataLen, PduInfoPtr);        /* SBSW_SOAD_POINTER_FORWARD */
    }
    /* #51 Indicate overflow of nPdu otherwise. */
    else
    {
      nPduRetVal = SOAD_NPDU_OVFL_NEW;
    }
  }

  /* #60 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return nPduRetVal;
} /* SoAd_TxNPduUdp_QueueAddOrUpdateElement() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueUpdateElement()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueUpdateElement(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType DataLen,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_VAR) ElementUpdated)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSocketType   sockIdx = SoAd_GetSocketIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_NPduReturnType     nPduRetVal = SOAD_NPDU_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if trigger transmit is enabled so that element update is possible. */
  if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx),
      PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
  {
    SoAd_NPduUdpTxQueueIterType nPduUdpTxQueueIter;

    /* #20 Iterate over all elements. */
    for ( nPduUdpTxQueueIter = SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
      nPduUdpTxQueueIter <
      (SoAd_NPduUdpTxQueueIterType) /* PRQA S 4391 */ /* MD_SoAd_GenericWiderTypeCast */
      (SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx) + SoAd_GetLvlOfNPduUdpTxDyn(NPduIdx,
        PartitionIdx)); nPduUdpTxQueueIter++ )
    {
      /* #30 Check if element contains the requested PDU. */
      if ( SoAd_GetPduRouteDestIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx) == PduRouteDestIdx )
      {
        /* #40 Check if no transmission is active on socket. */
        if ( SoAd_TxSocketManager_IsTxActiveOnSocket(sockIdx, PartitionIdx) == FALSE )
        {
          /* #50 Update element. */
          uint32 totalLen = SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) -
            SoAd_GetLenOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx) + DataLen;
          SoAd_SetLenOfNPduUdpTxQueue(nPduUdpTxQueueIter, DataLen, PartitionIdx);                                      /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
          SoAd_SetTotalLenOfNPduUdpTxDyn(NPduIdx, totalLen, PartitionIdx);                                             /* SBSW_SOAD_PARAMETER_IDX */
          *ElementUpdated = TRUE;                                                                                      /* SBSW_SOAD_POINTER_WRITE */

          /* #60 Check and indicate if update causes nPdu size overflow. */
          if ( SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) >
            SoAd_GetNPduUdpTxBufferMinOfSocketUdp(
              SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
          {
            nPduRetVal = SOAD_NPDU_OVFL_UPDATE;
          }
        }
        /* #41 Indicate that update is not possible otherwise. */
        else
        {
          nPduRetVal = SOAD_NPDU_NOT_OK;
        }

        break;
      }
    }
  }

  return nPduRetVal;
} /* SoAd_TxNPduUdp_QueueUpdateElement() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueAddElement()
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
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueAddElement(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType DataLen,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  SoAd_NPduReturnType     nPduRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if queue size is sufficient. */
  if ( SoAd_TxNPduUdp_QueueCheckSize(NPduIdx, PartitionIdx, DataLen) == SOAD_NPDU_OK )
  {
    /* #20 Check if timeout can be set if configured. */
    if ( SoAd_TxNPduUdp_SetTriggerTimeout(NPduIdx, PduRouteDestIdx, PartitionIdx) == E_OK )
    {
      SoAd_SizeOfNPduUdpTxQueueType currentIdx = (SoAd_SizeOfNPduUdpTxQueueType)
        (SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx) +
          SoAd_GetLvlOfNPduUdpTxDyn(NPduIdx, PartitionIdx));

      /* #30 Store element in queue. */
      SoAd_SetCopiedOfNPduUdpTxQueue(currentIdx, FALSE, PartitionIdx);                                                 /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
      SoAd_SetLenOfNPduUdpTxQueue(currentIdx, DataLen, PartitionIdx);                                                  /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
      SoAd_SetPduRouteDestIdxOfNPduUdpTxQueue(currentIdx, PduRouteDestIdx, PartitionIdx);                              /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
      SoAd_IncLvlOfNPduUdpTxDyn(NPduIdx, PartitionIdx);                                                                /* SBSW_SOAD_PARAMETER_IDX */

      /* #40 Store PDU in buffer if trigger transmit is not used. */
      if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) != SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
      {
        SoAd_SetNPduUdpTxBufferIdxOfNPduUdpTxQueue(currentIdx, (SoAd_NPduUdpTxBufferIdxOfNPduUdpTxQueueType)           /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
          (SoAd_GetNPduUdpTxBufferStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx) +
            SoAd_GetBufLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx)), PartitionIdx);

        (void)SoAd_TxNPduUdp_BufferAddElement(pduRouteIdx, PduRouteDestIdx, PartitionIdx, PduInfoPtr);                 /* SBSW_SOAD_POINTER_FORWARD */
      }
      /* #41 Store PDU length only and indicate to call TxConfirmation in main function otherwise. */
      else
      {
        SoAd_SetTotalLenOfNPduUdpTxDyn(NPduIdx, SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) + DataLen,       /* SBSW_SOAD_PARAMETER_IDX */
          PartitionIdx);

        SoAd_TxIf_AddPendingConf(pduRouteIdx, soConIdx, PartitionIdx);
      }

      nPduRetVal = SOAD_NPDU_OK;
    }
    /* #21 Indicate an error otherwise. */
    else
    {
      nPduRetVal = SOAD_NPDU_NOT_OK;
    }
  }
  /* #11 Indicate overflow otherwise. */
  else
  {
    nPduRetVal = SOAD_NPDU_OVFL_NEW;
  }

  return nPduRetVal;
} /* SoAd_TxNPduUdp_QueueAddElement() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCheckSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCheckSize(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType  socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_NPduReturnType       nPduRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if new element exceeds the configured queue size. */
  if ( SoAd_GetLvlOfNPduUdpTxDyn(NPduIdx, PartitionIdx) == SoAd_GetNPduUdpTxQueueSizeOfSocketUdp(socketUdpIdx,
      PartitionIdx) )
  {
    nPduRetVal = SOAD_NPDU_OVFL_NEW;
  }
  /* #20 Check if new element exceeds nPdu size otherwise. */
  else if ( (SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) + DataLen) >
    (SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx)) )
  {
    nPduRetVal = SOAD_NPDU_OVFL_NEW;
  }
  /* #30 Indicate that nPdu size is sufficient to store new PDU otherwise. */
  else
  {
    nPduRetVal = SOAD_NPDU_OK;
  }

  return nPduRetVal;
} /* SoAd_TxNPduUdp_QueueCheckSize() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTransmit()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueTransmit(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_TX_DYN_LEN == STD_ON )
  uint8_least retryCnt;
  boolean     stopRetry = FALSE;
#endif /* SOAD_TX_DYN_LEN == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TX_DYN_LEN == STD_ON )
  /* #10 Handle nPdu transmission if CopyTxData with dynamic length is enabled. */

  /* #100 Retry if nPdu becomes larger on trigger transmit. */
  for ( retryCnt = 0u; retryCnt < SOAD_N_PDU_UDP_TX_QUEUE_RETRY_CNT_DIRECT; retryCnt++ )
  {
    /* #1000 Prepare transmission of nPdu. */
    if ( SoAd_TxNPduUdp_QueuePrepareTransmitSegment(NPduIdx, PartitionIdx) != E_OK )
    {
      stopRetry = TRUE;
    }
    /* 1001 Transmit nPdu. */
    else if ( SoAd_TxNPduUdp_QueueTransmitSegment(NPduIdx, PartitionIdx) != E_OK )
    {
      stopRetry = TRUE;
    }
    /* #1002 Check if transmission of nPdu is finished. */
    else if ( SoAd_GetTransmissionStateOfNPduUdpTxDyn(NPduIdx, PartitionIdx) == SOAD_NPDU_UDP_TX_STATE_NONE )
    {
      stopRetry = TRUE;
    }
    /* #1003 Retry otherwise. */
    else
    {
      /* Nothing to do. */
    }

    /* #1004 Stop retry if required. */
    if ( stopRetry == TRUE )
    {
      break;
    }
  }
#else
  /* #20 Handle nPdu transmission if CopyTxData with dynamic length is disabled. */

  /* #200 Prepare transmission of nPdu. */
  if ( SoAd_TxNPduUdp_QueuePrepareTransmitSegment(NPduIdx, PartitionIdx) == E_OK )
  {
    /* #201 Transmit nPdu. */
    (void)SoAd_TxNPduUdp_QueueTransmitSegment(NPduIdx, PartitionIdx);
  }
#endif /* SOAD_TX_DYN_LEN == STD_ON */
} /* SoAd_TxNPduUdp_QueueTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueuePrepareTransmitSegment()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueuePrepareTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that another transmit interrupts this transmission. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check that socket connection is online and not requested to be closed. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  if ( (SoAd_SoCon_GetMode(soConIdx, PartitionIdx) == SOAD_SOCON_ONLINE) &&
    (SoAd_SoCon_GetCloseMode(soConIdx, PartitionIdx) == SOAD_CLOSE_NONE) )
  {
    /* #200 Check that no transmission is active and try to set transmission active. */
    if ( SoAd_TxSocketManager_CheckAndSetTxActive(soConIdx, PartitionIdx, TRUE) == E_OK )
    {
      /* #2000 Check and set transmission state for this nPdu. */
      if ( SoAd_TxNPduUdp_QueueCheckAndSetTransmitState(NPduIdx, PartitionIdx) == E_OK )
      {
        /* #20000 Prepare transmission with buffer if IP fragmentation is used. */
        if ( SoAd_IsIpFragMgtUsedOfSoCon(soConIdx, PartitionIdx) )
        {
          /* #200000 Leave critical section to copy data to IP fragmentation buffer. */
          SOAD_END_CRITICAL_SECTION();

          /* #200001 Check if PDUs can be copied to a IP fragmentation buffer. */
          if ( SoAd_TxNPduUdp_QueuePrepareTransmitSegmentFragment(NPduIdx, PartitionIdx) == E_OK )
          {
            retVal = E_OK;
          }
          /* #200002 Set timeout to retry in main function and deactivate transmission on socket connection and socket
           *         otherwise. */
          else
          {
            (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, 1u, instanceIdx, PartitionIdx);
            SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);
          }

          /* #200003 Enter critical section again. */
          SOAD_BEGIN_CRITICAL_SECTION();
        }
        /* #20001 Prepare direct transmission otherwise. */
        else
        {
          SoAd_TxNPduUdp_QueuePrepareTransmitSegmentDirect(NPduIdx, PartitionIdx);
          retVal = E_OK;
        }
      }
      /* #2001 Flush nPdu and reset Tx active state otherwise. */
      else
      {
        SoAd_TxNPduUdp_Clear(NPduIdx, PartitionIdx);
        SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);
      }
    }
  }
  /* #21 Set timeout to retry in main function otherwise. */
  else
  {
    (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, 1u, instanceIdx, PartitionIdx);
  }

  /* #30 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxNPduUdp_QueuePrepareTransmitSegment() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueuePrepareTransmitSegmentFragment()
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
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueuePrepareTransmitSegmentFragment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2VAR(uint8)                     ipFragBufPtr = NULL_PTR;
  uint16                                ipFragBufSize = 0u;
  SoAd_SizeOfPduRouteDestBySoConIndType pduRouteDestBySoConIndStartIdx;
  SoAd_SizeOfPduRouteDestType           pduRouteDestIdx;
  SoAd_SizeOfSoConType                  soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType                  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType               soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType              socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType                        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if nPdu segment is pending in IP fragmentation buffer (i.e. retry of previous segment). */
  if ( SoAd_TxIpFragBuf_Get(soConIdx, PartitionIdx, &ipFragBufSize, &ipFragBufPtr) == E_OK )                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #20 Mark to prepare transmission with already reserved IP fragmentation buffer. */
    retVal = E_OK;
  }
  /* #11 Try to use a new IP fragmentation otherwise. */
  else
  {
    /* #30 Calculate the required IP fragmentation buffer size depending on the current nPdu size. */
    uint32 totalLen = SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx);

    if ( totalLen > SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx) )
    {
      ipFragBufSize = SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow(NPduIdx, PartitionIdx);
    }
    else
    {
      ipFragBufSize = (uint16)totalLen;
    }

     /* #40 Try to reserve an IP fragmentation buffer. */
    if ( SoAd_TxIpFragBuf_Reserve(soConIdx, PartitionIdx, ipFragBufSize, &ipFragBufPtr) == E_OK )                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #50 Try to copy nPdu to fragmentation buffer. */
      if ( SoAd_TxNPduUdp_QueueCopyToBuffer(NPduIdx, PartitionIdx, ipFragBufPtr, &ipFragBufSize) == BUFREQ_OK )        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        /* #60 Set the used IP fragmentation buffer size depending on copied nPdu segment size. */
        SoAd_TxIpFragBuf_Set(soConIdx, PartitionIdx, ipFragBufSize);

        /* #70 Mark to prepare transmission with newly reserved IP fragmentation buffer. */
        retVal = E_OK;
      }
      /* #51 Release IP fragmentation buffer otherwise. */
      else
      {
        SoAd_TxIpFragBuf_Release(soConIdx, PartitionIdx);
      }
    }
    /* #41 Do not set DET error otherwise since nPdu retry is used. */
    else
    {
      /* Nothing to do. */
    }
  }

  /* #80 Prepare transmission for CopyTxData call if required. */
  if ( retVal == E_OK )
  {
    /* Set first PduRouteDest of socket connection as dummy identifier (used to identify API type in CopyTxData). */
    pduRouteDestBySoConIndStartIdx = SoAd_GetPduRouteDestBySoConIndStartIdxOfSoCon(soConIdx, PartitionIdx);
    pduRouteDestIdx = SoAd_GetPduRouteDestBySoConInd(pduRouteDestBySoConIndStartIdx, PartitionIdx);

    SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, pduRouteDestIdx, PartitionIdx);                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)ipFragBufSize, PartitionIdx);                               /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, (PduLengthType)ipFragBufSize, PartitionIdx);                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

    SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, ipFragBufPtr, PartitionIdx);                                                     /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueuePrepareTransmitSegmentFragment() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueuePrepareTransmitSegmentDirect()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueuePrepareTransmitSegmentDirect(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestBySoConIndType pduRouteDestBySoConIndStartIdx;
  SoAd_SizeOfPduRouteDestType           pduRouteDestIdx;
  SoAd_SizeOfSoConType                  soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType                  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType               soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType              socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set first PduRouteDest of socket connection as dummy identifier (used to identify API type in CopyTxData). */
  pduRouteDestBySoConIndStartIdx = SoAd_GetPduRouteDestBySoConIndStartIdxOfSoCon(soConIdx, PartitionIdx);
  pduRouteDestIdx = SoAd_GetPduRouteDestBySoConInd(pduRouteDestBySoConIndStartIdx, PartitionIdx);

  SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, pduRouteDestIdx, PartitionIdx);                                             /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

  /* #20 Check if the current nPdu size exceeds the configured nPdu size. */
  if ( SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) > SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx,
    PartitionIdx) )
  {
#if ( SOAD_TX_DYN_LEN == STD_ON )
    /* #200 Set the data length to the configured nPdu size (if CopyTxData with dynamic length is enabled). */
    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx),         /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      PartitionIdx);
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx),             /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      PartitionIdx);
#else
    /* #201 Set the data length to the size of nPdu up to overflow (if CopyTxData with dynamic length is disabled). */
    uint16 overflowNPduLength = SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow(NPduIdx, PartitionIdx);

    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)overflowNPduLength, PartitionIdx);                          /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, (PduLengthType)overflowNPduLength, PartitionIdx);                              /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
#endif /* SOAD_TX_DYN_LEN == STD_ON */
  }
  /* #21 Set the data length to the current nPdu size otherwise. */
  else
  {
    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx),      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      PartitionIdx);
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, (PduLengthType)SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx),          /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      PartitionIdx);
  }

  /* #30 Set dummy data pointer since data is retrieved later in CopyTxData. */
  SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, NULL_PTR, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
} /* SoAd_TxNPduUdp_QueuePrepareTransmitSegmentDirect() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCheckAndSetTransmitState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCheckAndSetTransmitState(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission of nPdu has already been started. */
  if ( SoAd_GetTransmissionStateOfNPduUdpTxDyn(NPduIdx, PartitionIdx) != SOAD_NPDU_UDP_TX_STATE_NONE )
  {
    /* #20 Check if retry has failed. */
    if ( SoAd_TxNPduUdp_QueueCheckRetry(NPduIdx, PartitionIdx) != E_OK )
    {
      retVal = E_NOT_OK;
    }
  }
  /* #11 Set transmission to active otherwise. */
  else
  {
    SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_ACTIVE, PartitionIdx);                     /* SBSW_SOAD_PARAMETER_IDX */
    SoAd_SetRetryCntOfNPduUdpTxDyn(NPduIdx, SOAD_N_PDU_UDP_TX_QUEUE_RETRY_CNT_MAIN, PartitionIdx);                     /* SBSW_SOAD_PARAMETER_IDX */
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCheckAndSetTransmitState() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCheckRetry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCheckRetry(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if retry is running. */
  if ( SoAd_GetRetryCntOfNPduUdpTxDyn(NPduIdx, PartitionIdx) > 0u )
  {
    /* #20 Decrement retry counter. */
    SoAd_DecRetryCntOfNPduUdpTxDyn(NPduIdx, PartitionIdx);                                                             /* SBSW_SOAD_PARAMETER_IDX */

    /* #30 Return that retry failed if retry counter is zero. */
    if ( SoAd_GetRetryCntOfNPduUdpTxDyn(NPduIdx, PartitionIdx) == 0u )
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCheckRetry() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(uint16, SOAD_CODE) SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_NPduUdpTxQueueIterType nPduUdpTxQueueIter;
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType     soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType    socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  uint16                      nPduLengthUpToOverflow = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements (without last element) to sum up all PDUs until overflow occurs. */
  for ( nPduUdpTxQueueIter = SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
    nPduUdpTxQueueIter <
      (SoAd_NPduUdpTxQueueIterType) /* PRQA S 4391 */ /* MD_SoAd_GenericWiderTypeCast */
        (SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx) + SoAd_GetLvlOfNPduUdpTxDyn(NPduIdx,
          PartitionIdx) - 1u);
    nPduUdpTxQueueIter++ )
  {
    /* #20 Check if queue element exceeds the nPdu size. */
    if ( (nPduLengthUpToOverflow + SoAd_GetLenOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx)) >
      SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx) )
    {
      /* #30 Stop iterating to return the length up to this queue element. */
      break;
    }
    else
    {
      /* #40 Add queue element length to the total length of queue elements until overflow occurs. */
      nPduLengthUpToOverflow += (uint16)SoAd_GetLenOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx);
    }
  }

  return nPduLengthUpToOverflow;
} /* SoAd_TxNPduUdp_QueueGetNPduLengthUpToOverflow() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTransmitSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean        retryTransmit = FALSE;
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to transmit nPdu segment and handle failure. */
  if ( SoAd_TxNPduUdp_QueueTryTransmitSegment(NPduIdx, PartitionIdx, &retryTransmit) != E_OK )                         /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
    SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
    SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
    /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
    SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);

    /* #20 Reset transmission management struct. */
    SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

    /* #30 Deactivate transmission on socket connection and socket. */
    SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);

    /* #40 Set timeout to retry transmission in main function if required. */
# if ( SOAD_TX_DYN_LEN == STD_OFF )
    if ( retryTransmit == TRUE )
# endif /* SOAD_TX_DYN_LEN == STD_OFF */
    {
      (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, 1u, instanceIdx, PartitionIdx);
    }

    retVal = E_NOT_OK;
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueTransmitSegment() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTryTransmitSegment()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueTryTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SOAD_P2VAR(boolean) RetryTransmit)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Transmit nPdu and check if transmission succeeds. */
  if ( SoAd_TxSocketManager_UdpTransmit(soConIdx, PartitionIdx,
      (uint16)SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx)) == E_OK )
  {
    /* #20 Finish transmission of nPdu elements. */
    SoAd_TxNPduUdp_QueueFinishTransmitSegment(NPduIdx, PartitionIdx);

    /* #30 Release IP fragmentation buffer. */
    SoAd_TxIpFragBuf_Release(soConIdx, PartitionIdx);

    /* #40 Reset transmission management struct. */
    SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

    /* #50 Deactivate transmission on socket connection and socket. */
    SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);

    /* #60 Finish transmission of nPdu if sent completely. */
    if ( SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) == 0u )
    {
      /* #600 Release queue. */
      SoAd_SetLvlOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                            /* SBSW_SOAD_PARAMETER_IDX */

      /* #601 Release buffer. */
      if ( SoAd_IsNPduUdpTxBufferUsedOfNPduUdpTx(NPduIdx, PartitionIdx) )
      {
        SoAd_SetBufLenOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                       /* SBSW_SOAD_PARAMETER_IDX */
      }

      /* #602 Remove timeout from list. */
      SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, instanceIdx, PartitionIdx);

      /* #603 Reset transmission state of nPdu. */
      SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_NONE, PartitionIdx);                     /* SBSW_SOAD_PARAMETER_IDX */
    }
    /* #61 Set timeout to retry transmission of remaining data in main function otherwise. */
    else
    {
      (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, 1u, instanceIdx, PartitionIdx);
    }

    retVal = E_OK;
  }
  /* #11 Handle failure of transmission otherwise. */
  else
  {
# if ( SOAD_TX_DYN_LEN == STD_OFF )
    SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(soConIdx, PartitionIdx);
    SoAd_SocketIdType     socketId = SoAd_GetSocketIdOfSocketDyn(sockIdx, PartitionIdx);

    /* #110 Clear nPdu if SoAd could not copy a PDU to the provided buffer but dynamic length is disabled. */
    if ( (SoAd_GetTransmissionStateOfNPduUdpTxDyn(NPduIdx, PartitionIdx) == SOAD_NPDU_UDP_TX_STATE_FAILED)
      && (socketId != SOAD_INV_SOCKET_ID) )
    {
      SoAd_TxNPduUdp_Clear(NPduIdx, PartitionIdx);
    }
    /* #111 Indicate that transmission shall be retried otherwise. */
    else
# endif /* SOAD_TX_DYN_LEN == STD_OFF */
    {
      *RetryTransmit = TRUE;                                                                                           /* SBSW_SOAD_POINTER_WRITE */
    }
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueTryTransmitSegment() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueFinishTransmitSegment()
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
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueFinishTransmitSegment(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_NPduUdpTxQueueIterType nPduUdpTxQueueIter;
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx;
  SoAd_SizeOfPduRouteType     pduRouteIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements. */
  for ( nPduUdpTxQueueIter = SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
    nPduUdpTxQueueIter <
    (SoAd_NPduUdpTxQueueIterType)(SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx) + /* PRQA S 4391 */ /* MD_SoAd_GenericWiderTypeCast */
      SoAd_GetLvlOfNPduUdpTxDyn(NPduIdx, PartitionIdx));
    nPduUdpTxQueueIter++ )
  {
    pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx);

    /* #20 Check if element is not yet released. */
    if ( pduRouteDestIdx != SoAd_GetSizeOfPduRouteDest(PartitionIdx) )
    {
      pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);

      /* #30 Check if element is copied and sent. */
      if ( SoAd_IsCopiedOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx) )
      {
        /* #40 Call TxConfirmation if PDU is retrieved via trigger transmit. */
        if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) ==
          SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
        {
          SoAd_TxNPduUdp_QueueTxConfirmation(soConIdx, pduRouteIdx, PartitionIdx, TRUE);
        }

        /* #50 Enter critical section to prevent that nPdu is updated inconsistently. */
        SOAD_BEGIN_CRITICAL_SECTION();

        /* #60 Release queue element. */
        SoAd_SetTotalLenOfNPduUdpTxDyn(NPduIdx,                                                                        /* SBSW_SOAD_PARAMETER_IDX */
          SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) - SoAd_GetLenOfNPduUdpTxQueue(nPduUdpTxQueueIter,
            PartitionIdx), PartitionIdx);
        SoAd_SetPduRouteDestIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter, SoAd_GetSizeOfPduRouteDest(PartitionIdx),          /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
          PartitionIdx);

        /* #70 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();
      }
      /* #31 Stop releasing elements otherwise. */
      else
      {
        break;
      }
    }
  }
} /* SoAd_TxNPduUdp_QueueFinishTransmitSegment() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueTxConfirmation()
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
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean AllowReconnect)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TxConfirmation is enabled for this PDU. */
  if ( SoAd_IsTxConfEnabledOfPduRoute(PduRouteIdx, PartitionIdx) )
  {
    /* #20 Enter critical section to prevent that other confirmation interrupts and TxConfirmation is called twice. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Check if a pending TxConfirmation is left. */
    if ( SoAd_GetPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx) > 0u )
    {
      /* #40 Decrement pending confirmation counter. */
      SoAd_DecPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx);                                                /* SBSW_SOAD_PARAMETER_IDX */

      /* #50 Check if no confirmation is pending (i.e. transmission confirmed on all PduRouteDests). */
      if ( (SoAd_GetPendingTxConfNumOfPduRouteDyn(PduRouteIdx, PartitionIdx) == 0u) &&
        !SoAd_IsPendingTxConfMainOfPduRouteDyn(PduRouteIdx, PartitionIdx) )
      {
        /* #60 Check if socket connection reconnect is allowed. */
        if ( AllowReconnect == TRUE )
        {
          /* #600 Reconnect socket connection if socket connection has auto setup enabled and wildcards configured. */
          SoAd_SoCon_RequestReconnect(SoConIdx, PartitionIdx);
        }

        /* #70 Leave critical section to be able to call TxConfirmation of upper layer. */
        SOAD_END_CRITICAL_SECTION();

        /* #80 Call TxConfirmation of upper layer. */
        SoAd_GetIfTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(           /* SBSW_SOAD_FUNCTION_PTR */
          SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx));
      }
      /* #51 Leave critical section otherwise. */
      else
      {
        SOAD_END_CRITICAL_SECTION();
      }
    }
    /* #31 Leave critical section otherwise. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }
  }
} /* SoAd_TxNPduUdp_QueueTxConfirmation() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_BufferAddElement()
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
SOAD_LOCAL FUNC(SoAd_NPduReturnType, SOAD_CODE) SoAd_TxNPduUdp_BufferAddElement(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType             dataLen = PduInfoPtr->SduLength;
  SoAd_SizeOfSoConType      soConIdx = SoAd_GetSoConIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfNPduUdpTxType  nPduIdx = SoAd_GetNPduUdpTxIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType  socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_NPduReturnType       nPduRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that another transmit interrupts this transmission. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check the socket connection states and if nPdu transmission is not active. */
  if ( (SoAd_SoCon_CheckConnectionStates(soConIdx, PartitionIdx) == E_OK) &&
    (SoAd_GetTransmissionStateOfNPduUdpTxDyn(nPduIdx, PartitionIdx) == SOAD_NPDU_UDP_TX_STATE_NONE) )
  {
    /* #30 Calculate PDU length considering PDU header. */
    if ( SoAd_IsPduHdrEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      dataLen += SOAD_PDU_HDR_SIZE;
    }

    /* #40 Check if nPdu size is not sufficient for new request. */
    if ( (SoAd_GetTotalLenOfNPduUdpTxDyn(nPduIdx, PartitionIdx) + dataLen) >
      SoAd_GetNPduUdpTxBufferMinOfSocketUdp(socketUdpIdx, PartitionIdx) )
    {
      nPduRetVal = SOAD_NPDU_OVFL_NEW;
    }
    /* #41 Check if timeout cannot be set if configured otherwise. */
    else if ( SoAd_TxNPduUdp_SetTriggerTimeout(nPduIdx, PduRouteDestIdx, PartitionIdx) != E_OK )
    {
      nPduRetVal = SOAD_NPDU_NOT_OK;
    }
    /* #42 Store PDU in nPdu buffer otherwise. */
    else
    {
      /* #420 Copy PDU header to nPdu buffer. */
      if ( SoAd_IsPduHdrEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
      {
        SoAd_Util_CopyPduHdr2Buf(                                                                                      /* SBSW_SOAD_POINTER_WITH_OFFSET_FORWARD_NPduUdpTxBuffer */
          SoAd_GetTxPduHdrIdOfPduRouteDest(PduRouteDestIdx, PartitionIdx),
          PduInfoPtr->SduLength,
          SoAd_GetAddrNPduUdpTxBuffer(SoAd_GetNPduUdpTxBufferStartIdxOfNPduUdpTx(nPduIdx, PartitionIdx) +
            SoAd_GetBufLenOfNPduUdpTxDyn(nPduIdx, PartitionIdx), PartitionIdx),
          0u,
          SOAD_PDU_HDR_SIZE);
        SoAd_SetBufLenOfNPduUdpTxDyn(nPduIdx, SoAd_GetBufLenOfNPduUdpTxDyn(nPduIdx, PartitionIdx) + SOAD_PDU_HDR_SIZE, /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
          PartitionIdx);
        SoAd_SetTotalLenOfNPduUdpTxDyn(nPduIdx, SoAd_GetTotalLenOfNPduUdpTxDyn(nPduIdx, PartitionIdx) +                /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
          SOAD_PDU_HDR_SIZE, PartitionIdx);
      }

      /* #421 Copy PDU data to nPdu buffer. */
      /* PRQA S 314 4 */ /* MD_MSR_VStdLibCopy */
      VStdMemCpy(                                                                                                      /* SBSW_SOAD_CONST_POINTER_AND_POINTER_WITH_OFFSET_FORWARD_NPduUdpTxBuffer */
        SOAD_A_P2VAR(void)SoAd_GetAddrNPduUdpTxBuffer(
          SoAd_GetNPduUdpTxBufferStartIdxOfNPduUdpTx(nPduIdx, PartitionIdx) + SoAd_GetBufLenOfNPduUdpTxDyn(nPduIdx,
            PartitionIdx), PartitionIdx), SOAD_A_P2CONST(void)&PduInfoPtr->SduDataPtr[0],
        PduInfoPtr->SduLength);
      SoAd_SetBufLenOfNPduUdpTxDyn(nPduIdx, (SoAd_BufLenOfNPduUdpTxDynType)(SoAd_GetBufLenOfNPduUdpTxDyn(nPduIdx,      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
            PartitionIdx) + PduInfoPtr->SduLength), PartitionIdx);
      SoAd_SetTotalLenOfNPduUdpTxDyn(nPduIdx, SoAd_GetTotalLenOfNPduUdpTxDyn(nPduIdx, PartitionIdx) +                  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        PduInfoPtr->SduLength, PartitionIdx);

      /* #422 Indicate that a TxConfirmation is pending for the copied PDU. */
      SoAd_TxIf_AddPendingConf(PduRouteIdx, soConIdx, PartitionIdx);

      nPduRetVal = SOAD_NPDU_OK;
    }
  }
  else
  {
    nPduRetVal = SOAD_NPDU_NOT_OK;
  }

  /* #70 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return nPduRetVal;
} /* SoAd_TxNPduUdp_BufferAddElement() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_BufferSend()
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
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_BufferSend(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestBySoConIndType pduRouteDestBySoConIndStartIdx;
  SoAd_SizeOfPduRouteDestType           pduRouteDestIdx;
  SoAd_SizeOfSoConType                  soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType               soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType               instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType                  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that another transmit interrupts this transmission. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if nPdu still contains PDUs. */
  if ( SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx) > 0u )
  {
    /* #30 Check that socket connection is online and not requested to be closed. */
    /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
    if ( (SoAd_SoCon_GetMode(soConIdx, PartitionIdx) == SOAD_SOCON_ONLINE) &&
      (SoAd_SoCon_GetCloseMode(soConIdx, PartitionIdx) == SOAD_CLOSE_NONE) )
    {
      /* #40 Check that no transmission is active and try to set transmission active. */
      if ( SoAd_TxSocketManager_CheckAndSetTxActive(soConIdx, PartitionIdx, TRUE) == E_OK )
      {
        uint32 totalLen = SoAd_GetTotalLenOfNPduUdpTxDyn(NPduIdx, PartitionIdx);

        /* Use first PduRouteDest of SoCon as dummy identifier (used to identify API type in CopyTxData). */
        pduRouteDestBySoConIndStartIdx = SoAd_GetPduRouteDestBySoConIndStartIdxOfSoCon(soConIdx, PartitionIdx);
        pduRouteDestIdx = SoAd_GetPduRouteDestBySoConInd(pduRouteDestBySoConIndStartIdx, PartitionIdx);

        /* #50 Set transmission active of nPdu. */
        SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_ACTIVE, PartitionIdx);                 /* SBSW_SOAD_PARAMETER_IDX */

        /* #60 Prepare transmission. */
        SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, pduRouteDestIdx, PartitionIdx);                                       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)totalLen, PartitionIdx);                                /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, (PduLengthType)totalLen, PartitionIdx);                                    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, NULL_PTR, PartitionIdx);                                                     /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

        /* #70 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #80 Try to send nPdu. */
        (void)SoAd_TxSocketManager_UdpTransmit(soConIdx, PartitionIdx, (uint16)totalLen);

        /* #90 Remove timeout from list. */
        SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_N_PDU, NPduIdx, instanceIdx, PartitionIdx);

        /* #100 Enter critical section to prevent interruption while resetting the parameters. */
        SOAD_BEGIN_CRITICAL_SECTION();

        /* #110 Finish transmission independent of transmission result since there is no retransmission specified. */
        SoAd_SetBufLenOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                       /* SBSW_SOAD_PARAMETER_IDX */
        SoAd_SetTotalLenOfNPduUdpTxDyn(NPduIdx, 0u, PartitionIdx);                                                     /* SBSW_SOAD_PARAMETER_IDX */
        SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_NONE, PartitionIdx);                   /* SBSW_SOAD_PARAMETER_IDX */

        /* #120 Reset transmission management struct. */
        SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);              /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

        /* #130 Deactivate transmission on socket connection and on socket. */
        SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, TRUE);

        /* #140 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();
      }
      /* #41 Leave critical section. */
      else
      {
        SOAD_END_CRITICAL_SECTION();
      }
    }
    /* #31 Skip transmission otherwise. */
    else
    {
      /* #310 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();
    }
  }
  /* #21 Skip transmission otherwise. */
  else
  {
    /* #210 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_TxNPduUdp_BufferSend() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_CopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfNPduUdpTxType  nPduIdx = SoAd_GetNPduUdpTxIdxOfSoCon(SoConIdx, PartitionIdx);
  BufReq_ReturnType         retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle CopyTxData in case nPdu queue is used. */
  if ( SoAd_IsNPduUdpTxQueueUsedOfNPduUdpTx(nPduIdx, PartitionIdx) )
  {
    retVal = SoAd_TxNPduUdp_QueueCopyTxData(nPduIdx, PartitionIdx, BufPtr, BufLengthPtr);                              /* SBSW_SOAD_POINTER_FORWARD */
  }
  /* #11 Handle CopyTxData for nPdu buffer otherwise. */
  else
  {
    SoAd_TxNPduUdp_BufferCopyTxData(nPduIdx, PartitionIdx, BufPtr, *BufLengthPtr);                                     /* SBSW_SOAD_POINTER_FORWARD */

    retVal = BUFREQ_OK;
  }

  return retVal;
} /* SoAd_TxNPduUdp_CopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyTxData()
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
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyTxData(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType           bytesCopied;
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(soConIdx, PartitionIdx);
  BufReq_ReturnType       retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IP fragmentation buffer is used. */
  if ( SoAd_IsIpFragMgtUsedOfSoCon(soConIdx, PartitionIdx) )
  {
    /* #100 Calculate data length which is already copied to lower layer. */
    bytesCopied = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) -
      SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx));

    /* #101 Copy IP fragmentation buffer to provided buffer. */
    /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
    VStdMemCpy(                                                                                                        /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
      SOAD_V_P2VAR(void)&BufPtr[0u],
      SOAD_V_P2CONST(void)&SoAd_GetTxBufPtrOfTxMgt(txMgtIdx, PartitionIdx)[bytesCopied],
      *BufLengthPtr);

    /* #102 Decrement pending data length. */
    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)(SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) -    /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      *BufLengthPtr), PartitionIdx);

    retVal = BUFREQ_OK;
  }
  /* #11 Assume to copy nPdu in current context otherwise. */
  else
  {
    /* #110 Copy nPdu to buffer. */
    retVal = SoAd_TxNPduUdp_QueueCopyToBuffer(NPduIdx, PartitionIdx, BufPtr, BufLengthPtr);                            /* SBSW_SOAD_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_BufferCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_BufferCopyTxData(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2CONST(uint8)   txBufPtr = SoAd_GetAddrNPduUdpTxBuffer(SoAd_GetNPduUdpTxBufferStartIdxOfNPduUdpTx(NPduIdx,
    PartitionIdx), PartitionIdx);
  PduLengthType         bytesCopied;
  /* Optional indirection: Refer to Safe BSW assumption SoCon -> TxMgt. */
  SoAd_SizeOfTxMgtType  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx),
    PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate data length which is already copied to lower layer. */
  bytesCopied = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SoAd_GetTxBytesPendingOfTxMgt(
      txMgtIdx, PartitionIdx));

  /* #20 Copy nPdu buffer to provided buffer. */
  /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
  VStdMemCpy(                                                                                                          /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
    SOAD_V_P2VAR(void)&BufPtr[0u],
    SOAD_V_P2CONST(void)&txBufPtr[bytesCopied],
    BufLength);

  /* #30 Decrement pending data length. */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)(SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    BufLength), PartitionIdx);
} /* SoAd_TxNPduUdp_BufferCopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBuffer()
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
SOAD_LOCAL FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBuffer(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_NPduUdpTxQueueIterType nPduUdpTxQueueIter;
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx;
  uint16                      bufLength = *BufLengthPtr;
  uint16                      bufIdx = 0u;
  BufReq_ReturnType           retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all elements in nPdu queue. */
  for ( nPduUdpTxQueueIter = SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
    nPduUdpTxQueueIter <
    (SoAd_NPduUdpTxQueueIterType)(SoAd_GetNPduUdpTxQueueStartIdxOfNPduUdpTx(NPduIdx, PartitionIdx) + /* PRQA S 4391 */ /* MD_SoAd_GenericWiderTypeCast */
      SoAd_GetLvlOfNPduUdpTxDyn(NPduIdx, PartitionIdx));
    nPduUdpTxQueueIter++ )
  {
    pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfNPduUdpTxQueue(nPduUdpTxQueueIter, PartitionIdx);

    /* #20 Check if nPdu element is pending. */
    if ( pduRouteDestIdx != SoAd_GetSizeOfPduRouteDest(PartitionIdx) )
    {
      /* #30 Copy element if PDU is located in buffer. */
      if ( SoAd_GetIfTriggerTransmitModeOfPduRoute(SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx),
        PartitionIdx) != SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI )
      {
        retVal = SoAd_TxNPduUdp_QueueCopyToBufferFromBuffer((SoAd_SizeOfNPduUdpTxQueueType)nPduUdpTxQueueIter,         /* SBSW_SOAD_POINTER_WITH_OFFSET_FORWARD */
          PartitionIdx, &BufPtr[bufIdx], &bufLength);
      }
      else
      /* #31 Copy element via trigger transmit otherwise. */
      {
        retVal = SoAd_TxNPduUdp_QueueCopyToBufferViaTrigger(NPduIdx, (SoAd_SizeOfNPduUdpTxQueueType)nPduUdpTxQueueIter,/* SBSW_SOAD_POINTER_WITH_OFFSET_FORWARD */
         PartitionIdx, &BufPtr[bufIdx], &bufLength);
      }

      /* #40 Indicate that element is finished if it is successfully copied to provided buffer. */
      if ( retVal == BUFREQ_OK )
      {
        bufIdx = *BufLengthPtr - bufLength;
        SoAd_SetCopiedOfNPduUdpTxQueue(nPduUdpTxQueueIter, TRUE, PartitionIdx);                                        /* SBSW_SOAD_CALCULATED_CSL03_CSL05_NPduUdpTxQueue */
      }
      /* #41 Stop iteration otherwise. */
      else
      {
        /* #410 Check if buffer size of provided buffer is not sufficient and if an element is copied in case dynamic
         *      length is enabled. */
        if ( (retVal == BUFREQ_E_OVFL)
#if ( SOAD_TX_DYN_LEN == STD_ON )
        /* It is only required to check if data has been copied in case dynamic length is enabled. In case dynamic
         * length is disabled BUFREQ_E_NOT_OK is returned whenever it is required. */
            && (bufLength < *BufLengthPtr)
#endif /* SOAD_TX_DYN_LEN == STD_ON */
          )
        {
          retVal = BUFREQ_OK;
        }
#if ( SOAD_TX_DYN_LEN == STD_OFF )
        /* #411 Indicate that transmission has failed if dynamic length is disabled otherwise. */
        else
        {
          SoAd_SetTransmissionStateOfNPduUdpTxDyn(NPduIdx, SOAD_NPDU_UDP_TX_STATE_FAILED, PartitionIdx);               /* SBSW_SOAD_PARAMETER_IDX */
        }
#endif /* SOAD_TX_DYN_LEN == STD_OFF */
        break;
      }
    }
  }

  /* #50 Return length of data copied effectively. */
  *BufLengthPtr = bufIdx;                                                                                              /* SBSW_SOAD_POINTER_WRITE */

  return retVal;
} /* SoAd_TxNPduUdp_QueueCopyToBuffer() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferFromBuffer()
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
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferFromBuffer(
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if provided buffer is sufficient to copy PDU. */
  if ( *BufLengthPtr >= SoAd_GetLenOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx) )
  {
    /* #20 Copy PDU to provided buffer. */
    /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
    VStdMemCpy(                                                                                                        /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
      SOAD_V_P2VAR(void)&BufPtr[0],
      SOAD_V_P2CONST(void)SoAd_GetAddrNPduUdpTxBuffer(
        SoAd_GetNPduUdpTxBufferIdxOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx), PartitionIdx),
      SoAd_GetLenOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx));

    /* #30 Indicate that PDU is copied completely. */
    *BufLengthPtr -= (uint16)SoAd_GetLenOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx);                                  /* SBSW_SOAD_POINTER_WRITE */

    retVal = BUFREQ_OK;
  }
#if ( SOAD_TX_DYN_LEN == STD_OFF )
  /* #11 Return failure if dynamic length is disabled and the provided buffer is not filled completely otherwise. */
  else if ( *BufLengthPtr != 0u )
  {
    retVal = BUFREQ_E_NOT_OK;
  }
#endif /* SOAD_TX_DYN_LEN == STD_OFF */
  /* #12 Indicate an overflow otherwise. */
  else
  {
    retVal = BUFREQ_E_OVFL;
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCopyToBufferFromBuffer() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTrigger()
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
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTrigger(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType             pduInfo;
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  BufReq_ReturnType       retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare buffer to copy data via trigger transmit. */
  retVal = SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerPrepare(NPduIdx, PartitionIdx, BufPtr, *BufLengthPtr, &pduInfo);  /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */

  /* #20 Check if buffer is prepared. */
  if ( retVal == BUFREQ_OK )
  {
    /* #30 Check if PDU contains data in case PDU header is used. */
    if ( !SoAd_IsPduHdrEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) ||
      ((SoAd_GetLenOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx) - SOAD_PDU_HDR_SIZE) != 0u) )
    {
      /* #40 Call trigger transmit to copy data to provided buffer. */
      retVal = SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerCall(NPduIdx, NPduQueueIdx, PartitionIdx, BufLengthPtr,       /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
        &pduInfo);
    }

    /* #50 Add PDU header considering length returned by trigger transmit call. */
    if ( (retVal == BUFREQ_OK) && SoAd_IsPduHdrEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerAddPduHeader(NPduQueueIdx, PartitionIdx, BufPtr, BufLengthPtr,         /* SBSW_SOAD_POINTER_EXPECTED_SIZE_AND_VARIABLE_POINTER_FORWARD */
        &pduInfo);
    }
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCopyToBufferViaTrigger() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerPrepare()
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
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerPrepare(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType  soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  BufReq_ReturnType     retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare buffer considering PDU header offset. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #100 Prepare buffer if provided buffer size is sufficient to store the PDU. */
    if ( BufLength >= SOAD_PDU_HDR_SIZE )
    {
      PduInfoPtr->SduDataPtr = &BufPtr[SOAD_PDU_HDR_SIZE];                                                             /* SBSW_SOAD_POINTER_WRITE */
      /* PRQA S 4391 1 */ /* MD_SoAd_GenericWiderTypeCast */
      PduInfoPtr->SduLength = (PduLengthType)(BufLength - SOAD_PDU_HDR_SIZE);                                          /* SBSW_SOAD_POINTER_WRITE */

      retVal = BUFREQ_OK;
    }
    /* #101 Return failure if dynamic length is disabled and the provided buffer is not filled completely otherwise. */
#if ( SOAD_TX_DYN_LEN == STD_OFF )
    else if ( BufLength != 0u )
    {
      retVal = BUFREQ_E_NOT_OK;
    }
#endif /* SOAD_TX_DYN_LEN == STD_OFF */
    /* #102 Indicate an overflow otherwise. */
    else
    {
      retVal = BUFREQ_E_OVFL;
    }
  }
  /* #11 Prepare buffer to copy data without offset otherwise. */
  else
  {
    PduInfoPtr->SduDataPtr = BufPtr;                                                                                   /* SBSW_SOAD_POINTER_WRITE */
    PduInfoPtr->SduLength = (PduLengthType)BufLength;                                                                  /* SBSW_SOAD_POINTER_WRITE */

    retVal = BUFREQ_OK;
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerPrepare() */

/**********************************************************************************************************************
 *   SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerCall()
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
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerCall(
  SoAd_SizeOfNPduUdpTxType NPduIdx,
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr,
  P2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  SoAd_SizeOfSoConType        soConIdx = SoAd_GetSoConIdxOfNPduUdpTx(NPduIdx, PartitionIdx);
  BufReq_ReturnType           retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call trigger transmit to copy PDU to provided buffer. */
  if ( SoAd_GetIfTriggerTransmitCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(pduRouteIdx, PartitionIdx))(           /* SBSW_SOAD_FUNCTION_PTR */
    SoAd_GetTxConfPduIdOfPduRoute(pduRouteIdx, PartitionIdx), PduInfoPtr) == E_OK )
  {
#if ( SOAD_TX_DYN_LEN == STD_ON )
    /* #20 Check if copied length is valid (if CopyTxData with dynamic length is enabled). */
    if ( SoAd_TxIf_TriggerTransmitCheckParameter(soConIdx, PartitionIdx, *BufLengthPtr, PduInfoPtr,                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      FALSE, TRUE) == E_OK )
#else
    /* #21 Check if copied length is valid (if CopyTxData with dynamic length is disabled). */
    if ( SoAd_TxIf_TriggerTransmitCheckParameter(soConIdx, PartitionIdx,                                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      (uint16)SoAd_GetLenOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx), PduInfoPtr, TRUE, TRUE) == E_OK )
#endif /* SOAD_TX_DYN_LEN == STD_ON */
    {
      /* #30 Decrement available buffer length of provided buffer by copied length. */
      *BufLengthPtr -= (uint16)PduInfoPtr->SduLength;                                                                  /* SBSW_SOAD_POINTER_WRITE */
      retVal = BUFREQ_OK;
    }
  }
  /* #11 Indicate overflow if dynamic length is enabled and failure if not otherwise. */
  else
  {
#if ( SOAD_TX_DYN_LEN == STD_ON )
    retVal = BUFREQ_E_OVFL;
#else
    retVal = BUFREQ_E_NOT_OK;
#endif /* SOAD_TX_DYN_LEN == STD_ON */
  }

  return retVal;
} /* SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerCall() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerAddPduHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerAddPduHeader(
  SoAd_SizeOfNPduUdpTxQueueType NPduQueueIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfNPduUdpTxQueue(NPduQueueIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy PDU header of PduRouteDest to provided buffer. */
  SoAd_Util_CopyPduHdr2Buf(                                                                                            /* SBSW_SOAD_POINTER_FORWARD */
    SoAd_GetTxPduHdrIdOfPduRouteDest(pduRouteDestIdx, PartitionIdx),
    PduInfoPtr->SduLength,
    BufPtr,
    0u,
    SOAD_PDU_HDR_SIZE);

  /* #20 Decrement available buffer length of provided buffer by length of PDU header. */
  *BufLengthPtr -= SOAD_PDU_HDR_SIZE;                                                                                  /* SBSW_SOAD_POINTER_WRITE */
} /* SoAd_TxNPduUdp_QueueCopyToBufferViaTriggerAddPduHeader() */

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIpFragBuf_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_IpFragBufConfigIterType  ipFragBufConfigIter;
  SoAd_IpFragMgtIterType        ipFragMgtIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all IP fragmentation buffer. */
  for ( ipFragBufConfigIter = 0u; ipFragBufConfigIter < SoAd_GetSizeOfIpFragBufConfig(PartitionIdx);
    ipFragBufConfigIter++ )
  {
    /* #20 Initialize IP fragmentation buffer. */
    SoAd_SetLengthOfIpFragBufConfigDyn(ipFragBufConfigIter, 0u, PartitionIdx);                                         /* SBSW_SOAD_CSL02_CSL05 */
  }

  /* #30 Iterate over all IP fragmentation related socket connections. */
  for ( ipFragMgtIter = 0u; ipFragMgtIter < SoAd_GetSizeOfIpFragMgt(PartitionIdx); ipFragMgtIter++ )
  {
    /* #40 Initialize IP fragmentation buffer usage on socket connection. */
    SoAd_SetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIter, SoAd_GetSizeOfIpFragBufConfig(PartitionIdx), PartitionIdx);   /* SBSW_SOAD_CSL01_CSL05 */
  }
} /* SoAd_TxIpFragBuf_Init() */

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Reserve()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIpFragBuf_Reserve(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint32 Length,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) IpFragBufPtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_IpFragBufConfigIterType   ipFragBufConfigIter;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfIpFragMgtType       ipFragMgtIdx = SoAd_GetIpFragMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType        soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType        instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfIpFragBufConfigType ipFragBufConfigStartIdx = SoAd_GetIpFragBufConfigStartIdxOfInstance(instanceIdx,
    PartitionIdx);
  SoAd_SizeOfIpFragBufConfigType ipFragBufConfigEndIdx = SoAd_GetIpFragBufConfigEndIdxOfInstance(instanceIdx,
    PartitionIdx);
  Std_ReturnType                 retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistent reservation of IP fragmentation buffer. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if requested socket connection does not use a buffer. */
  if ( SoAd_GetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIdx, PartitionIdx) ==
    SoAd_GetSizeOfIpFragBufConfig(PartitionIdx) )
  {
    /* #30 Iterate over all IP fragmentation buffer of the corresponding instance. */
    for ( ipFragBufConfigIter = ipFragBufConfigStartIdx; ipFragBufConfigIter < ipFragBufConfigEndIdx;
      ipFragBufConfigIter++ )
    {
      /* #40 Check if IP fragmentation buffer is available. */
      if ( SoAd_GetLengthOfIpFragBufConfigDyn(ipFragBufConfigIter, PartitionIdx) == 0u )
      {
        /* #50 Check if buffer size is sufficient for requested length. */
        if ( ((uint32)SoAd_GetIpFragBufEndIdxOfIpFragBufConfig(ipFragBufConfigIter, PartitionIdx) -
          (uint32)SoAd_GetIpFragBufStartIdxOfIpFragBufConfig(ipFragBufConfigIter, PartitionIdx)) >= Length )
        {
          /* #60 Reserve the buffer for the requested socket connection. */
          SoAd_SetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIdx, (SoAd_SizeOfIpFragBufConfigType)ipFragBufConfigIter,     /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
            PartitionIdx);
          /* Cast to uint16 since configuration prevents a bigger suitable buffer. */
          SoAd_SetLengthOfIpFragBufConfigDyn(ipFragBufConfigIter, (uint16)Length, PartitionIdx);                       /* SBSW_SOAD_CSL02_CSL05 */
          *IpFragBufPtrPtr = SoAd_GetAddrIpFragBuf(SoAd_GetIpFragBufStartIdxOfIpFragBufConfig(ipFragBufConfigIter,     /* SBSW_SOAD_POINTER_WRITE */
            PartitionIdx), PartitionIdx);
          retVal = E_OK;
          break;
        }
      }
    }
  }

  /* #70 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxIpFragBuf_Reserve() */

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Set()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIpFragBuf_Set(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfIpFragMgtType        ipFragMgtIdx = SoAd_GetIpFragMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfIpFragBufConfigType  ipFragBufConfigIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistent access to IP fragmentation buffer. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Get the buffer used by requested socket connection. */
  ipFragBufConfigIdx = SoAd_GetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIdx, PartitionIdx);

  if ( ipFragBufConfigIdx != SoAd_GetSizeOfIpFragBufConfig(PartitionIdx) )
  {
    /* #30 Set length of copied data. */
    SoAd_SetLengthOfIpFragBufConfigDyn(ipFragBufConfigIdx, Length, PartitionIdx);                                      /* SBSW_SOAD_CALCULATED_CSL03_CSL05_IpFragBufConfigDyn */
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_TxIpFragBuf_Set() */

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Get()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIpFragBuf_Get(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) IpFragBufPtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfIpFragMgtType        ipFragMgtIdx = SoAd_GetIpFragMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfIpFragBufConfigType  ipFragBufConfigIdx;
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistent access to IP fragmentation buffer. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Get the buffer used by requested socket connection. */
  ipFragBufConfigIdx = SoAd_GetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIdx, PartitionIdx);

  if ( ipFragBufConfigIdx != SoAd_GetSizeOfIpFragBufConfig(PartitionIdx) )
  {
    /* #30 Return length and pointer to buffer. */
    *LengthPtr = SoAd_GetLengthOfIpFragBufConfigDyn(ipFragBufConfigIdx, PartitionIdx);                                 /* SBSW_SOAD_POINTER_WRITE */
    *IpFragBufPtrPtr = SoAd_GetAddrIpFragBuf(SoAd_GetIpFragBufStartIdxOfIpFragBufConfig(ipFragBufConfigIdx,            /* SBSW_SOAD_POINTER_WRITE */
      PartitionIdx), PartitionIdx);
    retVal = E_OK;
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxIpFragBuf_Get() */

/**********************************************************************************************************************
 *  SoAd_TxIpFragBuf_Release()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIpFragBuf_Release(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfIpFragMgtType        ipFragMgtIdx;
  SoAd_SizeOfIpFragBufConfigType  ipFragBufConfigIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if usage of IP fragmentation buffer is enabled on requested socket connection. */
  if ( SoAd_IsIpFragMgtUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    /* #20 Enter critical section to prevent inconsistent access to IP fragmentation buffer. */
    SOAD_BEGIN_CRITICAL_SECTION();

    ipFragMgtIdx = SoAd_GetIpFragMgtIdxOfSoCon(SoConIdx, PartitionIdx);
    ipFragBufConfigIdx = SoAd_GetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIdx, PartitionIdx);

    /* #30 Get the buffer used by requested socket connection. */
    if ( ipFragBufConfigIdx != SoAd_GetSizeOfIpFragBufConfig(PartitionIdx) )
    {
      /* #40 Release buffer. */
      SoAd_SetLengthOfIpFragBufConfigDyn(ipFragBufConfigIdx, 0u, PartitionIdx);                                        /* SBSW_SOAD_CALCULATED_CSL03_CSL05_IpFragBufConfigDyn */
      SoAd_SetIpFragBufConfigIdxOfIpFragMgt(ipFragMgtIdx, SoAd_GetSizeOfIpFragBufConfig(PartitionIdx), PartitionIdx);  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    }

    /* #50 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_TxIpFragBuf_Release() */

/**********************************************************************************************************************
 *  SoAd_TxIfTriggerBuf_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIfTriggerBuf_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_TriggerBufConfigIterType triggerBufConfigIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all trigger transmit buffer. */
  for ( triggerBufConfigIter = 0u; triggerBufConfigIter < SoAd_GetSizeOfTriggerBufConfig(PartitionIdx);
    triggerBufConfigIter++ )
  {
    /* #20 Initialize the trigger transmit buffer. */
    SoAd_SetLengthOfTriggerBufConfigDyn(triggerBufConfigIter, 0u, PartitionIdx);                                       /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetPduRouteIdxOfTriggerBufConfigDyn(triggerBufConfigIter, SoAd_GetSizeOfPduRoute(PartitionIdx), PartitionIdx);/* SBSW_SOAD_CSL02_CSL05 */
  }
} /* SoAd_TxIfTriggerBuf_Init() */

/**********************************************************************************************************************
 *  SoAd_TxIfTriggerBuf_Reserve()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxIfTriggerBuf_Reserve(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) TriggerBufPtrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_TriggerBufConfigIterType   triggerBufConfigIter;
  SoAd_TriggerBufConfigIterType   freeTriggerBufConfigIdx = SoAd_GetSizeOfTriggerBufConfig(PartitionIdx);
  SoAd_SizeOfSoConType            soConIdx = SoAd_GetSoConIdxOfPduRouteDest(SoAd_GetPduRouteDestStartIdxOfPduRoute(
    PduRouteIdx, PartitionIdx), PartitionIdx);
  SoAd_SizeOfSoConGrpType         soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType         instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfTriggerBufConfigType triggerBufConfigStartIdx = SoAd_GetTriggerBufConfigStartIdxOfInstance(instanceIdx,
    PartitionIdx);
  SoAd_SizeOfTriggerBufConfigType triggerBufConfigEndIdx = SoAd_GetTriggerBufConfigEndIdxOfInstance(instanceIdx,
    PartitionIdx);
  PduLengthType                   length = *LengthPtr;
  boolean                         pduRouteAlreadyReserved = FALSE;
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent read/modify/write issues for the trigger transmit buffer. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Iterate over all trigger transmit buffer of the corresponding instance. */
  for ( triggerBufConfigIter = triggerBufConfigStartIdx; triggerBufConfigIter < triggerBufConfigEndIdx;
    triggerBufConfigIter++ )
  {
    /* #30 Check if buffer is available. */
    if ( SoAd_GetPduRouteIdxOfTriggerBufConfigDyn(triggerBufConfigIter, PartitionIdx) ==
      SoAd_GetSizeOfPduRoute(PartitionIdx) )
    {
      /* #300 Check if buffer size is sufficient or no specific buffer size is requested. */
      if ( (length == 0u) || (length <= (SoAd_SizeOfTriggerBufType)(
        SoAd_GetTriggerBufEndIdxOfTriggerBufConfig(triggerBufConfigIter, PartitionIdx) -
        SoAd_GetTriggerBufStartIdxOfTriggerBufConfig(triggerBufConfigIter, PartitionIdx))) )
      {
        /* #3000 Mark buffer for reservation. */
        freeTriggerBufConfigIdx = triggerBufConfigIter;
      }
    }
    /* #31 Check if buffer is already reserved for the requested PduRoute otherwise. */
    else if ( SoAd_GetPduRouteIdxOfTriggerBufConfigDyn(triggerBufConfigIter, PartitionIdx) == PduRouteIdx )
    {
      /* #310 Stop iteration since one PduRoute can only reserve one trigger transmit buffer at the same time. */
      pduRouteAlreadyReserved = TRUE;
      break;
    }
    /* #32 Skip buffer if it is already reserved by a different PduRoute otherwise. */
    else
    {
      /* Nothing to do. */
    }
  }

  /* #40 Check if a suitable buffer is available and PduRoute has no trigger transmit buffer reserved, yet. */
  if ( (freeTriggerBufConfigIdx < SoAd_GetSizeOfTriggerBufConfig(PartitionIdx)) && (pduRouteAlreadyReserved == FALSE) )
  {
    /* #400 Reserve the buffer. */
    SoAd_SetPduRouteIdxOfTriggerBufConfigDyn(freeTriggerBufConfigIdx, PduRouteIdx, PartitionIdx);                      /* SBSW_SOAD_CSL02_CSL05 */

    /* #401 Set and return the length which indicates the used part of the buffer. */
    if ( length == 0u )
    {
      boolean isUdp = SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx);
      length = SoAd_Util_GetSupportedPduLength((SoAd_SizeOfTriggerBufType)(
        SoAd_GetTriggerBufEndIdxOfTriggerBufConfig(freeTriggerBufConfigIdx, PartitionIdx) -
        SoAd_GetTriggerBufStartIdxOfTriggerBufConfig(freeTriggerBufConfigIdx, PartitionIdx)), isUdp);

      *LengthPtr = length;                                                                                             /* SBSW_SOAD_POINTER_WRITE */
    }
    SoAd_SetLengthOfTriggerBufConfigDyn(freeTriggerBufConfigIdx, (uint16)length, PartitionIdx);                        /* SBSW_SOAD_CSL02_CSL05 */

    /* #402 Return the pointer to the reserved buffer. */
    *TriggerBufPtrPtr = SoAd_GetAddrTriggerBuf(SoAd_GetTriggerBufStartIdxOfTriggerBufConfig(freeTriggerBufConfigIdx,   /* SBSW_SOAD_POINTER_WRITE */
      PartitionIdx), PartitionIdx);

    retVal = E_OK;
  }
  else
  {
    /* Check if enough SoAdIfTriggerTransmitBuffer are configured. */
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxIfTriggerBuf_Reserve() */

/**********************************************************************************************************************
 *  SoAd_TxIfTriggerBuf_Release()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxIfTriggerBuf_Release(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_TriggerBufConfigIterType   triggerBufConfigIter;
  SoAd_SizeOfSoConType            soConIdx = SoAd_GetSoConIdxOfPduRouteDest(SoAd_GetPduRouteDestStartIdxOfPduRoute(
    PduRouteIdx, PartitionIdx), PartitionIdx);
  SoAd_SizeOfSoConGrpType         soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType         instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfTriggerBufConfigType triggerBufConfigStartIdx = SoAd_GetTriggerBufConfigStartIdxOfInstance(instanceIdx,
    PartitionIdx);
  SoAd_SizeOfTriggerBufConfigType triggerBufConfigEndIdx = SoAd_GetTriggerBufConfigEndIdxOfInstance(instanceIdx,
    PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent read/modify/write issues for the trigger transmit buffer. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Iterate over all trigger transmit buffer of the corresponding instance. */
  for ( triggerBufConfigIter = triggerBufConfigStartIdx; triggerBufConfigIter < triggerBufConfigEndIdx;
    triggerBufConfigIter++ )
  {
    /* #30 Check if buffer is used by requested PduRoute. */
    if ( SoAd_GetPduRouteIdxOfTriggerBufConfigDyn(triggerBufConfigIter, PartitionIdx) == PduRouteIdx )
    {
      /* #40 Release the trigger transmit buffer. */
      SoAd_SetLengthOfTriggerBufConfigDyn(triggerBufConfigIter, 0u, PartitionIdx);                                     /* SBSW_SOAD_CSL02_CSL05 */
      SoAd_SetPduRouteIdxOfTriggerBufConfigDyn(triggerBufConfigIter, SoAd_GetSizeOfPduRoute(PartitionIdx),             /* SBSW_SOAD_CSL02_CSL05 */
        PartitionIdx);
      break;
    }
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_TxIfTriggerBuf_Release() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

 /*********************************************************************************************************************
 *  SoAd_Tx_Init()
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
FUNC(void, SOAD_CODE) SoAd_Tx_Init(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PduRouteIterType   pduRouteIter;
  SoAd_NPduUdpTxIterType  nPduUdpTxIter;

  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Iterate over all PduRoutes. */
  for ( pduRouteIter = 0u; pduRouteIter < SoAd_GetSizeOfPduRoute(PartitionIdx); pduRouteIter++ )
  {
    /* #100 Initialize PduRoute structs. */
    SoAd_SetPendingTxConfMainOfPduRouteDyn(pduRouteIter, FALSE, PartitionIdx);                                         /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetPendingTxConfNumOfPduRouteDyn(pduRouteIter, 0u, PartitionIdx);                                             /* SBSW_SOAD_CSL02_CSL05 */
  }

  /* #20 Iterate over all nPdus. */
  for ( nPduUdpTxIter = 0u; nPduUdpTxIter < SoAd_GetSizeOfNPduUdpTx(PartitionIdx); nPduUdpTxIter++ )
  {
    /* #200 Initialize nPdu structs. */
    SoAd_TxNPduUdp_Clear((SoAd_SizeOfNPduUdpTxType)nPduUdpTxIter, PartitionIdx);
  }

  /* #30 Initialize all IP fragmentation buffer. */
  SoAd_TxIpFragBuf_Init(PartitionIdx); /* PRQA S 2987 */ /* MD_SoAd_RedundantCallWithoutSideEffects */

  /* #40 Initialize all trigger transmit buffer. */
  SoAd_TxIfTriggerBuf_Init(PartitionIdx); /* PRQA S 2987 */ /* MD_SoAd_RedundantCallWithoutSideEffects */
} /* SoAd_Tx_Init() */

 /*********************************************************************************************************************
 *  SoAd_Tx_InitSoCon()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_Tx_InitSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfTxMgtType txMgtIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize tx struct elements. */
  if ( SoAd_IsTxMgtUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);

    SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                  /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
    SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                         /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
    SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                             /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
    SoAd_SetTxBufPtrOfTxMgt(txMgtIdx, NULL_PTR, PartitionIdx);                                                         /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
    SoAd_SetCancelRequestedOfTxMgt(txMgtIdx, FALSE, PartitionIdx);                                                     /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
  }

  /* #20 Initialize tx buffer. */
  if ( SoAd_IsTpTxBufferConfigUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    SoAd_SizeOfTpTxBufferConfigType tpTxBufferConfigIdx = SoAd_GetTpTxBufferConfigIdxOfSoCon(SoConIdx, PartitionIdx);

    SoAd_SetTpTxBufferIdxOfTpTxBufferConfigDyn(tpTxBufferConfigIdx,                                                    /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03 */
      SoAd_GetTpTxBufferStartIdxOfTpTxBufferConfig(tpTxBufferConfigIdx, PartitionIdx), PartitionIdx);
  }

#if ( SOAD_TCP == STD_ON )
  /* #30 Initialize Tx TCP queue (if enabled). */
  SoAd_TxTcpQueue_Init(SoConIdx, PartitionIdx);
#endif /* SOAD_TCP == STD_ON */
} /* SoAd_Tx_InitSoCon() */

/**********************************************************************************************************************
 *  SoAd_Tx_IfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_IfTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameters for transmission. */
  if ( SoAd_TxIf_CheckParameter(PduRouteIdx, PartitionIdx, PduInfoPtr->SduLength) == E_OK )
  {
    /* #20 Trigger transmission of PDU. */
    retVal = SoAd_TxIf_TransmitPdu(PduRouteIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                                 /* SBSW_SOAD_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_Tx_IfTransmit() */

/**********************************************************************************************************************
 *  SoAd_Tx_IfTransmitPduRouteDest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_IfTransmitPduRouteDest(
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_VAR) PduInfoPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward transmission request to sub-component. */
  return SoAd_TxIf_TransmitPduRouteDest(PduRouteDestIdx, PartitionIdx, PduInfoPtr, ErrorIdPtr);                        /* SBSW_SOAD_POINTER_FORWARD */
} /* SoAd_Tx_IfTransmitPduRouteDest() */

/**********************************************************************************************************************
 *  SoAd_Tx_IfReserveTriggerBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_IfReserveTriggerBuf(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) LengthPtr,
  P2VAR(uint8*, AUTOMATIC, SOAD_APPL_VAR) TriggerBufPtrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward request to sub-component. */
  return SoAd_TxIfTriggerBuf_Reserve(PduRouteIdx, PartitionIdx, LengthPtr, TriggerBufPtrPtr);                          /* SBSW_SOAD_POINTER_FORWARD */
} /* SoAd_Tx_IfReserveTriggerBuf() */

/**********************************************************************************************************************
 *  SoAd_Tx_IfReleaseTriggerBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_Tx_IfReleaseTriggerBuf(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward request to sub-component. */
  SoAd_TxIfTriggerBuf_Release(PduRouteIdx, PartitionIdx);
} /* SoAd_Tx_IfReleaseTriggerBuf() */

/**********************************************************************************************************************
 *  SoAd_Tx_TpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_TpTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameters for transmission. */
  if ( SoAd_TxTp_CheckParameter(PduRouteIdx, PartitionIdx, PduInfoPtr->SduLength) == E_OK )
  {
    /* #20 Try to store transmission request. */
    if ( SoAd_TxTp_TransmitPdu(PduRouteIdx, PartitionIdx, PduInfoPtr) == E_OK )                                        /* SBSW_SOAD_POINTER_FORWARD */
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* SoAd_Tx_TpTransmit() */

/**********************************************************************************************************************
 *  SoAd_Tx_TpCancelTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_Tx_TpCancelTransmit(
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward transmission cancellation request to sub-component.*/
  return SoAd_TxTp_CancelTransmit(PduRouteIdx, PartitionIdx);
} /* SoAd_Tx_TpCancelTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxIf_ScheduleTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxIf_ScheduleTxConfirmation(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueIfUdpPduRouteIterType  eventQueueIfUdpPduRouteIter;
  uint32                                handleIdx = 0u;
  uint32                                elementNumToHandle = SoAd_EventQueue_GetElementNumToHandle(
    SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE, InstMapIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements which have to be handled in this context considering configured limit. */
  for ( eventQueueIfUdpPduRouteIter = 0u;
    eventQueueIfUdpPduRouteIter < elementNumToHandle;
    eventQueueIfUdpPduRouteIter++ )
  {
    /* #20 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE, InstMapIdx, &handleIdx) == E_OK )           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      SoAd_SizeOfPduRouteType     pduRouteIdx = (SoAd_SizeOfPduRouteType)handleIdx;
      SoAd_PartitionConfigIdxType partitionIdx =
        SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);

      /* #30 Call TxConfirmation if pending. */
      SoAd_TxIf_TxConfirmation(pduRouteIdx, partitionIdx);
    }
    /* #21 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
} /* SoAd_TxIf_ScheduleTxConfirmation() */

/**********************************************************************************************************************
 *  SoAd_Tx_UdpImmediateIfTxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_Tx_UdpImmediateIfTxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfPduRouteDestBySoConIndType pduRouteDestBySoConIndStartIdx;
  SoAd_SizeOfPduRouteType               pduRouteIdx;
  SoAd_SizeOfSoConGrpType               soConGpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if immediate IF TxConfirmation is configured. */
  if ( SoAd_IsImmedIfTxConfOfSocketUdp(SoAd_GetSocketUdpIdxOfSoConGrp(soConGpIdx, PartitionIdx), PartitionIdx) )
  {
    /* #20 Get first and only PduRoute. */
    pduRouteDestBySoConIndStartIdx = SoAd_GetPduRouteDestBySoConIndStartIdxOfSoCon(SoConIdx, PartitionIdx);
    pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(SoAd_GetPduRouteDestBySoConInd(pduRouteDestBySoConIndStartIdx,
      PartitionIdx), PartitionIdx);

    /* #30 Reconnect socket connection if socket connection has auto setup enabled and wildcards configured. */
    SoAd_SoCon_Reconnect(SoConIdx, PartitionIdx);

    /* #40 Call TxConfirmation. */
    SoAd_TxIf_CallTxConfirmation(pduRouteIdx, PartitionIdx);
  }
} /* SoAd_Tx_UdpImmediateIfTxConfirmation() */

/**********************************************************************************************************************
 *  SoAd_Tx_IsRouteGrpActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_Tx_IsRouteGrpActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PduRouteDestBySoConIndIterType  pduRouteDestBySoConIndIter; /* Not initialized: For-loop counter. */
  boolean                              isTxActive = FALSE; /* Not initialized: Avoid multiple declaration for same
                                                            * variable. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all PduRouteDests. */
  for ( pduRouteDestBySoConIndIter = SoAd_GetPduRouteDestBySoConIndStartIdxOfSoCon(SoConIdx, PartitionIdx);
    pduRouteDestBySoConIndIter < SoAd_GetPduRouteDestBySoConIndEndIdxOfSoCon(SoConIdx, PartitionIdx);
    pduRouteDestBySoConIndIter++ )
  {
    SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestBySoConInd(
      pduRouteDestBySoConIndIter, PartitionIdx);
    SoAd_RouteGrpSoConByPduRouteDestIndIterType routeGrpSoConByPduRouteDestIndIter; /* Not initialized: For-loop
                                                                                     * counter. */

    /* #20 Iterate over all PduRouteDest related routing group states. */
    for ( routeGrpSoConByPduRouteDestIndIter =
      SoAd_GetRouteGrpSoConByPduRouteDestIndStartIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
      routeGrpSoConByPduRouteDestIndIter <
      SoAd_GetRouteGrpSoConByPduRouteDestIndEndIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
      routeGrpSoConByPduRouteDestIndIter++ )
    {
      SoAd_SizeOfRouteGrpSoConType routeGrpSoConIdx = SoAd_GetRouteGrpSoConByPduRouteDestInd(
        routeGrpSoConByPduRouteDestIndIter, PartitionIdx);

      /* #30 Check if routing group transmission is active. */
      if ( SoAd_NonAtomicIsTriggeredFlagOfRouteGrpSoConDyn(SoAd_GetAddrFlagsOfRouteGrpSoConDyn(
            routeGrpSoConIdx, PartitionIdx)) )
      {
        /* #40 Return that transmission is active. */
        isTxActive = TRUE;
        break;
      }
    }
    if ( isTxActive == TRUE )
    {
      break;
    }
  }

  return isTxActive;
} /* SoAd_Tx_IsRouteGrpActive() */

/**********************************************************************************************************************
 *  SoAd_TxIf_UdpCopyTxData()
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
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxIf_UdpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2VAR(uint8)           bufPtrTmp = BufPtr;
  uint16                      bufLengthTmp = *BufLengthPtr;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  BufReq_ReturnType           retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle transmission if transmission is handled from nPdu (if enabled). */
  if ( SoAd_IsNPduUdpTxUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    retVal = SoAd_TxNPduUdp_CopyTxData(SoConIdx, PartitionIdx, BufPtr, BufLengthPtr);                                  /* SBSW_SOAD_POINTER_FORWARD */
  }
  /* #11 Handle transmission of single PDU otherwise. */
  else
  {
    /* #110 Copy PDU header to provided buffer if used. */
    if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
    {
      SoAd_Tx_CopyTxDataPduHeader(SoConIdx, pduRouteDestIdx, PartitionIdx, &bufPtrTmp, &bufLengthTmp);                 /* SBSW_SOAD_POINTER_POINTER_AND_VARIABLE_POINTER_FORWARD */
    }

    /* #111 Copy PDU data to provided buffer. */
    if ( (SoAd_GetIfTriggerTransmitModeOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI) &&
      !SoAd_IsIpFragMgtUsedOfSoCon(SoConIdx, PartitionIdx) )
    {
      /* #1110 Copy data in case of UDP and trigger transmit is used without IP fragmentation. */
      retVal = SoAd_TxIf_UdpCopyTxDataTrigger(SoConIdx, pduRouteIdx, PartitionIdx, BufPtr, BufLengthPtr);              /* SBSW_SOAD_POINTER_FORWARD */
    }
    else
    {
      /* #1111 Copy data provided in transmit context directly otherwise. */
      SoAd_TxIf_UdpCopyTxDataDirect(SoConIdx, PartitionIdx, bufPtrTmp, bufLengthTmp);                                  /* SBSW_SOAD_POINTER_WITH_OFFSET_FORWARD */
      retVal = BUFREQ_OK;
    }
  }

  return retVal;
} /* SoAd_TxIf_UdpCopyTxData() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxIf_TcpCopyTxData()
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
 */
FUNC(void, SOAD_CODE) SoAd_TxIf_TcpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2VAR(uint8)           localBufPtr = BufPtr;
  uint16                      localBufLength = BufLength;
  PduLengthType               bytesCopied;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PDU header is used. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #100 Copy PDU header to provided buffer. */
    SoAd_Tx_CopyTxDataPduHeader(SoConIdx, pduRouteDestIdx, PartitionIdx, &localBufPtr, &localBufLength);               /* SBSW_SOAD_POINTER_POINTER_AND_VARIABLE_POINTER_FORWARD */

    /* #101 Calculcate number of copied bytes considering PDU header and check if PDU header is copied completely. */
    bytesCopied = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SoAd_GetTxBytesPendingOfTxMgt(
      txMgtIdx, PartitionIdx));
    if ( bytesCopied >= SOAD_PDU_HDR_SIZE )
    {
      bytesCopied -= SOAD_PDU_HDR_SIZE;

      /* #102 Copy PDU data from local transmission buffer if provided buffer is sufficient to copy data. */
      if ( localBufLength > 0u )
      {
        SOAD_P2CONST(uint8) txBufPtr = SoAd_GetTxBufPtrOfTxMgt(txMgtIdx, PartitionIdx);

        /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
        VStdMemCpy(                                                                                                    /* SBSW_SOAD_POINTER_WITH_OFFSET_AND_CONST_POINTER_FORWARD */
          SOAD_A_P2VAR(void)localBufPtr,
          SOAD_A_P2CONST(void)&txBufPtr[bytesCopied],
          localBufLength);
      }
    }
  }
  else
  /* #11 Copy PDU data otherwise. */
  {
    SOAD_P2CONST(uint8) txBufPtr = SoAd_GetTxBufPtrOfTxMgt(txMgtIdx, PartitionIdx);

    /* #110 Calculate number of copied bytes. */
    bytesCopied = (PduLengthType)(SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) -
      SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx));

    /* #111 Copy PDU data from local transmission buffer. */
    /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
    VStdMemCpy(                                                                                                        /* SBSW_SOAD_POINTER_WITH_OFFSET_AND_CONST_POINTER_FORWARD */
      SOAD_A_P2VAR(void)localBufPtr,
      SOAD_A_P2CONST(void)&txBufPtr[bytesCopied],
      localBufLength);
  }

  /* #20 Decrement pending data to be transmitted. */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)(SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    localBufLength), PartitionIdx);
} /* SoAd_TxIf_TcpCopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxTp_TcpCopyTxData()
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
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxTp_TcpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfPduRouteDestType PduRouteDestIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2VAR(uint8)       localBufPtr = BufPtr;
  uint16                  localBufLength = BufLength;
  PduInfoType             pduInfo;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType    txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(PduRouteDestIdx, PartitionIdx);
  PduLengthType           availableBufLength = 0u;
  BufReq_ReturnType       retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy PDU header to provided buffer if used. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    SoAd_Tx_CopyTxDataPduHeader(SoConIdx, PduRouteDestIdx, PartitionIdx, &localBufPtr, &localBufLength);               /* SBSW_SOAD_POINTER_POINTER_AND_VARIABLE_POINTER_FORWARD */
  }

  /* #20 Check if provided buffer is not sufficient to copy PDU data in this CopyTxData call. */
  if ( localBufLength == 0u )
  {
    retVal = BUFREQ_OK;
  }
  else
  /* #21 Copy PDU data otherwise. */
  {
    /* #30 Prepare buffer for TpCopyTxData call. */
    pduInfo.SduDataPtr = localBufPtr;
    pduInfo.SduLength = (PduLengthType)localBufLength;

    /* #40 Call TpCopyTxData of upper layer and check if calls succeeds. */
    if ( SoAd_TxTp_CallCopyTxData(pduRouteIdx, PartitionIdx, &pduInfo, &availableBufLength) == BUFREQ_OK )             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #50 Decrement pending data to be transmitted. */
      SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)(SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) -  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
        localBufLength), PartitionIdx);
      retVal = BUFREQ_OK;
    }
    /* #41 Indicate that transmission failed otherwise. */
    else
    {
      retVal = BUFREQ_E_NOT_OK;
    }
  }

  return retVal;
} /* SoAd_TxTp_TcpCopyTxData() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxTp_UdpCopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxTp_UdpCopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SOAD_P2CONST(uint8)   txBufPtr;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType  txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  PduLengthType         bytesCopied;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate pending data to be copied. */
  bytesCopied = SoAd_GetTxBytesLenOfTxMgt(txMgtIdx, PartitionIdx) - SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx,
    PartitionIdx);

  /* #20 Copy data from TP transmission buffer to provided buffer. */
  txBufPtr = SoAd_GetTxBufPtrOfTxMgt(txMgtIdx, PartitionIdx);
  /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
  VStdMemCpy(                                                                                                          /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
    SOAD_A_P2VAR(void)&BufPtr[0],
    SOAD_A_P2CONST(void)&txBufPtr[bytesCopied],
    BufLength);

  /* #30 Decrement pending length. */
  SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, (PduLengthType)(SoAd_GetTxBytesPendingOfTxMgt(txMgtIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    BufLength), PartitionIdx);
} /* SoAd_TxTp_UdpCopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxTp_Terminate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxTp_Terminate(
  SoAd_SizeOfTxMgtType TxMgtIdx,
  SoAd_SizeOfPduRouteType PduRouteIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Call confirmation callback with positive result if all data are sent. */
  if ( SoAd_GetTxBytesPendingOfTxMgt(TxMgtIdx, PartitionIdx) == 0u )
  {
    SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(               /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), E_OK);
  }
  /* #20 Call confirmation callback with negative result if not all data are sent. */
  else
#else
    SOAD_DUMMY_STATEMENT(TxMgtIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_TCP == STD_ON */
  {
    SoAd_GetTpTxConfirmationCbkOfUpperLayer(SoAd_GetUpperLayerIdxOfPduRoute(PduRouteIdx, PartitionIdx))(               /* SBSW_SOAD_FUNCTION_PTR */
      SoAd_GetTxConfPduIdOfPduRoute(PduRouteIdx, PartitionIdx), E_NOT_OK);
  }
} /* SoAd_TxTp_Terminate() */

/**********************************************************************************************************************
 *  SoAd_TxTp_ScheduleTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxTp_ScheduleTransmission(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueTpTxSoConIterType  eventQueueTpTxSoConIter;
  uint32                            handleIdx = 0u;
  uint32                            elementNumToHandle = SoAd_EventQueue_GetElementNumToHandle(
    SOAD_EVENT_QUEUE_TP_TX_SO_CON, InstMapIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements which have to be handled in this context considering configured limit. */
  for ( eventQueueTpTxSoConIter = 0u;
    eventQueueTpTxSoConIter < elementNumToHandle;
    eventQueueTpTxSoConIter++ )
  {
    /* #20 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SOAD_EVENT_QUEUE_TP_TX_SO_CON, InstMapIdx, &handleIdx) == E_OK )               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      SoAd_SizeOfSoConType        soConIdx = (SoAd_SizeOfSoConType)handleIdx;
      SoAd_PartitionConfigIdxType partitionIdx =
        SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);

      /* #30 Handle TP transmission. */
      (void)SoAd_TxTp_TransmitPduRouteDest(soConIdx, partitionIdx, TRUE);
    }
    /* #21 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
} /* SoAd_TxTp_ScheduleTransmission() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxTcp_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxTcp_CopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(
    SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  BufReq_ReturnType           retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle transmission for IF-API. */
  if ( SoAd_GetUpperLayerApiOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_UL_API_IF )
  {
    SoAd_TxIf_TcpCopyTxData(SoConIdx, PartitionIdx, BufPtr, BufLength);                                                /* SBSW_SOAD_POINTER_FORWARD */
    retVal = BUFREQ_OK;
  }
  /* #20 Handle transmission for TP-API otherwise. */
  else
  {
    retVal = SoAd_TxTp_TcpCopyTxData(SoConIdx, pduRouteDestIdx, PartitionIdx, BufPtr, BufLength);                      /* SBSW_SOAD_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_TxTcp_CopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxTcp_Terminate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxTcp_Terminate(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reinitialize Tx Tcp queue. */
  SoAd_TxTcpQueue_Clear(SoConIdx, PartitionIdx);

} /* SoAd_TxTcp_Terminate() */

/**********************************************************************************************************************
 *  SoAd_TxTcpQueue_TxConfirmation()
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
 */
FUNC(void, SOAD_CODE) SoAd_TxTcpQueue_TxConfirmation(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  PduLengthType Len)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType                 localLen = Len;
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTcpTxQueueType     tcpTxQueueIdx = SoAd_GetTcpTxQueueIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfTcpTxQueueDataType tcpTxQueueDataIdx;
  SoAd_SizeOfPduRouteType       pduRouteIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that call is interrupted and Tx TCP queue becomes inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Iterate over queue until all elements are released or length to be released is 0. */
  while ( localLen > 0u )
  {
    /* #30 Check if queue is filled. */
    if ( SoAd_TxTcpQueue_GetTotalLength(SoConIdx, PartitionIdx) > 0u )
    {
      tcpTxQueueDataIdx = SoAd_GetTcpTxQueueDataIdxOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx);

      /* #40 Check if PDU is confirmed completely. */
      if ( SoAd_GetLenOfTcpTxQueueData(tcpTxQueueDataIdx, PartitionIdx) <= localLen )
      {
        /* Calculate current index considering wrap around. */
        SoAd_SizeOfTcpTxQueueDataType currentIdx = (SoAd_SizeOfTcpTxQueueDataType)SoAd_Util_GetNextArrayElement(
          tcpTxQueueDataIdx, 1u, SoAd_GetTcpTxQueueDataStartIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx),
          SoAd_GetTcpTxQueueDataEndIdxOfTcpTxQueue(tcpTxQueueIdx, PartitionIdx));
        pduRouteIdx = SoAd_GetPduRouteIdxOfTcpTxQueueData(tcpTxQueueDataIdx, PartitionIdx);

        /* #400 Remove element from queue. */
        localLen -= SoAd_GetLenOfTcpTxQueueData(tcpTxQueueDataIdx, PartitionIdx);

        SoAd_SetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, SoAd_GetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
           SoAd_GetLenOfTcpTxQueueData(tcpTxQueueDataIdx, PartitionIdx), PartitionIdx);

        SoAd_SetLenOfTcpTxQueueData(tcpTxQueueDataIdx, 0u, PartitionIdx);                                              /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
        SoAd_SetPduRouteIdxOfTcpTxQueueData(tcpTxQueueDataIdx, SoAd_GetSizeOfPduRoute(PartitionIdx), PartitionIdx);    /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */

        SoAd_DecLvlOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx);                                                       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

        SoAd_SetTcpTxQueueDataIdxOfTcpTxQueueMgt(tcpTxQueueIdx, currentIdx, PartitionIdx);                             /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */

        /* #401 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #402 Confirm PDU transmission. */
        SoAd_TxTcpQueue_CallTxConfirmation(SoConIdx, pduRouteIdx, PartitionIdx);

        /* #403 Enter critical section again. */
        SOAD_BEGIN_CRITICAL_SECTION();
      }
      /* #41 Confirm PDU partly otherwise. */
      else
      {
        SoAd_SetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, SoAd_GetTotLenOfTcpTxQueueMgt(tcpTxQueueIdx, PartitionIdx) -      /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
          localLen, PartitionIdx);
        SoAd_SetLenOfTcpTxQueueData(tcpTxQueueDataIdx, SoAd_GetLenOfTcpTxQueueData(tcpTxQueueDataIdx, PartitionIdx) -  /* SBSW_SOAD_CALCULATED_CSL03_CSL05_TcpTxQueueData */
          localLen, PartitionIdx);

        localLen = 0u;
      }
    }
    /* #31 Abort confirmation otherwise. */
    else
    {
      break;
    }
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_TxTcpQueue_TxConfirmation() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxUdp_ScheduleTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxUdp_ScheduleTransmission(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle nPdu transmission. */
  if ( SoAd_HasNPduUdpTx(SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx)) )
  {
    SoAd_TxNPduUdp_HandleTransmission(InstMapIdx);
  }
} /* SoAd_TxUdp_ScheduleTransmission() */

/**********************************************************************************************************************
 *  SoAd_TxUdp_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxUdp_CopyTxData(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(
    SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx);
  SoAd_SizeOfPduRouteType     pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);
  BufReq_ReturnType           retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle transmission for IF-API. */
  if ( SoAd_GetUpperLayerApiOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_UL_API_IF )
  {
    retVal = SoAd_TxIf_UdpCopyTxData(SoConIdx, PartitionIdx, BufPtr, BufLengthPtr);                                    /* SBSW_SOAD_POINTER_FORWARD */
  }
  /* #20 Handle transmission for TP-API otherwise. */
  else
  {
    SoAd_TxTp_UdpCopyTxData(SoConIdx, PartitionIdx, BufPtr, *BufLengthPtr);                                            /* SBSW_SOAD_POINTER_FORWARD */
    retVal = BUFREQ_OK;
  }

  return retVal;
} /* SoAd_TxUdp_CopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxUdp_Terminate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxUdp_Terminate(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear nPdu. */
  if ( SoAd_IsNPduUdpTxUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    SoAd_TxNPduUdp_Clear(SoAd_GetNPduUdpTxIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx);
  }
} /* SoAd_TxUdp_Terminate() */

/**********************************************************************************************************************
 *  SoAd_TxNPduUdp_IsTxPending()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_TxNPduUdp_IsTxPending(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean TransmitImmediately)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTxPending = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for pending nPdu transmission. */
  if ( SoAd_IsNPduUdpTxUsedOfSoCon(SoConIdx, PartitionIdx) &&
    (SoAd_GetTotalLenOfNPduUdpTxDyn(SoAd_GetNPduUdpTxIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) > 0u) )
  {
    /* #20 Update timeout to send nPdu immediately in next main function if requested. */
    if ( TransmitImmediately == TRUE )
    {
      SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
      SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

      (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_N_PDU, SoAd_GetNPduUdpTxIdxOfSoCon(SoConIdx, PartitionIdx),
        1u, instanceIdx, PartitionIdx);
    }

    isTxPending = TRUE;
  }

  return isTxPending;
} /* SoAd_TxNPduUdp_IsTxPending() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */ /*lint !e451 */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Tx.c
 *********************************************************************************************************************/
