/***********************************************************************************************************************
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
/*!        \file  SomeIpTp_Cbk.h
 *        \brief  SomeIpTp callback header file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#if !defined (SOMEIPTP_CBK_H)
# define SOMEIPTP_CBK_H

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "SomeIpTp_Cfg.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
# define SOMEIPTP_START_SEC_CODE
# include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * SomeIpTp_TriggerTransmit()
 ***********************************************************************************************************************/
/*! \brief        Requests Data of a SomeIpTp Pdu from the upper layer and assembles it along with the TP header.
 *  \details      -
 *  \param[in]    TxPduId      Pdu ID passed by the lower layer which is used for transmission
 *  \param[in]    PduInfoPtr   Pointer to the buffer where the SDU data shall be copied
 *  \pre          Interrupts are enabled.
 *  \context      TASK
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 *  \config       This function is only available if SOMEIPTP_TXNSDU is STD_ON.
 *  \trace        CREQ-169364, CREQ-169365, CREQ-169367
 ***********************************************************************************************************************/
FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TriggerTransmit(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId, 
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

/***********************************************************************************************************************
 * SomeIpTp_TxConfirmation()
 ***********************************************************************************************************************/
/*! \brief        Confirmation by the PduR when the requested Tx Pdu is transmitted successfully.
 *  \details      -
 *  \param[in]    TxPduId      Pdu ID passed by the lower layer which is used for transmission
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 *  \config       This function is only available if SOMEIPTP_TXNSDU is STD_ON.
 *  \trace        CREQ-169364, CREQ-169365
 ***********************************************************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxConfirmation(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId);

/***********************************************************************************************************************
 * SomeIpTp_RxIndication()
 ***********************************************************************************************************************/
/*! \brief        Indication of a received SomeIp Pdu
 *  \details      -
 *  \param[in]    RxPduId       Pdu ID passed by the lower layer 
 *  \param[in]    PduInfoPtr    Pdu info passed by the lower layer. The passed buffer shall be at least as long as the
 *                              SOME/IP header (8 bytes) plus the length of the PDU's metadata.
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 *  \config       This function is only available if SOMEIPTP_RXNSDU is STD_ON.
 *  \trace        CREQ-169368, CREQ-169369, CREQ-169370, CREQ-169371, CREQ-169375
 ***********************************************************************************************************************/
FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxIndication(VAR(PduIdType, SOMEIPTP_APPL_VAR) RxPduId, 
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

# define SOMEIPTP_STOP_SEC_CODE
# include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif  /* SOMEIPTP_CBK_H */

/***********************************************************************************************************************
 *  END OF FILE: SomeIpTp_Cbk.h
 **********************************************************************************************************************/
