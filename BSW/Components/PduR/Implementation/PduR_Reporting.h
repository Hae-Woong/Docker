/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_Reporting.h
 *         \unit  Reporting
 *        \brief  Pdu Router Reporting header file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
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
#if !defined (PDUR_REPORTING_H)
# define PDUR_REPORTING_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "PduR_Types.h"
# include "PduR_Cfg.h"
# include "PduR_Lcfg.h"

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

# define PDUR_START_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * PduR_Reporting_ReportError
 *********************************************************************************************************************/
/*! \brief       Calls the Det module with a development error.
 *  \details     -
 *  \param[in]   ApiId     the Api Id in which this error occurred.
 *  \param[in]   ErrorCode the actual error code.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_ReportError(uint8 ApiId, uint8 ErrorCode);

/**********************************************************************************************************************
 * PduR_Reporting_ReportRuntimeError
 *********************************************************************************************************************/
/*! \brief       Calls the Det module with a runtime error.
 *  \details     -
 *  \param[in]   ApiId     the Api Id in which this error occurred.
 *  \param[in]   ErrorCode the actual error code.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_ReportRuntimeError(uint8 ApiId, uint8 ErrorCode);

/**********************************************************************************************************************
 * PduR_Reporting_CallPreTransmitCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for PreTransmit.
 *  \details     -
 *  \param[in]   rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu. The callback shall be called for this destination, if configured.
 *  \param[in]   info          Pointer to the data to be transmitted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallPreTransmitCallback(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
 * PduR_Reporting_CallTransmitCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for Transmit.
 *  \details     -
 *  \param[in]   tx2LoIdx ID of the upper layer I-PDU to be transmitted
 *  \param[in]   info     Pointer to the data to be transmitted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires tx2LoIdx < PduR_GetSizeOfTx2Lo();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTransmitCallback(PduR_Tx2LoIterType tx2LoIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
 * PduR_Reporting_CallRxIndicationCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for RxIndication.
 *  \details     -
 *  \param[in]   rxIf2DestIdx ID of the received lower layer I-PDU
 *  \param[in]   info         I-PDU data (pointer to data and data length)
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rxIf2DestIdx < PduR_GetSizeOfRxIf2Dest();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallRxIndicationCallback(PduR_RxIf2DestIterType rxIf2DestIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
 * PduR_Reporting_CallTxConfirmationCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for TxConfirmation.
 *  \details     -
 *  \param[in]   txIf2UpIdx  ID of the transmitted lower layer I-PDU
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires txIf2UpIdx < PduR_GetSizeOfTxIf2Up();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTxConfirmationCallback(PduR_TxIf2UpIterType txIf2UpIdx);

/**********************************************************************************************************************
 * PduR_Reporting_CallTpStartOfReceptionCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for TpStartOfReception.
 *  \details     -
 *  \param[in]   rxTp2DestIdx      ID of the lower layer Tp I-PDU that will be received.
 *  \param[in]   info              Pointer to the buffer with meta data if the meta data feature is used (SduDataPtr).
 *                                 SduLength must be the entire length of the the provided buffer including the metadata length.
 *  \param[in]   TpSduLength       Length of the entire the lower layer Tp SDU which will be received.
 *  \param[out]  bufferSizePtr     This parameter is used to return the available buffer size.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rxTp2DestIdx < PduR_GetSizeOfRxTp2Dest();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTpStartOfReceptionCallback(PduR_RxTp2DestIterType rxTp2DestIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
 * PduR_Reporting_CallTpRxIndicationCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for TpRxIndication.
 *  \details     -
 *  \param[in]   rxTp2DestIdx     ID of the received lower layer Tp I-PDU
 *  \param[in]   result           Result of the TP reception
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rxTp2DestIdx < PduR_GetSizeOfRxTp2Dest();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTpRxIndicationCallback(PduR_RxTp2DestIterType rxTp2DestIdx, Std_ReturnType result);

/**********************************************************************************************************************
 * PduR_Reporting_CallTpTxConfirmationCallback
 *********************************************************************************************************************/
/*! \brief       Calls the BswM callback for TpTxConfirmation.
 *  \details     -
 *  \param[in]   txTp2SrcIdx    ID of the transmitted lower layer Tp I-PDU
 *  \param[in]   result         Result of the TP transmission
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires txTp2SrcIdx < PduR_GetSizeOfTxTp2Src();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTpTxConfirmationCallback(PduR_TxTp2SrcIterType txTp2SrcIdx, Std_ReturnType result);

/**********************************************************************************************************************
 * PduR_Reporting_NotifyQueueOverflow
 *********************************************************************************************************************/
/*! \brief       Calls error notification for a queue overflow.
 *  \details     -
 *  \param[in]   rmGDestRomIdx     Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmGDestRomIdx < PduR_GetSizeOfRmGDestRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_NotifyQueueOverflow(PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx);

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_REPORTING_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Reporting.h
 *********************************************************************************************************************/
