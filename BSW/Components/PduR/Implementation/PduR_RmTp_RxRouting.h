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
 *         \file  PduR_RmTp_RxRouting.h
 *         \unit  RmTp_RxRouting
 *        \brief  Pdu Router Routing Manager Transport Protocol Rx Routing header file
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
#if !defined (PDUR_RMTP_RXROUTING_H)
# define PDUR_RMTP_RXROUTING_H

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
  PduR_RmTp_RxRouting_DispatchStartOfReception
**********************************************************************************************************************/
/*! \brief       Forwards the StartOfReception to the correct destinations.
 *  \details     These destinations could be unqueued upper layer destinations or the TP queue. It will handle potential
 *               on-the-fly behavior if both kind of destinations are configured for the routing path.
 *  \param[in]   rmSrcRomIdx       Routing Manager source Id
 *  \param[in]   info              Pointer to the buffer. This parameter is not evaluated and will be forwarded transparently.
 *  \param[in]   TpSduLength       Length of the entire TP SDU. This parameter is not evaluated and will be forwarded transparently.
 *  \param[out]  bufferSizePtr     Available receive buffer after reception has been started. The lowest value of all destinations will be returned.
 *  \return      The return values of all destinations will be merged. One negative return value will lead to an overall negative return value.
 *               BUFREQ_OK         StartOfReception was successful
 *               BUFREQ_E_NOT_OK   StartOfReception was not successful
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *  \endspec
***********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxRouting_DispatchStartOfReception(PduR_RmSrcRomIterType rmSrcRomIdx,
                                                              P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                              PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
  PduR_RmTp_RxRouting_DispatchCopyRxData
**********************************************************************************************************************/
/*! \brief       Forwards the CopyRxData to the correct destinations.
 *  \details     These destinations could be unqueued upper layer destinations or the TP queue. It will handle potential
 *               on-the-fly behavior if both kind of destinations are configured for the routing path.
 *  \param[in]   rmSrcRomIdx      Routing Manager source Id
 *  \param[in]   info             Pointer to the data (SduDataPtr) and its length (SduLength). This parameter is not evaluated and will be forwarded transparently.
 *  \param[out]  bufferSizePtr    Available receive buffer after data has been copied. The lowest value of all destinations will be returned.
 *  \return      The return values of all destinations will be merged. One negative return value will lead to an overall negative return value.
 *               BUFREQ_OK        CopyRxData was successful.
 *               BUFREQ_E_NOT_OK  CopyRxData was not successful.
 *               BUFREQ_E_BUSY    CopyRxData was not successful, but could be retried later.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *  \endspec
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_RmTp_RxRouting_DispatchCopyRxData(PduR_RmSrcRomIterType rmSrcRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
                                                        P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr);

/**********************************************************************************************************************
  PduR_RmTp_RxRouting_DispatchTpRxIndication
**********************************************************************************************************************/
/*! \brief       Forwards the TpRxIndication to the correct destinations.
 *  \details     These destinations could be unqueued upper layer destinations or the TP queue.
 *  \param[in]   rmSrcRomIdx   Routing Manager source Id
 *  \param[in]   result        Result of the reception. This parameter is not evaluated and will be forwarded transparently.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *  \endspec
***********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmTp_RxRouting_DispatchTpRxIndication(PduR_RmSrcRomIterType rmSrcRomIdx, Std_ReturnType result);

# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMTP_RXROUTING_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmTp_RxRouting.h
 *********************************************************************************************************************/
