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
 *         \file  PduR_RmTp_TxRouting.h
 *         \unit  RmTp_TxRouting
 *        \brief  Pdu Router Routing Manager Transport Protocol Tx Routing header file
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
#if !defined (PDUR_RMTP_TXROUTING_H)
# define PDUR_RMTP_TXROUTING_H

/**********************************************************************************************************************
   LOCAL MISRA / PCLINT JUSTIFICATION
**********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA  S 0777 EOF */ /* MD_MSR_Rule5.1_0777 */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/* \trace SPEC-2020230, SPEC-38402 */

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
  PduR_RmTp_TxRouting_DispatchCopyTxData
**********************************************************************************************************************/
/*! \brief       Forwards the CopyTxData call to the corresponding source.
 *  \details     -
 *  \param[in]   rmDestRomIdx
 *  \param[in]   info                       Pointer to the destination buffer and the number of bytes to copy.
 *  \param[in]   retry                      not supported
 *  \param[out]  availableDataPtr           Indicates the remaining number of bytes that are available in the PduR Tx buffer.
 *  \return      BUFREQ_OK                  The data has been copied to the transmit buffer successfully.
 *               BUFREQ_E_NOT_OK            No data has been copied.
 *               BUFREQ_E_BUSY              The request cannot be processed because no data is available. No data has been copied.
 *                                          The lower layer module might retry it later.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_TxRouting_DispatchCopyTxData(PduR_RmDestRomIterType rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                                              P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr);

/**********************************************************************************************************************
 * PduR_RmTp_TxRouting_DispatchTpTxConfirmation
 *********************************************************************************************************************/
/*! \brief       Forwards the TpTxConfirmation to the corresponding source.
 *  \details     -
 *  \param[in]   rmDestRomIdx   Id of destination PDU reference.
 *  \param[in]   result         result of the transmission
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
 ***********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_TxRouting_DispatchTpTxConfirmation(PduR_RmDestRomIterType rmDestRomIdx, Std_ReturnType result);

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMTP_TXROUTING_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_TxRouting.h
 *********************************************************************************************************************/
