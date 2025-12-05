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
/*!        \file  Com_MainFunctions.h
 *         \unit  MainFunctions
 *        \brief  MICROSAR Communication header file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_MAINFUNCTIONS_H)
# define COM_MAINFUNCTIONS_H

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
Com_MainFunctionRxInternal
**********************************************************************************************************************/
/*! \brief     This function shall perform the processing of the AUTOSAR COM receive processing that are not directly
               initiated by the calls from the RTE and PDU-R.
  \details     -
  \pre         -
  \param[in]   mainFunctionId ID of the calling rx main function.
  \context     TASK
  \synchronous TRUE
  \reentrant   TRUE, for different Handles
  \trace       DSGN-Com23764
  \spec
    requires mainFunctionId < Com_GetSizeOfMainFunctionRxStruct();
  \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_MainFunctionRxInternal(Com_MainFunctionRxStructIterType mainFunctionId);

/**********************************************************************************************************************
Com_MainFunctionTxInternal
**********************************************************************************************************************/
/*! \brief     This function shall perform the processing of the transmission activities that are not
               directly initiated by the calls from the RTE and PDU-R.
  \details     -
  \pre         -
  \param[in]   mainFunctionId ID of the calling tx main function.
  \context     TASK
  \synchronous TRUE
  \reentrant   TRUE, for different Handles
  \trace       DSGN-Com23764
  \spec
    requires mainFunctionId < Com_GetSizeOfMainFunctionTxStruct();
  \endspec
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_MainFunctionTxInternal(Com_MainFunctionTxStructIterType mainFunctionId);

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_MainFunctionRouteSignalsInternal
**********************************************************************************************************************/
/*! \brief       This function shall perform the signal and description gateway processing part of COM to forward received signals to be routed.
                 The insertion of this call is necessary for decoupling receive interrupts and signal gateway tasks.
    \details     -
    \pre         -
    \param[in]   mainFunctionId    ID of the calling route signals main function.
    \context     TASK
    \synchronous TRUE
    \reentrant   TRUE, for different Handles
    \warning     The time between two consecutive calls (perhaps the related task/thread cycle)
                 affects directly the signal gateway latency.
    \trace       SPEC-2736940, SPEC-2736941
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_MainFunctionRouteSignalsInternal(Com_MainFunctionRouteSignalsStructIterType mainFunctionId);
# endif

# define COM_STOP_SEC_CODE
# include "Com_MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_MAINFUNCTIONS_H */
/**********************************************************************************************************************
  END OF FILE: Com_MainFunctions.h
**********************************************************************************************************************/
