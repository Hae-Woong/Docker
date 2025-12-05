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
/*!        \file  Com_RxSignalProcessing.h
 *         \unit  RxSignalProcessing
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_RXSIGNALPROCESSING_H)
# define COM_RXSIGNALPROCESSING_H

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
  Com_RxSignalProcessing_RxPduSigEvent
**********************************************************************************************************************/
/*! \brief              This function processes all related Rx ComSignals of the received ComIPdu.
    \details            -
    \pre                -
    \param[in]          comRxPduInfoIdx     index of the received ComIPdu
    \param[in]          PduInfoPtr          contains the SDU buffer and the SDU length
    \param[in,out]      fctPtrCacheStrctPtr contains function ptr cache informations (cache location, size, index)
    \trace              SPEC-2736871, SPEC-2736852
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
      requires fctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSignalProcessing_RxPduSigEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                          CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  Com_RxSignalProcessing_RxPduSigGrpEvent
**********************************************************************************************************************/
/*! \brief              This function processes all related Rx ComSignalGroups of the received ComIPdu.
    \details            -
    \pre                -
    \param[in]          comRxPduInfoIdx     index of the received ComIPdu
    \param[in]          PduInfoPtr          contains the SDU buffer and the SDU length
    \param[in,out]      fctPtrCacheStrctPtr contains function ptr cache informations (cache location, size, index)
    \trace              SPEC-2736871, SPEC-2736854
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different handles
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
      requires fctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSignalProcessing_RxPduSigGrpEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                             CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXSIGNALPROCESSING_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxSignalProcessing.h
**********************************************************************************************************************/
