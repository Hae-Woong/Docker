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
 *         \file  PduR_RmIf.h
 *         \unit  RmIf
 *        \brief  Pdu Router Routing Manager Interface header file
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
#if !defined (PDUR_RMIF_H)
# define PDUR_RMIF_H

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
 * PduR_RmIf_RoutePdu
**********************************************************************************************************************/
/*! \brief       This function forwards the Pdu to the configured destinations.
 *  \details     -
 *  \param[in]   rmSrcRomIdx          Valid RmSrcRom table index. Internal handle of a PduRSrcPdu.
 *  \param[in]   info              Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType
 *               E_OK              The routing to at least one destination was successful.
 *               E_NOT_OK          The routing to none destination was successful.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmSrcRomIdx < PduR_GetSizeOfRmSrcRom();
 *  \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_RoutePdu(PduR_RmSrcRomIterType rmSrcRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
 * PduR_RmIf_DispatchDestPdu
**********************************************************************************************************************/
/*! \brief       This function forwards the received Pdu to the corresponding destination function.
 *  \details     -
 *  \param[in]   rmDestRomIdx      Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   info              Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType    E_OK:     The SDU has been routed successfully.
 *                                 E_NOT_OK: The SDU has not been routed successfully.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_DispatchDestPdu(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

 /**********************************************************************************************************************
  PduR_RmIf_DispatchTriggerTransmit
**********************************************************************************************************************/
/*! \brief       This function forwards the TriggerTransmit request to the correct (internal) destination.
 *  \details     -
 *  \param[in]   rmDestRomIdx      Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   info              Payload information of the received I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType    E_OK:     The SDU has been copied and the SduLength indicates the number of copied bytes.
 *                                 E_NOT_OK: The SDU has not been copied and the SduLength has not been set.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
***********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_DispatchTriggerTransmit(PduR_RmDestRomIterType rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

 /**********************************************************************************************************************
  PduR_RmIf_DispatchTxConfirmation
**********************************************************************************************************************/
/*! \brief       This function forwards the TxConfirmation to the correct (internal) destination.
 *  \details     -
 *  \param[in]   rmDestRomIdx      Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
***********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_DispatchTxConfirmation(PduR_RmDestRomIterType rmDestRomIdx);

/**********************************************************************************************************************
  PduR_RmIf_ProcessDestPdu
**********************************************************************************************************************/
/*! \brief       This function processes the PduLengthHandlingStrategy and forwards the Pdu correspondingly.
 *  \details     -
 *  \param[in]   rmDestRomIdx         Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   info              Payload information of the I-PDU (pointer to data and data length).
 *  \return      Std_ReturnType
 *               E_NOT_OK          The Pdu was not forwarded due to the PduLengthHandlingStrategy.
 *                                 Otherwise the return value of PduR_RmIf_DispatchDestPdu is returned.
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec
 *    requires rmDestRomIdx < PduR_GetSizeOfRmDestRom();
 *  \endspec
**********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_ProcessDestPdu(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info);

/**********************************************************************************************************************
  PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartition
**********************************************************************************************************************/
/*! \brief       This function verifies the validity of the destination handle in the McQ context
 *  \details     -
 *  \param[in]   rmDestRomIdx              Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   srcApplicationIdx         Valid index of the source application.
 *  \param[in]   destApplicationIdx        Valid index of the destination application.
 *  \return      boolean
 *
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
**********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartition(PduR_RmDestRomIterType rmDestRomIdx, PduR_DestApplicationManagerRomIterType srcApplicationIdx, PduR_DestApplicationManagerRomIterType destApplicationIdx);

/**********************************************************************************************************************
  PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartitionTxConfirmation
**********************************************************************************************************************/
/*! \brief       This function verifies the validity of the destination handle in the McQ context with TxConfirmation
 *  \details     -
 *  \param[in]   rmDestRomIdx              Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   srcApplicationIdx         Valid index of the source application.
 *  \param[in]   destApplicationIdx        Valid index of the destination application.
 *  \return      boolean
 *
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
**********************************************************************************************************************/
FUNC(boolean, PDUR_CODE) PduR_RmIf_IsRmDestRomIdxUnqueuedIFCrossPartitionTxConfirmation(PduR_RmDestRomIterType rmDestRomIdx, PduR_DestApplicationManagerRomIterType srcApplicationIdx, PduR_DestApplicationManagerRomIterType destApplicationIdx);


# define PDUR_STOP_SEC_CODE
# include "PduR_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif /* PDUR_RMIF_H */
/**********************************************************************************************************************
 *  END OF FILE: PduR_RmIf.h
 *********************************************************************************************************************/
