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
/*!        \file  DoIPInt_Cbk.h
 *        \brief  DoIPInt callback header file
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

#if !defined (DOIPINT_CBK_H)
# define DOIPINT_CBK_H

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
 *  DoIPInt_SoConModeChg()
 *********************************************************************************************************************/
/*! \brief        Receives notification for socket connection mode change.
 *  \details      -
 *  \param[in]    SoConId                 Socket connection index specifying the socket connection with mode change.
 *  \param[in]    Mode                    New socket connection mode.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same SoConId, TRUE otherwise
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_SoConModeChg(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode);

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpCopyTxData()
 *********************************************************************************************************************/
/*! \brief        Copies data for transmission to provided buffer.
 *  \details      -
 *  \param[in]    DoIPIntSoAdTxPduId      DoIPInt unique identifier of the PDU transmitted by the SoAd.
 *  \param[in]    DoIPIntSoAdTxInfoPtr    Provides the destination buffer and the number of bytes to be copied.
 *  \param[in]    RetryPtr                Parameter is not supported (NULL_PTR).
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *  \return       BUFREQ_OK               Data has been copied to the transmit buffer completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same DoIPIntSoAdTxPduId, TRUE otherwise
 *  \synchronous  FALSE
 *  \spec
 *    requires $external(DoIPIntSoAdTxInfoPtr);
 *    requires $external(DoIPIntSoAdTxInfoPtr->SduDataPtr);
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_SoAdTpCopyTxData(
  PduIdType DoIPIntSoAdTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) DoIPIntSoAdTxInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DOIPINT_VAR) RetryPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpTxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Receives confirmation for transmitted PDU.
 *  \details      -
 *  \param[in]    DoIPIntSoAdTxPduId      DoIPInt unique identifier of the PDU transmitted by the SoAd.
 *  \param[in]    Result                  Result of the transmission.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same DoIPIntSoAdRxPduId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_SoAdTpTxConfirmation(
  PduIdType DoIPIntSoAdTxPduId,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpStartOfReception()
 *********************************************************************************************************************/
/*! \brief        Indicates start of reception of a PDU.
 *  \details      DoIPInt extracts the diagnostic messages from TCP stream. Therefore, function expects to be called
 *                with TpSduLength set to 0.
 *  \param[in]    DoIPIntSoAdRxPduId      DoIPInt unique identifier of the PDU to be received from the SoAd.
 *  \param[in]    DoIPIntSoAdRxInfoPtr    Parameter is not supported (NULL_PTR).
 *  \param[in]    TpSduLength             Length of the PDU.
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *  \return       BUFREQ_OK               Reception request was accepted.
 *  \return       BUFREQ_E_NOT_OK         Reception request was not accepted.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same DoIPIntSoAdRxPduId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_SoAdTpStartOfReception(
  PduIdType DoIPIntSoAdRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) DoIPIntSoAdRxInfoPtr,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpCopyRxData()
 *********************************************************************************************************************/
/*! \brief        Copies data for reception from provided buffer.
 *  \details      -
 *  \param[in]    DoIPIntSoAdRxPduId      DoIPInt unique identifier of the PDU to be received from the SoAd.
 *  \param[in]    DoIPIntSoAdRxInfoPtr    Provides the buffer and the number of bytes to be copied.
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *  \return       BUFREQ_OK               Data has been copied completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same DoIPIntSoAdRxPduId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_SoAdTpCopyRxData(
  PduIdType DoIPIntSoAdRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) DoIPIntSoAdRxInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_SoAdTpRxIndication()
 *********************************************************************************************************************/
/*! \brief        Receives indication for received PDU.
 *  \details      Since DoIPInt extracts the diagnostic messages from TCP stream this indication also indicates that
 *                socket connection is closed.
 *  \param[in]    DoIPIntSoAdRxPduId      DoIPInt unique identifier of the PDU to be received from the SoAd.
 *  \param[in]    Result                  Result of the reception.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same DoIPIntSoAdRxPduId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_SoAdTpRxIndication(
  PduIdType DoIPIntSoAdRxPduId,
  Std_ReturnType Result);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_CBK_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Cbk.h
 *********************************************************************************************************************/
