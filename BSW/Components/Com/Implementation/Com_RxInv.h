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
/*!        \file  Com_RxInv.h
 *         \unit  RxInv
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_RXINV_H)
# define COM_RXINV_H

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
  Com_RxInv_EvaluateSigValidity
**********************************************************************************************************************/
/*! \brief             This function evaluates whether the new value of a ComSignals is an invalid value.
    \details           -
    \pre               -
    \param[in]         idxRxSigInfo         index of ComSignal
    \param[in,out]     fctPtrCacheStrctPtr  pointer containing all function pointer cache informations (location, size index)
    \param[in]         comDataPtr           pointer to a pointer to the memory where the tmp signal value is located and the length of the Sdu
    \return            boolean
                          FALSE if the ComSignal has an invalid value
                          TRUE  otherwise
    \context           TASK|ISR2
    \synchronous       TRUE
    \reentrant         TRUE, for different handles
    \spec
      requires idxRxSigInfo < Com_GetSizeOfRxSigInfo();
      requires fctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxInv_EvaluateSigValidity(Com_RxSigInfoIterType idxRxSigInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_RxInv_EvaluateSigGrpValidity
**********************************************************************************************************************/
/*! \brief             This function evaluates whether any new value of the ComGroupSignals is an invalid value.
    \details           -
    \pre               -
    \param[in]         idxRxSigGrpInfo      index of Rx SigGrpInfo
    \param[in,out]     fctPtrCacheStrctPtr  pointer containing all function pointer cache informations (location, size index)
    \param[in]         sduLength            Length of Sdu
    \return            boolean
                          FALSE if any ComGroupSignal has an invalid value
                          TRUE  otherwise
    \context           TASK|ISR2
    \synchronous       TRUE
    \reentrant         TRUE, for different handles
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
      requires fctPtrCacheStrctPtr != NULL_PTR;
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxInv_EvaluateSigGrpValidity(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr, PduLengthType sduLength);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXINV_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxInv.h
**********************************************************************************************************************/
