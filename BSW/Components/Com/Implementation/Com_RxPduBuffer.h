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
/*!        \file  Com_RxPduBuffer.h
 *         \unit  RxPduBuffer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_RXPDUBUFFER_H)
# define COM_RXPDUBUFFER_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

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
  Com_RxPduBuffer_CopyToRxDefPduBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the given data to the RxDefPduBuffer and stores the SduLength in the handle deferred buffer.
    \details      -
    \pre          IsUsed check required: Com_IsHandleRxPduDeferredUsedOfRxPduInfo()
    \param[in]    comRxPduInfoIdx   Handle Id of Rx Pdu
    \param[in]    PduInfoPtr        pointer to the data
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduBuffer_CopyToRxDefPduBuffer(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_RxPduBuffer_ResetLengthOfDeferredRxPdu
**********************************************************************************************************************/
/*! \brief        This function resets the length of the Rx deferred I-PDU to zero.
    \details      -
    \pre          -
    \param[in]    comRxPduInfoIdx    Rx PduId
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduBuffer_ResetLengthOfDeferredRxPdu(Com_RxPduInfoIterType comRxPduInfoIdx);

/**********************************************************************************************************************
  Com_RxPduBuffer_GetLengthOfDeferredRxPdu
**********************************************************************************************************************/
/*! \brief        This function returns the length of the deferred Rx I-PDU which was set before.
    \details      -
    \pre          -
    \param[in]    comRxPduInfoIdx    Rx PduId
    \return       deferredPduLength  Length of Rx deferred I-PDU
    \context      TASK|ISR
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxPduBuffer_GetLengthOfDeferredRxPdu(Com_RxPduInfoIterType comRxPduInfoIdx);

/**********************************************************************************************************************
  Com_RxPduBuffer_GetAddrOfRxDefPduBuffer
**********************************************************************************************************************/
/*! \brief        This function returns a pointer to the RxDefPduBuffer at the current index.
    \details      -
    \pre          -
    \param[in]    rxPduId                    ID of I-PDU
    \return       Com_RxDefPduBufferPtrType  pointer to the RxDefPduBuffer
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(Com_RxDefPduBufferPtrType, COM_CODE) Com_RxPduBuffer_GetAddrOfRxDefPduBuffer(Com_RxPduInfoIterType comRxPduInfoIdx);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXPDUBUFFER_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxPduBuffer.h
**********************************************************************************************************************/
