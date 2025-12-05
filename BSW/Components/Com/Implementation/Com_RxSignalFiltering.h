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
/*!        \file  Com_RxSignalFiltering.h
 *        \unit   RxSignalFiltering
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#if !defined (COM_RXSIGNALFILTERING_H)
# define COM_RXSIGNALFILTERING_H

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
  Com_RxSignalFiltering_SignalFilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates the filter of the passed ComSignal.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo   Handle Id of ComSignal
    \param[in]    comDataPtr        Pointer to pointer to the memory where the new signal value is located and dynamic length (if needed)
    \return       boolean
                    TRUE       if the filter is passed
                    FALSE      otherwise
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxAccessInfo < Com_GetSizeOfRxAccessInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxSignalFiltering_SignalFilterCheck(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_RxSignalFiltering_SignalGroupFilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates all filters of the ComGroupSignals in the passed ComSignalGroup.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo   Handle Id of ComSignalGroup
    \return       boolean
                    TRUE       if at least one filter of a group signal is passed
                    FALSE      otherwise
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
    \spec
      requires idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxSignalFiltering_SignalGroupFilterCheck(Com_RxSigGrpInfoIterType idxRxSigGrpInfo);

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_RXSIGNALFILTERING_H */
/**********************************************************************************************************************
  END OF FILE: Com_RxSignalFiltering.h
**********************************************************************************************************************/
