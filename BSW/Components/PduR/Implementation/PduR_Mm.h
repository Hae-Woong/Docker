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
 *         \file  PduR_Mm.h
 *         \unit  Mm
 *        \brief  Pdu Router Module Manager header file
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
#if !defined (PDUR_MM_H)
# define PDUR_MM_H

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
 * PduR_Mm_CallIfTxConfirmation
 *********************************************************************************************************************/
/*! \brief       Calls If-TxConfirmation of the source module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   srcHnd                   Valid handle of the source module.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallIfTxConfirmation(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd);

/**********************************************************************************************************************
 * PduR_Mm_CallIfTransmit
 *********************************************************************************************************************/
/*! \brief       Calls If-Transmit of the destination module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType
 *               E_OK                     Transmit was executed successfully.
 *               E_NOT_OK                 Transmit was not possible, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallIfTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo);

/**********************************************************************************************************************
 * PduR_Mm_CallIfCancelTransmit
 *********************************************************************************************************************/
/*! \brief       Calls If-CancelTransmit of the destination and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \return      Std_ReturnType
 *               E_OK                     Cancellation was executed successfully.
 *               E_NOT_OK                 Cancellation was rejected, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallIfCancelTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd);

/**********************************************************************************************************************
 * PduR_Mm_CallIfTriggerTransmit
 *********************************************************************************************************************/
/*! \brief       Calls If-TriggerTransmit of the source module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType
 *               E_OK                     Copying the data was executed successfully.
 *               E_NOT_OK                 Copying the data was not possible, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallIfTriggerTransmit(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo);

/**********************************************************************************************************************
 * PduR_Mm_CallIfRxIndication
 *********************************************************************************************************************/
/*! \brief       Calls If-RxIndication of the destination module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallIfRxIndication(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo);

/**********************************************************************************************************************
 * PduR_Mm_CallTpTransmit
 *********************************************************************************************************************/
/*! \brief       Calls Tp-Transmit of the destination module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType
 *               E_OK                     Transmit was executed successfully.
 *               E_NOT_OK                 Transmit was not possible, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo);

/**********************************************************************************************************************
 * PduR_Mm_CallTpCancelTransmit
 *********************************************************************************************************************/
/*! \brief       Calls Tp-CancelTransmit of the destination module and verifies validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \return      Std_ReturnType
 *               E_OK                     Cancellation was executed successfully.
 *               E_NOT_OK                 Cancellation was rejected, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpCancelTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd);

/**********************************************************************************************************************
 * PduR_Mm_CallTpCopyTxData
 *********************************************************************************************************************/
/*! \brief       Calls Tp-CopyTxData of the source module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   srcHnd                   Valid handle of the source module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \param[in]   retryInfo                Retry is not supported.
 *  \param[out]  availableDataPtr         Indicates the remaining number of bytes that are available in the PduR buffer.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK                Buffer request was successful.
 *               BUFREQ_E_BUSY            Required buffer amount currently not available.
 *               BUFREQ_E_NOT_OK          Currently no buffer is available, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Mm_CallTpCopyTxData(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo, P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retryInfo, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);

/**********************************************************************************************************************
 * PduR_Mm_CallTpTxConfirmation
 *********************************************************************************************************************/
/*! \brief       Calls Tp-TxConfirmation of the source module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   srcHnd                   Valid handle of the source module.
 *  \param[in]   result                   Result of the TP transmission.
 *               E_OK                     The TP transmission has been completed successfully.
 *               E_NOT_OK                 The TP transmission has been aborted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallTpTxConfirmation(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, Std_ReturnType result);

/**********************************************************************************************************************
 * PduR_Mm_CallTpStartOfReception
 *********************************************************************************************************************/
/*! \brief       Calls Tp-StartOfReception of the destination module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \param[in]   tpSduLength              Length of the entire Tp SDU which will be received.
 *  \param[out]  bufferSizePtr            This parameter is used to return the available buffer size.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK                Buffer request was successful.
 *               BUFREQ_E_NOT_OK          Currently no buffer is available, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Mm_CallTpStartOfReception(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo, PduLengthType tpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
 * PduR_Mm_CallTpRxIndication
 *********************************************************************************************************************/
/*! \brief       Calls Tp-RxIndication of the destination and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   result                   Result of the TP reception.
 *               E_OK                     The TP transmission has been completed successfully.
 *               E_NOT_OK                 The TP transmission has been aborted, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallTpRxIndication(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, Std_ReturnType result);

/**********************************************************************************************************************
 * PduR_Mm_CallTpCopyRxData
 *********************************************************************************************************************/
/*! \brief       Calls Tp-CopyRxData of the destination module and verifies validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   destHnd                  Valid handle of the destination module.
 *  \param[in]   pduInfo                  Payload information of the received I-PDU (pointer to data and data length).
 *  \param[out]  bufferSizePtr            This parameter is used to return the available buffer size.
 *  \return      BufReq_ReturnType
 *               BUFREQ_OK                Data has been copied successfully. The returned bufferSizePtr is valid.
 *               BUFREQ_E_NOT_OK          Currently no buffer is available, or function pointer was invalid.
 *               BUFREQ_E_OVFL            The upper TP module is not able to receive the number of bytes.
 *                                        The request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Mm_CallTpCopyRxData(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
 * PduR_Mm_CallTpCancelReceive
 *********************************************************************************************************************/
/*! \brief       Calls Tp-CancelReceive of the source module and verifies the validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   srcHnd                   Valid handle of the source module.
 *  \return      Std_ReturnType
 *               E_OK                     Cancellation was executed successfully.
 *               E_NOT_OK                 Cancellation was rejected, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpCancelReceive(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd);

/**********************************************************************************************************************
 * PduR_Mm_CallTpChangeParameter
 *********************************************************************************************************************/
/*! \brief       Calls Tp-ChangeParameter of the source module and verifies validity of its function pointer.
 *  \details     -
 *  \param[in]   mmRomIdx                 Valid MmRom table index. Contains respective functions pointer.
 *  \param[in]   srcHnd                   Valid handle of the source module.
 *  \param[in]   parameter                The TP parameter that shall be changed.
 *  \param[in]   value                    The new value for the TP parameter.
 *  \return      Std_ReturnType
 *               E_OK                     The parameter was changed successfully.
 *               E_NOT_OK                 The parameter change was rejected, or function pointer was invalid.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires mmRomIdx < PduR_GetSizeOfMmRom();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpChangeParameter(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, TPParameterType parameter, uint16 value);

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_MM_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Mm.h
 *********************************************************************************************************************/
