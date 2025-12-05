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
/*!        \file  Com_RxPduProcessing.h
 *         \unit  RxPduProcessing
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_RXPDUPROCESSING_H)
# define COM_RXPDUPROCESSING_H

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
  Com_RxPduProcessing_InitRxIpdu
**********************************************************************************************************************/
/*! \brief        This function initializes a rx I-PDU in for Rx Pdu Processing.
    \details      -
    \pre          -
    \param[in]    rxPduId Handle of the I-PDU.
    \context      TASK
    \trace        SPEC-2736775
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxPduId < Com_GetSizeOfRxPduInfo();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduProcessing_InitRxIpdu(Com_RxPduInfoIterType rxPduId);

/**********************************************************************************************************************
  Com_RxPduProcessing_MainFunctionRx
**********************************************************************************************************************/
/*! \brief              This function processes deferred Pdus in the context of the MainFunctionRx.
    \details            -
    \pre                -
    \param[in]          mainFunctionId    ID of the calling rx main function.
    \context            TASK
    \synchronous        TRUE
    \reentrant          TRUE, for different handles
    \spec
      requires mainFunctionId < Com_GetSizeOfMainFunctionRxStruct();
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduProcessing_MainFunctionRx(Com_MainFunctionRxStructIterType mainFunctionId);

/**********************************************************************************************************************
  Com_RxPduProcessing_ProcessImmediatePDU
**********************************************************************************************************************/
/*! \brief              This function processes immediate PDUs.
    \details            -
    \pre                -
    \param[in]          comRxPduInfoIdx               Index of the PDU that should be processed.
    \param[in]          PduInfoPtr                    Payload information of the received I-PDU (pointer to data and data length).
    \param[in,out]      immediatefctPtrCacheStrctPtr  Pointer to struct,
                                                      which contains all informations for deferred function pointer caching.
    \context            TASK|ISR2
    \reentrant          TRUE, for different handles
    \synchronous        TRUE
    \spec
      requires comRxPduInfoIdx < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
      requires immediatefctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduProcessing_ProcessImmediatePDU(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                             CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) immediatefctPtrCacheStrctPtr);


# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXPDUPROCESSING_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxPduProcessing.h
**********************************************************************************************************************/
