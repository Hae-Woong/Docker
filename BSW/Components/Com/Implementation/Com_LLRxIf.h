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
/*!        \file  Com_LLRxIf.h
 *         \unit  LLRxIf
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_LLRXIF_H)
# define COM_LLRXIF_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
# define COM_START_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_LLRxIf_RxIndicationProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_LLRxIf_IfRxIndicationProcessing() or Com_LLRxTp_TpRxIndicationProcessing() after an I-PDU
                  has been received and does the processing of the PDU.
    \details      -
    \pre          -
    \param[in]    rxPduId      ID of I-PDU that has been received. Identifies the data that has been received.
                               Range: 0..(maximum number of I-PDU IDs received by AUTOSAR COM) - 1
    \param[in]    PduInfoPtr   Payload information of the received I-PDU (pointer to data and data length).
    \return       TRUE         If Pdu was processed
                  FALSE        otherwise
    \trace        SPEC-2736845, SPEC-2736846
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLRxIf_RxIndicationProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_LLRxIf_IfRxIndicationProcessing
**********************************************************************************************************************/
/*! \brief        This function is called by Com_RxIndication() after an I-PDU has been received.
    \details      -
    \pre          -
    \param[in]    rxPduId      ID of I-PDU that has been received. Identifies the data that has been received.
                               Range: 0..(maximum number of I-PDU IDs received by AUTOSAR COM) - 1
    \param[in]    PduInfoPtr   Payload information of the received I-PDU (pointer to data and data length).
    \return       TRUE         If Pdu was processed
                  FALSE        otherwise
    \trace        SPEC-2736845, SPEC-2736846
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_LLRxIf_IfRxIndicationProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_LLRXIF_H */
/**********************************************************************************************************************
  END OF FILE: Com_LLRxIf.h
**********************************************************************************************************************/
